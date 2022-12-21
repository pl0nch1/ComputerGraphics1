static const float4 Ldir = {1.0f, 0.0f, -1.0f, 1.0f };
static const float4 Ldir2 = {-1.0f, 0.0f, -1.0f, 1.0f };

cbuffer ConstantBuffer : register(b0)
{
    row_major float4x4 World;
    row_major float4x4 View;
    row_major float4x4 Projection;
}

struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Norm : NORMAL;
};

// ���������, ����������� ��� ������� ������ ��� ���������� �������
struct VS_OUTPUT
{
    // ������ ���� ��� ������ �� ������ ����������, ������� ��������� ���������� �����
    float4 Pos : SV_POSITION;
    float3 Norm : TEXCOORD0;
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
    return output;
}


// ������ ������� ������� ����������� ������� (����������� �������� ������������), //������� ���������� ��� PS_OUTPUT
PS_OUTPUT ps_main(VS_OUTPUT inp) : SV_Target
{
    // ��������� � �������������� 1 ��������������� ���������� ������������� ���� float
    float dis = 1;
    PS_OUTPUT Pout; // ��������� ���������� ������������� ���� PS_OUTPUT
   // ����������� ���� ���������� �������� �������� ����� ������������� �������
    Pout.Color =  dot(Ldir, inp.Norm) * float4(1,0.0,0.0,1) + dot(Ldir2, inp.Norm) * float4(0,1,0,1);
    return Pout;
}
