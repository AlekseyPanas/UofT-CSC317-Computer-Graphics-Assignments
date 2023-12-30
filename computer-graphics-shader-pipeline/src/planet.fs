// Generate a procedural planet and orbiting moon. Use layers of (improved)
// Perlin noise to generate planetary features such as vegetation, gaseous
// clouds, mountains, valleys, ice caps, rivers, oceans. Don't forget about the
// moon. Use `animation_seconds` in your noise input to create (periodic)
// temporal effects.
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
// expects: model, blinn_phong, bump_height, bump_position,
// improved_perlin_noise, tangent
void main()
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code 

  // Invert the camera matrix to get the post-model position of the target fragment
  vec3 world_pos = (inverse(view) * view_pos_fs_in).xyz;

  // Compute point light orbiting position
  vec3 light_pos = (vec3(cos(animation_seconds * M_PI * 0.25), 0, sin(animation_seconds * M_PI * 0.25)) * 5) + vec3(0, 4, 0);

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

  vec3 shift = vec3(
    sin(animation_seconds / 10) * 0.5 + 
     sin(animation_seconds / 2) * 0.2 +
     sin(animation_seconds / 1) * 0.1 +
     1,
     sin(animation_seconds / 15) * -0.6 + 
     sin(animation_seconds / 4) * 0.25 +
     sin(animation_seconds / 2) * -0.05 +
     1,
     sin(animation_seconds / 9) * 0.7 + 
     sin(animation_seconds / 3) * 0.3 +
     sin(animation_seconds / 2) * -0.15 +
     1
  );

  vec3 s = sphere_fs_in + shift;

  float noise = 
    (improved_perlin_noise(vec3(s.x, s.y*10, s.z) * 1) + 
    improved_perlin_noise(vec3(s.x, s.y*10, s.z) * 3) * 0.5 + 
    improved_perlin_noise(vec3(s.x, s.y*10, s.z) * 10) * 0.4 + 
    improved_perlin_noise(vec3(s.x, s.y*10, s.z) * 20) * 0.2) + 0.4;

  if (is_moon) {
    color += vec3(noise, noise, noise);
  } else {
    if (noise > 0) {
    color += noise * 1 * vec3(0, 0.5, 0);
    } else {
      color += (-noise) * 1 * vec3(0.5, 0, 0);
    }
  }
  

  float land_height = smooth_heaviside(
      clamp((improved_perlin_noise(sphere_fs_in*1.3) + improved_perlin_noise(sphere_fs_in*2.5)) * 0.25, 0, 1), 
      100) + improved_perlin_noise(sphere_fs_in*20) * 0.03;

  if (land_height > 0.2 && !is_moon) {
    float arid_noise = improved_perlin_noise(sphere_fs_in * (5.0 + sin(animation_seconds / 5) * 3)) + 0.3;

    color = vec3(land_height * 0.5 + arid_noise, 1 * land_height + 0.5 - (arid_noise * 0.6), 0);
  }

  float cloud_t = animation_seconds / 10;
  vec3 cloud_pos = sphere_fs_in + vec3(cloud_t, cloud_t, cloud_t);
  float cloud_noise = 
    clamp(pow(
      improved_perlin_noise(vec3(cloud_pos.x, cloud_pos.y, cloud_pos.z)) * 0.7 + 
      improved_perlin_noise(cloud_pos * 5) * 0.4 +
      improved_perlin_noise(cloud_pos * 15) * 0.1 +
      improved_perlin_noise(cloud_pos * 25) * 0.05 +
      0.7, 8)
      , 0, 1);

  vec3 cloud_col = cloud_noise * vec3(0.8, 0.8, 0.8);

  color = blinn_phong(color * 0.1, color, vec3(1, 1, 1), 100.0, new_normal, normalize(view_pos_fs_in).xyz, light); 
  
  if (!is_moon) {
    color += cloud_col * 0.9;
  }

  /////////////////////////////////////////////////////////////////////////////
}
