#version 330 core
layout (location = 0) in vec3 apos;
layout (location =1 ) in vec3 anormal;
layout(location = 2) in vec2 atex;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 thetex;

void main()
{
    thetex = atex;
    gl_Position = projection * view  * vec4(fragmentposition, 1.0);

} 