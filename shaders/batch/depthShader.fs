#version 450 core

in vec4 fragPos;

uniform vec3 lightPosition;
uniform float far_plane;

out vec4 fragDepth;

void main()
{
	float lightDistance = length(fragPos.xyz - lightPosition);
	float moment1 = lightDistance / far_plane; //scale to [0,1] range
	
	//SHADOW MAPPING
	gl_FragDepth  =  moment1;

	float dx = dFdx(moment1);
	float dy = dFdy(moment1);
	float moment2 = moment1 * moment1;
	moment2 += 0.25*(dx*dx+dy*dy);

	//VARIANCE SHADOW MAPPING	
	fragDepth = vec4(moment1, moment2, 0.0f, 1.0f); 
}