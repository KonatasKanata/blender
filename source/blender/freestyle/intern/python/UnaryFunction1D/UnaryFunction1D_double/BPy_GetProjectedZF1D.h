/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup freestyle
 */

#pragma once

#include "../BPy_UnaryFunction1DDouble.h"

///////////////////////////////////////////////////////////////////////////////////////////

extern PyTypeObject GetProjectedZF1D_Type;

#define BPy_GetProjectedZF1D_Check(v) \
  (PyObject_IsInstance((PyObject *)v, (PyObject *)&GetProjectedZF1D_Type))

/*---------------------------Python BPy_GetProjectedZF1D structure definition----------*/
typedef struct {
  BPy_UnaryFunction1DDouble py_uf1D_double;
} BPy_GetProjectedZF1D;

///////////////////////////////////////////////////////////////////////////////////////////
