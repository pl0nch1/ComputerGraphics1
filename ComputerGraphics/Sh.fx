static const float4 Ldir = {1.0f, 1.0f, -1.0f, 1.0f }; 
static const float4 vLightColor = { .3, .3, .3, 1 };/*����������� ��������� � ��������
 �������� ��������� �����*/


cbuffer ConstantBuffer : register(b0)
{
    row_major float4x4 World;
    row_major float4x4 View;
    row_major float4x4 Projection;
}

Texture2D txDiffuse : register(t0);   // ������� ��� ��������� ��������     
SamplerState samLinear: register(s0);  // ������� ��� ��������� ������     

struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEX;
    float3 Norm : NORMAL;
};

// ���������, ����������� ��� ������� ������ ��� ���������� �������
struct VS_OUTPUT
{
    // ������ ���� ��� ������ �� ������ ����������, ������� ��������� ���������� �����
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Norm : TEXCOORD1;
};

// ���������, ����������� ��� ������� ������ ��� ����������� �������
struct PS_OUTPUT
{
    // ������ ���� ��� ������ �� ������ ����������, ������� ��������� ���� �����
    float4 Color : SV_TARGET;
};


// ������� ���������� �������
VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    // ������������� ������� ������� ��� ������ ��������� �� �������
    output.Pos = mul(input.Pos, World); // ������� �� ����
    output.Pos = mul(output.Pos, View); // ����� �� ������� ����
    output.Pos = mul(output.Pos, Projection); // � �� ������������ �������
    output.Norm = mul(input.Norm, World);
    output.Tex = input.Tex;  // ���������� ���������� �� �������� 
    return output;
}


// ������ ������� ������� ����������� ������� (����������� �������� ������������), //������� ���������� ��� PS_OUTPUT
PS_OUTPUT ps_main(VS_OUTPUT inp) : SV_Target
{
    // ��������� � �������������� 1 ��������������� ���������� ������������� ���� float
    float dis = 1;
    PS_OUTPUT Pout; // ��������� ���������� ������������� ���� PS_OUTPUT
   // ����������� ���� ���������� �������� �������� ����� ������������� �������
    Pout.Color = txDiffuse.Sample(samLinear, inp.Tex) + dot(Ldir, inp.Norm) * vLightColor;
    Pout.Color.a = 1;
    return Pout;
}
