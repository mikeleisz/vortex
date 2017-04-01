uniform sampler2DRect tex1;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;;
    gl_FragColor = texture2DRect(tex1, uv);
}
