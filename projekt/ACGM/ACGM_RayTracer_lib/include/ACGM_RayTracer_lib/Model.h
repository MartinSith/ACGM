#pragma once
#include <optional>
#include <ACGM_RayTracer_lib/Ray.h>
#include <ACGM_RayTracer_lib/Shader.h>

namespace acgm
{
  struct HitResult {
      glm::vec3 normal;
      glm::vec3 point;
      float distance;
  };

  //! Model  - abstract base class for scene models
  class Model
  {
  public:
    //! Constructors
    explicit Model(const std::string name);
    virtual ~Model() = default;

    //! Functions
    // function returning information about intersection with ray in parameter
    virtual std::optional<acgm::HitResult> Intersect(std::shared_ptr<acgm::Ray>& ray) const;

    //! Setters for private variables
    void SetShader(const std::shared_ptr<acgm::Shader> shader);

    //! Getters for private variables
    std::string GetName();
    std::shared_ptr<acgm::Shader> GetShader() const;

  private:
    std::string name_;
    std::shared_ptr<acgm::Shader> shader_;
  };
}
