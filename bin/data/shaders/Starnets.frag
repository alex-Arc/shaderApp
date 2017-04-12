// Star Nest by Pablo RomÃ¡n Andrioli

// This content is under the MIT License.

//uniforms
//tempodivision 100]
uniform vec2 u_resolution;//100,1000;100,1000]
uniform float u_time;//0.15,.01,1.]
uniform float u_xRot;//0.15,.1,1.]
uniform float u_yRot;//0.15,.1,1.]
uniform float u_zoom;//0.800,.1,1.]
uniform float u_brightness;//0.0015,.001,1.]
uniform float u_darkmatter;//0.300,.1,1.]
uniform float u_distfading;//0.730,.1,1.]
uniform float u_saturation;//0.850,.1,1.]
//uniforms


#define iterations 17
#define formuparam 0.53

#define volsteps 20
#define stepsize 0.1

// #define zoom   0.800
#define u_tile   0.850
// #define speed  0.010

// #define brightness 0.0015
// #define darkmatter 0.300
// #define distfading 0.730
// #define saturation 0.850


void main( )
{
	//get coords and direction
	vec2 uv= gl_FragCoord.xy/u_resolution.xy-.5;
	uv.y*=u_resolution.y/u_resolution.x;
	vec3 dir=vec3(uv*u_zoom,1.);
	float time=u_time+.25;

	//mouse rotation
	float a1=.5+u_xRot;
	float a2=.8+u_yRot;
	mat2 rot1=mat2(cos(a1),sin(a1),-sin(a1),cos(a1));
	mat2 rot2=mat2(cos(a2),sin(a2),-sin(a2),cos(a2));
	dir.xz*=rot1;
	dir.xy*=rot2;
	vec3 from=vec3(1.,.5,0.5);
	from+=vec3(time*2.,time,-2.);
	from.xz*=rot1;
	from.xy*=rot2;

	//volumetric rendering
	float s=0.1,fade=1.;
	vec3 v=vec3(0.);
	for (int r=0; r<volsteps; r++) {
		vec3 p=from+s*dir*.5;
		p = abs(vec3(u_tile)-mod(p,vec3(u_tile*2.))); // tiling fold
		float pa,a=pa=0.;
		for (int i=0; i<iterations; i++) {
			p=abs(p)/dot(p,p)-formuparam; // the magic formula
			a+=abs(length(p)-pa); // absolute sum of average change
			pa=length(p);
		}
		float dm=max(0.,u_darkmatter-a*a*.001); //dark matter
		a*=a*a; // add contrast
		if (r>6) fade*=1.-dm; // dark matter, don't render near
		//v+=vec3(dm,dm*.5,0.);
		v+=fade;
		v+=vec3(s,s*s,s*s*s*s)*a*u_brightness*fade; // coloring based on distance
		fade*=u_distfading; // distance fading
		s+=stepsize;
	}
	v=mix(vec3(length(v)),v,u_saturation); //color adjust
	gl_FragColor = vec4(v*.01,1.);

}
