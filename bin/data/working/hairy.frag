// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

// Volumetric clouds. It performs level of detail (LOD) for faster rendering

//uniforms
//tempodivisions 1]
uniform vec2 u_resolution;//]
uniform float u_time;//0,1]
uniform float u_contrast;//0,1]
uniform float divisions;//1,10]
uniform float u_smooth;//1,60]
uniform sampler2D tex;//img1.png]
uniform bool enableCircle;//]
uniform bool u_inverse;//]
uniform bool u_horisontal;//]
//uniforms

// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
                 43758.5453123);
}

//float hash( float n )
//{
//    return fract(sin(n)*u_time*0.1);
//}
//
//float noise( in vec2 x )
//{
//    vec2 p = floor(x);
//    vec2 f = fract(x);
//    f = f*f*(3.0-2.0*f);
//    float n = p.x + p.y*57.0;
//    return mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
//               mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y);
//}

// 2D Random
float hash (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    
    // Four corners in 2D of a tile
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    
    // Smooth Interpolation
    
    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);
    
    // Mix 4 coorners porcentages
    return mix(a, b, u.x) +
    (c - a)* u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;
}

float pcurve( float x, float a, float b )
{
    float k = pow(a+b,a+b) / (pow(a,a)*pow(b,b));
    return k * pow( x, a ) * pow( 1.0-x, b );
}


float parabola( float x, float k )
{
    return pow( 1.0*x*(1.0-x), k );
}

vec2 map( vec2 p, in float offset )
{
    p.x += 0.1*pcurve( u_time*0.1  , .001 ,0.) ;
    p.y += 0.1*pcurve( u_time*0.1 , 1. , 0. ) ; ;
    
    float a = noise(p*1.731 + sin(0.1*u_time))*6.9831;
    //float b = noise(p*0.976 + sin(0.1*u_time))*6.7831;
    a -= offset;
   // b -= offset;
    return vec2( cos(a), sin(a) );
}

void main()
{
    vec2 p = gl_FragCoord.xy / u_resolution.xy;
    if(u_horisontal){
        p =gl_FragCoord.xx / u_resolution.xy;
    }
    
    vec2 uv = -1.0 + divisions*p;
    uv.x *= u_resolution.x / u_resolution.y;
    uv.y *= u_resolution.y / u_resolution.x;
    
    float offset = u_time + gl_FragCoord.y/u_resolution.x;
    
    float acc = 0.0;
    vec3  col = vec3(0.0);
    for( int i=0; i<32; i++ )
    {
        vec2 dir = map( uv, offset );
        
        float h = float(i)/32.;
        float w = 4.0*h*(1.0-h);
        
        vec3 ttt = w*texture2D( tex, uv ).xyz;
        ttt *= mix( vec3(0.6,0.7,0.7), vec3(1.0,0.95,0.9), 0.5 - 0.5*dot( reflect(vec3(dir,0.0), vec3(1.0,0.0,0.0)).xy, vec2(0.707) ) );
        col += w*ttt;
        acc += w;
        
        uv += 0.008*dir;
    }
    col /= acc;
    
    float gg = dot( col, vec3(0.333) );
    vec3 nor = normalize( vec3( dFdx(gg), 0.5, dFdy(gg) ) );
    col += vec3(u_contrast)*dot( nor, vec3(0.7,0.71,0.7) );
    
    vec2 di = map( uv, offset );
    col *= 0.65 + 0.35*dot( di, vec2(0.707) );
    col *= 0.20 + 0.80*pow( 4.0*p.x*(1.0-p.x), 0.1 );
    col *= 1.7;
    
    if(u_inverse){
        col = 1.-col;
    }
   // col = pow(col,2.);
    
    if(enableCircle){
        vec2 st = gl_FragCoord.xy/u_resolution;
        float v = 0.0;
        v = 1.-distance(st,vec2(0.5));
        v = smoothstep(.5,.75,v);
        
        gl_FragColor = vec4( vec3(col*v), col*1.0);
    }
    else{
        gl_FragColor = vec4( col, 1.0);
    }
}
