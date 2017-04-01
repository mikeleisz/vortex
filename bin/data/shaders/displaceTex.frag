uniform sampler2DRect tex1;
uniform sampler2DRect tex2;

uniform float time;
uniform float displaceAmt;

uniform vec2 resolution;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;

    vec2 displace_uv = vec2(uv.x + time, uv.y + time);
    if (displace_uv.x > resolution.x){
    	displace_uv.x -= resolution.x;
    }
    if (displace_uv.y > resolution.y){
    	displace_uv.y -= resolution.y;
    }

    vec2 displace = texture2DRect(tex2, displace_uv).xy;
    displace = ((displace * (2.0 * resolution.x)) 
    					  - (1.0 * resolution.x)) * (displaceAmt*0.0001);

    gl_FragColor = texture2DRect(tex1, uv + displace);
}
