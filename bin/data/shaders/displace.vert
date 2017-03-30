// the time value is passed into the shader by the OF app.
uniform float time;

uniform sampler2DRect tex0;
vec2 texelWidth = vec2(1.0, 1.0); 

void main() {
	vec2 uv = gl_MultiTexCoord0.xy; //tex coords
	vec4 sourceColor = texture2DRect(tex0, uv);

    float df = (0.30*sourceColor.r + 0.59*sourceColor.g + 0.11*sourceColor.b); // brightness calculation to create displacement float from rgb values
  	vec4 newVertexPos = gl_Vertex + vec4(gl_Normal * df * 100.0, 0.0); // regular vertex position + direction * displacementMap * displaceStrength

  	gl_Position = gl_ModelViewProjectionMatrix * newVertexPos;

    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
}