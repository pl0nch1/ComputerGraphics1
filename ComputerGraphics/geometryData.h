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
	XMFLOAT2 Tex;     // координаты текстуры
	XMFLOAT3 Normal;
};

struct CustomTriangle {
	CustomVertex a, b, c;
};

class CharWriter {
private:
	int m_startX, m_startY;
	int m_counter = 0;
	int verticesLength = 3 * 100;
	CustomVertex* m_vertices;

	void pushVertex(float x, float y);

public:
	CharWriter(float startX, float startY);
	~CharWriter();

	CustomVertex* getVertices();

	int verticesCount();

	void writeChar(char ch);
};