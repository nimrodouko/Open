#version 330 core


in vec2 thetex;
out vec3 fragmentsout;
uniform sampler2D diffuseaspect;



void main(){

fragmentsout = texture(diffuseaspect, thetex);

}