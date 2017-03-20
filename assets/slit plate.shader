#version 440
uniform vec2 resolution;
uniform vec3 position;
uniform vec3 color;

out vec4 fragOut;

const float pi = atan(1.0)/4.0;
const float radius = 400.0;
const float width = 0.0;
const float feather = 4.0;

float torus(vec2 uv)
{
    float d = (1.0/resolution.x)*2.0;
    float t = abs(length(uv)-d*radius);
    vec2 k = vec2(d*vec2(width+vec2(feather,0)));
    return smoothstep(k.x,k.y,t);
}
void main()
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    uv=-1.0+2.0*uv;
    uv.x *= resolution.x/resolution.y;

    uv /= position.z;

    float t = torus(uv);

    fragOut = vec4(color * t,1.0);
}
