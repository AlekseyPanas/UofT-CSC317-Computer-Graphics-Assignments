// Set the pixel color to blue or gray depending on is_moon.
//
// Uniforms:
uniform bool is_moon;
// Outputs:
out vec3 color;
void main()
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code:
  color = vec3(float(is_moon) * 0.5, float(is_moon) * 0.5, 1 - (float(is_moon) * 0.5));
  //color = vec3(1,1,1);
  /////////////////////////////////////////////////////////////////////////////
}
