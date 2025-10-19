// VERTEX
#version 330 core

layout(location = 0) in vec3 aPos;

// Instance model matrix (4 vec4 columns)
layout(location = 1) in vec4 aModelCol0;
layout(location = 2) in vec4 aModelCol1;
layout(location = 3) in vec4 aModelCol2;
layout(location = 4) in vec4 aModelCol3;

// Instance material as vec4
layout(location = 5) in vec4 aMaterialColor;

uniform mat4 view;
uniform mat4 projection;

out vec4 vColor;

void main()
{
    mat4 model = mat4(aModelCol0, aModelCol1, aModelCol2, aModelCol3);
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Construct material struct if you want
    struct Material { vec4 color; };
    Material mat;
    mat.color = aMaterialColor;

    vColor = mat.color;
}


// FRAGMENT
#version 330 core

in vec4 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vColor;
}
