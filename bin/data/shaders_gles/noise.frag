#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif
uniform sampler2D s_texture;
uniform vec4 globalColor;

void main()
{
    gl_FragColor = texture2DRect( s_texture,  gl_TexCoord[0].st);
//    texture2D( samplerForMyTexture, vec2( x, y ) );
}
