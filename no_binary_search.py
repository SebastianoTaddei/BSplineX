"""

If we ha a cardinal B-Spline, meaning that the knots are equidistant, we do not
need to use the binary search to find the interval in which a given point lies.

Given that equidistant knots simply follow a linear progression

value = step_size * index + start

we can simply find the index by inverting the line and rounding down

index = floor((value - start) / step_size)

Let us try it out.

"""

from numpy import linspace
from math import floor

arr = linspace(1.2, 18.298, 25)
step_size = arr[1] - arr[0]
start = arr[0]

print(f"Array: {arr}")
print(f"Start: {start:6.3f}, Step size: {step_size:6.3f}")
for value in linspace(arr[0] + 1, arr[-1] - 1, 10):
    index = floor((value - start) / step_size)
    print(
        f"Value: {value:6.3f}, Index: {index:2d}, Interval: [{arr[index]:6.3f}, {arr[index+1]:6.3f}]"
    )
