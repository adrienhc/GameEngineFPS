#version 330 core

in vec4 FragPos;

uniform vec3 lightPosition;
uniform float far_plane;

void main()
{
	float lightDistance = length(FragPos.xyz - lightPosition);
	lightDistance = lightDistance / far_plane; //scale to [0,1] range
	gl_FragDepth = lightDistance;
}