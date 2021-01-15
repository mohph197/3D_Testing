#version 330 core

uniform vec3 LightColor;

void main()
{
	gl_FragColor = vec4(LightColor, 1.0f);
}
