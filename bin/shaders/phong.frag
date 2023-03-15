// a phong shader
#version 410

in vec3 vNormal;

out vec4 FragColor;

uniform vec4 BaseColor;
uniform vec3 LightDirection; 

void main()
{
    //Set the normal and light direction 
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

    // Calculate the negative light direction (Lambert Term)
    float lambertTerm = max(0, min(1, dot(N, -L)));

    FragColor = vec4(lambertTerm, lambertTerm, lambertTerm, 1);
}