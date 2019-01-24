#version 330 core

in vec3 vertex_viewspace;
in vec3 fragment_color;
in vec3 light_position;
in float light_intensity;

out vec4 color;

void main(){
    vec3 U = dFdx(vertex_viewspace);
    vec3 V = dFdy(vertex_viewspace);
    vec3 N = normalize(cross(U,V));

    float d = dot(light_position, N);
    d = clamp(d, 0.3, 1);
    vec3 color_with_light = fragment_color * d * light_intensity;

    color = vec4(color_with_light.x, color_with_light.y, color_with_light.z, 1.0);
}
