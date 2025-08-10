#version 440 core

in vec4 vertexShaderColor;              // Input color from VertexShader.vert
in vec2 vertexShaderTextureCoord;       // Input texture coordinate from VertexShader.vert

uniform sampler2D textureSampler;

out vec4 FragColor;                     // Output color

void main()
{
    vec4 texColor = texture(textureSampler, vertexShaderTextureCoord);
    FragColor = mix(texColor, vertexShaderColor, 0.5); // Mix color and texture and store use it as uutput
}
