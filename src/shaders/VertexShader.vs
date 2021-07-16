#version 330 core

#define PI 3.1415926535897932384626433832795
#define g 9.8
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;

// Values that stay constant for the whole mesh.
uniform mat4 modelView;
uniform mat4 projection;
uniform float time;

// uniform mat3 normalMatrix;

// structs
struct VertexData {
  vec3 position;
  vec3 normal;
};

struct WaveProperties {
  vec2 direction;
  // float amplitude; // removed because steepness accounts for amplitude
  float waveLength;
  // float speed;	// removed due to implementation of phase speed
  float steepness; // this measures the steepness of the waves crests
};

// uniform variables for wave calculation
uniform WaveProperties wave[3];

vec3 gerstnerWave(WaveProperties wave, vec3 p, inout vec3 tangent,
                  inout vec3 binormal) {
  // wave direction
  vec2 d = normalize(wave.direction);

  // wave equation: y = Asin(kx - wt);
  float k = (2.0 * PI) / wave.waveLength; // wave number
  float c = sqrt(9.8 / k); // phase speed of the wave, replaces the wave speed
  float f = k * (dot(d, p.xz) - c * time);
  float a = wave.steepness / k; // amplitude

  // tangent wrt x to the vertex at that point
  tangent += normalize(vec3(1 - d.x * d.x * wave.steepness * sin(f),
                            d.x * wave.steepness * cos(f),
                            -d.x * d.y * wave.steepness * sin(f)));
  // tangent wrt z to the vertex at that point
  binormal += normalize(vec3(-d.x * d.y * wave.steepness * sin(f),
                             d.y * wave.steepness * cos(f),
                             1 - d.y * d.y * wave.steepness * sin(f)));
  // calculate the change in position of the vertex
  // using gerstner waves, where a vertex is anchored at a specific point
  return vec3(d.x * a * cos(f), a * sin(f),
              d.y * a * cos(f)); // this is d.y because d is a vec2
}

void updateVertexData(inout VertexData vertexData) {
  vec3 gridPoint = vertexData.position.xyz;
  vec3 tangent = vec3(0.0);
  vec3 binormal = vec3(0.0);
  vec3 p = gridPoint;
  for (int i = 0; i < 3; i++) {
    p += gerstnerWave(wave[i], p, tangent, binormal);
  }

  vec3 normal = normalize(cross(binormal, tangent));
  vertexData.position = p;
  vertexData.normal = normal;
}

out vec3 eyeCoords_out;
out vec3 normal;

void main() {

  VertexData vertexData =
      VertexData(vertexPosition,
                 vec3(0.0, 1.0, 0.0) // default normal for the plane is point up
      );
  // Output position of the vertex, in clip space : MVP * position
  // calculate the new vertex position based on wave
  updateVertexData(vertexData);

  vec4 eyeCoords = projection * modelView * vec4(vertexData.position, 1);
  gl_Position = eyeCoords;

  normal = vertexData.normal;
  eyeCoords_out = eyeCoords.xyz;
}
