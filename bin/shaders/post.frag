// a simple flat-color shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colorTarget;
uniform int postProcessTarget;

out vec4 FragColor;

vec4 Default(vec2 TexCoord) 
{
    return texture(colorTarget, TexCoord);
}

vec4 BoxBlur(vec2 TexCoord) 
{ 
    vec2 texel = 1.0f / textureSize(colorTarget, 0);

    vec4 color = texture(colorTarget, TexCoord);
    color += texture(colorTarget, TexCoord + texel * vec2(-1, 1));
    color += texture(colorTarget, TexCoord + texel * vec2(-1, 0));
    color += texture(colorTarget, TexCoord + texel * vec2(-1, -1));
   
    color += texture(colorTarget, TexCoord + texel * vec2(0, 1));
    color += texture(colorTarget, TexCoord + texel * vec2(0, -1));

    color += texture(colorTarget, TexCoord + texel * vec2(1, 1));
    color += texture(colorTarget, TexCoord + texel * vec2(1, 0));
    color += texture(colorTarget, TexCoord + texel * vec2(1, -1));

    return color / 9;
}

vec4 Distort(vec2 texCoord)
{
    vec2 mid = vec2(0.5f);
    float distanceFromTheCenter = distance(texCoord, mid);
    vec2 normalizeCoord = normalize(texCoord - mid);
    float bias = distanceFromTheCenter + sin(distanceFromTheCenter * 15) * 0.05f;

    vec2 newCoord = mid + bias * normalizeCoord;
    return texture(colorTarget, newCoord);
}

void main()
{
    // this will calculate the texel size
    vec2 texSize = textureSize(colorTarget, 0);
    vec2 texelSize = 1.0f / texSize;

    // then we will adjust the coordinate
    vec2 scale = (texSize - texelSize) / texSize;
    vec2 TexCoord = vTexCoord / scale + texelSize * 0.5f;

    switch(postProcessTarget)
    {
        default: // default
        {
             // Output post process effect here
             FragColor = Default(TexCoord);
             break;
        }
        case 0: // box blur
        {
              FragColor = BoxBlur(TexCoord);
             break;
        }

        case 1: // distort
        {
              FragColor = Distort(TexCoord);
             break;
        }

        case 2: // edge detection
        {
              FragColor = Default(TexCoord);
             break;
        }

        case 3: // sepia
        {
              FragColor = Default(TexCoord);
             break;
        }

        case 4: // scanlines
        {
            FragColor = Default(TexCoord);
            break;
        }

        case 5: // distort
        {
            FragColor = Default(TexCoord);
            break;
        }

        case 6: // invert
        {
            FragColor = Default(TexCoord);
            break;
        }

        case 7: // Pxilizer
        {
            FragColor = Default(TexCoord);
            break;
        }

        case 8: // Posterization
        {
            FragColor = Default(TexCoord);
            break;
        }

        case 9: // Distance Fog
        {
            FragColor = Default(TexCoord);
            break;
        }

        case 10: // Depth of Field
        {
            FragColor = Default(TexCoord);
            break;
        }
    }
}