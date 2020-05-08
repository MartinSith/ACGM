#include "ACGM_RayTracer_lib/CheckerShader.h"

//! Constructors
acgm::CheckerShader::CheckerShader(float cube_size, const std::shared_ptr<Shader> shader0, const std::shared_ptr<Shader> shader1)
	: cube_size_(cube_size), shader0_(shader0), shader1_(shader1)
{
}

//! Functions
acgm::ShaderReturn acgm::CheckerShader::IdentifyColor(const ShaderStruct& point) const
{
	int32_t get_shader;
	extern float bias;

	get_shader = floor((point.point.x / cube_size_) + bias) + floor((point.point.y / cube_size_) + bias) + floor((point.point.z / cube_size_) + bias);

	if (get_shader % 2 == 0)
	{
		return shader0_->IdentifyColor(point);
	}
	else 
	{ 
		return shader1_->IdentifyColor(point);
	}
}

//! Getters
float acgm::CheckerShader::GetCubeSize()
{
	return cube_size_;
}
