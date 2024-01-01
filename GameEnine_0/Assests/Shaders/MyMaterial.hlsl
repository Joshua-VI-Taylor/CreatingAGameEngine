struct VS_INPUT
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

//Retrive data is c++ version of the constant buffer
cbuffer constant : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 proj;
};


VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
		
	//WORLD SPACE
    output.position = mul(input.position, world);
	//VIEW SPACE
    output.position = mul(output.position, view);
	//SCREEN SPACE
    output.position = mul(output.position, proj);
    output.texcoord = input.texcoord;
    //Computing World Normal
    output.normal = normalize(mul(input.normal, (float3x3) world));
    return output;
}

//Retrive Earth texture
Texture2D EarthColor : register(t0);
sampler EarthColorSampler : register(s0);
//Retrive EarthSpecular texture
Texture2D EarthSpecular : register(t1);
//Since there are multiple ways to sample textures, we need to know how to sazzmple. 
//This varaible hold the information on how to sample the texture.
sampler EarthSpecularSampler : register(s1);
//Retrive Clouds texture
Texture2D Clouds : register(t2);
//Since there are multiple ways to sample textures, we need to know how to sample. 
//This varaible hold the information on how to sample the texture.
sampler CloudsSampler : register(s2);
//Retrive EarthNight texture
Texture2D EarthNight : register(t3);
//Since there are multiple ways to sample textures, we need to know how to sample. 
//This varaible hold the information on how to sample the texture.
sampler EarthNightSampler : register(s3);
//Custon dat type to store the position, texcoord, normal, and direction_to_camera 
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

//Retrive Earth texture
Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

float4 psmain(PS_INPUT input) : SV_TARGET
{
    //Sample texture's data. This will allow us to get the color of every part of the texture and to 
    //help use assinge that color to the pixels that need it one the screen.
    float4 color = Color.Sample(ColorSampler, input.texcoord);
    float3 light_direction = normalize(float3(-1, 1, 1));
    
    
	//AMBIENT LIGHT
    float ka = 8.5;
    float3 ia = float3(0.09, 0.082, 0.082);
    ia *= (color.rgb);

    float3 ambient_light = ka * ia;

	//DIFFUSE LIGHT
    float kd = 0.7;
    float amount_diffuse_light = max(dot(light_direction.xyz, input.normal), 0.0f);
    float3 id = float3(1, 1, 1);
    id *= (color.rgb);
    float3 diffuse_light = kd * id * amount_diffuse_light;

	//SPECULAR LIGHT
    //float ks = 0.0;
    //float3 is = float3(1.0, 1.0, 1.0);
    //float3 reflected_light = reflect(light_direction.xyz, input.normal);
    //float shininess = 30.0;
    //float amount_specular_light = pow(max(0.0, dot(reflected_light, input.direction_to_camera)), shininess);

    //float3 specular_light = ks * amount_specular_light * is;

    float3 final_light = ambient_light + diffuse_light;

    return float4(final_light, 1.0);
}