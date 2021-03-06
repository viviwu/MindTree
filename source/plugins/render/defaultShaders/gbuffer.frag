#version 330

in vec3 pos;
in vec3 worldPos;
in vec3 cameraPos;
in vec3 cameraNormal;
in vec3 sn;
in vec3 worldNormal;

uniform float has_polygon_color = 0.0;
uniform vec4 diffuse_color = vec4(1);
uniform float specular_intensity = .5;
uniform float diffuse_intensity = .8;

out vec4 outnormal;
out vec4 outposition;
out vec4 worldposition;
out vec4 outdiffusecolor;
out vec4 outdiffuseintensity;
out vec4 outspecintensity;
out vec4 outcolor;

uniform int flatShading = 0;

uniform sampler1D polygon_color;

vec3 Nn;

void main() {
    Nn = mix(normalize(sn), normalize(cross(dFdx(pos), dFdy(pos))), flatShading);
    outnormal = vec4(Nn, 1);
    outposition = vec4(pos, 1);
    worldposition = vec4(worldPos, 1);

    //outdiffusecolor = vec4(vec3(has_polygon_color), 1.0);
    outdiffusecolor = diffuse_color;
    //outdiffusecolor = mix(diffuse_color,
    //                      texelFetch(polygon_color, gl_PrimitiveID, 0),
    //                      has_polygon_color);
    outdiffuseintensity = vec4(vec3(diffuse_intensity), 1);
    outspecintensity = vec4(vec3(specular_intensity), 1);
    //outcolor = vec4(0);
}
