#pragma once
#include <COGS/Color.h>
#include <glm/glm.hpp>

namespace acgm
{
    struct ShaderStruct
    {
        glm::vec3 point;
        glm::vec3 normal;
        glm::vec3 direction_to_eye;
        glm::vec3 direction_to_light;
        float light_intensity;
        bool is_in_shadow;
    };

    //! Representation of a shader
    class Shader
    {
    public:
        //! Constructors
        Shader();
        ~Shader() = default;
        explicit Shader(cogs::Color3f color);

        //! Functions
        virtual cogs::Color3f IdentifyColor(const ShaderStruct& point) const;

    private:
        cogs::Color3f color_;
    };
}