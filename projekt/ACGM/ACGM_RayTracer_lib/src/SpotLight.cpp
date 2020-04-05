#define _USE_MATH_DEFINES
#include <math.h>
#include "ACGM_RayTracer_lib/SpotLight.h"

//! Constructors
acgm::SpotLight::SpotLight(float intensity, glm::vec3 position, float range,
						   float linear_atten, float quadratic_atten, glm::vec3 spot_direction,
						   float cutoff_angle, float exponent)
	:PointLight(intensity, position, range, linear_atten, quadratic_atten)
{
	this->spot_direction_ = spot_direction;
	this->cutoff_angle_ = cutoff_angle;
	this->exponent_ = exponent;
}

//! Getters
glm::vec3 acgm::SpotLight::GetSpotDirection()
{
	return spot_direction_;
}

float acgm::SpotLight::GetCutoffAngle()
{
	return cutoff_angle_;
}

float acgm::SpotLight::GetExponent()
{
	return exponent_;
}

glm::vec3 acgm::SpotLight::GetDirectionToLight(const glm::vec3& point) const
{
	return acgm::PointLight::GetDirectionToLight(point);
}

float acgm::SpotLight::GetIntensityAt(const glm::vec3& point) const
{
	float point_intensity = acgm::PointLight::GetIntensityAt(point);
	glm::vec3 light_direction = GetDirectionToLight(point);

	float light_direction_exp = pow(light_direction.x, 2) + pow(light_direction.y, 2) + pow(light_direction.z, 2);
	float spot_direction_exp = pow(spot_direction_.x, 2) + pow(spot_direction_.y, 2) + pow(spot_direction_.z, 2);
	float angle = glm::acos(glm::dot(spot_direction_, -light_direction) / (sqrt(spot_direction_exp) * sqrt(light_direction_exp))) * 180.0f / float(M_PI); 

	float decay = 1.0f - pow((angle / cutoff_angle_), exponent_);
	float intensity = point_intensity * decay;

	return intensity;
}
