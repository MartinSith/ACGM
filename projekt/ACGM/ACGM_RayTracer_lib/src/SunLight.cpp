#include "ACGM_RayTracer_lib/SunLight.h"

//! Constructors
acgm::SunLight::SunLight(const float intensity, const glm::vec3 direct, const glm::vec3 & position):Light(intensity, position), direction_(direct)
{
}

//! Getters
glm::vec3 acgm::SunLight::GetDirectionToLight(const glm::vec3& point) const
{
	return -direction_;
}

float acgm::SunLight::GetIntensityAt(const glm::vec3& point) const
{
	return GetIntensity();
}
