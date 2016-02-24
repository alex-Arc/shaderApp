// shader based on https://www.shadertoy.com/view/MtBSRW
//

#ifdef GL_ES
precision mediump float;
#endif
//uniforms
//tempodivision 100]
uniform vec2 u_resolution;//100,1000;100,1000]
uniform float u_time;//0,50]
uniform float u_zoom;//1,10]
uniform float u_date;//1,3000]
uniform bool u_linesX;//1,3000]
uniform bool u_linesY;//1,3000]
//uniforms

// quadrilin: 252 cubic:269   + 21 for noise transform

#define h(n) fract(sin(n+vec2(0,157))*57.)
#define N m=fract(u*s); l=dot(u*s-m,vec2(1,157));s+=s; m*=m*(3.-m-m); r= mix(h(l),h(++l),m.x); f+= mix(r.x,r.y,m.y)/s;


void main() {
    
    vec2 u = gl_FragCoord.xy;
    //u.x+=u_time;
    
    if(u_linesY){
        u = gl_FragCoord.yy;
    }
    if(u_linesX){
        u = gl_FragCoord.xx;
    }
    //vec2 u = vec2(0.0);
    vec4 f = vec4(sin(u_time/1000.));
    
    u = (u_zoom)*u/u_resolution.y-vec2(7,4);
    vec2 m,r; float l,s=1.;
    
    N N N N
    
    // --- comment all below for brute noise, or chose your noise transform :
    f = sin(f*20.+u_time);
    
    gl_FragColor = f;
}