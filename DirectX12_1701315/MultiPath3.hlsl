
//テクスチャデータ
Texture2D<float4> tex : register(t0); //通常テクスチャ

Texture2D<float4> outline : register(t1); //アウトライン

Texture2D<float4> distortion : register(t2); //歪ませ後のテクスチャ

Texture2D<float4> depthoffield : register(t3); //被写界深度

Texture2D<float> depth : register(t4); //normal

SamplerState smp : register(s0);

struct Output
{
    float4 pos : POSITION;
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};

//頂点シェーダ
Output vs(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    Output output;
    output.pos = pos;
    output.svpos = pos;
    output.uv = uv;
    return output;
}

float SDFSphere3D(float3 pos, float r)
{
    return length(pos - float3(0,0,5)) - r;
}

//球
float SDFSpherelattice3D(float3 pos, float divider, float r)
{
    return length(fmod(pos,divider) - divider / 2) - r;
}

//四角
float SDFBoxlattice3D(float3 pos, float divider, float3 b)
{
    return length(max(abs(fmod(pos, divider) - divider / 2) - b, 0.0));
}

//ピクセルシェーダ
float4 ps(Output input) : SV_Target
{
    //テクスチャ情報
    float4 ret = tex.Sample(smp, input.uv);

    //輝度値
    float3 b = float3(0.298912f, 0.586611f, 0.114478f);

    float w, h, level;
    tex.GetDimensions(0, w, h, level);
    float dx = 1.0f / w; //一ピクセル分
    float dy = 1.0f / h; //一ピクセル分
    
    
    float4 outret = outline.Sample(smp, input.uv);
    float4 distret = distortion.Sample(smp, input.uv);
    
    //被写界深度
    if (input.uv.x < 0.2f && input.uv.y < 0.2f)
    {
        return depthoffield.Sample(smp, input.uv * 5);
    }
    
    //レイマーチング
    float d = depth.Sample(smp, input.uv);
    if (pow(d ,100) >= 0.99999f)
    {
        float m = min(w, h);
        float2 aspect = float2(w / m, h / m);
        float3 eye = float3(0, 0, -2.5); //視点
        float3 tpos = float3(input.pos.xy * aspect, 0);
        float3 ray = normalize(tpos - eye); //レイベクトル
        float r = 10.0f; //半径
    
        for (int i = 0; i < 64; ++i)
        {
            float len = SDFBoxlattice3D(abs(eye), r, float3(1, 1, 1));
            eye += ray * len;
            if (len < 0.001f)
            {
                return float4((float) (64 - i) / 64.0f, (float) (64 - i) / 64.0f, (float) (64 - i) / 64.0f, 1);
            }
        }
    }
    return tex.Sample(smp, input.uv);
    
    //通常
    return float4((outret * distret).rgb, 1);
}

