#version 120

uniform float timeVal = 1.0;

void main(){

	gl_TexCoord[0] = gl_MultiTexCoord0;

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

	//modify our color
	vec4 col = gl_Color;
	
	gl_FrontColor =  col;	
}
