#pragma once

#include <sstream>

namespace simulation {
namespace ui {
  class utils {
   public:
    static std::string fmt_precision(double val, int prec) {
      std::ostringstream out;
      out.precision(prec);
      out << std::fixed << val;
      return out.str();
    }
  };
}  // namespace ui
}  // namespace simulation