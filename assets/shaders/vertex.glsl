#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 colorRGB;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 colorVS;

void main() {
    colorVS = colorRGB;
    mat4 mvp = projection * view * model;
    gl_Position = mvp*vec4(pos.x, pos.y, pos.z, 1.0);
}
