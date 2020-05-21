#!/usr/bin/env python3

"""The setup script."""

import pathlib
from setuptools import setup, find_packages

here = pathlib.Path(__file__).parent

with open(here / "aerotech_101smc2" / "VERSION") as version_file:
    version = version_file.read().strip()


with open("README.md") as readme_file:
    readme = readme_file.read()


requirements = ["yaqd-core"]

extra_requirements = {"dev": ["black", "pre-commit"]}
extra_files = {"aerotech_101smc2": ["VERSION"]}

setup(
    author="kameyer226b",
    author_email="kentmeyer@wisc.edu",
    python_requires=">=3.7",
    classifiers=[
        "Development Status :: 2 - Pre-Alpha",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: GNU Lesser General Public License v3 (LGPLv3)",
        "Natural Language :: English",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Topic :: Scientific/Engineering",
    ],
    description="Daemon for the Aerotech 101SMC2EN translation stage",
    entry_points={
        "console_scripts": [
            "yaqd-aerotech-101smc2=aerotech_101smc2._aerotech:Aerotech.main",
        ],
    },
    install_requires=requirements,
    extras_require=extra_requirements,
    license="GNU Lesser General Public License v3 (LGPL)",
    long_description=readme,
    long_description_content_type="text/markdown",
    include_package_data=True,
    package_data=extra_files,
    keywords="aerotech-101smc2",
    name="aerotech-101smc2",
    packages=find_packages(include=["aerotech_101smc2", "aerotech_101smc2.*"]),
    url="https://gitlab.com/kameyer226b/aerotech-101smc2",
    version=version,
    zip_safe=False,
)
