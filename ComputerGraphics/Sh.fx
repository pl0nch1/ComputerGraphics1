cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}


// ���������, ����������� ��� ������� ������ ��� ���������� �������
struct VS_OUTPUT
{
    // ������ ���� ��� ������ �� ������ ����������, ������� ��������� ���������� �����
    float4 Pos : SV_POSITION;
};

// ���������, ����������� ��� ������� ������ ��� ����������� �������
struct PS_OUTPUT
{
    // ������ ���� ��� ������ �� ������ ����������, ������� ��������� ���� �����
    float4 Color : SV_TARGET;
};


// ������� ���������� �������
VS_OUTPUT vs_main(float4 Pos : POSITION)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    // ������������� ������� ������� ��� ������ ��������� �� �������
    output.Pos = mul(Pos, World); // ������� �� ����
    output.Pos = mul(output.Pos, View); // ����� �� ������� ����
    output.Pos = mul(output.Pos, Projection); // � �� ������������ �������
    return output;
}


// ������ ������� ������� ����������� ������� (����������� �������� ������������), //������� ���������� ��� PS_OUTPUT
PS_OUTPUT ps_main(VS_OUTPUT inp) : SV_Target
{
    // ��������� � �������������� 1 ��������������� ���������� ������������� ���� float
    float dis = 1;
    PS_OUTPUT Pout; // ��������� ���������� ������������� ���� PS_OUTPUT
   // ����������� ���� ���������� �������� �������� ����� ������������� �������
    Pout.Color = float4(dis, 0, 0, dis);
    return Pout;
}
