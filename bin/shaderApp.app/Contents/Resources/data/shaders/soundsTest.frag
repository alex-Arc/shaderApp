// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

// Volumetric clouds. It performs level of detail (LOD) for faster rendering

//uniforms
//tempodivisions 1]
uniform vec2 u_resolution;//]
uniform float u_time;//0,1]
//uniform float u_gamma;//1,10]
//uniform sampler2D tex0;//img1.png]
uniform sampler2D sounds;//img1.png]
uniform float soundInput;//sounds]

//uniforms

// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

void main(  )
{
    // create pixel coordinates
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    
    // first texture row is frequency data
    float fft  = texture2D( sounds, vec2(uv.x,0.25) ).x;
    
    // second texture row is the sound wave
    float wave = texture2D( sounds, vec2(uv.x,0.75) ).x;
    
    // convert frequency to colors
    vec3 col = vec3( fft, 4.0*fft*(1.0-fft), 1.0-fft ) * fft;
    
    // add wave form on top
    col += soundInput -  smoothstep( 0.0, 0.15, abs(wave - uv.y) );
    
    // output final color
    gl_FragColor = vec4(col,1.0);
}