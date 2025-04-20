#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <vector>


struct Face
{
	unsigned int vIndices[3]{ 0,0,0 };
	unsigned int vtIndices[3]{ 0,0,0 };
	unsigned int nIndices[3]{ 0,0,0 };
};

struct Vector
{
	float x = 0;
	float y = 0;
	float z = 0;

	Vector() = default;
	Vector(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Vertex
{
	Vector position;
	Vector normal;
	Vector texture;
};

struct MeshData
{
	std::vector<Vertex> vertices;
	unsigned int indicesSize;
};

void loadModelFromFile(const std::streamsize buffer_size, const char* file_path, MeshData& result); 