#include "ACGM_RayTracer_lib/PointLight.h"
#include <cmath>

//! Constructors
acgm::PointLight::PointLight(float intensity, glm::vec3 position, float range, float linear_atten, float quadratic_atten) :
	Light(intensity, position)
{
	this->range_ = range;
	this->linear_attenuation_ = linear_atten;
	this->quadratic_attenuation_ = quadratic_atten;
}

//! Getters
float acgm::PointLight::GetRange()
{
	return range_;
}

float acgm::PointLight::GetLinearAttenuation()
{
	return linear_attenuation_;
}

float acgm::PointLight::GetQuadraticAttenuation()
{
	return quadratic_attenuation_;
}

glm::vec3 acgm::PointLight::GetDirectionToLight(const glm::vec3& point) const
{
	glm::vec3 position = acgm::Light::GetPosition();
	return glm::vec3(position.x - point.x, position.y - point.y, position.z - point.z);
}

float acgm::PointLight::GetIntensityAt(const glm::vec3& point) const
{
	glm::vec3 position = acgm::Light::GetPosition();
	float distance = glm::distance(position, point);
	
	float range_exp = pow(range_, 2);
	float linear = range_ / (range_ + (distance * linear_attenuation_));
	float quadratic = range_exp / (range_exp + (pow(distance, 2) * quadratic_attenuation_));
	
	float intensity = Light::GetIntensityAt(point) * linear * quadratic;

	return intensity;
}
