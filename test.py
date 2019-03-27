import numpy as np

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

print(np.frombuffer(vdata, 'f4').reshape(-1, 3))
print(np.frombuffer(idata, 'i4'))
