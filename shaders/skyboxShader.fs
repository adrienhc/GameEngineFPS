#version 330 core 

in vec3 TexCoords;
uniform samplerCube skybox;
out vec4 FragColor;

void main()
{
	vec4 col = texture(skybox, TexCoords);

	FragColor = col; 
} 