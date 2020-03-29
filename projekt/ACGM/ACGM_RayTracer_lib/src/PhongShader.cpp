#include "ACGM_RayTracer_lib/PhongShader.h"

//! Constructors
acgm::PhongShader::PhongShader(cogs::Color3f color, float shininess,
    float ambient, float diffuse, float specular): Shader(color)
{
    this->shininess_ = shininess;
    this->ambient_ = ambient;
    this->diffuse_ = diffuse;
    this->specular_ = specular;
}

//! Functions
cogs::Color3f acgm::PhongShader::IdentifyColor(const ShaderStruct& point) const
{
    cogs::Color3f specular_phong, ambient_phong, diffuse_phong;
    glm::vec3 resultant;
    float normal_size, direction_to_light_size, angle;

    normal_size = sqrt(pow(point.normal.x, 2) + pow(point.normal.y, 2) + pow(point.normal.z, 2));
    direction_to_light_size = sqrt(pow(point.direction_to_light.x, 2) + pow(point.direction_to_light.y, 2) + pow(point.direction_to_light.z, 2));
    angle = glm::dot(point.normal, point.direction_to_light) / (direction_to_light_size * normal_size);

    //! Specular phong
    resultant = glm::normalize(point.direction_to_eye + point.direction_to_light);
    specular_phong = cogs::Color3f(1.0f, 1.0f, 1.0f) * specular_ * point.light_intensity * pow(glm::dot(point.normal, resultant), shininess_);

    //! Ambient phong
    if (point.is_in_shadow)
    {
        ambient_phong = Shader::IdentifyColor(point) * ambient_ * point.light_intensity * angle;
        return ambient_phong;
    }
    else
    {
        ambient_phong = Shader::IdentifyColor(point) * (ambient_ + ((1 - ambient_) * point.light_intensity));
    }

    //! Diffuse phong
    diffuse_phong = Shader::IdentifyColor(point) * diffuse_ * point.light_intensity * angle;

    return diffuse_phong + specular_phong;
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