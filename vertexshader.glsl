
#version 330 core
layout (location = 0) in vec3 apos;
layout (location =1 ) in vec3 anormal;
layout(location = 2) in vec2 atex;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 Normal;
out vec3 fragmentposition;
out vec2 thetex;

void main()
{
    thetex = atex;
    fragmentposition = vec3(model * vec4(apos,1.0)); 
    Normal = mat3(transpose(inverse(model))) * anormal;
    gl_Position = projection * view  * vec4(fragmentposition, 1.0);

} 