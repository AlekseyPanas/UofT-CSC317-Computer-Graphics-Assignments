// Input:
//   N  3D unit normal vector
// Outputs:
//   T  3D unit tangent vector
//   B  3D unit bitangent vector
void tangent(in vec3 N, out vec3 T, out vec3 B)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code 

  N = normalize(N);

  T = cross(N, vec3(1,1,1));
  B = normalize(cross(T, N));
  /////////////////////////////////////////////////////////////////////////////
}
