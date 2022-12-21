static const float4 Ldir = {1.0f, 1.0f, -1.0f, 1.0f }; 
static const float4 vLightColor = { .3, .3, .3, 1 };/*статическая константа с вектором
 цветовых компонент света*/


cbuffer ConstantBuffer : register(b0)
{
    row_major float4x4 World;
    row_major float4x4 View;
    row_major float4x4 Projection;
}

Texture2D txDiffuse : register(t0);   // регистр для получения текстуры     
SamplerState samLinear: register(s0);  // регистр для получения сэмпла     

struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEX;
    float3 Norm : NORMAL;
};

// структура, описывающая тип входных данных для вершинного шейдера
struct VS_OUTPUT
{
    // задаем поле как вектор из четыре переменных, которые описывают координаты точки
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Norm : TEXCOORD1;
};

// структура, описывающая тип входных данных для пиксельного шейдера
struct PS_OUTPUT
{
    // задаем поле как вектор из четыре переменных, которые описывают цвет точки
    float4 Color : SV_TARGET;
};


// функция вершинного шейдера
VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    // Трансформация позиции вершины при помощи умножения на матрицу
    output.Pos = mul(input.Pos, World); // сначала на мира
    output.Pos = mul(output.Pos, View); // затем на матрицу вида
    output.Pos = mul(output.Pos, Projection); // и на проекционную матрицу
    output.Norm = mul(input.Norm, World);
    output.Tex = input.Tex;  // текстурные координаты не меняются 
    return output;
}


// первый вариант функции пиксельного шейдера (равномерная закраска треугольника), //которая возвращает тип PS_OUTPUT
PS_OUTPUT ps_main(VS_OUTPUT inp) : SV_Target
{
    // объявляем и инициализируем 1 вспомогательную переменную возвращаемого типа float
    float dis = 1;
    PS_OUTPUT Pout; // объявляем переменную возвращаемого типа PS_OUTPUT
   // присваиваем этой переменной значение красного цвета отображаемого пикселя
    Pout.Color = txDiffuse.Sample(samLinear, inp.Tex) + dot(Ldir, inp.Norm) * vLightColor;
    Pout.Color.a = 1;
    return Pout;
}
