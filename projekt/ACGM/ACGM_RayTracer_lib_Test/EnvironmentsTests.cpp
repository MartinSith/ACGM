#include "pch.h"

// Testing intensity of SunLight
TEST(EnvironmentsTests, TestImageColorAt)
{
    std::shared_ptr<acgm::Image> image = std::make_shared<acgm::Image>("enviro0.jpg");

    EXPECT_TRUE(glm::epsilonEqual<float>(
        0.71372551f,
        image->GetColorAt(glm::vec2(0, 0)).r,
        glm::epsilon<float>()
        ));

    EXPECT_TRUE(glm::epsilonEqual<float>(
        0.6509804130f,
        image->GetColorAt(glm::vec2(0, 0)).g,
        glm::epsilon<float>()
        ));

    EXPECT_TRUE(glm::epsilonEqual<float>(
        0.5215686560f,
        image->GetColorAt(glm::vec2(0, 0)).b,
        glm::epsilon<float>()
        ));
}