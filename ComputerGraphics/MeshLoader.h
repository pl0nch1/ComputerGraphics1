#pragma once
#include "geometryData.h"
#include <string>


class MeshLoader {
private:
	std::string m_filename;
	unsigned int m_triangleCount;
	CustomTriangle* m_triangles;
public:
	~MeshLoader();
	MeshLoader(std::string filepath);
	void load(float scale, float xOffset, float yOffset);
	const CustomTriangle* getTriangles();
	int getTriangleCount();
};