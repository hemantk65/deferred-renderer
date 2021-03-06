#version 450

#define NUM_LIGHTS 1
//#define PBR

layout(location = 0) out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D positionMap;
uniform sampler2D reflectanceMap;

// lights
//uniform vec3 lightPositions[NUM_LIGHTS];
//uniform vec3 lightColors[NUM_LIGHTS];

uniform vec3 camPos;

const float PI = 3.14159265359;
#ifdef PBR
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float nom   = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float nom   = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
void main()
{
	vec3 albedo = texture(albedoMap, TexCoords).rgb;
	vec3 Normal = (texture(normalMap, TexCoords).rgb * 2.0f) - 1.0f;
	vec3 WorldPos = (texture(positionMap, TexCoords).rgb * 2.0f) - 1.0f;

	vec3 N = normalize(Normal);
	vec3 V = normalize(camPos - WorldPos);
	
	float ao = 1.0f;
	float metallic = 0.0f;
	float roughness = 0.0f;

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, albedo, metallic);

	// reflectance equation
	vec3 Lo = vec3(0.0);
	
	vec3 lightPositions[NUM_LIGHTS] = { vec3(0.0f, 1.0f, 0.0f) };
	vec3 lightColors[NUM_LIGHTS] = { vec3(1.0f) };

	for(int i = 0; i < NUM_LIGHTS; ++i) 
	{
		vec3 L = normalize(lightPositions[i] - WorldPos);

		// scale light by NdotL
		float NdotL = max(dot(N, L), 0.0);   

		// calculate per-light radiance
		vec3 H = normalize(V + L);
		float distance = length(lightPositions[i] - WorldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lightColors[i] * attenuation;

		// Cook-Torrance BRDF
		float NDF = DistributionGGX(N, H, roughness);   
		float G   = GeometrySmith(N, V, L, roughness);      
		vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
		   
		vec3 nominator    = NDF * G * F; 
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
		
		// kS is equal to Fresnel
		vec3 kS = F;
		// for energy conservation, the diffuse and specular light can't
		// be above 1.0 (unless the surface emits light); to preserve this
		// relationship the diffuse component (kD) should equal 1.0 - kS.
		vec3 kD = vec3(1.0) - kS;
		// multiply kD by the inverse metalness such that only non-metals 
		// have diffuse lighting, or a linear blend if partly metal (pure metals
		// have no diffuse light).
		kD *= 1.0 - metallic;	  
     

		// add to outgoing radiance Lo
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}
	// ambient lighting (note that the next IBL tutorial will replace 
	// this ambient lighting with environment lighting).
	vec3 ambient = vec3(0.03) * albedo * ao;

	vec3 color = ambient + Lo;

	// HDR tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0/2.2)); 

	FragColor = vec4(color, 1.0);
}
#else
void main()
{
	vec3 albedo = texture(albedoMap, TexCoords).rgb;
	vec3 Normal = (texture(normalMap, TexCoords).rgb * 2.0f) - 1.0f;
	vec3 WorldPos = (texture(positionMap, TexCoords).rgb * 2.0f) - 1.0f;
	vec4 reflectance = texture(reflectanceMap, TexCoords);
	float Ka = reflectance.r;
	float Kd = reflectance.g;
	float Ks = reflectance.b;
	float Ns = reflectance.a;

	vec3 N = normalize(Normal);
	vec3 V = normalize(camPos - WorldPos);
	
	float ao = 1.0f;

	// reflectance equation
	vec3 Lo = vec3(0.0);
	
	vec3 lightPositions[NUM_LIGHTS] = { vec3(0.0f, 1.0f, 0.0f) };
	vec3 lightColors[NUM_LIGHTS] = { vec3(1.0f) };

	float Ia = 2.5;
	float Id = 5.0;
	float Is = 0.0;

	for(int i = 0; i < NUM_LIGHTS; ++i)
	{
		vec3 L = normalize(lightPositions[i] - WorldPos);
		vec3 R = -reflect(L, N);

		float NdotL = max(dot(N, L), 0.0);
		float RdotV = max(dot(R, V), 0.0);

		Lo += (Kd * Id * NdotL) + (Ks * pow(RdotV, Ns) * Is);
	}

	vec3 color = albedo * (Ka * Ia + Lo) * ao;

//	// HDR tonemapping
//	color = color / (color + vec3(1.0));
//	// gamma correct
//	color = pow(color, vec3(1.0/2.2));

	FragColor = vec4(color, 1.0);
}
#endif