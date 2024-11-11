#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

layout(location = 2) in mat4 instanceModel;
layout(location = 6) in vec3 instanceColor;

out vec3 Normal;
out vec3 FragPos;
out vec3 objectColor;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * instanceModel  * position;
    FragPos = vec3(instanceModel * position);
    Normal = vec3(mat3(instanceModel) * normal);
    objectColor = instanceColor;
}

#shader fragment
#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec3 objectColor;
out vec4 color;

uniform vec3 lightPos; 
uniform vec3 lightColor;

void main()
{
    // Ambient
    float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    color = vec4(result, 1.0f);
}
