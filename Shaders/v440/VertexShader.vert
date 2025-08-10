#version 440 core

layout(location = 0) in vec4 pointsAttr;          // Input coordinate per vertex
layout(location = 1) in vec4 colorAttr;           // Input color per vertex
layout(location = 2) in vec2 textureCoordAttr;    // Input texture coordinate per vertex

out vec4 vertexShaderColor;                       // Create a color buffer to fill and pass it to FragmentShader.frag
out vec2 vertexShaderTextureCoord;                // Create a texture points buffer of  to fill and pass it to FragmentShader.frag

uniform mat4 matrix;                              // Main matrix than handle translates , rotates , scales & ...

void main()
{
    vertexShaderColor = colorAttr;
    vertexShaderTextureCoord = textureCoordAttr;
    gl_Position = matrix * pointsAttr;            // Calculate final positions
}
