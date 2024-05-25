#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>
#include "details.hpp"
#include "elements.hpp"
#include "exercise.hpp"
#include "json.hpp"
#include "utils.hpp"

namespace py = pybind11;

PYBIND11_MODULE(iq_rings, m) {
    py::class_<Exercise>(m, "Exercise")
        .def(py::init<const std::string&>())
        .def("isDetailFits", &Exercise::isDetailFits)
        .def("insertDetail", &Exercise::insertDetail)
        .def("removeDetail", &Exercise::removeDetail)
        .def("isComplited", &Exercise::isComplited);
    
    m.def("makeExercise", &makeExercise, "Create an Exercise instance from a config file path");
}
