#pragma once
#include <Utils/Dialogs.h>
#include <glm/glm.hpp>
#include <ACGM_RayTracer_lib/Ray.h>

namespace acgm
{
    //! Representation of a triangle in mesh object
    class Triangle
    {
    public:
        //! Constructors
        explicit Triangle(const glm::vec3 vertex_a, const glm::vec3 vertex_b, const glm::vec3 vertex_c);
        virtual ~Triangle() = default;

        //! Functions
        // function returning information about intersection with ray in parameter
        float Intersect(std::shared_ptr<acgm::Ray> ray) const;

    private:
        glm::vec3 vertex_a_;
        glm::vec3 vertex_b_;
        glm::vec3 vertex_c_;
    };
}
