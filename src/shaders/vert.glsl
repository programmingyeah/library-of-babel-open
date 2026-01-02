#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aUv;

// mat4 takes up locations 3,4,5,6
layout(location = 3) in mat4 instanceModel;

out vec3 vPos;
out vec3 vNorm;
out vec2 vUv;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = instanceModel * vec4(aPos, 1.0);
    vPos = worldPos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(instanceModel)));
    vNorm = normalize(normalMatrix * aNorm);

    vUv = aUv;
    gl_Position = projection * view * worldPos;
}
