# python-mesh-join

A simple module to join indexed meshes.

## Install

This module is **not available through pip**, it was designed to be forked and extended. Simplicity is a key goal in design. This module is using the Python's [c-api](https://docs.python.org/3/c-api/). The implementation can be found in the [mesh_join.cpp](mesh_join.cpp).

```
git clone https://github.com/cprogrammer1994/python-mesh-join
cd python-mesh-join
python setup.py develop
```

## Cheat Sheet

```py
import mesh_join

vertices1 = np.array([
    [0.0, 0.0, 1.0],
    [0.0, 0.0, 2.0],
    [0.0, 0.0, 3.0],
], dtype='f4')

vertices2 = np.array([
    [0.0, 0.0, 4.0],
    [0.0, 0.0, 5.0],
    [0.0, 0.0, 6.0],
], dtype='f4')

index1 = np.array([0, 1, 2], dtype='i4')
index2 = np.array([0, 1, 2], dtype='i4')

vdata, idata = mesh_join.join([vertices1, vertices2], [index1, index2], stride=12)

vertices = np.frombuffer(vdata, 'f4').reshape(-1, 3)
index = np.frombuffer(idata, 'i4')
```
