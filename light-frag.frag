#version 400
#extension GL_ARB_separate_shader_objects  : enable
#extension GL_ARB_shading_language_420pack : enable

//uniform float uKa, uKd, uKs;
//uniform vec4 uColor;
//uniform vec4 uLightSpecularColor;
//uniform float uShininess;


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
		vec4  uLightColor;
        vec4  uLightSpecularColor;
        float uShininess;
        vec4  uEyePos;
} Light;

layout( std140, set = 2, binding = 0 ) uniform miscBuf
{
		float uTime;
		int   uMode;
		int  uUseAmbient;
		int  uUseDiffuse;
		int  uUseSpecular;
} Misc;

layout( set = 3, binding = 0 ) uniform sampler2D uSampler;

layout ( location = 0 ) in vec3 vNs;
layout ( location = 1 ) in vec3 vLs;
layout ( location = 2 ) in vec3 vEs;
layout ( location = 3 ) in vec3 vColor;
layout ( location = 4 ) in vec2 vTexCoord;

layout ( location = 0 ) out vec4 fFragColor;


void main( )
{
	// Vectors
	vec3 Normal = normalize(vNs);
	vec3 LightPos = normalize(vLs);
	vec3 Eye = normalize(vEs);

	vec3 finalColor = vec3(0.,0.,0.);

	if (Misc.uMode == 1){
		finalColor = texture( uSampler, vTexCoord ).rgb;
	}
	else {
	
		// Lighting calculations
		vec4 ambient = Light.uKa * Light.uLightColor;
		float d = max( dot(Normal, LightPos), 0. );
		vec4 diffuse = Light.uKd * d * Light.uLightColor;
		float s = 0.;

		if( dot(Normal, LightPos) > 0. ) // only do specular if the light can see the point
		{
			vec3 ref = normalize( 2. * Normal * dot(Normal, LightPos) - LightPos );
			s = pow( max( dot(Eye, ref),0. ), Light.uShininess );
		}

		vec4 specular = Light.uKs * s * Light.uLightSpecularColor;


		// Get final color
		if (Misc.uUseAmbient != 0) { finalColor += ambient.rgb; }
		if (Misc.uUseDiffuse != 0) { finalColor += diffuse.rgb; }
		if (Misc.uUseSpecular != 0) { finalColor += specular.rgb; }
	}

	fFragColor = vec4( finalColor, 1.0 );
}