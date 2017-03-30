uniform mat4 modelViewMatrix;

// the time value is passed into the shader by the OF app.
uniform float time;

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

vec2 texelWidth = vec2(1.0, 1.0);

uniform float downsampleAmt;
uniform float displaceAmt;

mat3 rotate2d(float _angle1, float _angle2, float _angle3){
    return mat3(cos(_angle1 + time),-sin(_angle1 + time), 0.0,
                sin(_angle2 + time), cos(_angle2 + time), 0.0,
                sin(_angle3 + time),-cos(_angle3 + time), 1.0);
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
	uv += vec2(0.5);

	vec4 sourceColor = texture2DRect(tex0, uv);

    float df = (0.30*sourceColor.r + 0.59*sourceColor.g + 0.11*sourceColor.b);

    vec3 hsv = rgb2hsv(sourceColor.rgb);
    vec3 norms = gl_Vertex.xyz * rotate2d(hsv.r, hsv.g, hsv.b);

  	vec4 newVertexPos = gl_Vertex + vec4(norms * df * displaceAmt, 0.0);

  	gl_Position = gl_ModelViewProjectionMatrix * newVertexPos;

    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
}