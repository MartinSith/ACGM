#pragma once

#include <COGS/Mesh.h>
#include <ACGM_RayTracer_lib/Model.h>

namespace acgm
{
    //! Mesh representation
    class Mesh : public Model
    {
    public:
        //! Constructors
        explicit Mesh(std::string file_name, glm::mat4 transform, std::string name);
        ~Mesh() = default;

        //! Functions
        virtual std::optional<acgm::HitResult> Intersect(std::shared_ptr<acgm::Ray>& ray) const override;
       
    private:
        std::string file_name_;
        glm::mat4 transform_;
        cogs::Mesh mesh_;
    };
}