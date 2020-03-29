#include <ACGM_RayTracer_lib/Mesh.h>
#include <ACGM_RayTracer_lib/Triangle.h>

//! Constructors
acgm::Mesh::Mesh(std::string file_name, glm::mat4 transform, std::string name):file_name_(file_name), transform_(transform), Model(name)
{
	mesh_.Import(file_name);
	mesh_.points->Transform(transform);
}

//! Functions
std::optional<acgm::HitResult> acgm::Mesh::Intersect(std::shared_ptr<acgm::Ray>& ray) const
{
    std::optional<HitResult> min_hit;
    min_hit->distance = 10000.0f;
    int32_t j, troj = 0;
    glm::uint point_X, point_Y, point_Z;
    float t;

    //! Check each triangle for intersect
 #pragma omp parallel for private(j, t, point_X, point_Y, point_Z) shared(troj)
    for (j = 0; j < mesh_.faces->GetFaceCount(); j++)
    {
        point_X = mesh_.faces->GetFaces()[j].x;
        point_Y = mesh_.faces->GetFaces()[j].y;
        point_Z = mesh_.faces->GetFaces()[j].z;
        
        Triangle triangle = Triangle(mesh_.points->GetPositions()[point_X], mesh_.points->GetPositions()[point_Y], mesh_.points->GetPositions()[point_Z]);
        t = triangle.Intersect(ray);

        if (t > 0 && t < min_hit->distance)
        {
            min_hit->distance = t;
            troj = j;
        }
    }

    //! Attributes of nearest intersect
    point_X = mesh_.faces->GetFaces()[troj].x;
    point_Y = mesh_.faces->GetFaces()[troj].y;
    point_Z = mesh_.faces->GetFaces()[troj].z;

    min_hit->normal = glm::cross(mesh_.points->GetPositions()[point_Y] - mesh_.points->GetPositions()[point_X], mesh_.points->GetPositions()[point_Z] - mesh_.points->GetPositions()[point_X]);
    min_hit->point = ray->GetPoint(min_hit->distance) + (min_hit->normal * ray->GetBias());

    return min_hit;
}
