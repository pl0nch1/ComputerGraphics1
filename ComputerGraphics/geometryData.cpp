#include "geometryData.h"


CharWriter::CharWriter(float startX, float startY) : m_startX(startX), m_startY(startY) {
	m_vertices = new CustomVertex[verticesLength];
}

CharWriter::~CharWriter() {
	delete[] m_vertices;
}

CustomVertex* CharWriter::getVertices() {
	return m_vertices;
}

int CharWriter::verticesCount() {
	return verticesLength;
}

void CharWriter::writeChar(char ch) {
	if (ch == 'ì') {
		XMFLOAT2 vertices[] = {
			{0, 0}, {0, 30}, {3,30},
			{0,0}, {3,30}, {5,0},
			{3, 30}, {7,10}, {2, 25},
			{3, 30}, {9, 10}, {7,10},
		};

		for (XMFLOAT2 vertex : vertices) {
			pushVertex(vertex.x, vertex.y);
		}
	}
}

void CharWriter::pushVertex(float x, float y) {
	m_vertices[m_counter].Pos = XMFLOAT3((m_startX + x)/100, (m_startY + y) / 100, 1);
	m_counter++;
}