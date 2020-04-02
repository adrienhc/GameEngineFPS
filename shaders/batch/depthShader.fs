#version 450 core

in vec4 fragPos;

uniform vec3 lightPosition;
uniform float far_plane;

void main()
{
	float lightDistance = length(fragPos.xyz - lightPosition);
	gl_FragDepth  = lightDistance / far_plane; //scale to [0,1] range
}