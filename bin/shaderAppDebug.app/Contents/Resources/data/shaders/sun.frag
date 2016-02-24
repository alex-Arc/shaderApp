// shader based on https://www.shadertoy.com/view/MtBSRW
// edited for own use Jonas Fehr

#ifdef GL_ES
precision mediump float;
#endif
//uniforms
//tempodivision 10]
uniform vec2 u_resolution;//100,1000;100,1000]
uniform float u_time;//.1,1.]
uniform float u_backgroundsColor;//.0,1.]
uniform float u_amountLines;//.1,1.]
uniform float u_wavemask;//0.0,10.0]
//uniforms

float hash(float n) {
    return fract(sin(n) * 43758.5453);
}

float noise(in vec2 x) {
    vec2 p = floor(x);
    vec2 f = fract(x);
    
    f = f * f * (3.0 - 2.0 * f);
    
    float n = p.x + p.y * 57.0;
    
    float res = mix(mix(hash(n +  0.0), hash(n +  1.0), f.x),
                    mix(hash(n + 57.0), hash(n + 58.0), f.x), f.y);
    
    return res;
}

float sun(vec2 p) {
    p.x += sin(p.y * 2.0 + u_time) * 0.05;
    p.y += cos(p.x * 0.5 + u_time) * 0.01;
    
    float sun = length(p);
    sun = 1.0 - smoothstep(0.39, 0.4, sun);
    
    return sun;
}

float wave_mask(vec2 p) {
    p /= 1.01;
    
    p.x += sin(p.y + hash(p.y) + u_time) * 0.05;
    p.y += cos(p.x + hash(p.x) + u_time) * 0.05;
    
    float wave_mask = length(p);
    
    wave_mask = u_wavemask - smoothstep(0.39, 0.4, wave_mask);
    
    
    return wave_mask;
}

float waves(vec2 p) {
    p.y += cos(p.x * 15.0 * noise(p * 15.25) * length(p) / 10.0 + u_time* 0.2);
    
    float lines = u_amountLines;
    p.y = smoothstep(0.0, lines, mod(p.y, lines));
    
    float waves = abs(p.y);
    waves = smoothstep(0.3, 0.2, waves);
    
    return waves;
}


void main(  ) {
    vec2 p = gl_FragCoord.xy;
    
    p /= u_resolution.xy;
    p -= 0.5;
    p.x *= u_resolution.x / u_resolution.y;
    
    vec3 col = vec3(0.0);
    
    p.y *= -1.0;
    col -= min(sun(p), 1.0 - waves(p)) * vec3(p.y - 0.5, p.y - 0.5, p.y - 0.5) * 1.0;
    col -= min(waves(p), wave_mask(p));
    col += (1.0 - sun(p)) * u_backgroundsColor - vec3(0.0);
    
    
    gl_FragColor = vec4(col,1.0);
}