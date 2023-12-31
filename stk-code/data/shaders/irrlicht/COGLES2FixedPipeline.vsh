/* Attributes */

attribute vec3 inVertexPosition;
attribute vec3 inVertexNormal;
attribute vec4 inVertexColor;
attribute vec2 inTexCoord0;
//attribute vec2 inTexCoord1;

/* Uniforms */

uniform mat4 uMvpMatrix;

uniform vec2 uTextureTrans0;
//uniform mat4 uTextureMatrix1;

/* Varyings */

varying vec2 varTexCoord0;
//varying vec2 varTexCoord1;
varying vec4 varVertexColor;
varying float varEyeDist;

void main(void)
{
	gl_Position = uMvpMatrix * vec4(inVertexPosition,1.0);

	varTexCoord0 = inTexCoord0 + uTextureTrans0;

	//vec4 TexCoord1 = vec4(inTexCoord1.x, inTexCoord1.y, 0.0, 0.0);
	//varTexCoord1 = vec4(uTextureMatrix1 * TexCoord1).xy;

	varVertexColor = inVertexColor.zyxw;
}
