#include <algorithm>
#include <vector>

#include <matplot/matplot.h>

#include "bsplinex.h"

int main()
{
  int p     = 4;
  int k     = 0;
  int n_pts = 100;

  std::vector<double> t = {0, 1, 2, 3, 4, 5};
  std::vector<double> c = {0, 0, 0, 0, 1, 0, 0, 0, 0};
  std::vector<double> x = matplot::linspace(0, 5, n_pts);
  std::vector<double> y(x.size());
  std::vector<double> dy(x.size());

  bsplinex::BSpline<double> bspline{};
  bspline.setup(p, t, c);

  for (int i = 0; i < x.size(); i++)
  {
    y[i]  = bspline.evaluate(x[i]);
    dy[i] = bspline.evaluate(x[i], 1);
    printf("x: %f, y: %f, dy: %f\n", x[i], y[i], dy[i]);
  }

  // Plot the curve
  matplot::plot(x, y);
  matplot::hold(matplot::on);
  matplot::plot(x, dy);
  matplot::grid(true);
  matplot::axis(matplot::equal);
  matplot::show();

  return 0;
}
