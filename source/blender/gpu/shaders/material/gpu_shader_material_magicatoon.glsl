/* SPDX-FileCopyrightText: 2019-2022 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */


 #ifndef VOLUMETRICS

void node_bsdf_magicatoon(vec4 color, float normalsmoothness, vec3 N, float weight, out Closure result)
{
/*
  vec3 vP;
  point_transform_world_to_view(g_data.P, vP);
  vP.z = -vP.z;
  float outdepth = abs(vP.z);
  float outdist = length(vP);
  vec3  outview = normalize(vP);
  */
  vec3 V = coordinate_incoming(g_data.P);
  float NV = dot(N, V);

  float is_camera_ray = float(g_data.ray_type == RAY_TYPE_CAMERA);
  float is_shadow_ray = float(g_data.ray_type == RAY_TYPE_SHADOW);
  float is_diffuse_ray = float(g_data.ray_type == RAY_TYPE_DIFFUSE);
  float is_glossy_ray = float(g_data.ray_type == RAY_TYPE_GLOSSY);
  /* Kind of supported. */
  /*
  is_singular_ray = is_glossy_ray;
  is_reflection_ray = is_glossy_ray;
  is_transmission_ray = is_glossy_ray;
  ray_depth = g_data.ray_depth;
  diffuse_depth = (is_diffuse_ray == 1.0) ? g_data.ray_depth : 0.0;
  glossy_depth = (is_glossy_ray == 1.0) ? g_data.ray_depth : 0.0;
  transmission_depth = (is_transmission_ray == 1.0) ? glossy_depth : 0.0;
  */
  float  ray_length = g_data.ray_length;

  /*
  ClosureDiffuse diffuse_data;
  diffuse_data.weight = weight;
  // diffuse_data.color =  clamp(color.rgb + normalsmoothness * vec3(0.5, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1));
  diffuse_data.color =  saturate(color.rgb + normalsmoothness * vec3(NV , 0, 0));
  diffuse_data.N = safe_normalize(N);
  */
  
  ClosureMagicaToon diffuse_data;
  diffuse_data.weight = weight;
  // diffuse_data.color =  clamp(color.rgb + normalsmoothness * vec3(0.5, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1));
  diffuse_data.color =  saturate(color.rgb + normalsmoothness * vec3(NV , 0, 0));
  diffuse_data.N = safe_normalize(N);

  result = closure_eval(diffuse_data);
}

#endif
