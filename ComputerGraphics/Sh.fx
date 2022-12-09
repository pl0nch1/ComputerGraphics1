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

// структура, описывающая тип входных данных для вершинного шейдера
struct VS_OUTPUT
{
    // задаем поле как вектор из четыре переменных, которые описывают координаты точки
    float4 Pos : SV_POSITION;
    float3 Norm : TEXCOORD0;
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
    output.Norm = input.Norm; // mul(input.Norm, World);
    return output;
}


// первый вариант функции пиксельного шейдера (равномерная закраска треугольника), //которая возвращает тип PS_OUTPUT
PS_OUTPUT ps_main(VS_OUTPUT inp) : SV_Target
{
    // объявляем и инициализируем 1 вспомогательную переменную возвращаемого типа float
    float dis = 1;
    PS_OUTPUT Pout; // объявляем переменную возвращаемого типа PS_OUTPUT
   // присваиваем этой переменной значение красного цвета отображаемого пикселя
    Pout.Color = float4(inp.Norm.xyz, 0); //float4(0.9, 0.9, 0.9, 0.9) - dot(inp.Norm, (float3) LightDir) * float4(1, 1, 0, 1);
    return Pout;
}
