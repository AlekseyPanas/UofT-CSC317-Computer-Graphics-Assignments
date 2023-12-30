// Set the pixel color using Blinn-Phong shading (e.g., with constant blue and
// gray material color) with a bumpy texture.
// 
// Uniforms:
uniform mat4 view;
uniform mat4 proj;
uniform float animation_seconds;
uniform bool is_moon;
// Inputs:
//                     linearly interpolated from tessellation evaluation shader
//                     output
in vec3 sphere_fs_in;
in vec3 normal_fs_in;
in vec4 pos_fs_in; 
in vec4 view_pos_fs_in; 
// Outputs:
//               rgb color of this pixel
out vec3 color;
// expects: model, blinn_phong, bump_height, bump_position,
// improved_perlin_noise, tangent
void main()
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code 
  
  // Get pi
  float pi = 3.141592653589;

  // Invert the camera matrix to get the post-model position of the target fragment
  vec3 world_pos = (inverse(view) * view_pos_fs_in).xyz;

  // Compute point light orbiting position
  //vec3 light_pos = (vec3(cos(animation_seconds * pi * 0.25), sin(animation_seconds * pi * 0.25), 0) * 5) + vec3(0, 0, 2);
  vec3 light_pos = (vec3(cos(animation_seconds * pi * 0.25), 0, sin(animation_seconds * pi * 0.25)) * 5) + vec3(0, 5, 0);

  // Compute difference to get vector towards light
  vec3 light = world_pos - light_pos;

  // Find tangents to sphere
  vec3 T;
  vec3 B;
  tangent(normal_fs_in, T, B);

  float delta = 0.01;

  vec3 pos = bump_position(is_moon, sphere_fs_in, animation_seconds);
  vec3 pos_B = bump_position(is_moon, sphere_fs_in + (delta * B), animation_seconds);
  vec3 pos_T = bump_position(is_moon, sphere_fs_in + (delta * T), animation_seconds);

  vec3 new_normal = normalize(cross( (pos_T - pos) / delta, (pos_B - pos) / delta  ));
  if (dot(new_normal, normal_fs_in) < 0) {
    new_normal = -new_normal;
  }
  
  color = vec3(float(is_moon) * 0.5, float(is_moon) * 0.5, 1 - (float(is_moon) * 0.5));
  
  //color = 0.5 + 0.5 * normal_fs_in;
  //color = 0.5 + 0.5 * new_normal;

  vec3 s = sphere_fs_in + ((delta + (animation_seconds * 0.02) - 2) * B);

  float noise = (smooth_heaviside(

    clamp((improved_perlin_noise(s*1.3) + improved_perlin_noise(s*2.5)) * 0.25, 0, 1), 
  
  50) + (improved_perlin_noise(s * 15) * 0.3)) * 0.1;

  noise = improved_perlin_noise(s * ( 1.0f /*mod(animation_seconds / 3, 10.0)*/  ));

  // if (noise > 0) {
  //   color = noise * 10 * vec3(0, 1, 0);
  // } else {
  //   color = (-noise) * 10 * vec3(1, 0, 0);
  // }

  color = blinn_phong(color * 0.1, color, vec3(1, 1, 1), 100.0, new_normal, normalize(view_pos_fs_in).xyz, light);
  /////////////////////////////////////////////////////////////////////////////
}
