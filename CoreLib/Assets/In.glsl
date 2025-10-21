// VERTEX
#version 330 core
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aUVs;

out vec2 vUVs;

void main() {
    vUVs = aUVs;
    gl_Position = vec4(aPosition, 0.0, 1.0);
}


// FRAGMENT
#version 330 core

in vec2 vUVs;
out vec4 oColor;

uniform sampler2D uScreenTexture;

void main() {
    vec3 color = texture(uScreenTexture, vUVs).rgb;
    color = vec3(1.0) - color; // invert
    oColor = vec4(color, 1.0);
}
