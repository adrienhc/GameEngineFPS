#version 450 core
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTex;
layout (location = 3) in vec4 vParticleData; //Life, Speed, Size, MAX_Rotation; 
layout (location = 4) in vec4 vCol;

out VS_OUT
{
	vec4 fragPos;
	vec3 fragNorm;
	vec2 fragTex;
	vec4 fragParticleData;
	vec4 fragCol;
} vs_out;

/*
/// MATH ///
const float PI = 3.1415926535897932384626433832795;

/// FUNCTIONS ///
mat4 RotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

vec3 RandRotation(vec3 direction, float degree_max)
{
    vec4 perp_direction = vec4(-1/direction.x, -1/direction.y, 2/direction.z, 1.0f);
    perp_direction = RotationMatrix(direction, vTex.x * 2.0f * PI) * perp_direction;
    perp_direction /= perp_direction.w;

    degree_max *= (PI/180.0f);
    vec4 final_direction = RotationMatrix(perp_direction.xyz, vTex.y * degree_max) * vec4(direction, 1.0f);
    return final_direction.xyz /= final_direction.w;
}
*/
void main()
{
	vs_out.fragPos = vPos;
	vs_out.fragNorm = vNorm; //RandRotation(vNorm, 180.0f);
	vs_out.fragTex = vTex;
	vs_out.fragParticleData = vParticleData;
	vs_out.fragCol = vCol;
}
