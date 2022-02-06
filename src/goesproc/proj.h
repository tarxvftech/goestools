#pragma once

#if PROJ_VERSION_MAJOR < 8 
#error "proj version 8 required"
#endif

#include <map>
#include <string>
#include <tuple>
#include <vector>

#include <proj.h>

class Proj {
public:
  explicit Proj(const std::vector<std::string>& args);

  explicit Proj(const std::map<std::string, std::string>& args);

  ~Proj();

  std::tuple<double, double> fwd(double lon, double lat);

  std::tuple<double, double> inv(double x, double y);

protected:
  PJ * proj_;
};
