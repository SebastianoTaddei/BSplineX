"""
Python setup file for the BSplineX C++ library. It uses the SWIG interface to
generate the Python bindings for the C++ library.

Authors:
 - Sebastiano Taddei
 - Gabriele Masina
"""

import os
from setuptools import setup, Extension

dirname = os.path.dirname(os.path.abspath(__file__))

# Define the include directories
include_dirs = [
    os.path.join(dirname, *[".."] * 2, "include"),
    os.path.join(dirname, *[".."] * 2, "third_party", "eigen", "include", "eigen3"),
]

bsplinex_module = Extension(
    "_BSplineX",
    sources=["BSplineX.i"],
    include_dirs=include_dirs,
    library_dirs=[],
    libraries=[],
    swig_opts=["-c++", f"-I{include_dirs[0]}"],
    extra_compile_args=["-std=c++17"],
)

setup(
    ext_modules=[bsplinex_module],
    py_modules=["BSplineX"],
    package_dir={"": "."},
)
