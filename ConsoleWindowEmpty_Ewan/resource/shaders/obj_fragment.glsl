#version 400

smooth in vec4 vertPos;
smooth in vec4 vertNormal;
smooth in vec2 vertUV;

out vec4 outputColour;

uniform vec4 camPos;

uniform vec4 kA;
uniform vec4 kD;
uniform vec4 kS;
//uniforms for texture data
uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform sampler2D NormalTexture;


vec3 iA = vec3(0.25f, 0.25f, 0.25f);
vec3 iD = vec3(1.f, 1.f, 1.f);
vec3 iS = vec3(1.f, 1.f, 1.f);

vec4 lightDir = normalize(vec4(0.f) - vec4(10.f, 8.f, 10.f, 0.f));

void main()
{
	//Get texture data from UV coords
	vec4 textureData = texture(NormalTexture, vertUV);
	vec3 Ambient = kA.xyz * iA; //ambient light
	
	//Get lambertian Term
	float nDl = max(0.f, dot(normalize(vertNormal), -lightDir));
	vec3 Diffuse = kD.xyz * iD * nDl * textureData.rgb;

	vec3 R = reflect(lightDir, normalize(vertNormal)).xyz;	//reflected light vector
	vec3 E = normalize(camPos - vertPos).xyz;		//Surface to eye vector

	float specTerm = pow(max(0.f, dot(E, R)), kS.a);	//Specular Term
	vec3 Specular = kS.xyz * iS * specTerm;

	vec4 vertColour = vec4(Ambient + Diffuse + Specular, 1.0f);
    vec4 litColour = vec4(vertColour.xyz * nDl, 1.0);
    outputColour = vertColour + litColour;
	//outputColour = vec4(Ambient + Diffuse + Specular, 1.f);
}