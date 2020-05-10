#pragma once
#include <COGS/Color.h>
#include <glm/glm.hpp>

namespace acgm
{
    // structure for information about intersection point
    struct ShaderStruct
    {
        glm::vec3 point;
        glm::vec3 normal;
        glm::vec3 direction_to_eye;
        glm::vec3 direction_to_light;
        float light_intensity;
        bool is_in_shadow;
    };

    // structure for information related to color of shader
    struct ShaderReturn
    {
        cogs::Color3f color;
        float glossiness;
        float transparency;
        float refractive_index;
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
        // function returning information related do color at point in parameter
        virtual acgm::ShaderReturn IdentifyColor(const ShaderStruct& point) const;

    private:
        cogs::Color3f color_;
    };
}