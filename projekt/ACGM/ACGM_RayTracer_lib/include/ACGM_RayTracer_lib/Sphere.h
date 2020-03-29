#pragma once
#include <ACGM_RayTracer_lib/Model.h>

namespace acgm
{
  class Sphere : public Model
  {
  public:
    //! Constructors
    explicit Sphere(const glm::vec3 &position, float radius, std::string name);
    virtual ~Sphere() = default;
    
    //! Functions
    virtual std::optional<HitResult> Intersect(std::shared_ptr<acgm::Ray>& ray) const override;

    //! Getters
    const glm::vec3 &GetPosition() const;
    float GetRadius() const;

  private:
    glm::vec3 position_;
    float radius_;
  };
}
