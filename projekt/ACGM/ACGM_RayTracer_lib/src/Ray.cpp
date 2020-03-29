#include <ACGM_RayTracer_lib/Ray.h>

//! Constructors
acgm::Ray::Ray(glm::vec3 orig, glm::vec3 direct, const float bias)
    : origin_(orig), direction_(direct), bias_(bias)
{
}

//! Getters
glm::vec3 acgm::Ray::GetOrigin() 
{ 
	return origin_; 
}

glm::vec3 acgm::Ray::GetDirection() 
{ 
	return direction_; 
}

glm::vec3 acgm::Ray::GetPoint(const float t)
{
    return origin_ + (direction_ * t);
}

float acgm::Ray::GetBias()
{
    return bias_;
}
