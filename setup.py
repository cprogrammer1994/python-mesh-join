from setuptools import Extension, setup

ext = Extension(
    name='mesh_join',
    sources=['mesh_join.cpp'],
)

setup(
    name='mesh_join',
    version='0.1.0',
    ext_modules=[ext],
)
