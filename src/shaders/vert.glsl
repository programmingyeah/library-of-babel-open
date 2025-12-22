#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aUv;

out vec3 vPos;
out vec3 vNorm;
out vec2 vUv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    vPos = worldPos.xyz;

    // Proper normal transform
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vNorm = normalize(normalMatrix * aNorm);

    vUv = aUv;

    gl_Position = projection * view * worldPos;
}