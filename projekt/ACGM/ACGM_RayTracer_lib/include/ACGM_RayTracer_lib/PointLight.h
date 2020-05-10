#pragma once
#include <glm/glm.hpp>
#include <ACGM_RayTracer_lib/Light.h>

namespace acgm
{
    //! Representation of a point light
    class PointLight: public Light
    {
    public:
        //! Constructors
        explicit PointLight(float intensity, glm::vec3 position, float range, float linear_atten, float quadratic_atten);

        //! Getters for private variables
        float GetRange();
        float GetQuadraticAttenuation();
        float GetLinearAttenuation();

        // Overriden function from Light class, returning direction to light source
        virtual glm::vec3 GetDirectionToLight(const glm::vec3& point) const override;

        // Overriden function from Light class, returning light intensity at point in parameter
        virtual float GetIntensityAt(const glm::vec3& point) const override;

    private:
        float range_;
        float quadratic_attenuation_;
        float linear_attenuation_;
    };
}
