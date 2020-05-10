#include "pch.h"

// Testing intersection of Ray with Plane
TEST(TransparencyTests, TestTransparency)
{
    glm::vec3 normal = glm::vec3(0, 1, 0);
    glm::vec3 ray_direction = glm::vec3(1, 1, 1);
    float n1 = 1.02f;
    float n2 = 0.92f;
    float n;
    glm::vec3 norm = normal;

    float cos1 = glm::dot(ray_direction, normal);
    if (cos1 < 0)
    {
        n = n1 / n2;
        cos1 = -cos1;
    }
    else
    {
        n = n2 / n1;
        norm = -normal;
    }

    float cos2 = 1 - (n * n) * (1 - (cos1 * cos1));

    glm::vec3 transparency = (n * ray_direction + (n * cos1 - sqrtf(cos2)) * norm);

    EXPECT_TRUE(glm::epsilonEqual<float>(
        0.9019607902f,
        transparency.r,
        glm::epsilon<float>()
        ));

    EXPECT_TRUE(glm::epsilonEqual<float>(
        1.0f,
        transparency.g,
        glm::epsilon<float>()
        ));

    EXPECT_TRUE(glm::epsilonEqual<float>(
        0.9019607902f,
        transparency.b,
        glm::epsilon<float>()
        ));
}