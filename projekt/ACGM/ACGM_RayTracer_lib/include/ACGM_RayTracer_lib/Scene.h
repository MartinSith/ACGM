#pragma once
#include <vector>
#include <HIRO_DRAW/renderers/RasterRenderer.h>
#include <ACGM_RayTracer_lib/Camera.h>
#include <ACGM_RayTracer_lib/Model.h>
#include <ACGM_RayTracer_lib/Light.h>
#include <ACGM_RayTracer_lib/Shader.h>
#include <ACGM_RayTracer_lib/Image.h>

namespace acgm
{
  //! Scene representation
  class Scene
  {
  public:
    //! Constructors
    Scene(const std::shared_ptr<acgm::Camera>& camera, const std::shared_ptr<acgm::Light>& light, const std::vector<std::shared_ptr<acgm::Model>>& models, glm::vec3 enviro_up, glm::vec3 enviro_seam, std::string enviro_image_file, float input_bias, float index_of_refraction, int reflection_number, int transparency_number);
    ~Scene() = default;

    //! Functions
    // Function for scene raytrace
    void Raytrace(hiro::draw::RasterRenderer &renderer) const;

    // Recursive function returns mixed color of transparency and reflection
    cogs::Color3f acgm::Scene::CalculateColorRecursion(int number_of_reflection_hits, int number_of_transparency_hits, int model_index, acgm::ShaderStruct intersected_point, glm::vec3 ray_direction, std::shared_ptr<acgm::Image> image) const;
    
    // Function returns information about intersected point
    acgm::ShaderStruct acgm::Scene::CalculateIntersectPoint(std::optional<acgm::HitResult> hitresult) const;
    
    // Function returns color of image pixel calculated from ray direction
    cogs::Color3f acgm::Scene::CalculateImageColor(glm::vec3 ray_direction, std::shared_ptr<acgm::Image> image) const;

  private:
      std::shared_ptr<acgm::Camera> camera_;
      std::shared_ptr<acgm::Light> light_;
      std::vector<std::shared_ptr<acgm::Model>> models_;
      glm::vec3 enviro_up_;
      glm::vec3 enviro_seam_;
      std::string enviro_image_file_;
      float bias_;
      float index_of_refraction_;
      int reflection_number_;
      int transparency_number_;
  };
}
