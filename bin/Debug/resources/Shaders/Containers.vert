#version 330 core

layout (location = 0) in vec3 myFragPos;
layout (location = 1) in vec2 myTexCoords;
layout (location = 2) in vec3 myNormal;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
   FragPos = vec3(Model * vec4(myFragPos, 1.0f));
   TexCoords = myTexCoords;
   Normal = mat3(transpose(inverse(Model))) * myNormal;

   gl_Position = Projection * View * Model * vec4(myFragPos, 1.0f);
}