#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

in VS_OUT {
    vec3 fragPos;
    vec3 fragNorm;
    vec2 texCoords;
} gs_in[];

out vec3 FragPos;
out vec3 FragNorm;
out vec2 TexCoords; 

uniform float time;

uniform mat4 model_transform;
uniform mat4 viewing;
uniform mat4 projection;
uniform mat3 normal_transform;


vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
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

void main() {    
    
    float r = rand(gs_in[2].texCoords);
    vec3 normal = GetNormal(); // + vec3(r);

    for(int i = 0; i < 3; i++)
    {
        gl_Position = projection * viewing * model_transform * explode(gl_in[i].gl_Position, normal);
        FragPos = vec3( model_transform * vec4(gs_in[i].fragPos, 1.0f) );
        FragNorm = normal_transform * gs_in[i].fragNorm;
        TexCoords = gs_in[i].texCoords;
        EmitVertex();
    }

    EndPrimitive();
}  
