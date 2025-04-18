/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

/** \file
 * \ingroup pythonintern
 */

#include <Python.h>

void bpy_intern_string_init();
void bpy_intern_string_exit();

extern PyObject *bpy_intern_str___annotations__;
extern PyObject *bpy_intern_str___doc__;
extern PyObject *bpy_intern_str___main__;
extern PyObject *bpy_intern_str___module__;
extern PyObject *bpy_intern_str___name__;
extern PyObject *bpy_intern_str___slots__;
extern PyObject *bpy_intern_str_attr;
extern PyObject *bpy_intern_str_bl_property;
extern PyObject *bpy_intern_str_bl_rna;
extern PyObject *bpy_intern_str_bl_target_properties;
extern PyObject *bpy_intern_str_bpy_types;
extern PyObject *bpy_intern_str_frame;
extern PyObject *bpy_intern_str_properties;
extern PyObject *bpy_intern_str_register;
extern PyObject *bpy_intern_str_self;
extern PyObject *bpy_intern_str_depsgraph;
extern PyObject *bpy_intern_str_unregister;
