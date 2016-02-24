// shader based on https://www.shadertoy.com/view/MtBSRW
// edited for own use Jonas Fehr

#ifdef GL_ES
precision mediump float;
#endif
//uniforms
//tempodivision 10]
uniform vec2 u_resolution;//100,1000;100,1000]
uniform float u_time;//.1,1.]
uniform float u_zoom;//.1,1.]
uniform float u_colorsThres;//.1,1.]
//uniforms

float compute(vec2 p, float i){
    float a = atan(p.y,p.x);
    float l = length(p);
    float d = mod(l-i*.5,1.);
    d-=.5;
    d = abs(d);
    d-=sin(d)*.05+.1;
    d = abs(d);
    d/=sin(a*12.+sin(l*l+i*.3)*10.)+2.;
    return 1.0-smoothstep(0.,.05, d/max(.15,p.y));
}

void main(  )
{
    float i=  u_time*.5;
    vec4 c = vec4(1.0);
    vec2 uv = gl_FragCoord.xy / u_resolution.xx*u_zoom-1.;
    uv.y += 1.;
    
    float l = length(uv);
    float a = atan(uv.y,uv.x);
    float deg = sin(i)*2.+8.;
    float j = sin(i)*.3;
    float d = sin(i*3.1415)*.03;
    uv*=mat2(sin(a*deg+j-deg)*l,cos(a*deg-deg)*l,-cos(a*deg+j-deg)*l,sin(a*deg-deg)*l);
    
    
    c.r = compute(uv,i);
    uv.y-=d;
    c.g = compute(uv,i);
    uv.y+=d*2.;
    c.b = compute(uv,i);
    
    if(c.r>u_colorsThres||c.b>u_colorsThres||c.g>u_colorsThres){
        c.b=8.0;
        c.g=8.0;
        c.r=8.0;
        
    }
    
    gl_FragColor = c;
}