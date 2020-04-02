#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 24) out;

/// INPUT OUTPUT ///

in VS_OUT {
    vec3 fragPos;
    vec3 fragNorm;
    vec2 texCoords;
} gs_in[];

out vec3 VertexColor;


/// TRANSFORMS ///

uniform mat4 model_transform;
uniform mat4 viewing;
uniform mat4 projection;
uniform mat3 normal_transform;


/// PARTICLES ///
uniform float deltaTime;
const float size = 0.1;
const vec3 color =  vec3(0.576f, 0.164f, 0.164f);

/// LIGHTNING ///

uniform vec3 cameraPos;

uniform vec3 lightColor;
uniform vec3 lightPos;

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

#define NUM_POINT_LIGHTS 3
uniform int numLights;
uniform PointLight pointLight[NUM_POINT_LIGHTS];


/// FUNCTIONS ///

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 15.0f;
    vec3 direction = normal * deltaTime * magnitude; 
    return position + vec4(direction, 0.0);
} 


vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 CalcPointLight(PointLight pointLight, vec3 fragPos, vec3 norm)
{
    //ambient
    vec3 ambient = pointLight.ambient * color;

    //diffuse
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diff = max(dot(norm, lightDir) , 0.0);
    vec3 diffuse = pointLight.diffuse * color * diff;

    //attenuation
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / ( pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance) );

    ambient *= attenuation;
    diffuse *= attenuation;

    return (ambient + diffuse);
}


void createVertex(vec3 offset, vec4 center, vec3 faceNormal)
{
    vec3 actualOffset = size * offset;
    vec4 position = center + vec4(actualOffset, 1.0f);
    
    gl_Position = projection * viewing * model_transform * position;
    vec4 FragPos4d = model_transform * position;
    vec3 FragPos = vec3(FragPos4d.xyz) / FragPos4d.w;
    vec3 FragNorm = normal_transform * faceNormal;

    vec3 Norm = normalize(FragNorm);
    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    for(int i = 0; i < numLights; i++)
        result += CalcPointLight(pointLight[i], FragPos, Norm);

    VertexColor = result;
    EmitVertex();
}

void main() {    
    
    float r = rand(gs_in[2].texCoords);
    vec3 dir = GetNormal() + vec3(r); //direction moves to

    vec4 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position)/3.0f;
    center = explode(center, dir) + vec4(0.0f, 100.0f * (deltaTime * deltaTime), 0.0f, 0.0f);

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
