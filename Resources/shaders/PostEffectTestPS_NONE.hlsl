#include "PostEffectTest_NONE.hlsli"

Texture2D<float4> tex0 : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1); //1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��

SamplerState smp : register(s0);


float4 main(VSOutput input) : SV_TARGET
{
    return tex0.Sample(smp, input.uv);
}
