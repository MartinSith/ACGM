#pragma once
#include <glm/glm.hpp>
#include <ACGM_RayTracer_lib/Light.h>

namespace acgm
{
    //! Representation of a SunLight
    class SunLight : public Light
    {
    public:
        //! Constructors
        explicit SunLight(const float intensity, const glm::vec3 direct, const glm::vec3& position);

        //! Getters
        virtual glm::vec3 GetDirectionToLight(const glm::vec3& point) const override;
        virtual float GetIntensityAt(const glm::vec3& point) const override;

    private:
        glm::vec3 direction_;
    };
}
