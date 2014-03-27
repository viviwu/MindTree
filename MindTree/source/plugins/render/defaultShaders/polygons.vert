#version 330
uniform mat4 modelView;
uniform mat4 projection;
in vec3 P;
in vec3 N;
out vec3 pos;
out vec3 sn;
void main(){
   gl_Position = projection * modelView * vec4(P, 1);
   pos = (modelView * vec4(P, 1)).xyz;
   sn = (modelView * vec4(N, 0)).xyz;
};