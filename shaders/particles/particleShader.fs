#version 450 core

uniform sampler2D smokeTexture;

in vec4 fragColor;
flat in int fragHasTex;
in vec2 fragTex;

out vec4 FragColor;

void main()
{
	if(fragHasTex == 1)
	{
		FragColor = texture(smokeTexture, fragTex);
	}
	else
	{
		FragColor = fragColor;
	}
}
