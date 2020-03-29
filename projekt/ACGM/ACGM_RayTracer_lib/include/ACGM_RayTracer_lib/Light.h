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

        //! Getters
        float GetIntensity() const;
        glm::vec3 GetPosition() const;
        virtual glm::vec3 GetDirectionToLight(const glm::vec3& point) const;
        virtual float GetIntensityAt(const glm::vec3& point) const;

    private:
        float intensity_;
        glm::vec3 position_;
    };
}
