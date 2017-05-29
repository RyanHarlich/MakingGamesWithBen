#version 130
//The vertex shader operates on each vertex


//input data from the VBO. Each vertex is 2 floats
in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 P; //projection matrix

void main(){
	//Set the x,y,z position (ignore z since in 2D)
	gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	// indicates that the coordinates are normalized deviced coordinates
	gl_Position.w = 1.0; 

	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
	
	// not flipping uv y coordinates for lightening
	fragmentUV = vertexUV;
}