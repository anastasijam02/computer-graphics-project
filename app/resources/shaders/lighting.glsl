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

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform SpotLight spot_light;

uniform vec3 view_position;
uniform sampler2D texture_diffuse1;

vec3 calculate_directional_light(
    DirectionalLight light,
    vec3 normal,
    vec3 view_direction,
    vec3 object_color
) {
    // Ambient
    vec3 ambient = light.ambient * object_color;

    // Diffuse
    vec3 light_direction = normalize(-light.direction);
    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diff * object_color;

    // Specular
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32.0);
    vec3 specular = light.specular * spec;

    return ambient + diffuse + specular;
}


vec3 calculate_point_light(
    PointLight light,
    vec3 normal,
    vec3 frag_position,
    vec3 view_direction,
    vec3 object_color
){
    vec3 light_direction = normalize(light.position - frag_position);

    // Ambient
    vec3 ambient = light.ambient * object_color;

    // Diffuse
    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diff * object_color;

    // Specular
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32.0);

    vec3 specular = light.specular * spec;

    float distance = length(light.position - frag_position);

    float attenuation =
        1.0 /
        (
            light.constant +
            light.linear * distance +
            light.quadratic * distance * distance
        );

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 calculate_spot_light(
    SpotLight light,
    vec3 normal,
    vec3 frag_position,
    vec3 view_direction,
    vec3 object_color
) {
    vec3 light_direction = normalize(light.position - frag_position);

    float theta = dot(light_direction, normalize(-light.direction));

    float epsilon = light.cutOff - light.outerCutOff;

    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Ambient
    vec3 ambient = light.ambient * object_color;

    // Diffuse
    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diff * object_color;

    // Specular
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32.0);
    vec3 specular = light.specular * spec;

    float distance = length(light.position - frag_position);

    float attenuation =
        1.0 /
        (
            light.constant +
            light.linear * distance +
            light.quadratic * distance * distance
        );

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}



void main(){

    vec3 object_color = texture(texture_diffuse1, TexCoords).rgb;
    vec3 norm = normalize(Normal);
    vec3 view_direction = normalize(view_position - FragPos);

    vec3 directional_result = calculate_directional_light(
                directional_light,
                norm,
                view_direction,
                object_color
    );

    vec3 point_result = calculate_point_light(
                point_light,
                norm,
                FragPos,
                view_direction,
                object_color
    );

    vec3 spot_result = calculate_spot_light(
            spot_light,
            norm,
            FragPos,
            view_direction,
            object_color
    );

    vec3 result = directional_result + point_result + spot_result;

    FragColor = vec4(result, 1.0);
}