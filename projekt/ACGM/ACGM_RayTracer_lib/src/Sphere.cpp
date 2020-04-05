#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <ACGM_RayTracer_lib/Sphere.h>

//! Constructors
acgm::Sphere::Sphere(const glm::vec3 &position, float radius, std::string name)
  : Model(name), position_(position), radius_(radius)
{
}

//! Functions
std::optional<acgm::HitResult> acgm::Sphere::Intersect(std::shared_ptr<acgm::Ray>& ray) const
{
    glm::vec3 oo;
    float A, B, C, D, sD, t1, t2, t;
    HitResult hitresult;

    oo = ray->GetOrigin() - position_;
    A = glm::dot(ray->GetDirection(), ray->GetDirection());
    B = -2.0f * glm::dot(oo, ray->GetDirection());
    C = glm::dot(oo, oo) - radius_ * radius_;
    D = B * B - 4.0f * A * C;

    if (D < 0)
    {
        return std::nullopt;
    }

    sD = glm::sqrt(D);
    t1 = 0.5f * (B + sD) / A;

    if (t1 < ray->GetBias())
    {
        t1 = INFINITY;
    }

    t2 = 0.5f * (B - sD) / A;

    if (t2 < ray->GetBias())
    {
        t2 = INFINITY;
    }

    t = glm::min(t1, t2);

    if (glm::isinf(t) || t < 0.0f)
    {
        return std::nullopt;
    }

    hitresult.distance = t;
    hitresult.normal = glm::normalize(ray->GetPoint(t) - position_);
    hitresult.point = ray->GetPoint(hitresult.distance);
  
    return hitresult;
}

//! Getters
const glm::vec3 &acgm::Sphere::GetPosition() const
{
    return position_;
}

float acgm::Sphere::GetRadius() const
{
    return radius_;
}

