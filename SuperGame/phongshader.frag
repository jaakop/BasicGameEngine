uniform sampler2D texture01;

varying vec2 outUv;
varying vec3 eyespacePosition;
varying vec3 eyespaceNormal;

void main(void) 
{
	gl_FragColor = texture2D(texture01, outUv);
}