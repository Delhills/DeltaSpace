uniform int u_lap;

void main( out vec4 fragColor, in vec2 fragCoord ) {
    vec4 color;

    if (u_lap == 1){
        color.xyz = vec3(1.0, 0.0, 0.0);
    }
    else if (u_lap == 2){
        color.xyz = vec3(1.0, 0.65, 0.0);
    }
    else{
        color.xyz = vec3(1.0, 0.84, 0.0);
    }
    
    gl_FragColor = color;
}
