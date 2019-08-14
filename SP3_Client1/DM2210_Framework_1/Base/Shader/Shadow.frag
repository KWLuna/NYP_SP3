#version 330 core

// Interpolated values from the vertex shaders
in vec3 vertexPosition_cameraspace;
in vec3 fragmentColor;
in vec3 vertexNormal_cameraspace;
in vec2 texCoord;
in vec2 blurVec;
in vec4 shadowCoord;

// Ouput data
out vec4 color;

struct Light {
	int type;
	vec3 position_cameraspace;
	vec3 color;
	float power;
	float kC;
	float kL;
	float kQ;
	vec3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;
};

struct Material {
	vec3 kAmbient;
	vec3 kDiffuse;
	vec3 kSpecular;
	float kShininess;
};

float getAttenuation(Light light, float distance) {
	if(light.type == 1)
		return 1;
	else
		return 1 / max(1, light.kC + light.kL * distance + light.kQ * distance * distance);
}

float getSpotlightEffect(Light light, vec3 lightDirection) {
	vec3 S = normalize(light.spotDirection);
	vec3 L = normalize(lightDirection);
	float cosDirection = dot(L, S);
	//return smoothstep(light.cosCutoff, light.cosInner, cosDirection);
	if(cosDirection < light.cosCutoff)
		return 0;
	else
		return 1; //pow(cosDirection, light.exponent);
}

struct FogParam
{
	vec3 color;		// Fog color
	float start;	// For linear fog
	float end;		// For linear fog
	float density;	// For exponential fog
	int type;		// 0 = linear, 1 = exp, 2 = exp2
	bool enabled;	// Toggle fog
};

float getFogFactor(FogParam fog, float fogDistance)
{
	if(fog.enabled == false)
		return 0.0;

	float factor = 0.0;
	if(fog.type == 0) 
		factor = (fog.end - fogDistance) / (fog.end - fog.start); 
	else if(fog.type == 1) 
		factor = exp(- fog.density * fogDistance); 
	else if(fog.type == 2) 
		factor = exp(- pow(fog.density * fogDistance, 2.0) ); 
       
	factor = 1.0 - clamp(factor, 0.0, 1.0); 
    
	return factor;
}

// Constant values
const int MAX_LIGHTS = 8;
const int MAX_TEXTURES = 8;

// Values that stay constant for the whole mesh.
uniform bool lightEnabled;
uniform Light lights[MAX_LIGHTS];
uniform Material material;
uniform int numLights;
uniform bool colorTextureEnabled[MAX_TEXTURES];
//uniform bool colorTextureEnabled1;
uniform sampler2D colorTexture[MAX_TEXTURES];
//uniform sampler2D colorTexture1;
uniform bool textEnabled;
uniform vec3 textColor;
uniform FogParam fogParam;
uniform sampler2D shadowMap;

vec2 poissonDisk[16] = vec2[] (
	vec2(-0.94201624, -0.39906216),
	vec2(0.94558609, -0.76890725),
	vec2(-0.094184101, -0.92938870),
	vec2(0.34495938, 0.29387760),
	vec2(-0.91588581, 0.45771432),
	vec2(-0.81544232, -0.87912464),
	vec2(-0.38277543, 0.27676845),
	vec2(0.97484398, 0.75648379),
	vec2(0.44323325, -0.97511554),
	vec2(0.53742981, -0.47373420),
	vec2(-0.26496911, -0.41893023),
	vec2(0.79197514, 0.19090188),
	vec2(-0.24188840, 0.99706507),
	vec2(-0.81409955, 0.91437590),
	vec2(0.19984126, 0.78641367),
	vec2(0.14383161, -0.14100790)
);

float random(vec3 seed, int i)
{
	vec4 seed4 = vec4(seed, i);
	float dot_product = dot(seed4,vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

float getShadowEffect()
{
	float visibility = 1.0;
	float bias = 0.005;
/*
	if(texture2D(shadowMap, shadowCoord.xy).x < shadowCoord.z - bias) {
		visibility = 0.25;
	}
	return visibility;
*/
	for(int i = 0; i < 16; ++i)
	{
	int index = int(16.0 * random(gl_FragCoord.xyy, i)) % 16;
	if(texture2D(shadowMap, shadowCoord.xy + poissonDisk[i] / 700.0).x < shadowCoord.z - bias) 
		{
			visibility -= 0.0625;
		}
	}
		return max(0.25, visibility);
}


void main(){
	if(lightEnabled == true)
	{
		// Material properties
		vec4 materialColor = vec4(0, 0, 0, 1);
		
		int texCount = 0;
		
		for (int i = 0; i < MAX_TEXTURES; ++i)
		{
			if(colorTextureEnabled[i] == true)
			{
				materialColor += texture2D( colorTexture[i], texCoord );
				++texCount;

			}		
		}
		if (texCount > 0)
		{
			materialColor = materialColor/texCount;
		}
		else
			materialColor = vec4( fragmentColor, 1 );
	
		// Vectors
		vec3 eyeDirection_cameraspace = - vertexPosition_cameraspace;
		vec3 E = normalize(eyeDirection_cameraspace);
		vec3 N = normalize( vertexNormal_cameraspace );
		
		// Ambient : simulates indirect lighting
		color = materialColor * vec4(material.kAmbient, 1);
		
		for(int i = 0; i < numLights; ++i)
		{
			// Light direction
			float spotlightEffect = 1;
			vec3 lightDirection_cameraspace;
			if(lights[i].type == 1) {
				lightDirection_cameraspace = lights[i].position_cameraspace;
			}
			else if(lights[i].type == 2) {
				lightDirection_cameraspace = lights[i].position_cameraspace - vertexPosition_cameraspace;
				spotlightEffect = getSpotlightEffect(lights[i], lightDirection_cameraspace);
			}
			else {
				lightDirection_cameraspace = lights[i].position_cameraspace - vertexPosition_cameraspace;
			}
			// Distance to the light
			float distance = length( lightDirection_cameraspace );
			
			// Light attenuation
			float attenuationFactor = getAttenuation(lights[i], distance);

			vec3 L = normalize( lightDirection_cameraspace );
			float cosTheta = clamp( dot( N, L ), 0, 1 );
			
			vec3 R = reflect(-L, N);
			float cosAlpha = clamp( dot( E, R ), 0, 1 );
			
			float visibility =getShadowEffect();
			
			color +=
				// Diffuse : "color" of the object
				visibility * materialColor * vec4(material.kDiffuse, 1) * vec4(lights[i].color, 1) * lights[i].power * cosTheta * attenuationFactor * spotlightEffect +
				
				// Specular : reflective highlight, like a mirror
				visibility * vec4(material.kSpecular, 1) * vec4(lights[i].color, 1) * lights[i].power * pow(cosAlpha, material.kShininess) * attenuationFactor * spotlightEffect;

		}
	}
	else
	{
		if(colorTextureEnabled[0] == true && textEnabled == true)
		{
			color = texture2D( colorTexture[0], texCoord ) * vec4( textColor, 1 );
		}
		else
		{
			int texCount = 0;
		
			color = vec4(0, 0, 0, 0);
			for (int i = 0; i < MAX_TEXTURES; ++i)
			{
				if(colorTextureEnabled[i] == true)
				{
					color += texture2D( colorTexture[i], texCoord );
					++texCount;
				}
			}
			if (texCount > 0)
			{
				color = color/texCount;
			}
			else
				color = vec4( fragmentColor, 1 );
		}
		
	}

	if(fogParam.enabled)
	{
		float fogDistance = abs(vertexPosition_cameraspace.z);
		float fogFactor = getFogFactor(fogParam, fogDistance);
		
		if (color.a != 0) 
			color.xyz = mix(color.xyz, fogParam.color, fogFactor);
		//color = mix(color, vec4(fogParam.color, 1), fogFactor);
	}


}