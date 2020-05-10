#pragma once
//#include <glm/glm.hpp>
//#include <ACGM_RayTracer_lib\PhongShader.h>

#include <ACGM_RayTracer_lib/Shader.h>

namespace acgm
{
    //! Representation of checker shader
    class CheckerShader: public Shader
    {
    public:
        //! Constructors
        explicit CheckerShader(float cube_size, const std::shared_ptr<Shader> shader0, const std::shared_ptr<Shader> shader1);

        //! Functions
        // function returning information related do color at point in parameter
        virtual acgm::ShaderReturn IdentifyColor(const ShaderStruct& point) const override;

        //! Getters for private variables
        float GetCubeSize();

    private:
        float cube_size_;
        std::shared_ptr<Shader> shader0_;
        std::shared_ptr<Shader> shader1_;
    };
}