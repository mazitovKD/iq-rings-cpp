from setuptools import setup
from Cython.Build import cythonize

setup(
    ext_modules=cythonize("py/exercise_wrapper.pyx"),
    language_level=3
)