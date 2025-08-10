#version 110 core

varying lowp vec4 vertexShaderColor;               // Input color from VertexShader.vert
varying highp vec2 vertexShaderTextureCoord;       // Input texture coordinate from VertexShader.vert

uniform sampler2D textureSampler;

void main()
{
   vec4 texColor = texture2D(textureSampler, vertexShaderTextureCoord);
   gl_FragColor = mix(texColor, vertexShaderColor, 0.5); // Mix color and texture and store use it as uutput
}

