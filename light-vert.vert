#version 400
#extension GL_ARB_separate_shader_objects  : enable
#extension GL_ARB_shading_language_420pack : enable


// non-opaque must be in a uniform block:
layout( std140, set = 0, binding = 0 ) uniform matBuf
{
        mat4 uModelMatrix;
        mat4 uViewMatrix;
        mat4 uProjectionMatrix;
        mat4 uNormalMatrix;
} Matrices;

layout( std140, set = 1, binding = 0 ) uniform lightBuf
{
        float uKa;
        float uKd;
        float uKs;
        vec4  uLightPos;
        vec3  uLightSpecularColor;
        float uShininess;
        vec4  uEyePos;
} Light;

layout( location = 0 ) in vec3 aVertex;
layout( location = 1 ) in vec3 aNormal;
layout( location = 2 ) in vec3 aColor;
layout( location = 3 ) in vec2 aTexCoord;

layout ( location = 0 ) out vec3 vNs;
layout ( location = 1 ) out vec3 vLs;
layout ( location = 2 ) out vec3 vEs;
layout ( location = 3 ) out vec3 vColor;
layout ( location = 4 ) out vec2 vTexCoord;


void main( )
{
	// Texture coords
	vTexCoord = aTexCoord;

	// Vertices
	vec4 ECposition = Matrices.uViewMatrix * Matrices.uModelMatrix * vec4(aVertex, 1);

	// Normal
	vNs = (Matrices.uNormalMatrix * vec4(aNormal, 1)).xyz;

	// Positions
	vLs = (Light.uLightPos - ECposition).xyz; // vector from the point
	vEs = vec3( 0., 0., 0. ) - ECposition.xyz; // vector from the point

	vColor    = aColor;
	vTexCoord = aTexCoord;
	gl_Position = Matrices.uProjectionMatrix * Matrices.uViewMatrix * Matrices.uModelMatrix * vec4( aVertex, 1. );
}