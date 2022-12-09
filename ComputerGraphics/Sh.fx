cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 LightDir;
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
    output.Norm = input.Norm; // mul(input.Norm, World);
    return output;
}


// ������ ������� ������� ����������� ������� (����������� �������� ������������), //������� ���������� ��� PS_OUTPUT
PS_OUTPUT ps_main(VS_OUTPUT inp) : SV_Target
{
    // ��������� � �������������� 1 ��������������� ���������� ������������� ���� float
    float dis = 1;
    PS_OUTPUT Pout; // ��������� ���������� ������������� ���� PS_OUTPUT
   // ����������� ���� ���������� �������� �������� ����� ������������� �������
    Pout.Color = float4(inp.Norm.xyz, 0); //float4(0.9, 0.9, 0.9, 0.9) - dot(inp.Norm, (float3) LightDir) * float4(1, 1, 0, 1);
    return Pout;
}
