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

void acgm::Image::FreeImageData()
{
	stbi_image_free(image_);
}

cogs::Color3f acgm::Image::GetColorAt(const glm::vec2& uvs) const 
{
	uint32_t pixel_x = uvs.x * (width_ - 1);
	uint32_t pixel_y = uvs.y * (height_ - 1);

	unsigned char* pixelOffset = image_ + (pixel_x + width_ * pixel_y) * 3;

	float r = pixelOffset[0] / 255.0f;
	float g = pixelOffset[1] / 255.0f;
	float b = pixelOffset[2] / 255.0f;

	return cogs::Color3f(r, g, b);
}