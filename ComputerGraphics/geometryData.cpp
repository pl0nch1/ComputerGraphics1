#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <iostream>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")

// структуры
struct SimpleVertex {
	XMFLOAT3 Pos;
};

int vertCount = 6;

class CharWriter {
private:
	int m_startX, m_startY;
	int m_counter = 0;
	SimpleVertex* m_vertices = new SimpleVertex[vertCount];
	
	void pushVertex(float x, float y) {
		m_vertices[m_counter].Pos = XMFLOAT3(m_startX + x, m_startY + y, 1);
		m_counter++;
	}

public:
	CharWriter(float startX, float startY) : m_startX(startX), m_startY(startY) {}
	~CharWriter(){
		delete[] m_vertices; 
	}

	SimpleVertex* getVertices() {
		return m_vertices;
	}

	int verticesCount() {
		return vertCount;
	}

	void writeChar(char ch) {
		pushVertex(0.5, 0.5);
		pushVertex(0, 0);
		pushVertex(0, 0.5);

		pushVertex(0.23, 0.75);
		pushVertex(0, 0.7);
		pushVertex(0, 0.1);
	}
};