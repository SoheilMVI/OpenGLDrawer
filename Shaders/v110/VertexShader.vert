#version 110 core

attribute highp vec4 pointsAttr;                // Input coordinate per vertex
attribute lowp vec4 colorAttr;                  // Input color per vertex
attribute highp vec2 textureCoordAttr;          // Input texture coordinate per vertex

varying lowp vec4 vertexShaderColor;            // Create a color buffer to fill and pass it to FragmentShader.frag
varying highp vec2 vertexShaderTextureCoord;    // Create a texture points buffer of  to fill and pass it to FragmentShader.frag

uniform highp mat4 matrix;                      // Main matrix than handle translates , rotates , scales & ...

void main()
{
   vertexShaderColor = colorAttr;
   vertexShaderTextureCoord = textureCoordAttr;
   gl_Position = matrix * pointsAttr;           // Calculate final positions
}
