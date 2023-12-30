// Given a 3d position as a seed, compute a smooth procedural noise
// value: "Perlin Noise", also known as "Gradient noise".
//
// Inputs:
//   st  3D seed
// Returns a smooth value between (-1,1)
//
// expects: random_direction, smooth_step
float perlin_noise( vec3 st) 
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code 
  vec3 corners[8] = vec3[](
    vec3(floor(st.x), floor(st.y), floor(st.z)),
    vec3(floor(st.x), floor(st.y), floor(st.z) + 1),
    vec3(floor(st.x), floor(st.y) + 1, floor(st.z)),
    vec3(floor(st.x), floor(st.y) + 1, floor(st.z) + 1),

    vec3(floor(st.x) + 1, floor(st.y), floor(st.z)),
    vec3(floor(st.x) + 1, floor(st.y), floor(st.z) + 1),
    vec3(floor(st.x) + 1, floor(st.y) + 1, floor(st.z)),
    vec3(floor(st.x) + 1, floor(st.y) + 1, floor(st.z) + 1)
  );

  vec3 pos_in_grid_box = vec3(st.x - corners[0].x, st.y - corners[0].y, st.z - corners[0].z);

  vec3 gradients[8];
  float dot_prods[8];

  for (int i = 0; i < 8; i ++) {
    gradients[i] = random_direction(corners[i]);
    dot_prods[i] = dot(st - corners[i], gradients[i]);
  }

  vec3 smth_pos = smooth_step(pos_in_grid_box);

  float intrp_x[4] = float[](
    dot_prods[0] + (smth_pos.x * (dot_prods[4] - dot_prods[0])),
    dot_prods[1] + (smth_pos.x * (dot_prods[5] - dot_prods[1])),
    dot_prods[2] + (smth_pos.x * (dot_prods[6] - dot_prods[2])),
    dot_prods[3] + (smth_pos.x * (dot_prods[7] - dot_prods[3]))
  );

  float intrp_y[2] = float[](
    intrp_x[0] + (smth_pos.y * (intrp_x[2] - intrp_x[0])),
    intrp_x[1] + (smth_pos.y * (intrp_x[3] - intrp_x[1]))
  );

  return intrp_y[0] + (smth_pos.z * (intrp_y[1] - intrp_y[0]));
  /////////////////////////////////////////////////////////////////////////////
}

