#pragma once

/**
 * \brief A depth buffer used by renderer objects to depth-test fragments
 */
// Strictly speaking, this class is a simple wrapper that stores a dynamically-sized 2D array in a 1D array and allows
// the set all of it's values at once with clear()
class DepthBuffer
{
public:
	DepthBuffer(int w, int h);
	DepthBuffer(const DepthBuffer&) = delete;
	DepthBuffer(DepthBuffer&&) = delete;

	void operator=(const DepthBuffer&) = delete;
	void operator=(DepthBuffer&&) = delete;

	void clear(float value) const;
	float get(int x, int y) const;
	void set(int x, int y, float val) const;

	~DepthBuffer();
private:
	// It's a simple array behind the scenes
	float* buffer;
	int width, height;
};
