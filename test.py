import matplotlib.pyplot as plt
import numpy as np

# Set a random generator
# np.random.seed(0)


def deBoor(k: int, x: int, t, c, p: int):
    """Evaluates S(x).

    Arguments
    ---------
    k: Index of knot interval that contains x.
    x: Position.
    t: Array of knot positions, needs to be padded as described above.
    c: Array of control points.
    p: Degree of B-spline.
    """
    d = [c[j + k - p] for j in range(0, p + 1)]

    for r in range(1, p + 1):
        for j in range(p, r - 1, -1):
            alpha = (x - t[j + k - p]) / (t[j + 1 + k - r] - t[j + k - p])
            d[j] = (1.0 - alpha) * d[j - 1] + alpha * d[j]

    return d[p]


# Create a small class that extends the list class to be circular
class CircularList(list):
    def __getitem__(self, index):
        return super(CircularList, self).__getitem__(index % len(self))

    def __setitem__(self, index, value):
        super(CircularList, self).__setitem__(index % len(self), value)

    def __delitem__(self, index):
        raise NotImplementedError

    def insert(self, index, value):
        raise NotImplementedError


# Function to randomly generate a knot vector
def generate_knot_vector(number_of_knots, degree, max_value=10, min_value=0):
    # Start creating a random knot vector
    knots = np.sort(np.random.uniform(min_value, max_value, number_of_knots))

    # Compute the period of the knot vector
    period = np.abs(knots[-1] - knots[0])

    # Compute the domain of the knot vector
    domain = [knots[0], knots[-1]]

    # Compute the difference between each knot
    diff = np.diff(knots)

    # Add degree knots at the beginning and end periodically
    for i in range(degree):
        knots = np.insert(knots, 0, knots[0] - diff[-1 - i])
        knots = np.append(knots, knots[-1] + diff[i])

    return knots, period, domain


# Function to randomly generate a control point vector
def generate_control_points(number_of_knots, degree, max_value=10, min_value=0):
    # Generate random control points
    control_points = np.random.uniform(min_value, max_value, number_of_knots - 1)

    # Add the first degree control points at the end
    control_points = np.append(control_points, control_points[:degree])

    return control_points


# Function to evaluate the B-Spline
def evaluate_bspline(x, knots, control_points, degree, domain, period):
    # Evaluate
    y_eval = []
    for val in x:
        if val >= domain[-1]:
            val = val - period * (np.floor(np.abs(val - domain[-1]) / period) + 1)
        elif val < domain[0]:
            val = val + period * (np.floor(np.abs(val - domain[0]) / period) + 1)
        k = np.searchsorted(knots, val, side="right") - 1
        y_eval.append(deBoor(k, val, knots, control_points, degree))

    return y_eval


p = 3
n = 20
max_value = 12
min_value = -2
knots, period, domain = generate_knot_vector(n, p, max_value, min_value)
ctrl_pts = generate_control_points(n, p, max_value, min_value)

x_eval = np.linspace(domain[0] - 10, domain[-1] + 10, 5000)
y_eval = evaluate_bspline(x_eval, knots, ctrl_pts, p, domain, period)

plt.plot(x_eval, y_eval, label="Closed")
plt.legend()
plt.grid()
plt.show()

"""
From what I understand, to build a closed BSpline you take the original knot
vector, extend it periodically with p-knots at the beginning and end, then the
number of free control points is the number of original knots minus 1. Finally,
add repeat the first p-control points at the end. This gives you:
- len(t_original) = m
- len(t_closed) = m + 2 * p
- len(c_free) = m - 1
- len(c_closed) = m - 1 + p
Which satisfies the condition m = n + p + 1
-> m + 2 * p = m - 1 + p + p + 1
-> m = m
"""
