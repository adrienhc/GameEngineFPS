#version 450 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

/// INPUT OUTPUT ///
in VS_OUT
{
    vec4 fragPos;
    vec3 fragNorm;
    vec2 fragTex; //xy in [0,1] 
                  //for RANDOM ROTATION want x [0,2PI] (neg angle taken care of)  --  y [0, MAX_Rotation] 
    vec4 fragParticleData; //Life, Speed, Size, MAX_Rotation; 
    vec4 fragCol;
} gs_in[];

out vec4 fragColor;
out vec2 fragTex;
flat out int fragHasTex;

/// MATH ///
const float PI = 3.1415926535897932384626433832795;

/// TRANSFORMS ///
uniform mat4 viewing;
uniform mat4 projection;

/// NOISE ///
uniform sampler2D noiseRGB;

// TIME ///
uniform float Time;


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
    vec4 perp_direction = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    if(direction != vec3(0.0f, 0.0f, 1.0f))
    {
        perp_direction = vec4(cross(direction, vec3(0.0f, 0.0f, 1.0f)), 1.0f);
    }
    perp_direction = RotationMatrix(direction, gs_in[0].fragTex.x * 2.0f * PI) * perp_direction;
    perp_direction /= perp_direction.w;

    degree_max *= (PI/180.0f);
    vec4 final_direction = RotationMatrix(perp_direction.xyz, gs_in[0].fragTex.y * degree_max) * vec4(direction, 1.0f);
    return final_direction.xyz /= final_direction.w;
}

vec4 RandTextureOffset()
{
    // RESCALE DIR TO [-1, 1] RANGE INSTEAD!!!!!!
    vec3 dir = (texture(noiseRGB, gs_in[0].fragTex).xyz * 2.0f) - vec3(1.0f);
    float mag = 5.0f;

    //vec2 rand_fragTex = mod(gs_in[0].fragTex + 10.0f * vec2(Time, Time), vec2(1.0f));
    //vec3 rand_vel = clamp((texture(noiseRGB, rand_fragTex).xyz), vec3(1.0f, 0.0f, 1.0f), vec3(1.0f));
    
    //vec3 rand_vel = texture(noiseRGB, rand_fragTex).xyz;
    //rand_dir *= rand_dir;
    //float rand_mag = 0.02f;
    
    return abs(sin(2.0f * Time)) * mag * vec4(dir, 0.0f); // + vec4(rand_mag * rand_dir, 0.0f));
    // fragColor = vec4(texture(noiseRGB, gs_in[0].fragTex).xyz, 1.0f);

}

void main() 
{    
    fragHasTex = 0;

    vec4 center = gs_in[0].fragPos;
    vec4 offset = gs_in[0].fragParticleData.x * gs_in[0].fragParticleData.y * vec4(RandRotation(gs_in[0].fragNorm, gs_in[0].fragParticleData.w), 0.0f);
    vec4 drop = 20.0f * vec4(0.0f, gs_in[0].fragParticleData.x * gs_in[0].fragParticleData.z, 0.0f, 0.0f);

    vec4 start = center + offset - (drop * drop);

    gl_Position = projection * viewing * start;
    gl_PointSize = 20.0f/gl_Position.z;
    fragColor = gs_in[0].fragCol;
    EmitVertex();

    float drag = 0.5f;
    vec4 end = center + drag * (offset - (drop * drop));
    gl_Position = projection * viewing * end;
    gl_PointSize = 20.0f/gl_Position.z;
    fragColor = gs_in[0].fragCol;
    EmitVertex();
    EndPrimitive();
    // 
    
}
  
