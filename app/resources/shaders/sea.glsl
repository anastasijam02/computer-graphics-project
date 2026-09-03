//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    TexCoords = aTexCoords;

    FragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;

uniform sampler2D water_texture;


struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform PointLight point_light;


void main(){
    vec3 water_color = texture(water_texture, TexCoords).rgb;

    vec3 normal = vec3(0.0, 1.0, 0.0);

    vec3 light_direction = normalize(point_light.position - FragPos);

    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = point_light.diffuse * diff * water_color;

    float distance = length(point_light.position - FragPos);

    float attenuation =
        1.0 /
        (
            point_light.constant +
            point_light.linear * distance +
            point_light.quadratic * distance * distance
        );

    diffuse *= attenuation;

    vec3 result = water_color + diffuse;

    FragColor = vec4(result, 1.0);

}