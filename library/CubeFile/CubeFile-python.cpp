#include "CubeFile.h"
#include <Python.h>
#include <boost/python.hpp>

using namespace std;
using namespace boost::python;


BOOST_PYTHON_MODULE(CubeFile)
{
    class_<CubeFile>("CubeFile")
        //Additional Constructors
        .def(init<const string&>())
        .def(init<const CubeFile&>())
        //Member Functions
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
        .def("absolute_value", &CubeFile::absolute_value)
        .def("square_root", &CubeFile::square_root)
        .def("integrate", &CubeFile::integrate)
        .def("iso_volume", &CubeFile::iso_volume)
        .def("centroid", &CubeFile::centroid)
        .def("print", &CubeFile::print)
        //Operator Overload
        .def(self += CubeFile())
        .def(self += double())
        .def(self -= CubeFile())
        .def(self -= double())
        .def(self *= CubeFile())
        .def(self *= double())
        .def(self /= double())
        .def(self + CubeFile())
        .def(self + double())
        .def(double() + self)
        .def(double() - self)
        .def(double() * self)
        .def(double() / self)
        .def(self - CubeFile())
        .def(self - double())
        .def(self * CubeFile())
        .def(self * double())
        .def(self / double());

    //Friend and global functions
    def("g_return_overlap", g_return_overlap);
    def("g_return_modulo_overlap", g_return_modulo_overlap);
    def("g_scalar_add", g_scalar_add);
    def("g_scalar_subtract", g_scalar_subtract);
    def("g_scalar_multiply", g_scalar_multiply);
    def("g_scalar_divide", g_scalar_divide);
    def("g_cube_add", g_cube_add);
    def("g_cube_subtract", g_cube_subtract);
    def("g_cube_multiply", g_cube_multiply);
    def("g_cube_absolute_value", g_cube_absolute_value);
    def("g_cube_square_root", g_cube_square_root);
}
