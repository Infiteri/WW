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

uniform float uTime; // increment each frame

vec3 palette(float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);
    return a + b * cos(6.28318 * (c * t + d));
}

void main() {
    // Correct for 16:9 aspect ratio
    vec2 uv = vUVs * 2.0 - 1.0;
    uv.x *= 1920.0 / 1080.0; // aspect correction
    vec2 uv0 = uv;

    vec3 finalColor = vec3(0.0);

    for (float i = 0.0; i < 4.0; i++) {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i * 0.4 + uTime * 0.4);

        d = sin(d * 8.0 + uTime) / 8.0;
        d = abs(d);
        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }

    oColor = vec4(finalColor, 1.0);
}
