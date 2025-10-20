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

uniform float uTime;

void main() {
    // Center coordinates
    vec2 uv = vUVs - 0.5;
    float time = uTime / 5;

    // Flowing wave movement
    float waveX = sin(uv.y * 10.0 + time * 0.8) * 0.05;
    float waveY = cos(uv.x * 10.0 + time * 0.7) * 0.05;
    uv += vec2(waveX, waveY);

    // Radius and angle for radial patterns
    float radius = length(uv);
    float angle = atan(uv.y, uv.x);

    // Smooth dynamic colors
    vec3 color;
    color.r = 0.5 + 0.5 * sin(time + radius * 6.0);
    color.g = 0.5 + 0.5 * sin(time * 0.7 + angle * 5.0);
    color.b = 0.5 + 0.5 * cos(time * 0.9 + radius * 3.0);

    // Soft vignette for depth
    float vignette = smoothstep(0.5, 0.0, radius);

    oColor = vec4(color * vignette, 1.0);
}
