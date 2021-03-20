#version 460 core

in vec4 fragColor;
in vec2 fragTexCoord;

out vec4 resultColor;

uniform sampler2D font;
uniform float th0, th1;

void main()
{
    vec4 sdfNotext = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    float sdfSample = texture(font, fragTexCoord).x;
    resultColor = mix(sdfNotext, fragColor, smoothstep(th0, th1, sdfSample));
}