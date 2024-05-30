#include "Object3d.hlsli"

float32_t4 main(VertexShaderOutput input) : SV_TARGET
{
    // インデックスを抽出
    uint32_t m = gIndex[input.id].material;
    uint32_t t = gIndex[input.id].tex2d;

    // 最終的な結果
    float32_t4 output;
    
    // Lightingの計算は行わない
    float4 transformUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial[m].uvTransform);
    output = input.color * gTexture[t].Sample(gSampler, transformUV.xy);

    // 透明なら消す
    if (output.a <= 0.5f) { discard; }

    return output;
}