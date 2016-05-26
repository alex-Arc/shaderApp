// shader based on https://www.shadertoy.com/view/MtBSRW
// edited for own use Jonas Fehr

#ifdef GL_ES
precision mediump float;
#endif
//uniforms
//tempodivision 10]
uniform vec2 u_resolution;//100,1000;100,1000]
uniform float u_time;//0.15,.1,1.]
uniform float u_numDrops;//36.,0.,100.]
uniform float u_colorsThres;//0.6,.1,3.]
uniform float u_contrast;//1.2,0.,2.]
uniform float u_waveSize;//0.7,0.,3.]
uniform bool u_inverse;//]

//uniforms

float clamps(float a) {
    return clamp(a,0.,1.);
}
float make_wave(vec2 uv, vec2 position, float dist, float freq, float time) {
    return clamps(u_waveSize-(length(uv-position)*dist))*sin((length(uv-position)*freq)-time);
}
float rand(float co){
    return fract(sin(dot(vec2(co*1.21,co*2.131) ,vec2(12.9898,78.233))) * 431758.51453);
}
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
vec2 rand2(float co){
    return vec2(fract(sin(dot(vec2(co,co*2.) ,vec2(12.9898,78.233))) * 43758.5453),fract(sin(dot(vec2(co+1.,co*2.) ,vec2(12.9198,78.323))) * 43558.1453));
}
vec2 rand2(vec2 co){
    return vec2(fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453),fract(sin(dot(co.xy*3. ,vec2(12.9198,78.323))) * 43558.1453));
}

void main()
{
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    uv = vec2(((uv.x-0.5)*(u_resolution.x / u_resolution.y))+0.5,uv.y);
    
    float time = u_time;
    
    float waves = 0.;
    for (float i = 0.; i < u_numDrops; i++) {
        float delay = (rand(i*2.42)*2.)+1.;
        float time2 = time+(rand(i)*6.12);
        float fracttime = fract(time2/delay);
        waves += make_wave(uv,rand2(floor(time2/delay)+(i*2.)),((1.-(fracttime-0.1))*10.),100.+(rand(floor(time2/delay)+i)*20.),time2*4.)*(1.-fract(time2/delay));
    }
    
    
    
    float displays = (waves+1.);///u_colorsThres;
    displays = ((displays - 0.5) * max(u_contrast, 0.0)) + 0.5;
    displays = pow(displays, u_colorsThres);
    if(u_inverse) displays = 1.-displays;
    
    gl_FragColor = vec4(vec3(displays),1.0);
}