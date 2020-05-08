#include <ACGM_RayTracer_lib/Shader.h>

//! Constructors
acgm::Shader::Shader(cogs::Color3f color):color_(color)
{
}

acgm::Shader::Shader()
{
}

//! Functions
acgm::ShaderReturn acgm::Shader::IdentifyColor(const ShaderStruct& point) const
{
	ShaderReturn shader_info;
	shader_info.color = color_;
	return shader_info;
}