#include <ACGM_RayTracer_lib/Plane.h>

//! Constructors
acgm::Plane::Plane(glm::vec3 plane_pt, glm::vec3 plane_norm, std::string name) : Model(name), plane_point_(plane_pt), plane_normal_(plane_norm)
{
}

//! Functions
std::optional<acgm::HitResult> acgm::Plane::Intersect(std::shared_ptr<acgm::Ray>& ray) const
{
    float dot_product;
    HitResult hitresult;

    dot_product = glm::dot(ray->GetDirection(), plane_normal_);

    if (dot_product == 0)
    {
        return std::nullopt;
    }

    //! Attributes of hit result
    hitresult.distance = glm::dot((plane_point_ - ray->GetOrigin()), plane_normal_) / dot_product;
    hitresult.normal = plane_normal_;
    hitresult.point = ray->GetPoint(hitresult.distance) + (hitresult.normal * ray->GetBias());;

    return hitresult;
}

//! Getters
glm::vec3 acgm::Plane::GetPlanePoint() 
{ 
	return plane_point_; 
}

glm::vec3 acgm::Plane::GetPlaneNormal() 
{ 
	return plane_normal_; 
}