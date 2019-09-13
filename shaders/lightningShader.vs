#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model_transform;
uniform mat4 viewing;
uniform mat4 projection;

out vec3 Normal;
out vec3 fragPos;

void main()
{
    gl_Position = projection * viewing * model_transform * vec4(aPos, 1.0);
    fragPos = vec3(model_transform * vec4(aPos, 1.0));
    Normal = aNormal;
}