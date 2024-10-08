attribute vec3 position;
attribute vec2 uv;

varying vec2 outUv;

void main(void) 
{
	outUv = uv;
	gl_Position = vec4(position, 1.0);
}