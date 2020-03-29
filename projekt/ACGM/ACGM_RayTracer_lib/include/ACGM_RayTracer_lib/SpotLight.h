#pragma once
#include <glm/glm.hpp>
#include <ACGM_RayTracer_lib/PointLight.h>

namespace acgm
{
    //! Representation of a SpotLight
    class SpotLight : public PointLight
    {
    public:
        //! Constructors
        explicit SpotLight(float intensity, glm::vec3 position, float range,
                           float linear_atten, float quadratic_atten, glm::vec3 spot_direction,
                           float cutoff_angle, float exponent);

        //! Getters
        glm::vec3 GetSpotDirection();
        float GetCutoffAngle();
        float GetExponent();
        virtual glm::vec3 GetDirectionToLight(const glm::vec3& point) const override;
        virtual float GetIntensityAt(const glm::vec3& point) const override;

    private:
        glm::vec3 spot_direction_;
        float cutoff_angle_;
        float exponent_;
    };
}
