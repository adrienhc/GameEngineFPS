#version 450 core
layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

/// INPUT OUTPUT ///

in VS_OUT
{
    vec4 fragPos;
    vec4 fragCol;
    float fragSize;
} gs_in[];

out vec4 fragColor;

/// TRANSFORMS ///
uniform mat4 viewing;
uniform mat4 projection;

/// LIGHTNING ///

struct PointLight
{
    vec3 position;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define NUM_POINT_LIGHTS 9
uniform int numLights;
uniform PointLight pointLight[NUM_POINT_LIGHTS];


/// FUNCTIONS ///
vec3 CalcPointLight(PointLight pointLight, vec3 fragPos, vec3 norm)
{
    //ambient
    vec3 ambient = pointLight.ambient * gs_in[0].fragCol.xyz;

    //diffuse
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diff = max(dot(norm, lightDir) , 0.0);
    vec3 diffuse = pointLight.diffuse * gs_in[0].fragCol.xyz * diff;

    //attenuation
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / ( pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance) );

    ambient *= attenuation;
    diffuse *= attenuation;

    return (ambient + diffuse);
}


void createVertex(vec3 offset, vec4 center, vec3 faceNormal)
{
    vec3 actualOffset = gs_in[0].fragSize * offset;
    vec4 position = vec4(center.xyz + actualOffset, center.w);
    
    gl_Position = projection * viewing * position;
    
    vec3 FragPos = vec3(position.xyz) / position.w;
    vec3 Norm = faceNormal;
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    
    for(int i = 0; i < numLights; i++)
        result += CalcPointLight(pointLight[i], FragPos, Norm);

    fragColor = vec4(result, gs_in[0].fragCol.w);
    EmitVertex();
}

void main() {    

    vec4 center = gs_in[0].fragPos;

    vec3 faceNormal = vec3(0.0, 0.0, 1.0);
    createVertex(vec3(-1.0, 1.0, 1.0), center, faceNormal);
    createVertex(vec3(-1.0, -1.0, 1.0), center, faceNormal);
    createVertex(vec3(1.0, 1.0, 1.0), center, faceNormal);
    createVertex(vec3(1.0, -1.0, 1.0), center, faceNormal);
    
    EndPrimitive();
    
    faceNormal = vec3(1.0, 0.0, 0.0);
    createVertex(vec3(1.0, 1.0, 1.0), center, faceNormal);
    createVertex(vec3(1.0, -1.0, 1.0), center, faceNormal);
    createVertex(vec3(1.0, 1.0, -1.0), center, faceNormal);
    createVertex(vec3(1.0, -1.0, -1.0), center, faceNormal);
    
    EndPrimitive();
    
    faceNormal = vec3(0.0, 0.0, -1.0);
    createVertex(vec3(1.0, 1.0, -1.0), center, faceNormal);
    createVertex(vec3(1.0, -1.0, -1.0), center, faceNormal);
    createVertex(vec3(-1.0, 1.0, -1.0), center, faceNormal);
    createVertex(vec3(-1.0, -1.0, -1.0), center, faceNormal);
    
    EndPrimitive();
    
    faceNormal = vec3(-1.0, 0.0, 0.0);
    createVertex(vec3(-1.0, 1.0, -1.0), center, faceNormal);
    createVertex(vec3(-1.0, -1.0, -1.0), center, faceNormal);
    createVertex(vec3(-1.0, 1.0, 1.0), center, faceNormal);
    createVertex(vec3(-1.0, -1.0, 1.0), center, faceNormal);
    
    EndPrimitive();
    
    faceNormal = vec3(0.0, 1.0, 0.0);
    createVertex(vec3(1.0, 1.0, 1.0), center, faceNormal);
    createVertex(vec3(1.0, 1.0, -1.0), center, faceNormal);
    createVertex(vec3(-1.0, 1.0, 1.0), center, faceNormal);
    createVertex(vec3(-1.0, 1.0, -1.0), center, faceNormal);
    
    EndPrimitive();
    
    faceNormal = vec3(0.0, -1.0, 0.0);
    createVertex(vec3(-1.0, -1.0, 1.0), center, faceNormal);
    createVertex(vec3(-1.0, -1.0, -1.0), center, faceNormal);
    createVertex(vec3(1.0, -1.0, 1.0), center, faceNormal);
    createVertex(vec3(1.0, -1.0, -1.0), center, faceNormal);
    
    EndPrimitive();
}
  
