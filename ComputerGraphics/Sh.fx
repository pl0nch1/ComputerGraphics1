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
    output.Norm = mul(input.Norm, World);
    return output;
}


// первый вариант функции пиксельного шейдера (равномерная закраска треугольника), //которая возвращает тип PS_OUTPUT
PS_OUTPUT ps_main(VS_OUTPUT inp) : SV_Target
{
    // объявляем и инициализируем 1 вспомогательную переменную возвращаемого типа float
    float dis = 1;
    PS_OUTPUT Pout; // объявляем переменную возвращаемого типа PS_OUTPUT
   // присваиваем этой переменной значение красного цвета отображаемого пикселя
    Pout.Color =  dot(Ldir, inp.Norm) * float4(1,0.0,0.0,1) + dot(Ldir2, inp.Norm) * float4(0,1,0,1);
    return Pout;
}
