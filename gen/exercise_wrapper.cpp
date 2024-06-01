#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>
// #include "details.hpp"
// #include "elements.hpp"
#include "exercise.hpp"
// #include "json.hpp"
// #include "utils.hpp"

namespace py = pybind11;

PYBIND11_MODULE(native_rings_engine, m) {
    py::class_<Exercise>(m, "Exercise")
        .def(py::init<const std::string&>())
        .def("allActive", &Exercise::allActive)
        .def("tryInsert", &Exercise::tryInsert)
        .def("removeDetail", &Exercise::removeDetail)
        .def("getOccupiedCells", &Exercise::getOccupiedCells)
        .def("getVacantCells", &Exercise::getVacantCells)
        .def("isDetailConnected", &Exercise::isDetailConnected)
        .def("isConnected", &Exercise::isConnected);
}
