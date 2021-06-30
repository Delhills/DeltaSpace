varying vec2 v_uv;

uniform float u_time;

void main()
{
    vec2 uv = v_uv;
    
    float dist = abs(0.5-uv.x);
    // Time varying pixel color
    
    float time = -u_time*5.0;
    
    vec3 col;
    col.x = 0.5 + 0.5*cos(time+uv.y);
    col.y = 0.5 + 0.5*cos(time+uv.y+1.0);;
    col.z = 0.5 + 0.5*cos(time+uv.y+4.0);

    col*= 1.2 + mod((time+uv.y + dist), 1.0);
 
    gl_FragColor = vec4(col, 1.0);
}