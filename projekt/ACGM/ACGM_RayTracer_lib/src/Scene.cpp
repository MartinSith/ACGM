#include <ACGM_RayTracer_lib/Scene.h>
#include <omp.h>

float bias = 0.001f;
float epsilon = 0.00001f;

//! Constructors
acgm::Scene::Scene(const std::shared_ptr<acgm::Camera>& camera, const std::shared_ptr<acgm::Light>& light, const std::vector<std::shared_ptr<acgm::Model>>& models) : 
    models_(models), camera_(camera), light_(light)
{
}

//! Functions
void acgm::Scene::Raytrace(hiro::draw::RasterRenderer &renderer) const
{
    int32_t row, column;
    float y, x, dx, dy, base_x, base_y;

    //! Variables for ray direction
    dy = 2 * tan(camera_->GetFovYRad() / 2.0f) / float(renderer.GetResolution().y);
    dx = 2 * tan(camera_->GetFovYRad() / 2.0f) / float(renderer.GetResolution().x);
    base_y = tan(camera_->GetFovYRad() / 2.0f);
    base_x = -base_y;

    //! Loop through each pixel
    #pragma omp parallel for private(row, column) shared(dx, dy, base_x, base_y)
    for (row = 0; row < renderer.GetResolution().y; row++)
    {
        y = base_y - ((row + 1) * dy);
        x = -tan(camera_->GetFovYRad() / 2.0f);

        for (column = 0; column < renderer.GetResolution().x; column++)
        {
            std::shared_ptr <acgm::Ray> ray;
            std::optional<HitResult> ray_hitresult, min_hitresult;
            glm::vec3 ray_direction, ray_position;
            ShaderStruct intersected_point;
            float min;
            int i, index;

            index = -1;
            min_hitresult->distance = INFINITY;

            //! Ray from camera to scene
            ray_direction = glm::normalize(camera_->GetForwardDirection() + x * camera_->GetRightDirection() + y * camera_->GetUpDirection());
            ray_position = camera_->GetPosition();
            ray = std::make_shared<acgm::Ray>(ray_position, ray_direction, bias);

            //! Find nearest object intersect
            for (i = 0; i < models_.size(); i++)
            {
                ray_hitresult = models_.at(i)->Intersect(ray);
             
                if (ray_hitresult->distance > 0 && ray_hitresult->distance < min_hitresult->distance)
                {
                    if (ray_hitresult->distance < camera_->GetZFar() && ray_hitresult->distance > camera_->GetZNear())
                    {
                        min_hitresult->distance = ray_hitresult->distance;
                        min_hitresult->normal = ray_hitresult->normal;
                        min_hitresult->point = ray_hitresult->point;
                        index = i;
                    }
                }
            }       

            //! If no hit, continue for another pixel
            if (index == -1) 
            {
                //x += dx;
                x = ((column + 1) * dx) + base_x;
                continue;
            }

            //! If hit, calculate attributes of intersection point
            intersected_point.direction_to_light = glm::normalize(light_->GetDirectionToLight(min_hitresult->point));
            intersected_point.normal = glm::normalize(min_hitresult->normal);
            intersected_point.point = min_hitresult->point;
            intersected_point.direction_to_eye = glm::normalize(camera_->GetPosition() - min_hitresult->point);
            intersected_point.light_intensity = light_->GetIntensityAt(min_hitresult->point);

            //! Ray from intersect point to light
            ray_position = intersected_point.point;
            ray_direction = intersected_point.direction_to_light;
            ray = std::make_shared<acgm::Ray>(ray_position, ray_direction, bias);

            min = INFINITY;

            //! Find nearest object intersect
            for (i = 0; i < models_.size(); i++)
            {
                ray_hitresult = models_.at(i)->Intersect(ray);
                
                if (ray_hitresult->distance > 0 && ray_hitresult->distance < min)
                {
                     min = ray_hitresult->distance;
                }
            }
            
            float distance_from_light = glm::distance(intersected_point.point, light_->GetPosition());

            //! If shadow ray hit something, point is in shadow
            if (min < distance_from_light)
            {
                intersected_point.is_in_shadow = true;
            }
            else intersected_point.is_in_shadow = false;

            //! Render pixel in color indentified from shader and light
            renderer.SetPixel(column, renderer.GetResolution().y - row - 1, models_.at(index)->GetShader()->IdentifyColor(intersected_point));

            //x += dx;
            x = ((column + 1) * dx) + base_x;
        }
        //y -= dy;
    }
}
