#include "PostEffectTest_NONE.hlsli"

float4 main() : SV_TARGET
{
    uniform int samples : hint_range;
    (2, 64, 2) = 16; // �T���v����
    uniform float strength : hint_range;
    (0.0, 1.0) = 0.0; // �u���[�̍L���鋭��
    uniform float2 center = float2(0.5, 0.5); // ���S���W

    // �ŏI�I�ȐF
    float4 color = float4(0.0); // ���ŏ�����
    // ���S����ɂ���
    float2 pos = UV - center;
    // ���S����̋���
    float dist = length(pos);
    float factor = strength / float(samples) * dist;
    for (int i = 0; i < samples; i++)
    {
        float uvOffset = 1.0 - factor * float(i);
        color += texture(TEXTURE, pos * uvOffset + center);
    }
    // ���ς����߂�
    color /= float(samples);

    return color;

    // �e�X�g�p
    //COLOR.rgb = vec3(dist);
}

void fragment()
{
    // �ŏI�I�ȐF
    vec4 color = vec4(0.0); // ���ŏ�����
    // ���S����ɂ���
    vec2 pos = UV - center;
    // ���S����̋���
    float dist = length(pos);
    float factor = strength / float(samples) * dist;
    for (int i = 0; i < samples; i++)
    {
        float uvOffset = 1.0 - factor * float(i);
        color += texture(TEXTURE, pos * uvOffset + center);
    }
    // ���ς����߂�
    color /= float(samples);

    COLOR = color;

    // �e�X�g�p
    //COLOR.rgb = vec3(dist);
}