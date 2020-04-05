#include "pch.h"

// Testing intensity of SunLight
TEST(LightsTests, TestSunLightIntensity)
{
    const auto sun = std::make_shared<acgm::SunLight>(0.8, glm::vec3(0, 0, -1), glm::vec3(1000.0f, 1000.0f, 1000.0f));
   
    EXPECT_TRUE(glm::epsilonEqual<float>(
        0.8f,
        sun->GetIntensityAt(glm::vec3(1.0f, 1.0f, -1.0f)),
        glm::epsilon<float>()
        ));
}

// Testing intensity of PointLight
TEST(LightsTests, TestPointLightIntensity)
{
    const auto point = std::make_shared<acgm::PointLight>(1, glm::vec3(0, 3, -2), 50, 0, 1);

    EXPECT_TRUE(glm::epsilonEqual<float>(
        0.9948269f,
        point->GetIntensityAt(glm::vec3(0.0f, 0.0f, 0.0f)),
        glm::epsilon<float>()
        ));
}

// Testing intensity of SpotLight
TEST(LightsTests, TestSpotLightIntensity)
{
    const auto spot = std::make_shared<acgm::SpotLight>(1, glm::vec3(10, 10, 10), 50, 0, 1, glm::vec3(-1, -1, -1), 40, 3);

    EXPECT_TRUE(glm::epsilonEqual<float>(
        0.8928571343f,
        spot->GetIntensityAt(glm::vec3(0.0f, 0.0f, 0.0f)),
        glm::epsilon<float>()
        ));
}
