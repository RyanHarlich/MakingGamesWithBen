#version 130
//The fragmnet shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

//This is the 3 component float vector that gets outputted to the screen for each pixel
out vec4 color;


void main() {

	float distance = length(fragmentUV);
	
	// not realistic lighting
	//color = vec4(fragmentColor.rgb, fragmentColor.a * 1.0 - distance);
	// this is better realistic lighting
	color = vec4(fragmentColor.rgb, fragmentColor.a * pow(0.01, distance) - 0.01);

}