#include "Image.h"
#include <fstream>
#include <sstream>

Image::Image(const std::string& Filename, const Vector2i& OutputResolution)
	: mPixelColors(OutputResolution.y)
	, mOutputResolution(OutputResolution)
	, mFilename(Filename)
{
	// Resize each vector to the amount of vertical pixels
	for (auto& row : mPixelColors)
		row.resize(OutputResolution.x);
}

void Image::setPixel(const int& X, const int& Y, const Color& Color)
{
	mPixelColors[Y][X] = Color;
}

void Image::writeImage()
{
	// Write to a .ppm image
	std::ostringstream headerStream;
	headerStream << "P6\n";
	headerStream << mOutputResolution.x << ' ' << mOutputResolution.y << '\n';
	headerStream << "255\n";
	std::ofstream fileStream(mFilename + ".ppm", std::ios::out | std::ios::binary);
	fileStream << headerStream.str();

	for (const auto& row : mPixelColors)
		for (const auto& pixel : row)
			fileStream << static_cast<unsigned char>(pixel.r) 
				<< static_cast<unsigned char>(pixel.g) 
				<< static_cast<unsigned char>(pixel.b);

	fileStream.flush();
	fileStream.close();
}

void Image::setFilename(const std::string& Filename)
{
	mFilename = Filename;
}

std::string Image::getFilename() const
{
	return mFilename;
}