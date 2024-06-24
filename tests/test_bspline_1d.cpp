#include <vector>

#include <matplot/matplot.h>

#include "bsplinex.h"

int main()
{
  int p     = 4;
  int k     = 0;
  int n_pts = 100;

  std::vector<double> t = {0, 1, 2, 3, 4, 5};
  std::vector<double> c = {0, 1, 2, 1, 0};
  std::vector<double> x = matplot::linspace(0, 4.99, n_pts);
  std::vector<double> y(x.size());
  std::vector<double> dy(x.size());
  std::vector<double> ddy(x.size());

  bsplinex::BSpline<double> bspline{};
  bspline.setup(p, t, c, bsplinex::BoundaryCondition::PERIODIC);

  for (int i = 0; i < x.size(); i++)
  {
    y[i]   = bspline.evaluate(x[i]);
    dy[i]  = bspline.evaluate(x[i], 1);
    ddy[i] = bspline.evaluate(x[i], 2);
  }

  // Plot the curve
  matplot::plot(x, y);
  matplot::hold(matplot::on);
  matplot::plot(x, dy);
  matplot::plot(x, ddy);
  matplot::grid(true);
  matplot::axis(matplot::equal);
  matplot::show();

  return 0;
}
