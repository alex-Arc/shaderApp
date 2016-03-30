//.... something

//uniforms
//tempodivisions 1]
uniform vec2 u_resolution;//]
uniform float u_time;//0,1]
uniform sampler2D tex0;//0,1]
//uniforms


void main(  )
{
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    vec3 tex = texture2D(tex0, uv ).xyz;
    
    
    vec3 defaultColor = vec3(1.0);
    
    vec3 finalColor = defaultColor;
    float value = ceil(u_time / 10.0) - u_time / 10.0;
    vec2 aniuv = uv - value;
    finalColor *= abs (0.01 / sin((aniuv.y)) * 0.2 ) + 0.0;
    finalColor += tex * defaultColor;
    
    gl_FragColor = vec4(finalColor, 1.0);
}