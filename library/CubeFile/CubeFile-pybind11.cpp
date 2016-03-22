#include "CubeFile.h"
#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_PLUGIN(libCubeFile) {
  py::module m("libCubeFile", "pybind11 CubeFile shared lib");

  py::class_<CubeFile>(m, "CubeFile")
    .def(py::init<>())
    .def(py::init<const std::string&>())
    .def(py::init<const CubeFile&>())
    .def("read", &CubeFile::read)
    .def("write", &CubeFile::write)
    .def("copy", &CubeFile::copy)
    .def("check", &CubeFile::check)
    .def("scalar_add", &CubeFile::scalar_add)
    .def("scalar_subtract", &CubeFile::scalar_subtract)
    .def("scalar_multiply", &CubeFile::scalar_multiply)
    .def("scalar_divide", &CubeFile::scalar_divide)
    .def("cube_add", &CubeFile::cube_add)
    .def("cube_subtract", &CubeFile::cube_subtract)
    .def("cube_multiply", &CubeFile::cube_multiply)
    .def("zero_cubeVals", &CubeFile::zero_cubeVals)
    .def("integrate", &CubeFile::integrate)
    .def("iso_volume", &CubeFile::iso_volume)
    .def("centroid", &CubeFile::centroid)
    .def("dens_centroid", &CubeFile::dens_centroid)
    .def("print", &CubeFile::print)
    //Operator Overload
    .def(py::self += CubeFile())
    .def(py::self += double())
    .def(py::self -= CubeFile())
    .def(py::self -= double())
    .def(py::self *= CubeFile())
    .def(py::self *= double())
    .def(py::self /= double())
    .def(py::self + CubeFile())
    .def(py::self + double())
    .def(double() + py::self)
    .def(double() - py::self)
    .def(double() * py::self)
    .def(double() / py::self)
    .def(py::self - CubeFile())
    .def(py::self - double())
    .def(py::self * CubeFile())
    .def(py::self * double())
    .def(py::self / double());

  //Friend and global functions
  m.def("g_return_overlap", g_return_overlap);
  m.def("g_return_modulo_overlap", g_return_modulo_overlap);
  m.def("g_scalar_add", g_scalar_add);
  m.def("g_scalar_subtract", g_scalar_subtract);
  m.def("g_scalar_multiply", g_scalar_multiply);
  m.def("g_scalar_divide", g_scalar_divide);
  m.def("g_cube_add", g_cube_add);
  m.def("g_cube_subtract", g_cube_subtract);
  m.def("g_cube_multiply", g_cube_multiply);
  m.def("g_cube_absolute_value", g_cube_absolute_value);
  m.def("g_cube_square_root", g_cube_square_root);
  m.def("g_cube_partition_plus", g_cube_partition_plus);
  m.def("g_cube_partition_minus", g_cube_partition_minus);

  return m.ptr();  
}
