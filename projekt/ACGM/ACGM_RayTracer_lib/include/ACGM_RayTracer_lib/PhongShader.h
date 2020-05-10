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
        explicit PhongShader(cogs::Color3f color, float shininess, float ambient, float diffuse, float specular, float glossiness, float transparency, float refractive_index);

        //! Functions
        // function returning information related do color at point in parameter
        virtual acgm::ShaderReturn IdentifyColor(const ShaderStruct& point) const override;

        //! Getters for private variables
        float GetShininess();
        float GetAmbient();
        float GetDiffuse();
        float GetSpecular();
        float GetGlossiness(); 
        float GetRefractiveIndex();

    private:
        float shininess_;
        float ambient_;
        float diffuse_;
        float specular_;
        float glossiness_;
        float transparency_;
        float refractive_index_;
    };
}