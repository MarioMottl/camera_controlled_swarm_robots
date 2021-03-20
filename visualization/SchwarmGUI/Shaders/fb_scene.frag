#version 460 core

uniform sampler2D visSceneTexture;

in vec2 fragTexCoord;

out vec4 resultColor;

void main()
{
    resultColor = texture(visSceneTexture, fragTexCoord);
}