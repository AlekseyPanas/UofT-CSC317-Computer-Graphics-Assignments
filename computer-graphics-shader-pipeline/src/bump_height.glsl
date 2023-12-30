// Create a bumpy surface by using procedural noise to generate a height (
// displacement in normal direction).
//
// Inputs:
//   is_moon  whether we're looking at the moon or centre planet
//   s  3D position of seed for noise generation
// Returns elevation adjust along normal (values between -0.1 and 0.1 are
//   reasonable.
float bump_height( bool is_moon, vec3 s, float animation_seconds)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code 

  // Earth-like setup:

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

  vec3 st = s;

  if(!is_moon) {
    s += shift;
  }

  if(is_moon) {
    return ((improved_perlin_noise(vec3(s.x, s.y*10, s.z) * 1) + 
      improved_perlin_noise(vec3(s.x, s.y*10, s.z) * 3) * 0.5 + 
      improved_perlin_noise(vec3(s.x, s.y*10, s.z) * 10) * 0.4 + 
      improved_perlin_noise(vec3(s.x, s.y*10, s.z) * 20) * 0.2) + 0.4) * 0.02 * (1/(float(is_moon) + 1));
  }
  else {
  return max(      
    
    ((improved_perlin_noise(vec3(s.x, s.y*10, s.z) * 1) + 
      improved_perlin_noise(vec3(s.x, s.y*10, s.z) * 3) * 0.5 + 
      improved_perlin_noise(vec3(s.x, s.y*10, s.z) * 10) * 0.4 + 
      improved_perlin_noise(vec3(s.x, s.y*10, s.z) * 20) * 0.2) + 0.4) * 0.02 * (1/(float(is_moon) + 1)),
    
      smooth_heaviside(
      clamp((improved_perlin_noise(st*1.3) + improved_perlin_noise(st*2.5)) * 0.25, 0, 1), 
      100) + improved_perlin_noise(st*20) * 0.02
    );}
    

  /////////////////////////////////////////////////////////////////////////////
}
