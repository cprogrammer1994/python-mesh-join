#define PY_SSIZE_T_CLEAN
#include <Python.h>

PyObject * meth_join(PyObject * self, PyObject * args, PyObject * kwargs) {
    static char * keywords[] = {"mesh", "index", "stride", NULL};

    PyObject * meshes;
    PyObject * indexes;
    int stride;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OOi", keywords, &meshes, &indexes, &stride)) {
        return 0;
    }

    meshes = PySequence_Fast(meshes, "not iterable");
    if (!meshes) {
        return 0;
    }

    indexes = PySequence_Fast(indexes, "not iterable");
    if (!indexes) {
        return 0;
    }

    int num_meshes = (int)PySequence_Fast_GET_SIZE(meshes);
    int num_indexes = (int)PySequence_Fast_GET_SIZE(indexes);

    if (num_meshes != num_indexes) {
        PyErr_Format(PyExc_ValueError, "wrong number of indexes");
        return 0;
    }

    PyObject ** mesh_items = PySequence_Fast_ITEMS(meshes);
    PyObject ** index_items = PySequence_Fast_ITEMS(indexes);

    Py_buffer * mesh_views = (Py_buffer *)malloc(sizeof(Py_buffer) * num_meshes  * 2);
    Py_buffer * index_views = mesh_views + num_meshes;

    int total_mesh_size = 0;
    int total_index_size = 0;

    for (int i = 0; i < num_meshes; ++i) {
        if (PyObject_GetBuffer(mesh_items[i], &mesh_views[i], PyBUF_STRIDED_RO)) {
            return 0;
        }
        if (PyObject_GetBuffer(index_items[i], &index_views[i], PyBUF_SIMPLE)) {
            return 0;
        }
        total_mesh_size += (int)mesh_views[i].len;
        total_index_size += (int)index_views[i].len;
    }

    PyObject * final_mesh = PyBytes_FromStringAndSize(NULL, total_mesh_size);
    PyObject * final_index = PyBytes_FromStringAndSize(NULL, total_index_size);

    char * mesh_ptr = PyBytes_AS_STRING(final_mesh);
    int * index_ptr = (int *)PyBytes_AS_STRING(final_index);

    int base_index = 0;

    for (int i = 0; i < num_meshes; ++i) {
        PyBuffer_ToContiguous(mesh_ptr, &mesh_views[i], mesh_views[i].len, 'C');
        mesh_ptr += mesh_views[i].len;

        int count = (int)(index_views[i].len / sizeof(int));
        int * index_array = (int *)index_views[i].buf;
        for (int j = 0; j < count; ++j) {
            *index_ptr++ = index_array[j] != -1 ? base_index + index_array[j] : -1;
        }

        base_index += (int)(mesh_views[i].len / stride);
        PyBuffer_Release(&mesh_views[i]);
        PyBuffer_Release(&index_views[i]);
    }

    free(mesh_views);
    Py_DECREF(indexes);
    Py_DECREF(meshes);

    PyObject * res = PyTuple_New(2);
    PyTuple_SET_ITEM(res, 0, final_mesh);
    PyTuple_SET_ITEM(res, 1, final_index);
    return res;
}

PyMethodDef module_methods[] = {
    {"join", (PyCFunction)meth_join, METH_VARARGS | METH_KEYWORDS, 0},
    {0},
};

PyModuleDef module_def = {PyModuleDef_HEAD_INIT, "mesh_join", 0, -1, module_methods, 0, 0, 0, 0};

extern "C" PyObject * PyInit_mesh_join() {
    PyObject * module = PyModule_Create(&module_def);
    return module;
}
