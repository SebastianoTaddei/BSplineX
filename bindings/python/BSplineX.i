%module BSplineX

// Injected C++ code
%{
#include "bspline/bspline.hpp"
#include "bspline/bspline_types.hpp"
#include "bspline/bspline_factory.hpp"
using namespace bsplinex;
%}

%include "std_pair.i"
%include "std_vector.i"
%template() std::pair<double, double>;
%template() std::vector<double>;


%include "bspline/bspline.hpp"
%include "bspline/bspline_factory.hpp"
%include "bspline/bspline_types.hpp"
%include "knots/t_data.hpp"
%include "control_points/c_data.hpp"

using namespace bsplinex;
%template(KnotsData) knots::Data<double, Curve::UNIFORM>;
%template(CtrlData) control_points::Data<double>;
%template(ciao) bspline::BSpline<double, Curve::UNIFORM, BoundaryCondition::OPEN, Extrapolation::NONE>;
%template(OpenUniform) types::OpenUniform<double>;
%template(open_uniform) factory::open_uniform<double>;

