#pragma once
#include <glm/glm.hpp>

namespace acgm
{
    //! Representation of a light
    class Light
    {
    public:
        //! Constructors
        explicit Light(float intensity, const glm::vec3 &position);
        virtual ~Light() = default;

        //! Getters for private variables
        float GetIntensity() const;
        glm::vec3 GetPosition() const;

        // function returning direction to light source
        virtual glm::vec3 GetDirectionToLight(const glm::vec3& point) const;

        // function returning light intensity at point in parameter
        virtual float GetIntensityAt(const glm::vec3& point) const;

    private:
        float intensity_;
        glm::vec3 position_;
    };
}
