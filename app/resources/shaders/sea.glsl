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

struct PointLight{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform PointLight point_light;


struct SpotLight{
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

vec3 calculate_point_light(
    PointLight light,
    vec3 normal,
    vec3 frag_position,
    vec3 water_color
)
{
    vec3 light_direction = normalize(light.position - frag_position);

    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diff * water_color;

    float distance = length(light.position - frag_position);

    float attenuation =
        1.0 /
        (
            light.constant +
            light.linear * distance +
            light.quadratic *
            distance *
            distance
        );

    diffuse *= attenuation;

    return diffuse;
}


vec3 calculate_spot_light(
    SpotLight light,
    vec3 normal,
    vec3 frag_position,
    vec3 water_color
)
{
    vec3 light_direction = normalize(light.position - frag_position);

    float theta = dot(light_direction, normalize(-light.direction));

    float epsilon = light.cutOff - light.outerCutOff;

    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diff * water_color;

    float distance = length(light.position - frag_position);

    float attenuation =
        1.0 /
        (
            light.constant +
            light.linear * distance +
            light.quadratic *
            distance *
            distance
        );


    diffuse *= attenuation * intensity;

    return diffuse;
}


void main(){
    vec3 water_color = texture(water_texture, TexCoords).rgb;
    water_color *= 0.35;

    vec3 normal = vec3(0.0, 1.0, 0.0);


    vec3 point_result =
        calculate_point_light(
            point_light,
            normal,
            FragPos,
            water_color
        );


    vec3 spot_result =
        calculate_spot_light(
            spot_light,
            normal,
            FragPos,
            water_color
        );


    vec3 result = water_color + point_result + spot_result;

    FragColor = vec4(result, 1.0);

}