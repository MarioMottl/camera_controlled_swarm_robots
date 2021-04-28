#version 460 core

uniform sampler2D sceneTexture;
uniform float gamma;
uniform bool autoGammaCorrection;

in vec2 fragTexCoord;

out vec4 resultColor;

void main()
{
    resultColor = vec4(pow(texture(sceneTexture, fragTexCoord).rgb, vec3(((autoGammaCorrection) ? 2.2f : 1.0f) / gamma)), 1.0f);
}