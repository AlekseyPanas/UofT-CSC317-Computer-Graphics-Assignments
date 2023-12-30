// Compute Blinn-Phong Shading given a material specification, a point on a
// surface and a light direction. Assume the light is white and has a low
// ambient intensity.
//
// Inputs:
//   ka  rgb ambient color
//   kd  rgb diffuse color
//   ks  rgb specular color
//   p  specular exponent (shininess)
//   n  unit surface normal direction
//   v  unit direction from point on object to eye
//   l  unit light direction
// Returns rgb color
vec3 blinn_phong(
  vec3 ka,
  vec3 kd,
  vec3 ks,
  float p,
  vec3 n,
  vec3 v,
  vec3 l)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code 
  
  // return ka;

  //return normalize(-v);
  // return -1 * ((vec3(normalize(-1 * v).xy, 0)) * 5);
  //return normalize(-1 * l);
  //return vec3(pow(normalize(normalize(-v) + normalize(-l)).x, 3), 0, 0);

  //return vec3(   pow(   max(dot(normalize(normalize(-v) + normalize(-l)), normalize(n)), 0)    ,100)   , 0, 0);
  //return 0.5+0.5*n;

  return (pow(max(0, dot(normalize(normalize(-v) + normalize(-l)), normalize(n))), p) * ks) +
   (max(0, dot(normalize(-l), normalize(n))) * kd) + ka;
  /////////////////////////////////////////////////////////////////////////////
}


