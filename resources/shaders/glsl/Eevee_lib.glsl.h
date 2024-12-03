// /* SPDX-FileCopyrightText: 2022-2023 Blender Authors
//  *
//  * SPDX-License-Identifier: GPL-2.0-or-later */

// /* Simplified form of F_eta(eta, 1.0). */
// float F0_from_ior(float eta)
// {
//   float A = (eta - 1.0) / (eta + 1.0);
//   return A * A;
// }

// /* Return the fresnel color from a precomputed LUT value (from brdf_lut). */
// vec3 F_brdf_single_scatter(vec3 f0, vec3 f90, vec2 lut)
// {
//   return f0 * lut.x + f90 * lut.y;
// }

// /* Multi-scattering brdf approximation from
//  * "A Multiple-Scattering Microfacet Model for Real-Time Image-based Lighting"
//  * https://jcgt.org/published/0008/01/03/paper.pdf by Carmelo J. Fdez-Agüera. */
// vec3 F_brdf_multi_scatter(vec3 f0, vec3 f90, vec2 lut)
// {
//   vec3 FssEss = F_brdf_single_scatter(f0, f90, lut);

//   float Ess = lut.x + lut.y;
//   float Ems = 1.0 - Ess;
//   vec3 Favg = f0 + (f90 - f0) / 21.0;

//   /* The original paper uses `FssEss * radiance + Fms*Ems * irradiance`, but
//    * "A Journey Through Implementing Multi-scattering BRDFs and Area Lights" by Steve McAuley
//    * suggests to use `FssEss * radiance + Fms*Ems * radiance` which results in comparable quality.
//    * We handle `radiance` outside of this function, so the result simplifies to:
//    * `FssEss + Fms*Ems = FssEss * (1 + Ems*Favg / (1 - Ems*Favg)) = FssEss / (1 - Ems*Favg)`.
//    * This is a simple albedo scaling very similar to the approach used by Cycles:
//    * "Practical multiple scattering compensation for microfacet model". */
//   return FssEss / (1.0 - Ems * Favg);
// }

// vec2 brdf_lut(float cos_theta, float roughness)
// {
// #ifdef EEVEE_UTILITY_TX
//   return utility_tx_sample_lut(utility_tx, cos_theta, roughness, UTIL_BSDF_LAYER).rg;
// #else
//   return vec2(1.0, 0.0);
// #endif
// }

// void brdf_f82_tint_lut(vec3 F0,
//                        vec3 F82,
//                        float cos_theta,
//                        float roughness,
//                        bool do_multiscatter,
//                        out vec3 reflectance)
// {
// #ifdef EEVEE_UTILITY_TX
//   vec3 split_sum = utility_tx_sample_lut(utility_tx, cos_theta, roughness, UTIL_BSDF_LAYER).rgb;
// #else
//   vec3 split_sum = vec3(1.0, 0.0, 0.0);
// #endif

//   reflectance = do_multiscatter ? F_brdf_multi_scatter(F0, vec3(1.0), split_sum.xy) :
//                                   F_brdf_single_scatter(F0, vec3(1.0), split_sum.xy);

//   /* Precompute the F82 term factor for the Fresnel model.
//    * In the classic F82 model, the F82 input directly determines the value of the Fresnel
//    * model at ~82°, similar to F0 and F90.
//    * With F82-Tint, on the other hand, the value at 82° is the value of the classic Schlick
//    * model multiplied by the tint input.
//    * Therefore, the factor follows by setting `F82Tint(cosI) = FSchlick(cosI) - b*cosI*(1-cosI)^6`
//    * and `F82Tint(acos(1/7)) = FSchlick(acos(1/7)) * f82_tint` and solving for `b`. */
//   const float f = 6.0 / 7.0;
//   const float f5 = (f * f) * (f * f) * f;
//   const float f6 = (f * f) * (f * f) * (f * f);
//   vec3 F_schlick = mix(F0, vec3(1.0), f5);
//   vec3 b = F_schlick * (7.0 / f6) * (1.0 - F82);
//   reflectance -= b * split_sum.z;
// }

// /* Return texture coordinates to sample BSDF LUT. */
// vec3 lut_coords_bsdf(float cos_theta, float roughness, float ior)
// {
//   /* IOR is the sine of the critical angle. */
//   float critical_cos = sqrt(1.0 - ior * ior);

//   vec3 coords;
//   coords.x = pow(ior, 2);
//   coords.y = cos_theta;
//   coords.y -= critical_cos;
//   coords.y /= (coords.y > 0.0) ? (1.0 - critical_cos) : critical_cos;
//   coords.y = coords.y * 0.5 + 0.5;
//   coords.z = roughness;

//   return clamp(coords, vec3(0.0), vec3(1.0));
// }

// /* Return texture coordinates to sample Surface LUT. */
// vec3 lut_coords_btdf(float cos_theta, float roughness, float ior)
// {
//   return vec3(sqrt((ior - 1.0) / (ior + 1.0)), sqrt(1.0 - cos_theta), roughness);
// }

// /* Computes the reflectance and transmittance based on the tint (`f0`, `f90`, `transmission_tint`)
//  * and the BSDF LUT. */
// void bsdf_lut(vec3 F0,
//               vec3 F90,
//               vec3 transmission_tint,
//               float cos_theta,
//               float roughness,
//               float ior,
//               bool do_multiscatter,
//               out vec3 reflectance,
//               out vec3 transmittance)
// {
// #ifdef EEVEE_UTILITY_TX
//   if (ior == 1.0) {
//     reflectance = vec3(0.0);
//     transmittance = transmission_tint;
//     return;
//   }

//   vec2 split_sum;
//   float transmission_factor;

//   if (ior > 1.0) {
//     split_sum = brdf_lut(cos_theta, roughness);
//     vec3 coords = lut_coords_btdf(cos_theta, roughness, ior);
//     transmission_factor = utility_tx_sample_bsdf_lut(utility_tx, coords.xy, coords.z).a;
//     /* Gradually increase `f90` from 0 to 1 when IOR is in the range of [1.0, 1.33], to avoid harsh
//      * transition at `IOR == 1`. */
//     if (all(equal(F90, vec3(1.0)))) {
//       F90 = vec3(saturate(2.33 / 0.33 * (ior - 1.0) / (ior + 1.0)));
//     }
//   }
//   else {
//     vec3 coords = lut_coords_bsdf(cos_theta, roughness, ior);
//     vec3 bsdf = utility_tx_sample_bsdf_lut(utility_tx, coords.xy, coords.z).rgb;
//     split_sum = bsdf.rg;
//     transmission_factor = bsdf.b;
//   }

//   reflectance = F_brdf_single_scatter(F0, F90, split_sum);
//   transmittance = (vec3(1.0) - F0) * transmission_factor * transmission_tint;

//   if (do_multiscatter) {
//     float real_F0 = F0_from_ior(ior);
//     float Ess = real_F0 * split_sum.x + split_sum.y + (1.0 - real_F0) * transmission_factor;
//     float Ems = 1.0 - Ess;
//     /* Assume that the transmissive tint makes up most of the overall color if it's not zero. */
//     vec3 Favg = all(equal(transmission_tint, vec3(0.0))) ? F0 + (F90 - F0) / 21.0 :
//                                                            transmission_tint;

//     vec3 scale = 1.0 / (1.0 - Ems * Favg);
//     reflectance *= scale;
//     transmittance *= scale;
//   }
// #else
//   reflectance = vec3(0.0);
//   transmittance = vec3(0.0);
// #endif
//   return;
// }

// /* Computes the reflectance and transmittance based on the BSDF LUT. */
// vec2 bsdf_lut(float cos_theta, float roughness, float ior, bool do_multiscatter)
// {
//   float F0 = F0_from_ior(ior);
//   vec3 color = vec3(1.0);
//   vec3 reflectance, transmittance;
//   bsdf_lut(vec3(F0),
//            color,
//            color,
//            cos_theta,
//            roughness,
//            ior,
//            do_multiscatter,
//            reflectance,
//            transmittance);
//   return vec2(reflectance.r, transmittance.r);
// }

// // #ifdef EEVEE_MATERIAL_STUBS
// // #  define attrib_load()
// // #  define nodetree_displacement() vec3(0.0)
// // #  define nodetree_surface(closure_rand) Closure(0)
// // #  define nodetree_volume() Closure(0)
// // #  define nodetree_thickness() 0.1
// // #  define thickness_mode 1.0
// // #endif

// // #ifdef GPU_VERTEX_SHADER
// // #  define closure_to_rgba(a) vec4(0.0)
// // #endif

// /* -------------------------------------------------------------------- */
// /** \name Fragment Displacement
//  *
//  * Displacement happening in the fragment shader.
//  * Can be used in conjunction with a per vertex displacement.
//  *
//  * \{ */

// // #ifdef MAT_DISPLACEMENT_BUMP
// // /* Return new shading normal. */
// // vec3 displacement_bump()
// // {
// // #  if defined(GPU_FRAGMENT_SHADER) && !defined(MAT_GEOM_CURVES)
// //   vec2 dHd;
// //   dF_branch(dot(nodetree_displacement(), g_data.N + dF_impl(g_data.N)), dHd);

// //   vec3 dPdx = dFdx(g_data.P);
// //   vec3 dPdy = dFdy(g_data.P);

// //   /* Get surface tangents from normal. */
// //   vec3 Rx = cross(dPdy, g_data.N);
// //   vec3 Ry = cross(g_data.N, dPdx);

// //   /* Compute surface gradient and determinant. */
// //   float det = dot(dPdx, Rx);

// //   vec3 surfgrad = dHd.x * Rx + dHd.y * Ry;

// //   float facing = FrontFacing ? 1.0 : -1.0;
// //   return normalize(abs(det) * g_data.N - facing * sign(det) * surfgrad);
// // #  else
// //   return g_data.N;
// // #  endif
// // }
// // #endif

// // void fragment_displacement()
// // {
// // #ifdef MAT_DISPLACEMENT_BUMP
// //   g_data.N = g_data.Ni = displacement_bump();
// // #endif
// // }

// // /** \} */

// // /* -------------------------------------------------------------------- */
// // /** \name Coordinate implementations
// //  *
// //  * Callbacks for the texture coordinate node.
// //  *
// //  * \{ */

// // vec3 coordinate_camera(vec3 P)
// // {
// //   vec3 vP;
// //   if (false /* Probe. */) {
// //     /* Unsupported. It would make the probe camera-dependent. */
// //     vP = P;
// //   }
// //   else {
// // #ifdef MAT_GEOM_WORLD
// //     vP = drw_normal_world_to_view(P);
// // #else
// //     vP = drw_point_world_to_view(P);
// // #endif
// //   }
// //   vP.z = -vP.z;
// //   return vP;
// // }

// // vec3 coordinate_screen(vec3 P)
// // {
// //   vec3 window = vec3(0.0);
// //   if (false /* Probe. */) {
// //     /* Unsupported. It would make the probe camera-dependent. */
// //     window.xy = vec2(0.5);
// //   }
// //   else {
// // #ifdef MAT_GEOM_WORLD
// //     window.xy = drw_point_view_to_screen(interp.P).xy;
// // #else
// //     /* TODO(fclem): Actual camera transform. */
// //     window.xy = drw_point_world_to_screen(P).xy;
// // #endif
// //     window.xy = window.xy * uniform_buf.camera.uv_scale + uniform_buf.camera.uv_bias;
// //   }
// //   return window;
// // }

// // vec3 coordinate_reflect(vec3 P, vec3 N)
// // {
// // #ifdef MAT_GEOM_WORLD
// //   return N;
// // #else
// //   return -reflect(drw_world_incident_vector(P), N);
// // #endif
// // }

// // vec3 coordinate_incoming(vec3 P)
// // {
// // #ifdef MAT_GEOM_WORLD
// //   return -P;
// // #else
// //   return drw_world_incident_vector(P);
// // #endif
// // }

// /** \} */