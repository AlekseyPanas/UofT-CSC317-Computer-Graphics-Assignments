// Add (hard code) an orbiting (point or directional) light to the scene. Light
// the scene using the Blinn-Phong Lighting Model.
//
// Uniforms:
uniform mat4 view;
uniform mat4 proj;
uniform float animation_seconds;
uniform bool is_moon;
// Inputs:
in vec3 sphere_fs_in;
in vec3 normal_fs_in;
in vec4 pos_fs_in; 
in vec4 view_pos_fs_in; 
// Outputs:
out vec3 color;
// expects: PI, blinn_phong
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

  // Set base color based on moon
  vec3 col = vec3(float(is_moon) * 0.3 + 0.2, float(is_moon) * 0.3 + 0.2, 1 - (float(is_moon) * 0.5));

  // Compute shading
  color = blinn_phong(col * 0.1, col, vec3(1, 1, 1), 1000.0, normal_fs_in, normalize(view_pos_fs_in).xyz, light);

  //color = (vec3(normalize(view_pos_fs_in).xy, 0)) * 5;

  //color = col;

  //color = 0.5+0.5*normal_fs_in;

  /////////////////////////////////////////////////////////////////////////////
}
