#include <pybind11/pybind11.h>

#include "pypalp/polytope.hpp"

PYBIND11_MODULE(pypalp, m) {
  pybind11::class_<Polytope>(m, "Polytope")
      .def(pybind11::init<std::string const &>())
      .def(pybind11::init<pybind11::array_t<int> const &>())
      .def("vertices", &Polytope::vertices)
      .def("points", &Polytope::points)
      .def("is_ip", &Polytope::is_ip)
      .def("is_reflexive", &Polytope::is_reflexive);
}
