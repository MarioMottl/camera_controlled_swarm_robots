#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 fragColor;
out vec2 fragTexCoord;

uniform mat4 projection2D;

void main()
{
    gl_Position = projection2D * vec4(aPos, 1.0f);
    fragColor = aColor;
    fragTexCoord = aTexCoord;
}