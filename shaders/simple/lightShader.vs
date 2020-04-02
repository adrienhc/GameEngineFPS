#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model_transform;
uniform mat4 viewing;
uniform mat4 projection;

void main()
{
    gl_Position = projection * viewing * model_transform * vec4(aPos, 1.0);
}