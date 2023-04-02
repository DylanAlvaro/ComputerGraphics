// Our simple shader
#version 410
layout(location = 0) in vec2 Position;
out vec2 vTexCoord;

void main()
{
    //Generate the texture coordinates
    vTexCoord = Position * .5f + .5f;
    gl_Position = vec4(Position, 0, 1);
}