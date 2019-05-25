#version 330 core

//input to be used with our texture to sampel the texture
in vec2 texCoordOut;
in vec3 normalOut;

//output of the program
out vec4 color;

uniform sampler2D ourTexture;

void main(){
	vec3 lightSource = vec3(1,2,3);
	vec3 normLight = normalize(lightSource);
	float mod = clamp(dot(normalOut, normLight), 0.3, 1);
	//changes the color of every fragment
	color = texture(ourTexture, texCoordOut) * mod;
}