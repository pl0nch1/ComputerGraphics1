#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <iostream>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")

// структуры
struct CustomVertex {
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
};

struct CustomTriangle {
	CustomVertex a, b, c;
};