#include "pch.h"

/*

// Testing intersection of Ray with Plane
TEST(IntersectionTests, TestPlaneIntersection)
{
    const auto ray = std::make_shared<acgm::Ray>(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
    const auto plane = std::make_shared<acgm::Plane>(glm::vec3(0, 0, 10), glm::vec3(0, 0, 1));

    std::optional<acgm::HitResult> ray_hitresult;
    ray_hitresult = plane->Intersect(ray);

    EXPECT_TRUE(glm::epsilonEqual<float>(
        10.0f,
        ray_hitresult->distance,
        glm::epsilon<float>()
    ));
}

// Testing intersection of Ray with Sphere
TEST(IntersectionTests, TestSphereIntersection)
{
    const auto ray = std::make_shared<acgm::Ray>(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
    const auto sphere = std::make_shared<acgm::Sphere>(glm::vec3(0, 0.5, 0), 0.5);

    std::optional<acgm::HitResult> ray_hitresult;
    ray_hitresult = sphere->Intersect(ray);

    EXPECT_TRUE(glm::epsilonEqual<float>(
        7.5f,
        ray_hitresult->distance,
        glm::epsilon<float>();
    ));
}

// Testing intersection of Ray with Mesh
TEST(IntersectionTests, TestMeshIntersection)
{
    const auto ray = std::make_shared<acgm::Ray>(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
    const auto mesh = std::make_shared<acgm::Mesh>("bunny.fbx", glm::mat4(0.566312, 0.41145, 0, 0, -0.41145, 0.566312, 0, 0, 0, 0, 0.7, 0, 0, 0, 0.5, 1));

    std::optional<acgm::HitResult> ray_hitresult;
    ray_hitresult = mesh->Intersect(ray);

    EXPECT_TRUE(glm::epsilonEqual<float>(
        3.25f,
        ray_hitresult->distance,
        glm::epsilon<float>();
    ));
}

*/