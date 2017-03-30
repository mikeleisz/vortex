uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

void main()
{
    gl_FragColor = texture2DRect(tex1, gl_TexCoord[0].xy);
}
