
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec3 u_topVector;

void main()
{
	gl_FragColor = vec4(abs(normalize(u_topVector)),1.0);
}
