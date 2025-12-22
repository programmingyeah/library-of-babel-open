#version 330 core

in vec3 vPos;
in vec3 vNorm;
in vec2 vUv;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};

uniform Material material;

vec3 uLightDir = vec3(-1,-1,-1);
vec3 uLightColor = vec3(1,1.0,0.8);
uniform vec3 uViewPos;

void main()
{
    vec3 N = normalize(vNorm);
    vec3 L = normalize(-uLightDir);

    float diff = max(dot(N, L), 0.0);

    vec3 diffuseColor = texture(material.texture_diffuse1, vUv).rgb;
    vec3 specularColor = texture(material.texture_specular1, vUv).rgb;

    vec3 V = normalize(uViewPos - vPos);
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(V, R), 0.0), 32.0);

    vec3 color =
        diffuseColor * diff * uLightColor +
        specularColor * spec * uLightColor;
    
    FragColor = vec4(color, 1.0);
}
