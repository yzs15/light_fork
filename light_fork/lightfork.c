#include <Python.h>

extern int ipdos_set_mem_file_name(const char* filename);
extern int ipdos_remove_parent_mem_file();

// C函数，实现set_mem_file_name功能
static PyObject* set_mem_file_name(PyObject* self, PyObject* args) {
    const char* filename;

    // 解析从Python传来的字符串参数
    if (!PyArg_ParseTuple(args, "s", &filename)) {
        return NULL;
    }

    ipdos_set_mem_file_name(filename);

    // 返回Python的None
    Py_RETURN_NONE;
}


// C函数，实现remove_mem_file
static PyObject* remove_mem_file(PyObject* self, PyObject* args) {
    

    ipdos_remove_parent_mem_file();

    // 返回Python的None
    Py_RETURN_NONE;
}

// 方法表
static PyMethodDef LightforkMethods[] = {
    {"set_mem_file_name", set_mem_file_name, METH_VARARGS, "Set the name of the memory file."},
    {"remove_mem_file", remove_mem_file, METH_NOARGS, "Remove the memory file of parent."},
    {NULL, NULL, 0, NULL} // 结尾标记
};

// 模块定义
static struct PyModuleDef lightforkmodule = {
    PyModuleDef_HEAD_INIT,
    "lightfork", // 模块名
    NULL, // 模块文档，可设为NULL
    -1, // 模块保持状态在全局变量中
    LightforkMethods
};

// 初始化函数，Python在导入这个模块时调用
PyMODINIT_FUNC PyInit_lightfork(void) {
    return PyModule_Create(&lightforkmodule);
}
