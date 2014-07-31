#version 120
uniform sampler2DRect s_texture;
void main()
{
    gl_FragColor = texture2DRect( s_texture,  gl_TexCoord[0].st);
}