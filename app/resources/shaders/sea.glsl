//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D water_texture;

void main()
{
    vec3 water_color = texture(water_texture, TexCoords).rgb;
    water_color *= 0.35;

    FragColor = vec4(water_color, 1.0);
}