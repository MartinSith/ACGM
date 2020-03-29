#include "ACGM_RayTracer_lib/Light.h"

//! Constructors
acgm::Light::Light(float intensity, const glm::vec3& position): intensity_(intensity), position_(position)
{
}

//! Getters
float acgm::Light::GetIntensity() const
{
	return intensity_;
}

glm::vec3 acgm::Light::GetDirectionToLight(const glm::vec3& point) const
{
	return point;
}

float acgm::Light::GetIntensityAt(const glm::vec3& point) const
{
	return intensity_;
}

glm::vec3 acgm::Light::GetPosition() const
{
	return position_;
}