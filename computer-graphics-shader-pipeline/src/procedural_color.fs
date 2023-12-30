// Set the pixel color to an interesting procedural color generated by mixing
// and filtering Perlin noise of different frequencies.
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

// expects: blinn_phong, perlin_noise
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

  float noise = perlin_noise(
    vec3(
      sin(sphere_fs_in.x),
      tan(sphere_fs_in.y),
      cos(3 * sphere_fs_in.z)
    ) * 3
  );
  if (noise > 0) {
    color = noise * vec3(1.5, 3, 1.5);
  } else {
    color = (-noise) * vec3(0.7, 0.7, 1.5);
  }
  
  color = blinn_phong(color * 0.1, color, vec3(1, 1, 1), 1000.0, normal_fs_in, normalize(view_pos_fs_in).xyz, light);

  /////////////////////////////////////////////////////////////////////////////
}
