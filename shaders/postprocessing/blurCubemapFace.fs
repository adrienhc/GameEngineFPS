#version 450 core


in vec3 fragTex;
out vec4 FragColor;

uniform samplerCube cubemapSrc; //to pass in the 2D texture object to the fs
//uniform vec3 blurSize;

void main()
{	
    FragColor = texture(cubemapSrc, fragTex);

    /*
    vec4 color = vec4(0.0);
	color += texture( cubemap, Texcoord.st + (vec2(-3.0) * blurSize.xy)) * (1.0f/64.0f);
	color += texture( cubemap, Texcoord.st + (vec2(-2.0) * blurSize.xy)) * (6.0f/64.0f);
	color += texture( cubemap, Texcoord.st + (vec2(-1.0) * blurSize.xy)) * (15.0f/64.0f);
	color += texture( cubemap, Texcoord.st + (vec2(0.0) * blurSize.xy)) * (20.0f/64.0f);
	color += texture( cubemap, Texcoord.st + (vec2(1.0) * blurSize.xy)) * (15.0f/64.0f);
	color += texture( cubemap, Texcoord.st + (vec2(2.0) * blurSize.xy)) * (6.0f/64.0f);
	color += texture( cubemap, Texcoord.st + (vec2(3.0) * blurSize.xy)) * (1.0f/64.0f);
	FragColor = vec4(color.xyz, 1.0);
	*/
} 