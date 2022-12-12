#include "PostEffectTest_NONE.hlsli"

float4 main() : SV_TARGET
{
    uniform int samples : hint_range;
    (2, 64, 2) = 16; // サンプル回数
    uniform float strength : hint_range;
    (0.0, 1.0) = 0.0; // ブラーの広がる強さ
    uniform float2 center = float2(0.5, 0.5); // 中心座標

    // 最終的な色
    float4 color = float4(0.0); // 黒で初期化
    // 中心を基準にする
    float2 pos = UV - center;
    // 中心からの距離
    float dist = length(pos);
    float factor = strength / float(samples) * dist;
    for (int i = 0; i < samples; i++)
    {
        float uvOffset = 1.0 - factor * float(i);
        color += texture(TEXTURE, pos * uvOffset + center);
    }
    // 平均を求める
    color /= float(samples);

    return color;

    // テスト用
    //COLOR.rgb = vec3(dist);
}

void fragment()
{
    // 最終的な色
    vec4 color = vec4(0.0); // 黒で初期化
    // 中心を基準にする
    vec2 pos = UV - center;
    // 中心からの距離
    float dist = length(pos);
    float factor = strength / float(samples) * dist;
    for (int i = 0; i < samples; i++)
    {
        float uvOffset = 1.0 - factor * float(i);
        color += texture(TEXTURE, pos * uvOffset + center);
    }
    // 平均を求める
    color /= float(samples);

    COLOR = color;

    // テスト用
    //COLOR.rgb = vec3(dist);
}