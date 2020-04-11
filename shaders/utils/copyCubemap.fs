#version 450 core

in vec3 fragTex;
out vec4 FragColor;

uniform samplerCube cubemapSrc; 

void main()
{	
    FragColor = texture(cubemapSrc, fragTex);
} 