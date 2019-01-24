#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 normal_position;

out vec3 vertex_viewspace;
out vec3 fragment_color;
out vec3 light_position;
out float light_intensity;

uniform mat4 MVP;
uniform mat4 model_matrix;
uniform vec3 diffuse_color;
uniform vec3 light_position_in;
uniform float light_intensity_in;

void main(){
	gl_Position =  MVP * vec4(vertex_position,1);
    vertex_viewspace = (model_matrix * vec4(vertex_position, 1)).xyz;
    fragment_color = diffuse_color;
    light_intensity = light_intensity_in;
    light_position = light_position_in;
}

