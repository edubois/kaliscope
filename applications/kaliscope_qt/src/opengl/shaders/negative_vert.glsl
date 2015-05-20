varying vec2 vTexCoord;
attribute vec2 textureCoords;
attribute highp vec3 vertex;

void main(void)
{
   vTexCoord = textureCoords;
   gl_Position = gl_ModelViewProjectionMatrix * vec4( vertex, 1.0 );
}
