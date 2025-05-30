/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

#include "infos/eevee_tracing_info.hh"

COMPUTE_SHADER_CREATE_INFO(eevee_horizon_scan)

#include "draw_view_lib.glsl"
#include "eevee_horizon_scan_eval_lib.glsl"
#include "eevee_sampling_lib.glsl"
#include "gpu_shader_math_vector_lib.glsl"
#include "gpu_shader_utildefines_lib.glsl"

void main()
{
  const uint tile_size = RAYTRACE_GROUP_SIZE;
  uvec2 tile_coord = unpackUvec2x16(tiles_coord_buf[gl_WorkGroupID.x]);
  ivec2 texel = ivec2(gl_LocalInvocationID.xy + tile_coord * tile_size);

  ivec2 texel_fullres = texel * uniform_buf.raytrace.horizon_resolution_scale +
                        uniform_buf.raytrace.horizon_resolution_bias;

  /* Avoid tracing the outside border if dispatch is too big. */
  ivec2 extent = textureSize(gbuf_header_tx, 0).xy;
  if (any(greaterThanEqual(texel * uniform_buf.raytrace.horizon_resolution_scale, extent))) {
    return;
  }

  /* Avoid loading texels outside texture range.
   * This can happen even after the check above in non-power-of-2 textures. */
  texel_fullres = min(texel_fullres, extent - 1);

  /* Do not trace where nothing was rendered. */
  if (texelFetch(gbuf_header_tx, ivec3(texel_fullres, 0), 0).r == 0u) {
#if 0 /* This is not needed as the next stage doesn't do bilinear filtering. */
    imageStore(horizon_radiance_0_img, texel, vec4(0.0));
    imageStore(horizon_radiance_1_img, texel, vec4(0.0));
    imageStore(horizon_radiance_2_img, texel, vec4(0.0));
    imageStore(horizon_radiance_3_img, texel, vec4(0.0));
#endif
    return;
  }

  vec2 uv = (vec2(texel_fullres) + 0.5) * uniform_buf.raytrace.full_resolution_inv;
  float depth = texelFetch(hiz_tx, texel_fullres, 0).r;
  vec3 vP = drw_point_screen_to_view(vec3(uv, depth));
  vec3 vN = horizon_scan_sample_normal(uv);

  vec4 noise = utility_tx_fetch(utility_tx, vec2(texel), UTIL_BLUE_NOISE_LAYER);
  noise = fract(noise + sampling_rng_3D_get(SAMPLING_AO_U).xyzx);

  HorizonScanResult scan = horizon_scan_eval(vP,
                                             vN,
                                             noise,
                                             uniform_buf.ao.pixel_size,
                                             uniform_buf.ao.gi_distance,
                                             uniform_buf.ao.thickness_near,
                                             uniform_buf.ao.thickness_far,
                                             uniform_buf.ao.angle_bias,
                                             fast_gi_slice_count,
                                             fast_gi_step_count,
                                             false,
                                             fast_gi_ao_only);

  scan.result = spherical_harmonics_compress(scan.result);

  imageStore(horizon_radiance_0_img, texel, scan.result.L0.M0);
  imageStore(horizon_radiance_1_img, texel, scan.result.L1.Mn1);
  imageStore(horizon_radiance_2_img, texel, scan.result.L1.M0);
  imageStore(horizon_radiance_3_img, texel, scan.result.L1.Mp1);
}
