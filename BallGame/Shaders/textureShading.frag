#version 130
//The fragmnet shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

//This is the 3 component float vector that gets outputted to the screen for each pixel
out vec4 color;

uniform sampler2D mySampler;


void main() {
	
	vec4 textureColor = texture(mySampler, fragmentUV);

	color = fragmentColor * textureColor;

}