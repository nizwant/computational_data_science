import setuptools
import numpy

setuptools.setup(
    name="pypackage",
    packages=setuptools.find_packages(),
    include_dirs=[numpy.get_include()],
    ext_modules=[
        setuptools.Extension(
            "pypackage.cmodule",
            sources=["src/cmodule.c"]
        )
    ]
)
