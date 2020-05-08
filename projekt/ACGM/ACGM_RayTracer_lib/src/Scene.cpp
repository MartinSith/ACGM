#include <ACGM_RayTracer_lib/Scene.h>
#include <ACGM_RayTracer_lib/Image.h>
#include <omp.h>
#include <glm/gtx/vector_angle.hpp>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

float bias = 0.001f;
float epsilon = 0.00001f;

//! Constructors
acgm::Scene::Scene(const std::shared_ptr<acgm::Camera>& camera, const std::shared_ptr<acgm::Light>& light, 
                   const std::vector<std::shared_ptr<acgm::Model>>& models, glm::vec3 enviro_up, 
                   glm::vec3 enviro_seam, std::string enviro_image_file, float bias, float index_of_refraction) :
    models_(models), camera_(camera), light_(light), enviro_up_(enviro_up), enviro_seam_(enviro_seam), 
    enviro_image_file_(enviro_image_file), bias_(bias), index_of_refraction_(index_of_refraction)
{
    //bias = bias_;
}

glm::vec3 reflect(const glm::vec3& I, const glm::vec3& N)
{
    return I - 2 * glm::dot(I, N) * N;
}

glm::vec3 refract(const glm::vec3& ray_direction, const glm::vec3& normal, const float& n_1, const float& n_2)
{
    float dot_product = glm::dot(ray_direction, normal);
    float cos1 = std::clamp(-1.f, 1.f, dot_product);
    float n1 = n_1, n2 = n_2;
    glm::vec3 norm = normal;
    if (cos1 < 0) 
    { 
        cos1 = -cos1; 
    }
    else 
    { 
        std::swap(n1, n2); 
        norm = -normal;
    }
    float n = n1 / n2;
    float cos2 = 1 - n * n * (1 - cos1 * cos1);
    glm::vec3 refracted = (n * ray_direction + (n * cos1 - sqrtf(cos2)) * n);
    //if (k < 0) 
    //{
    //    return std::nullopt;
    //}
    //else
    //{
        return refracted;
    //}
}

cogs::Color3f Reflection_new(bool typ_funkcie, float number_of_reflection_hits, float number_of_transparency_hits, int past_model_index, int model_index, acgm::ShaderStruct intersected_point, glm::vec3 ray_direction, const std::vector<std::shared_ptr<acgm::Model>>& models, const std::shared_ptr<acgm::Light>& light, const std::shared_ptr<acgm::Camera>& camera, glm::vec3 enviro_up, glm::vec3 enviro_seam, std::string enviro_image_file)
{

    if (typ_funkcie == true) // ide o reflekciu
    {
        // if more hits than 10, return color of initial model
        if (number_of_reflection_hits > 10)
        {
            return models.at(model_index)->GetShader()->IdentifyColor(intersected_point).color;
        }

        std::shared_ptr <acgm::Ray> reflection_ray;
        std::optional<acgm::HitResult> ray_hitresult, min_hitresult;
        int index;
        min_hitresult->distance = INFINITY;
        index = -1;

        // calculate angle and new ray
        glm::vec3 R = reflect(ray_direction, intersected_point.normal);
        reflection_ray = std::make_shared<acgm::Ray>(intersected_point.point, R, bias);

        //! Find nearest object intersect
        for (int i = 0; i < models.size(); i++)
        {
            ray_hitresult = models.at(i)->Intersect(reflection_ray);

            if (ray_hitresult->distance > 0 && ray_hitresult->distance < min_hitresult->distance)
            {
                min_hitresult->distance = ray_hitresult->distance;
                min_hitresult->normal = ray_hitresult->normal;
                min_hitresult->point = ray_hitresult->point;
                index = i;
            }
        }

        // if hit
        if (index != -1)
        {
            acgm::ShaderStruct intersected_point_past = intersected_point;

            float old_glossiness = models.at(model_index)->GetShader()->IdentifyColor(intersected_point_past).glossiness;
            float old_transparency = models.at(model_index)->GetShader()->IdentifyColor(intersected_point_past).transparency;
            cogs::Color3f old_color = models.at(model_index)->GetShader()->IdentifyColor(intersected_point_past).color;

            if (old_glossiness == 0)
            {
                return old_color;
            }

            intersected_point.direction_to_light = glm::normalize(light->GetDirectionToLight(min_hitresult->point));
            intersected_point.normal = glm::normalize(min_hitresult->normal);
            intersected_point.point = min_hitresult->point;
            intersected_point.direction_to_eye = glm::normalize(camera->GetPosition() - min_hitresult->point);
            intersected_point.light_intensity = light->GetIntensityAt(min_hitresult->point);

            return (1 - old_glossiness - old_transparency) * old_color 
                    + (old_glossiness * Reflection_new(true, number_of_reflection_hits + 1, number_of_transparency_hits, model_index, index, intersected_point, R, models, light, camera, enviro_up, enviro_seam, enviro_image_file)
                    + (old_transparency * Reflection_new(false, number_of_reflection_hits, number_of_transparency_hits + 1, model_index, index, intersected_point, R, models, light, camera, enviro_up, enviro_seam, enviro_image_file)));
        }
        else
            if (number_of_reflection_hits == 0)
            {
                acgm::ShaderStruct intersected_point_past = intersected_point;
                float old_glossiness = models.at(model_index)->GetShader()->IdentifyColor(intersected_point_past).glossiness;
                cogs::Color3f old_color = models.at(model_index)->GetShader()->IdentifyColor(intersected_point_past).color;

                return (1 - old_glossiness) * old_color;
            }
    }
    else // transparency
    {
        // if more hits than 10, return color of initial model
        if (number_of_transparency_hits > 5)
        {
            return models.at(model_index)->GetShader()->IdentifyColor(intersected_point).color;
        }

        std::shared_ptr <acgm::Ray> transparency_ray;
        std::optional<acgm::HitResult> ray_hitresult, min_hitresult;
        int index;
        min_hitresult->distance = INFINITY;
        index = -1;

        float n1 = 1;
        float n2 = models.at(model_index)->GetShader()->IdentifyColor(intersected_point).refractive_index;
        glm::vec3 T = refract(ray_direction, intersected_point.normal, n1, n2);
        transparency_ray = std::make_shared<acgm::Ray>(intersected_point.point, T, bias);

        //! Find nearest object intersect
        for (int i = 0; i < models.size(); i++)
        {
            ray_hitresult = models.at(i)->Intersect(transparency_ray);

            if (ray_hitresult->distance > 0 && ray_hitresult->distance < min_hitresult->distance)
            {
                min_hitresult->distance = ray_hitresult->distance;
                min_hitresult->normal = ray_hitresult->normal;
                min_hitresult->point = ray_hitresult->point;
                index = i;
            }
        }

        if (index != -1)
        {
            acgm::ShaderStruct intersected_point_past = intersected_point;

            intersected_point.direction_to_light = glm::normalize(light->GetDirectionToLight(min_hitresult->point));
            intersected_point.normal = glm::normalize(min_hitresult->normal);
            intersected_point.point = min_hitresult->point;
            intersected_point.direction_to_eye = glm::normalize(camera->GetPosition() - min_hitresult->point);
            intersected_point.light_intensity = light->GetIntensityAt(min_hitresult->point);

            float old_glossiness = models.at(model_index)->GetShader()->IdentifyColor(intersected_point_past).glossiness;
            float old_transparency = models.at(model_index)->GetShader()->IdentifyColor(intersected_point_past).transparency;
            cogs::Color3f old_color = models.at(model_index)->GetShader()->IdentifyColor(intersected_point_past).color;

            return (1 - old_glossiness - old_transparency) * old_color
                    + (old_glossiness * Reflection_new(true, number_of_reflection_hits + 1, number_of_transparency_hits, model_index, index, intersected_point, T, models, light, camera, enviro_up, enviro_seam, enviro_image_file)
                    + (old_transparency * Reflection_new(false, number_of_reflection_hits, number_of_transparency_hits + 1, model_index, index, intersected_point, T, models, light, camera, enviro_up, enviro_seam, enviro_image_file)));
        }
        else
        {
            /*if (enviro_image_file.compare("") != 0)
            {
                glm::vec3 V, U, X, S;
                float old_range, new_range, longitude, latitude, lon, lat;

                V = ray_direction;
                U = enviro_up;
                S = enviro_seam;
                X = V - U * glm::dot(U, V);

                longitude = glm::orientedAngle(X, S, U); // <-PI, PI>
                latitude = std::acos(glm::dot(V, U)); // <0, PI>

                old_range = (float(M_PI) + float(M_PI));
                new_range = (1 - 0);
                lon = (((longitude - (-float(M_PI))) * new_range) / old_range) + 0;
                old_range = (float(M_PI) - 0);
                lat = (((latitude - 0) * new_range) / old_range) + 0;

                std::shared_ptr<acgm::Image> image = std::make_shared<acgm::Image>(enviro_image_file);

                return image->GetColorAt(glm::vec2(lon, lat));
            }
            else 
            {*/
                return cogs::Color3f(0, 0, 0);
            //}
        }
    }
   
}

//! Functions
void acgm::Scene::Raytrace(hiro::draw::RasterRenderer &renderer) const
{
    int32_t row, column;
    float y, x, dx, dy, base_x, base_y;

    std::shared_ptr<acgm::Image> image = std::make_shared<acgm::Image>(enviro_image_file_);

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
            std::shared_ptr <acgm::Ray> ray, reflection_ray;
            std::optional<HitResult> ray_hitresult, min_hitresult;
            glm::vec3 ray_direction, ray_position;
            cogs::Color3f final_color;
            ShaderStruct intersected_point;
            float min, reflection_hit_count;
            int i, index, ref_index;
            boolean reflection_hit;

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
            
            //! If no hit, check image or background color
            if (index == -1) 
            {
                if (enviro_image_file_.compare("") != 0) 
                {
                    glm::vec3 V, U, X, S;
                    float old_range, new_range, longitude, latitude, lon, lat;

                    V = ray_direction;
                    U = enviro_up_;
                    S = enviro_seam_;
                    X = V - U * glm::dot(U, V);

                    longitude = glm::orientedAngle(X, S, U); // <-PI, PI>
                    latitude = std::acos(glm::dot(V, U)); // <0, PI>

                    old_range = (float(M_PI) + float(M_PI));
                    new_range = (1 - 0);
                    lon = (((longitude - (-float(M_PI))) * new_range) / old_range) + 0;
                    old_range = (float(M_PI) - 0);
                    lat = (((latitude - 0) * new_range) / old_range) + 0;

                    renderer.SetPixel(column, renderer.GetResolution().y - row - 1, image->GetColorAt(glm::vec2(lon, lat)));
                }

                //x += dx;
                x = ((column + 1) * dx) + base_x;
                continue;
            }

            // docasne ak trafim --------------------------------------------------

            // vypocitam bod, kde sa pretli
            intersected_point.direction_to_light = glm::normalize(light_->GetDirectionToLight(min_hitresult->point));
            intersected_point.normal = glm::normalize(min_hitresult->normal);
            intersected_point.point = min_hitresult->point;
            intersected_point.direction_to_eye = glm::normalize(camera_->GetPosition() - min_hitresult->point);
            intersected_point.light_intensity = light_->GetIntensityAt(min_hitresult->point);

            // zavolam reflection
            final_color = Reflection_new(true, 1, 0, 1, index, intersected_point, ray_direction, models_, light_, camera_, enviro_up_, enviro_seam_, enviro_image_file_) 
                          + Reflection_new(false, 0, 1, 1, index, intersected_point, ray_direction, models_, light_, camera_, enviro_up_, enviro_seam_, enviro_image_file_);

            renderer.SetPixel(column, renderer.GetResolution().y - row - 1, final_color);
            //std::cout << "final color: '" << final_color.r << "'" << final_color.g << "'" << final_color.b << "'" << std::endl;

            // docasne ak trafim --------------------------------------------------

            //! If hit, check reflections and shadows
            /*if (index != -1)
            {
                reflection_hit = true;
                reflection_hit_count = 0;
                ref_index = -1;
                min_hitresult->distance = INFINITY;

                while (reflection_hit == true || reflection_hit_count != index_of_refraction_)
                {
                    //! Calculate attributes of intersection point
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

                    final_color += models_.at(index)->GetShader()->IdentifyColor(intersected_point);

                    glm::vec3 R = reflect(ray_direction, intersected_point.normal);
                    reflection_ray = std::make_shared<acgm::Ray>(intersected_point.point, R, bias);

                    //! Find nearest object intersect
                    for (i = 0; i < models_.size(); i++)
                    {
                        ray_hitresult = models_.at(i)->Intersect(reflection_ray);

                        if (ray_hitresult->distance > 0 && ray_hitresult->distance < min_hitresult->distance)
                        {
                            if (ray_hitresult->distance < camera_->GetZFar() && ray_hitresult->distance > camera_->GetZNear())
                            {
                                min_hitresult->distance = ray_hitresult->distance;
                                min_hitresult->normal = ray_hitresult->normal;
                                min_hitresult->point = ray_hitresult->point;
                                ref_index = i;
                            }
                        }
                    }

                    if (ref_index == -1)
                    {
                        reflection_hit = false;
                    }
                    ray_direction = R;
                }
            }*/

            //! If hit, calculate attributes of intersection point
            /*intersected_point.direction_to_light = glm::normalize(light_->GetDirectionToLight(min_hitresult->point));
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
            renderer.SetPixel(column, renderer.GetResolution().y - row - 1, models_.at(index)->GetShader()->IdentifyColor(intersected_point).color);*/

            //x += dx;
            x = ((column + 1) * dx) + base_x;
        }
        //y -= dy;
    }
}
