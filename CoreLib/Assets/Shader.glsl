// VERTEX
#version 330 core

// Vertex Attributes
layout(location = 0) in vec3 aPos;               // Vertex position
layout(location = 1) in vec4 aModelCol0;         // Instance: model matrix column 0
layout(location = 2) in vec4 aModelCol1;         // Instance: model matrix column 1
layout(location = 3) in vec4 aModelCol2;         // Instance: model matrix column 2
layout(location = 4) in vec4 aModelCol3;         // Instance: model matrix column 3
layout(location = 5) in vec4 aMaterialColor;     // Instance: color (r,g,b,a)
layout(location = 6) in vec2 aUV;                // Vertex UVs
layout(location = 7) in float aMaterialTexIndex; // Instance: texture index

// Uniforms
uniform mat4 view;
uniform mat4 projection;

// Outputs to fragment shader
out vec4 vColor;
out vec2 vUVs;
out float vTexIndex;

void main()
{
    // Rebuild model matrix
    mat4 model = mat4(aModelCol0, aModelCol1, aModelCol2, aModelCol3);

    // Final position
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Pass data to fragment
    vColor = aMaterialColor;
    vUVs = aUV;
    vTexIndex = aMaterialTexIndex;
}


// FRAGMENT
#version 330 core

in vec4 vColor;
in vec2 vUVs;
in float vTexIndex;

uniform sampler2D uTextures[16];

out vec4 FragColor;

void main()
{
    int texIndex = int(vTexIndex);

    vec4 texColor = texture(uTextures[texIndex], vUVs);
    FragColor = texColor * vColor;
}
