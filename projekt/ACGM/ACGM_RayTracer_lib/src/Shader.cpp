#include <ACGM_RayTracer_lib/Shader.h>

//! Constructors
acgm::Shader::Shader(cogs::Color3f color):color_(color)
{
}

acgm::Shader::Shader()
{
}

//! Functions
cogs::Color3f acgm::Shader::IdentifyColor(const ShaderStruct& point) const
{
	return color_;
}