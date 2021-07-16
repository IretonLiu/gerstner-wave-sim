#version 330 core

// Interpolated values from the vertex shaders
uniform mat4 normalMatrix;

struct LightProperties {
  vec4 position;
  vec3 color;
};

struct MaterialProperties {
  vec3 diffuseColor;
  vec3 specularColor;
  float specularExponent;
};

vec3 lightingEquation(LightProperties light,       // light
                      MaterialProperties material, // material
                      vec3 eyeCoords,              // pixel in eye coord
                      vec3 N,                      // normal of the pixel
                      vec3 V                       // direction to the viewer
) {
  vec3 L, R; // Light and reflected light direction;

  if (light.position.w == 0.0) { // directional light
    L = normalize(light.position.xyz);
  } else { // point light
    L = normalize(light.position.xyz / light.position.w - eyeCoords);
  };

  if (dot(L, N) <= 0.0) { // light does not illuminate the surface
    return vec3(0.0);
  };

  vec3 reflection = dot(L, N) * light.color * material.diffuseColor;

  R = -reflect(L, N);

  if (dot(R, V) > 0.0) { // ray is reflected toward the the viewer
    float factor = pow(dot(R, V), material.specularExponent);
    reflection = reflection + material.specularColor * light.color * factor;
  }

  return reflection;
}

in vec3 eyeCoords_out;
in vec3 normal;
// Output data
out vec3 color;

void main() {

  // TODO: implement this in c++ instead of shader
  LightProperties light = LightProperties(vec4(5.0, 10.0, 0.0, 0.0), vec3(1.0));
  MaterialProperties material =
      MaterialProperties(vec3(0.4, 0.57, 0.8), vec3(1.0, 1.0, 1.0), 48);

  vec3 V = normalize(eyeCoords_out.xyz);
  vec3 N = normalize((normalMatrix * vec4(normal, 0.0)).xyz);
  color = lightingEquation(light, material, eyeCoords_out, N, V);
  // Output color = color specified in the vertex shader,
  // interpolated between all 3 surrounding vertices
  // color = N;
}