#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 aModel;

uniform mat4 VP;    // screenSpaceMat
uniform mat4 lightSpaceMat;

out VS_OUT
{
    vec2 fragTexCoord;
    vec3 fragPos;
    vec3 normalVec;
    vec4 fragPosLightSpace;
} vs_out;

void main()
{
    vs_out.fragTexCoord = aTexCoord;
    vs_out.fragPos = vec3(aModel * vec4(aPos, 1.0f));                       // absolute position = relative model-transformation * world position
    vs_out.normalVec = transpose(inverse(mat3(aModel))) * aNormal;          // model matrix for normal-vector (normal-matrix)
    vs_out.fragPosLightSpace = lightSpaceMat * vec4(vs_out.fragPos, 1.0f);  // lightSpaceMat * model * vec4(aPos, 1.0f);
    gl_Position = VP * vec4(vs_out.fragPos, 1.0f);                          // VP * model * vec4(aPos, 1.0f);
}