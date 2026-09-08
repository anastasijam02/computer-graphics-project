//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 LocalPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    LocalPos = aPos;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}


//#shader fragment
#version 330 core

in vec3 LocalPos;

out vec4 FragColor;

void main(){
    float length_fade = 1.0 - smoothstep(2.0, 15.0, LocalPos.z);

    float alpha = 0.08 * length_fade;

    FragColor = vec4(1.0, 0.82, 0.35, alpha);
}