#include "proj.h"

#include <cstring>
#include <sstream>

namespace {

std::vector<std::string> toVector(
  const std::map<std::string, std::string>& args) {
  std::vector<std::string> vargs;
  vargs.reserve(args.size());
  for (const auto& arg : args) {
    std::stringstream ss;
    ss << arg.first << "=" << arg.second;
    vargs.push_back(ss.str());
  }
  return vargs;
}

std::string pj_error(
    PJ * proj_,
    std::string prefix = "proj: "
    ) {
  std::stringstream ss;
  ss << prefix << proj_errno_string(proj_errno(proj_));
  return ss.str();
}

} // namespace

Proj::Proj(const std::vector<std::string>& args) {
  std::vector<char*> argv;
  for (const auto& arg : args) {
    argv.push_back(strdup(arg.c_str()));
  }
  proj_ = proj_create_argv(0, argv.size(), argv.data());
  if (!proj_) {
    throw std::runtime_error(pj_error(proj_, "proj initialization error: "));
  }
  for (auto& arg : argv) {
    free(arg);
  }
}

Proj::Proj(const std::map<std::string, std::string>& args)
  : Proj(toVector(args)) {
}

Proj::~Proj() {
  proj_destroy(proj_);
}

std::tuple<double, double> Proj::fwd(double lon, double lat) {
  PJ_COORD in = { lon, lat };
  PJ_COORD out = proj_trans( proj_, PJ_FWD, in);
  return std::make_tuple<double, double>(std::move(out.xy.x), std::move(out.xy.y));
}

std::tuple<double, double> Proj::inv(double x, double y) {
  PJ_COORD in = { x, y };
  PJ_COORD out = proj_trans( proj_, PJ_INV, in);
  return std::make_tuple<double, double>(std::move(out.uv.u), std::move(out.uv.v));
}
