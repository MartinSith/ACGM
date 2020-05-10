#include "pch.h"

// Testing intersection of Ray with Plane
TEST(ReflectionTests, TestReflection)
{
    glm::vec3 I = glm::vec3(0 ,1 ,0);
    glm::vec3 N = glm::vec3(1, 1, 0);
    glm::vec3 reflection;

    reflection = I - 2 * glm::dot(I, N) * N;

    EXPECT_TRUE(glm::epsilonEqual<float>(
        -2.0f,
        reflection.r,
        glm::epsilon<float>()
    ));

    EXPECT_TRUE(glm::epsilonEqual<float>(
        -1.0f,
        reflection.g,
        glm::epsilon<float>()
        ));

    EXPECT_TRUE(glm::epsilonEqual<float>(
        0.0f,
        reflection.b,
        glm::epsilon<float>()
        ));
}