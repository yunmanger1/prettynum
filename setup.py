from distutils.core import setup, Extension

setup(name = "prettynum", version = "0.0",
    ext_modules = [Extension("prettynum", ["prettynum.c"])])
