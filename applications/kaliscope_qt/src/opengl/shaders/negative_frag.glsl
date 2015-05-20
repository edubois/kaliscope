#version 110

varying vec2 vTexCoord;
uniform sampler2D myTexture;

void main()
{
    vec4 texColor = texture2D( myTexture, vTexCoord );
    texColor.rgb = 1.0 - texColor.rgb;
    gl_FragColor = vec4( texColor.rgb, 1.0 );
}
