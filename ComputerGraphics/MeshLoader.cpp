#include "MeshLoader.h"
#include <fstream>

float get32Float(std::ifstream* file) {
	char* buffer = new char[4];
	file->read(buffer, 4);
	return *(float*) buffer;
}

unsigned int get32UnsignedInt(std::ifstream* file) {
	char* buffer = new char[4];
	file->read(buffer, 4);
	return *(unsigned int*) buffer;
}

void skipBytes(std::ifstream* file, int numBytes) {
	char* buffer = new char[numBytes];
	file->read(buffer, numBytes);
}

int MeshLoader::getTriangleCount() {
	return m_triangleCount;
}

const CustomTriangle* MeshLoader::getTriangles() {
	return m_triangles;
}

MeshLoader::MeshLoader(std::string filename): m_filename(filename), m_triangleCount(0) {
}
 

void MeshLoader::load(int scale, float xOffset, float yOffset) {
	std::ifstream file(m_filename, std::ios::binary);
	skipBytes(&file, 80);
	m_triangleCount = get32UnsignedInt(&file);
	m_triangles = new CustomTriangle[m_triangleCount];

	for (int i = 0; i < m_triangleCount; i++) {
		CustomVertex normalVector = { {get32Float(&file), get32Float(&file), get32Float(&file)} };
		CustomVertex a = { {xOffset + get32Float(&file) / scale, yOffset + get32Float(&file) / scale, 1.0f/scale} }; get32Float(&file);
		// Swap vertices to flip normals
		CustomVertex c = { {xOffset + get32Float(&file) / scale, yOffset + get32Float(&file) / scale, 1.0f / scale} }; get32Float(&file);
		CustomVertex b = { {xOffset + get32Float(&file) / scale, yOffset + get32Float(&file) / scale, 1.0f / scale} }; get32Float(&file);
		m_triangles[i] = { normalVector, a, b, c };
		skipBytes(&file, 2);
	}
	file.close();
}