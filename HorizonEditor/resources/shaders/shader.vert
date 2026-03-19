#version 450 core

out gl_PerVertex {
	vec4 gl_Position;
};

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

out vec3 vColor;

uniform mat4 uPvm;

void main()
{
	gl_Position = uPvm * vec4(aPosition, 1.0);
	vColor = aColor;
}