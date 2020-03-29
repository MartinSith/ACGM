#pragma once
//#include <COGS/Color.h>
//#include <glm/glm.hpp>

#include <ACGM_RayTracer_lib/Shader.h>

namespace acgm
{
    //! Representation of phong shader
    class PhongShader:public Shader
    {
    public:
        //! Constructors
        explicit PhongShader(cogs::Color3f color, float shininess, float ambient, float diffuse, float specular);

        //! Functions
        virtual cogs::Color3f IdentifyColor(const ShaderStruct& point) const override;

        //! Getters
        float GetShininess();
        float GetAmbient();
        float GetDiffuse();
        float GetSpecular();

    private:
        float shininess_;
        float ambient_;
        float diffuse_;
        float specular_;
    };
}