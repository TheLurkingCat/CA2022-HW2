#version 330 core
layout(location = 0) in vec4 position_in;
layout(location = 1) in vec4 normal_in;
layout(location = 2) in vec2 textureCoordinate_in;

out VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 textureCoordinate;
} vs_out;

layout (std140) uniform model {
  mat4 modelMatrix;
  mat4 normalMatrix;
};

layout (std140) uniform camera {
  mat4 viewProjectionMatrix;
  vec4 viewPosition;
};

void main() {
  vs_out.position = vec3(model * position);
  vs_out.normal = vec3(normalMatrix * normal_in);
  vs_out.textureCoordinate = textureCoordinate_in;
  gl_Position = viewProjectionMatrix * model * position;
}
