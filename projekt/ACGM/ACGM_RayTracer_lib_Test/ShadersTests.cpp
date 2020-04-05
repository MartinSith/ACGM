#include "pch.h"

/*

// Testing Phong shader color not in shadow
TEST(ShadersTests, TestPhongColor)
{
    const auto phong = std::make_shared<acgm::PhongShader>(cogs::Color3f(1.0f, 1.0f, 1.0f), 5, 0.3f, 0.7f, 0);

    acgm::ShaderStruct intersected_point;
    intersected_point.point = glm::vec3(1, 2, 1);
    intersected_point.normal = glm::vec3(1.5, 2.5, 1);
    intersected_point.direction_to_eye = glm::vec3(-1, -0.5, 1);
    intersected_point.direction_to_light = glm::vec3(-1, 1, 0.5);
    intersected_point.light_intensity = 1.0f;
    intersected_point.is_in_shadow = false;

    EXPECT_TRUE(glm::epsilonEqual<cogs::Color3f>(
        cogs::Color3f(0.035f, 0.035f, 0.035f),
        phong->IdentifyColor(intersected_point),
        glm::epsilon<cogs::Color3f>()
        ));
}

// Testing Phong shader color in shadow
TEST(ShadersTests, TestPhongShadowColor)
{
    const auto phong = std::make_shared<acgm::PhongShader>(cogs::Color3f(1.0f, 1.0f, 1.0f), 5, 0.3f, 0.7f, 0);

    acgm::ShaderStruct intersected_point;
    intersected_point.point = glm::vec3(1, 2, 1);
    intersected_point.normal = glm::vec3(1.5, 2.5, 1);
    intersected_point.direction_to_eye = glm::vec3(-1, -0.5, 1);
    intersected_point.direction_to_light = glm::vec3(-1, 1, 0.5);
    intersected_point.light_intensity = 1.0f;
    intersected_point.is_in_shadow = true;

    EXPECT_TRUE(glm::epsilonEqual<cogs::Color3f>(
        cogs::Color3f(0.02f, 0.02f, 0.02f),
        phong->IdentifyColor(intersected_point),
        glm::epsilon<cogs::Color3f>()
        ));
}

// Testing Checker shader color not in shadow
TEST(ShadersTests, TestCheckerColor)
{
    const auto phong_1 = std::make_shared<acgm::PhongShader>(cogs::Color3f(0.121569f, 0.54902f, 0.270588f), 50, 0.3f, 0.8f, 0.8f);
    const auto phong_2 = std::make_shared<acgm::PhongShader>(cogs::Color3f(0.901961f, 0.901961f, 0.901961f), 50, 0.3f, 0.8f, 0.8f);
    const auto checker = std::make_shared<acgm::CheckerShader>(0.5f, phong_1, phong_2);

    acgm::ShaderStruct intersected_point;
    intersected_point.point = glm::vec3(1, 2, 1);
    intersected_point.normal = glm::vec3(1.5, 2.5, 1);
    intersected_point.direction_to_eye = glm::vec3(-1, -0.5, 1);
    intersected_point.direction_to_light = glm::vec3(-1, 1, 0.5);
    intersected_point.light_intensity = 1.0f;
    intersected_point.is_in_shadow = false;

    EXPECT_TRUE(glm::epsilonEqual<cogs::Color3f>(
        cogs::Color3f(0.042f, 0.042f, 0.042f),
        checker->IdentifyColor(intersected_point),
        glm::epsilon<cogs::Color3f>()
        ));
}

// Testing Phong shader color in shadow
TEST(ShadersTests, TestCheckerShadowColor)
{
    const auto phong_1 = std::make_shared<acgm::PhongShader>(cogs::Color3f(0.121569f, 0.54902f, 0.270588f), 50, 0.3f, 0.8f, 0.8f);
    const auto phong_2 = std::make_shared<acgm::PhongShader>(cogs::Color3f(0.901961f, 0.901961f, 0.901961f), 50, 0.3f, 0.8f, 0.8f);
    const auto checker = std::make_shared<acgm::CheckerShader>(0.5f, phong_1, phong_2);

    acgm::ShaderStruct intersected_point;
    intersected_point.point = glm::vec3(1, 2, 1);
    intersected_point.normal = glm::vec3(1.5, 2.5, 1);
    intersected_point.direction_to_eye = glm::vec3(-1, -0.5, 1);
    intersected_point.direction_to_light = glm::vec3(-1, 1, 0.5);
    intersected_point.light_intensity = 1.0f;
    intersected_point.is_in_shadow = true;

    EXPECT_TRUE(glm::epsilonEqual<cogs::Color3f>(
        cogs::Color3f(0.023f, 0.023f, 0.023f),
        checker->IdentifyColor(intersected_point),
        glm::epsilon<cogs::Color3f>()
        ));
}

*/