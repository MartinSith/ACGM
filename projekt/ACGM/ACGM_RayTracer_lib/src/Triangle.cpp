#include<ACGM_RayTracer_lib/Triangle.h>

//! Constructors
acgm::Triangle::Triangle(const glm::vec3 vertex_a, const glm::vec3 vertex_b, const glm::vec3 vertex_c):
    vertex_a_(vertex_a), vertex_b_(vertex_b), vertex_c_(vertex_c)
{
}

//! Functions
float acgm::Triangle::Intersect(std::shared_ptr<acgm::Ray> ray) const
{
    glm::vec3 edge_a, edge_b, cross_product_a, cross_product_b, s;
    float dot_product_a, f, dot_product_b, dot_product_c, t;
    extern float epsilon;

    edge_a = vertex_b_ - vertex_a_;
    edge_b = vertex_c_ - vertex_a_;
    cross_product_a = glm::cross(ray->GetDirection(), edge_b);
    dot_product_a = glm::dot(edge_a, cross_product_a);

    if (dot_product_a > -epsilon && dot_product_a < epsilon)
    {
        return -1;
    }
        
    f = 1.0 / dot_product_a;
    s = ray->GetOrigin() - vertex_a_;
    dot_product_b = f * glm::dot(s, cross_product_a);

    if (dot_product_b <= 0.0f || dot_product_b > 1.0f)
    {
        return -1;
    }

    cross_product_b = glm::cross(s, edge_a);
    dot_product_c = f * glm::dot(ray->GetDirection(), cross_product_b);

    if (dot_product_c <= 0.0f || (dot_product_b + dot_product_c) > 1.0f)
    {
        return -1;
    }

    t = f * glm::dot(edge_b, cross_product_b);

    return t;
}