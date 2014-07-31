attribute vec4 position;
attribute vec4 color;
attribute vec4 normal;
attribute vec2 texcoord;

uniform mat4 modelViewProjectionMatrix;

varying vec4 colorVarying;
varying vec2 texCoordVarying;

uniform float timeVal;

void main()
{
	//get our current vertex position so we can modify it
	vec4 pos = modelViewProjectionMatrix * position;

	//finally set the pos to be that actual position rendered
	gl_Position = modelViewProjectionMatrix * position;
}
