//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}


//#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionalLight directional_light;
uniform vec3 view_position;

uniform sampler2D texture_diffuse1;

void main()
{
    vec3 object_color = texture(texture_diffuse1, TexCoords).rgb;

    // AMBIENT
    vec3 ambient = directional_light.ambient * object_color;

    // DIFFUSE
    vec3 norm = normalize(Normal);
    vec3 light_direction = normalize(-directional_light.direction);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = directional_light.diffuse * diff * object_color;

    // SPECULAR
    vec3 view_direction = normalize(view_position - FragPos);
    vec3 reflect_direction = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32.0);
    vec3 specular = directional_light.specular * spec;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}