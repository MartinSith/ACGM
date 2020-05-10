#include "ACGM_RayTracer_lib/PhongShader.h"

//! Constructors
acgm::PhongShader::PhongShader(cogs::Color3f color, float shininess,
    float ambient, float diffuse, float specular, float glossiness, 
    float transparency, float refractive_index): Shader(color)
{
    this->shininess_ = shininess;
    this->ambient_ = ambient;
    this->diffuse_ = diffuse;
    this->specular_ = specular;
    this->glossiness_ = glossiness;
    this->transparency_ = transparency;
    this->refractive_index_ = refractive_index;
}

//! Functions
acgm::ShaderReturn acgm::PhongShader::IdentifyColor(const ShaderStruct& point) const
{
    ShaderReturn shader_info;
    cogs::Color3f specular_phong;
    float diffuse_phong;
    glm::vec3 resultant;
    float normal_size, direction_to_light_size, angle;

    normal_size = sqrt((point.normal.x * point.normal.x) + (point.normal.y * point.normal.y) + (point.normal.z * point.normal.z));
    direction_to_light_size = sqrt((point.direction_to_light.x * point.direction_to_light.x) + (point.direction_to_light.y * point.direction_to_light.y) + (point.direction_to_light.z * point.direction_to_light.z));
    angle = glm::dot(point.normal, point.direction_to_light) / (direction_to_light_size * normal_size);

    //! Ambient phong
    if (point.is_in_shadow)
    {
        shader_info.color = Shader::IdentifyColor(point).color * ambient_;
        shader_info.glossiness = glossiness_;
        shader_info.transparency = transparency_;
        shader_info.refractive_index = refractive_index_;
        return shader_info;
    }

    //! Specular phong
    resultant = glm::normalize(point.direction_to_eye + point.direction_to_light);
    specular_phong = cogs::Color3f(1.0f, 1.0f, 1.0f) * specular_ * point.light_intensity * pow(glm::dot(point.normal, resultant), shininess_);

    //! Diffuse phong
    diffuse_phong = diffuse_ * point.light_intensity * angle;

    shader_info.color = (diffuse_phong + ambient_) * Shader::IdentifyColor(point).color + specular_phong;
    shader_info.glossiness = glossiness_;
    shader_info.transparency = transparency_;
    shader_info.refractive_index = refractive_index_;
    return shader_info;
}


//! Getters
float acgm::PhongShader::GetShininess()
{
    return shininess_;
}

float acgm::PhongShader::GetAmbient()
{
    return ambient_;
}

float acgm::PhongShader::GetDiffuse()
{
    return diffuse_;
}

float acgm::PhongShader::GetSpecular()
{
    return specular_;
}

float acgm::PhongShader::GetGlossiness()
{
    return glossiness_;
}

float acgm::PhongShader::GetRefractiveIndex()
{
    return refractive_index_;
}