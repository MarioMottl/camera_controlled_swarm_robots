#version 460 core

layout (depth_greater) out float gl_FragDepth;

void main()
{
    gl_FragDepth = gl_FragCoord.z + ((gl_FrontFacing) ? 0.005f : 0.0f);
}