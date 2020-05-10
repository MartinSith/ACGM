#pragma once
#include <glm/glm.hpp>

namespace acgm
{
  //! Representation of a ray
  class Ray
  {
  public:
      //! Constructors
      explicit Ray(glm::vec3 orig, glm::vec3 direct, const float bias);

      //! Getters for private variables
      glm::vec3 GetOrigin();
      glm::vec3 GetDirection();
      glm::vec3 GetPoint(const float t);
      float GetBias();

  private:
      glm::vec3 origin_;
      glm::vec3 direction_;
      const float bias_;
  };
}
