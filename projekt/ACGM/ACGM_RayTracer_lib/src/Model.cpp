#include <ACGM_RayTracer_lib/Model.h>

//! Constructors
acgm::Model::Model(const std::string name)
  : name_(name)
{
}

//! Functions
std::optional<acgm::HitResult> acgm::Model::Intersect(std::shared_ptr<acgm::Ray>& ray) const
{
    acgm::HitResult result;
    return result;
}

//! Setters
void acgm::Model::SetShader(const std::shared_ptr<acgm::Shader> shader)
{
    this->shader_ = shader;
}

//! Getters
std::string acgm::Model::GetName()
{
    return name_;
}

std::shared_ptr<acgm::Shader> acgm::Model::GetShader() const
{
    return shader_;
}