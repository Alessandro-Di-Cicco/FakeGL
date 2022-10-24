#include "DepthBuffer.h"

#include <algorithm>

DepthBuffer::DepthBuffer(int w, int h) : width{ w }, height{ h }
{
	buffer = new float[width * height];
}

DepthBuffer::~DepthBuffer()
{
	delete[] buffer;
}

void DepthBuffer::clear(float value) const
{
	std::fill_n(buffer, width * height, value);
}

float DepthBuffer::get(int x, int y) const
{
	return buffer[y * width + x];
}

void DepthBuffer::set(int x, int y, float val) const
{
	buffer[y * width + x] = val;
}
