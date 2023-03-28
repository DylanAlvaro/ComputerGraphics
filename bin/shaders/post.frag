// a simple flat-color shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colorTarget;
uniform int postProcessTarget;
uniform int windowWidth;
uniform int windowHeight;
uniform float intensity;
uniform float time;

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

vec4 EdgeDetection(vec2 texCoord)
{
    float w = 1.0f / windowWidth;
    float h = 1.0f / windowHeight;

    vec4 k[9];
    k[0] = texture(colorTarget, texCoord + vec2(-w, -h));
    k[1] = texture(colorTarget, texCoord + vec2(0, -h));
    k[2] = texture(colorTarget, texCoord + vec2(w, -h));
    k[3] = texture(colorTarget, texCoord + vec2(-w, 0));
    k[4] = texture(colorTarget, texCoord);
    k[5] = texture(colorTarget, texCoord + vec2(w, 0));
    k[6] = texture(colorTarget, texCoord + vec2(-w, h));
    k[7] = texture(colorTarget, texCoord + vec2(0, h));
    k[8] = texture(colorTarget, texCoord + vec2(w, h));

    vec4 sobelEdgeH = k[2] + (2.0f * k[5]) + k[8] - (k[0] + (2.0f * k[3]) + k[6]);
    vec4 sobelEdgeV = k[0] + (2.0f * k[1]) + k[2] - (k[6] + (2.0f * k[7]) + k[8]);

    vec4 sobel = sqrt((sobelEdgeH * sobelEdgeH) + (sobelEdgeV * sobelEdgeV));

    return vec4(1.0f - sobel.rgb, 1.0f);
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

vec4 Sepia(vec2 texCoord)
{
   vec4 color = texture(colorTarget, texCoord);
   vec4 outColor;

   outColor.r = (color.r * .393) + (color.g * .769) + (color.b * .189);
   outColor.g = (color.r * .349) + (color.g * .686) + (color.b * .168);
   outColor.b = (color.r * .273) + (color.g * .534) + (color.b * .131);
   outColor.w = 1;

   return outColor;
}

vec4 GrayScale(vec2 texCoord)
{
   vec4 color = texture(colorTarget, texCoord);
   vec4 outColor;

   outColor.r = (color.r * .5) + (color.g * .5) + (color.b * .5);
   outColor.g = (color.r * .5) + (color.g * .5) + (color.b * .5);
   outColor.b = (color.r * .5) + (color.g * .5) + (color.b * .5);
   outColor.w = 1;

   return outColor;
}

vec4 Invert(vec2 texCoord)
{
    vec4 startColor = texture(colorTarget, texCoord);

    return vec4(1 - startColor.r, 1 - startColor.g, 1 - startColor.b, 1);
}

float random (vec2 st) 
{
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

vec4 Scanlines(vec2 texCoord)
{
    float density = 30;
    float opacityScanline = .3;
    float opacityNoise = .2;
    float flickering = 0.03;

   vec4 color = texture(colorTarget, texCoord);
   float count = windowHeight * density;

   vec2 s1 = vec2(sin(texCoord.y * count), cos(texCoord.y * count));
   vec3 scanlines = vec3(s1.x, s1.y, s1.x);

   color.rgb += color.rgb * (scanlines * opacityScanline);
   color.rgb += color.rgb * vec3(random(texCoord * time)) * opacityNoise;
   color.rgb += color.rgb * sin(110.0 * time) * flickering;

   return vec4(color);
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
              FragColor = EdgeDetection(TexCoord);
             break;
        }

        case 3: // sepia
        {
              FragColor = Sepia(TexCoord);
             break;
        }

        case 4: // scanlines
        {
            FragColor = Scanlines(TexCoord);
            break;
        }

        case 5: // grayscale
        {
            FragColor = GrayScale(TexCoord);
            break;
        }

        case 6: // invert
        {
            FragColor = Invert(TexCoord);
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