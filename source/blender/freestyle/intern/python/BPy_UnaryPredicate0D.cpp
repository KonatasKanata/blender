/* SPDX-FileCopyrightText: 2004-2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup freestyle
 */

#include "BPy_UnaryPredicate0D.h"

#include "BPy_Convert.h"
#include "Iterator/BPy_Interface0DIterator.h"
#include "UnaryPredicate0D/BPy_FalseUP0D.h"
#include "UnaryPredicate0D/BPy_TrueUP0D.h"

using namespace Freestyle;

///////////////////////////////////////////////////////////////////////////////////////////

//-------------------MODULE INITIALIZATION--------------------------------
int UnaryPredicate0D_Init(PyObject *module)
{
  if (module == nullptr) {
    return -1;
  }

  if (PyType_Ready(&UnaryPredicate0D_Type) < 0) {
    return -1;
  }
  PyModule_AddObjectRef(module, "UnaryPredicate0D", (PyObject *)&UnaryPredicate0D_Type);

  if (PyType_Ready(&FalseUP0D_Type) < 0) {
    return -1;
  }
  PyModule_AddObjectRef(module, "FalseUP0D", (PyObject *)&FalseUP0D_Type);

  if (PyType_Ready(&TrueUP0D_Type) < 0) {
    return -1;
  }
  PyModule_AddObjectRef(module, "TrueUP0D", (PyObject *)&TrueUP0D_Type);

  return 0;
}

//------------------------INSTANCE METHODS ----------------------------------

PyDoc_STRVAR(
    /* Wrap. */
    UnaryPredicate0D___doc__,
    "Base class for unary predicates that work on\n"
    ":class:`Interface0DIterator`. A UnaryPredicate0D is a functor that\n"
    "evaluates a condition on an Interface0DIterator and returns true or\n"
    "false depending on whether this condition is satisfied or not. The\n"
    "UnaryPredicate0D is used by invoking its __call__() method. Any\n"
    "inherited class must overload the __call__() method.\n"
    "\n"
    ".. method:: __init__()\n"
    "\n"
    "   Default constructor.\n"
    "\n"
    ".. method:: __call__(it)\n"
    "\n"
    "   Must be overload by inherited classes.\n"
    "\n"
    "   :arg it: The Interface0DIterator pointing onto the Interface0D at\n"
    "      which we wish to evaluate the predicate.\n"
    "   :type it: :class:`Interface0DIterator`\n"
    "   :return: True if the condition is satisfied, false otherwise.\n"
    "   :rtype: bool\n");

static int UnaryPredicate0D___init__(BPy_UnaryPredicate0D *self, PyObject *args, PyObject *kwds)
{
  static const char *kwlist[] = {nullptr};

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "", (char **)kwlist)) {
    return -1;
  }
  self->up0D = new UnaryPredicate0D();
  self->up0D->py_up0D = (PyObject *)self;
  return 0;
}

static void UnaryPredicate0D___dealloc__(BPy_UnaryPredicate0D *self)
{
  delete self->up0D;
  Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *UnaryPredicate0D___repr__(BPy_UnaryPredicate0D *self)
{
  return PyUnicode_FromFormat("type: %s - address: %p", Py_TYPE(self)->tp_name, self->up0D);
}

static PyObject *UnaryPredicate0D___call__(BPy_UnaryPredicate0D *self,
                                           PyObject *args,
                                           PyObject *kwds)
{
  static const char *kwlist[] = {"it", nullptr};
  PyObject *py_if0D_it;

  if (!PyArg_ParseTupleAndKeywords(
          args, kwds, "O!", (char **)kwlist, &Interface0DIterator_Type, &py_if0D_it))
  {
    return nullptr;
  }

  Interface0DIterator *if0D_it = ((BPy_Interface0DIterator *)py_if0D_it)->if0D_it;

  if (!if0D_it) {
    string class_name(Py_TYPE(self)->tp_name);
    PyErr_SetString(PyExc_RuntimeError, (class_name + " has no Interface0DIterator").c_str());
    return nullptr;
  }
  if (typeid(*(self->up0D)) == typeid(UnaryPredicate0D)) {
    PyErr_SetString(PyExc_TypeError, "__call__ method not properly overridden");
    return nullptr;
  }
  if (self->up0D->operator()(*if0D_it) < 0) {
    if (!PyErr_Occurred()) {
      string class_name(Py_TYPE(self)->tp_name);
      PyErr_SetString(PyExc_RuntimeError, (class_name + " __call__ method failed").c_str());
    }
    return nullptr;
  }
  return PyBool_from_bool(self->up0D->result);
}

/*----------------------UnaryPredicate0D get/setters ----------------------------*/

PyDoc_STRVAR(
    /* Wrap. */
    UnaryPredicate0D_name_doc,
    "The name of the unary 0D predicate.\n"
    "\n"
    ":type: str");

static PyObject *UnaryPredicate0D_name_get(BPy_UnaryPredicate0D *self, void * /*closure*/)
{
  return PyUnicode_FromString(Py_TYPE(self)->tp_name);
}

static PyGetSetDef BPy_UnaryPredicate0D_getseters[] = {
    {"name",
     (getter)UnaryPredicate0D_name_get,
     (setter) nullptr,
     UnaryPredicate0D_name_doc,
     nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr} /* Sentinel */
};

/*-----------------------BPy_UnaryPredicate0D type definition ------------------------------*/

PyTypeObject UnaryPredicate0D_Type = {
    /*ob_base*/ PyVarObject_HEAD_INIT(nullptr, 0)
    /*tp_name*/ "UnaryPredicate0D",
    /*tp_basicsize*/ sizeof(BPy_UnaryPredicate0D),
    /*tp_itemsize*/ 0,
    /*tp_dealloc*/ (destructor)UnaryPredicate0D___dealloc__,
    /*tp_vectorcall_offset*/ 0,
    /*tp_getattr*/ nullptr,
    /*tp_setattr*/ nullptr,
    /*tp_as_async*/ nullptr,
    /*tp_repr*/ (reprfunc)UnaryPredicate0D___repr__,
    /*tp_as_number*/ nullptr,
    /*tp_as_sequence*/ nullptr,
    /*tp_as_mapping*/ nullptr,
    /*tp_hash*/ nullptr,
    /*tp_call*/ (ternaryfunc)UnaryPredicate0D___call__,
    /*tp_str*/ nullptr,
    /*tp_getattro*/ nullptr,
    /*tp_setattro*/ nullptr,
    /*tp_as_buffer*/ nullptr,
    /*tp_flags*/ Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    /*tp_doc*/ UnaryPredicate0D___doc__,
    /*tp_traverse*/ nullptr,
    /*tp_clear*/ nullptr,
    /*tp_richcompare*/ nullptr,
    /*tp_weaklistoffset*/ 0,
    /*tp_iter*/ nullptr,
    /*tp_iternext*/ nullptr,
    /*tp_methods*/ nullptr,
    /*tp_members*/ nullptr,
    /*tp_getset*/ BPy_UnaryPredicate0D_getseters,
    /*tp_base*/ nullptr,
    /*tp_dict*/ nullptr,
    /*tp_descr_get*/ nullptr,
    /*tp_descr_set*/ nullptr,
    /*tp_dictoffset*/ 0,
    /*tp_init*/ (initproc)UnaryPredicate0D___init__,
    /*tp_alloc*/ nullptr,
    /*tp_new*/ PyType_GenericNew,
};

///////////////////////////////////////////////////////////////////////////////////////////
