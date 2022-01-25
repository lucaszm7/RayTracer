//#pragma once
//
//#include <unordered_map>
//#include <iostream>
//#include <fstream>
//#include "rtweekend.h"
//
//struct PixelPos
//{
//	unsigned int x, y;
//
//	PixelPos(int x, int y)
//		:x(x), y(y) {}
//
//	bool operator == (PixelPos& e)
//	{
//		return x == e.x && y == e.y;
//	}
//
//};
//
//struct PixelCor
//{
//	float r, g, b;
//
//	PixelCor(float r, float b, float g)
//		:r(r), b(b), g(g) {}
//
//};
//
//class Image
//{
//private:
//	std::ifstream m_File;
//
//public:
//	int width, height;
//	std::unordered_map<int, PixelCor> pixels;
//
//public:
//	Image(const char* file_name)
//	{
//		m_File.open(file_name);
//		float trash;
//		m_File >> trash >> width >> height;
//
//		float r, g, b;
//		for (int j = height; j > 0; --j)
//		{
//			for (int i = 0; i < width; ++i)
//			{
//				// PixelPos pixelCoord = {i, j};
//				m_File >> r;
//				m_File >> g;
//				m_File >> b;
//				// color pixelColor = color(r, g, b);
//				pixels.emplace(((j*height)+i), (r, g, b));
//			}
//		}
//
//	}
//	
//	/*color GetPixel(unsigned int x, unsigned int y)
//	{
//		PixelPos p(x, y);
//		return pixels.find(p);
//	}*/
//
//	~Image()
//	{
//		m_File.close();
//	}
//
//};