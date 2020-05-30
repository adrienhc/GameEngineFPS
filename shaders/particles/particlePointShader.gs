#version 450 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;
// layout (points, max_vertices=1) out;
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

/// CAMERA ///
uniform vec3 cameraRight;
uniform vec3 cameraUp;

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

    fragHasTex = 1;

    vec4 center = gs_in[0].fragPos;
    center += gs_in[0].fragParticleData.x * gs_in[0].fragParticleData.y * vec4(RandRotation(gs_in[0].fragNorm, gs_in[0].fragParticleData.w), 0.0f);
    

    float size = 3.0f * gs_in[0].fragParticleData.z;
    size *= (1.0f + 2.0f * gs_in[0].fragParticleData.x); //Increase over life time
    vec4 top_right = vec4(center.xyz + size * (cameraUp + cameraRight), center.w);
    gl_Position = projection * viewing * top_right;
    gl_PointSize = 20.0f;
    float alpha = (1.0f - gs_in[0].fragParticleData.x);
    alpha = alpha * alpha * alpha;
    fragColor = vec4(gs_in[0].fragCol.xyz, alpha * gs_in[0].fragCol.w);
    fragTex = vec2(1.0f, 1.0f);
    EmitVertex(); 

    vec4 top_left = vec4(center.xyz + size * (cameraUp - cameraRight), center.w);
    gl_Position = projection * viewing * top_left;
    fragColor = vec4(gs_in[0].fragCol.xyz, alpha * gs_in[0].fragCol.w);
    fragTex = vec2(0.0f, 1.0f);
    EmitVertex(); 


    vec4 bottom_right = vec4(center.xyz + size * (- cameraUp + cameraRight), center.w);
    gl_Position = projection * viewing * bottom_right;
    fragColor = vec4(gs_in[0].fragCol.xyz, alpha * gs_in[0].fragCol.w);
    fragTex = vec2(1.0f, 0.0f);
    EmitVertex();

    vec4 bottom_left = vec4(center.xyz + size * (- cameraUp - cameraRight), center.w);
    gl_Position = projection * viewing * bottom_left;
    fragColor = vec4(gs_in[0].fragCol.xyz, alpha * gs_in[0].fragCol.w);
    fragTex = vec2(0.0f, 0.0f);
    EmitVertex();
    
    EndPrimitive();
}
  
