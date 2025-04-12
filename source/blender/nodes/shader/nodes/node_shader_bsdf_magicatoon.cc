/* SPDX-FileCopyrightText: 2005 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

#include "node_shader_util.hh"

namespace blender::nodes::node_shader_bsdf_magicatoon_cc {

static void node_declare(NodeDeclarationBuilder &b)
{
  b.add_input<decl::Color>("Color").default_value({0.8f, 0.8f, 0.8f, 1.0f});
  b.add_input<decl::Float>("NormalSmoothness")
      .default_value(1.0f)
      .min(0.0f)
      .max(1.0f)
      .subtype(PROP_FACTOR);
  b.add_input<decl::Vector>("Normal").hide_value();
  b.add_input<decl::Float>("Weight").available(false);
  b.add_output<decl::Shader>("BSDF");
}

static int node_shader_gpu_bsdf_toon_magicatoon(GPUMaterial *mat,
                                                bNode *node,
                                                bNodeExecData * /*execdata*/,
                                                GPUNodeStack *in,
                                                GPUNodeStack *out)
{
  if (!in[2].link) {
    GPU_link(mat, "world_normals_get", &in[2].link);
  }

  GPU_material_flag_set(mat, GPU_MATFLAG_MAGICATOON);

  return GPU_stack_link(mat, node, "node_bsdf_magicatoon", in, out);
}

NODE_SHADER_MATERIALX_BEGIN
#ifdef WITH_MATERIALX
{
  if (to_type_ != NodeItem::Type::BSDF) {
    return empty();
  }

  NodeItem color = get_input_value("Color", NodeItem::Type::Color3);
  NodeItem normalsmoothness = get_input_value("NormalSmoothness", NodeItem::Type::Float);
  NodeItem normal = get_input_link("Normal", NodeItem::Type::Vector3);

  return create_node("oren_nayar_magicatoon_bsdf",
                     NodeItem::Type::BSDF,
                     {{"color", color}, {"normalsmoothness", normalsmoothness}, {"normal", normal}});
}
#endif
NODE_SHADER_MATERIALX_END

}  // namespace blender::nodes::node_shader_bsdf_magicatoon_cc

/* node type definition */
void register_node_type_sh_bsdf_magicatoon()
{
  namespace file_ns = blender::nodes::node_shader_bsdf_magicatoon_cc;

  static blender::bke::bNodeType ntype;

  sh_node_type_base(&ntype, "ShaderNodeBsdfMagicaToon", SH_NODE_BSDF_MAGICATOON);
  ntype.ui_name = "MagicaToon BSDF";
  ntype.ui_description = "MagicaToon BSDF";
  ntype.enum_name_legacy = "BSDF_MAGICATOON";
  ntype.nclass = NODE_CLASS_SHADER;
  ntype.declare = file_ns::node_declare;
  ntype.add_ui_poll = object_shader_nodes_poll;
  blender::bke::node_type_size_preset(&ntype, blender::bke::eNodeSizePreset::Middle);
  ntype.gpu_fn = file_ns::node_shader_gpu_bsdf_toon_magicatoon;
  ntype.materialx_fn = file_ns::node_shader_materialx;

  blender::bke::node_register_type(&ntype);
}
