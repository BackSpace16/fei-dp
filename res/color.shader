#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * position;
}

#shader fragment
#version 330 core

uniform vec4 out_color;
layout(location = 0) out vec4 color;

void main()
{
   color = out_color;
}
