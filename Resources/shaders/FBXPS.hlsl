#include "FBX.hlsli"

Texture2D<float4> tex0 : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1); //1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp :register(s0);

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input)
{
    PSOutput output;
    float4 texcolor = tex0.Sample(smp, input.uv);
	float3 light = normalize(float3(40, 0, 0));
	//float diffuse = saturate(dot(-light, input.normal));
	//float brightness = diffuse + 0.3f;
	//float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
        
    
    
    //�A���r�G���g
    float4 ambient = texcolor * float4(0.5, 0.5, 0.5, 1);

	//�f�B�t���[�Y
    float diffuseIntensity = saturate(dot(normalize(input.normal), light));
    float4 diffuse = texcolor * diffuseIntensity;


	//�X�y�L�����[
    float3 eyeDir = normalize(cameraPos.xyz - input.worldPos.xyz);
    float3 halfVec = normalize(eyeDir + light);
    float intensity = saturate(dot(normalize(input.normal), halfVec));
    float reflection = pow(intensity, 20);
    float4 specularColor = float4(1, 1, 1, 1);
    float4 specular = specularColor * reflection;

    
    //�A�e�ƃe�N�X�`���̐F������    
    output.target0 = ambient + diffuse + specular;
   // output.target1 = float4(1 - (shadecolor * texcolor).rgb, 1);
    

    

    return output;

}