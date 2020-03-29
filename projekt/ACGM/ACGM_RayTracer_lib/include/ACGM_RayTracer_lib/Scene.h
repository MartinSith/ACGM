#pragma once
#include <vector>
#include <HIRO_DRAW/renderers/RasterRenderer.h>
#include <ACGM_RayTracer_lib/Camera.h>
#include <ACGM_RayTracer_lib/Model.h>
#include <ACGM_RayTracer_lib/Light.h>
#include <ACGM_RayTracer_lib/Shader.h>

namespace acgm
{
  //! Scene representation
  class Scene
  {
  public:
    //! Constructors
    Scene(const std::shared_ptr<acgm::Camera>& camera, const std::shared_ptr<acgm::Light>& light, const std::vector<std::shared_ptr<acgm::Model>>& models);
    ~Scene() = default;

    //! Functions
    void Raytrace(hiro::draw::RasterRenderer &renderer) const;
    std::optional<acgm::HitResult> acgm::Scene::SearchModel(std::shared_ptr<acgm::Ray> ray, std::shared_ptr<acgm::Camera> camera, int& index) const;

  private:
      std::shared_ptr<acgm::Camera> camera_;
      std::shared_ptr<acgm::Light> light_;
      std::vector<std::shared_ptr<acgm::Model>> models_;
  };
}