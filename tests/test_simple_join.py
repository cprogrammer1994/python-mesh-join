import numpy as np
import pytest

import mesh_join


def test_simple_join():
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

    np.testing.assert_equal(vertices, [
        [0.0, 0.0, 1.0],
        [0.0, 0.0, 2.0],
        [0.0, 0.0, 3.0],
        [0.0, 0.0, 4.0],
        [0.0, 0.0, 5.0],
        [0.0, 0.0, 6.0],
    ])

    np.testing.assert_equal(index, [0, 1, 2, 3, 4, 5])


def test_join_with_restart_index():
    vertices1 = np.array([
        [0.0, 0.0, 1.0],
        [0.0, 0.0, 2.0],
        [0.0, 0.0, 3.0],
        [0.0, 0.0, 4.0],
    ], dtype='f4')

    vertices2 = np.array([
        [0.0, 0.0, 5.0],
        [0.0, 0.0, 6.0],
    ], dtype='f4')

    index1 = np.array([0, 1, 2, -1, 2, 3], dtype='i4')
    index2 = np.array([0, 1], dtype='i4')

    vdata, idata = mesh_join.join([vertices1, vertices2], [index1, index2], stride=12, separator=True)

    vertices = np.frombuffer(vdata, 'f4').reshape(-1, 3)
    index = np.frombuffer(idata, 'i4')

    np.testing.assert_equal(vertices, [
        [0.0, 0.0, 1.0],
        [0.0, 0.0, 2.0],
        [0.0, 0.0, 3.0],
        [0.0, 0.0, 4.0],
        [0.0, 0.0, 5.0],
        [0.0, 0.0, 6.0],
    ])

    np.testing.assert_equal(index, [0, 1, 2, -1, 2, 3, -1, 4, 5])


if __name__ == '__main__':
    pytest.main([__file__])
