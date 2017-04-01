uniform mat4 modelViewMatrix;

uniform sampler2DRect tex0;

uniform float time;

uniform float downsampleAmt;
uniform float displaceAmt;

mat3 rotate3d(float _angle1, float _angle2, float _angle3){
    return mat3(cos(_angle1),-sin(_angle1), 0.0,
                sin(_angle2), cos(_angle2), 0.0,
                sin(_angle3),-cos(_angle3), 1.0);
}

mat3 rotate3d(float _angle1, float _angle2, float _angle3, float _t){
    return mat3(cos(_angle1 + _t),-sin(_angle1 + _t), 0.0,
                sin(_angle2 + _t), cos(_angle2 + _t), 0.0,
                sin(_angle3 + _t),-cos(_angle3 + _t), 1.0);
}

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}


void main() {
	vec2 uv = gl_MultiTexCoord0.xy; //tex coords
	uv *= mat2(downsampleAmt, 0.0, 0.0, downsampleAmt);

	vec4 sourceColor = texture2DRect(tex0, uv);

    float df = (0.30*sourceColor.r + 0.59*sourceColor.g + 0.11*sourceColor.b);

    vec3 hsv = rgb2hsv(sourceColor.rgb);

    vec3 norms = gl_Normal;
    norms *= rotate3d(hsv.r, hsv.g, hsv.b, time);

  	vec4 newVertexPos = gl_Vertex + vec4(norms * df * displaceAmt, 0.0);

  	gl_Position = gl_ModelViewProjectionMatrix * newVertexPos;

    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
}