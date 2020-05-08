#include <Utils/Dialogs.h>
#include <ACGM_RayTracer_lib/Image.h>
#define STB_IMAGE_IMPLEMENTATION
#include <ACGM_RayTracer_lib/stb_image.h>

//! Constructors
acgm::Image::Image(std::string file_path)
{
	if (file_path.compare("") != 0)
	{
		image_ = stbi_load(file_path.c_str(), &width_, &height_, &bpp_, 3);
	}
}

cogs::Color3f acgm::Image::GetColorAt(const glm::vec2& uvs) const {
	/*uint32_t prvy = uvs.x * (width_ - 1);
	uint32_t druhy = uvs.y * (height_ - 1);
	uint32_t pixelOffset = (prvy + width_ * druhy) * 3;
	float r = float(image_[pixelOffset]) / 256.0f;
	float g = float(image_[pixelOffset + 1]) / 256.0f;
	float b = float(image_[pixelOffset + 2]) / 256.0f;*/

	uint32_t prvy = uvs.x * (width_ - 1);
	uint32_t druhy = uvs.y * (height_ - 1);
	unsigned char* pixelOffset = image_ + (prvy + width_ * druhy) * 3;
	unsigned char ar = pixelOffset[0];
	unsigned char ag = pixelOffset[1];
	unsigned char ab = pixelOffset[2];

	float r = float(ar) / 255.0f;
	float g = float(ag) / 255.0f;
	float b = float(ab) / 255.0f;

	return cogs::Color3f(r, g, b);
}