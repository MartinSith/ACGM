#include <ACGM_RayTracer_lib/Scene.h>
#include <ACGM_RayTracer_lib/Image.h>
#include <omp.h>
#include <glm/gtx/vector_angle.hpp>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

float bias;
float epsilon = 0.00001f;

//! Constructors
acgm::Scene::Scene(const std::shared_ptr<acgm::Camera>& camera, const std::shared_ptr<acgm::Light>& light, 
                   const std::vector<std::shared_ptr<acgm::Model>>& models, glm::vec3 enviro_up, 
                   glm::vec3 enviro_seam, std::string enviro_image_file, float input_bias, float index_of_refraction, 
                   int reflection_number, int transparency_number) :
    models_(models), camera_(camera), light_(light), enviro_up_(enviro_up), enviro_seam_(enviro_seam), 
    enviro_image_file_(enviro_image_file), bias_(input_bias), index_of_refraction_(index_of_refraction),
    reflection_number_(reflection_number), transparency_number_(transparency_number)
{
    bias = bias_;
}

cogs::Color3f acgm::Scene::CalculateImageColor(glm::vec3 ray_direction, std::shared_ptr<acgm::Image> image) const
{
    glm::vec3 V, U, X, S;
    float old_range, new_range, longitude, latitude, lon, lat;

    //! prepare variables
    V = ray_direction;
    U = enviro_up_;
    S = enviro_seam_;
    X = V - U * glm::dot(U, V);

    //! calculate longitude and latitude
    longitude = glm::orientedAngle(glm::normalize(X), glm::normalize(S), glm::normalize(U)); // <-PI, PI>
    latitude = std::acos(glm::dot(glm::normalize(V), glm::normalize(U)));                    // <  0, PI>

    //! change values to range <0, 1>
    old_range = (float(M_PI) + float(M_PI));
    new_range = (1 - 0);
    lon = (((longitude + float(M_PI)) * new_range) / old_range) + 0;
    old_range = (float(M_PI) - 0);
    lat = (((latitude - 0) * new_range) / old_range) + 0;

    return image->GetColorAt(glm::vec2(lon, lat));
}

acgm::ShaderStruct acgm::Scene::CalculateIntersectPoint(std::optional<acgm::HitResult> hitresult) const
{
    acgm::ShaderStruct intersected_point;
    std::optional<acgm::HitResult> ray_hitresult;
    float min = INFINITY;

    //! calculate intersected point info
    intersected_point.direction_to_light = glm::normalize(light_->GetDirectionToLight(hitresult->point));
    intersected_point.normal = glm::normalize(hitresult->normal);
    intersected_point.point = hitresult->point;
    intersected_point.direction_to_eye = glm::normalize(camera_->GetPosition() - hitresult->point);
    intersected_point.light_intensity = light_->GetIntensityAt(hitresult->point);

    std::shared_ptr <acgm::Ray> ray = std::make_shared<acgm::Ray>(intersected_point.point + bias * intersected_point.direction_to_light, intersected_point.direction_to_light, bias);

    //! find nearest object intersect
    for (int i = 0; i < models_.size(); i++)
    {
        ray_hitresult = models_.at(i)->Intersect(ray);

        if (ray_hitresult->distance > 0 && ray_hitresult->distance < min)
        {
            min = ray_hitresult->distance;
        }
    }

    //! calculate if point is in shadow
    float distance_from_light = glm::distance(intersected_point.point, light_->GetPosition());
    if (min < distance_from_light)
    {
        intersected_point.is_in_shadow = true;
    }
    else intersected_point.is_in_shadow = false;

    return intersected_point;
}

glm::vec3 reflect(const glm::vec3& I, const glm::vec3& N)
{
    return I - 2 * glm::dot(I, N) * N;
}

std::optional<glm::vec3> refract(const glm::vec3& ray_direction, const glm::vec3& normal, const float& n1, const float& n2)
{
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
    if (cos2 < 0.0f)
    {
        return std::nullopt;
    }
    
    return (n * ray_direction + (n * cos1 - sqrtf(cos2)) * norm);
}

cogs::Color3f acgm::Scene::CalculateColorRecursion(int number_of_reflection_hits, int number_of_transparency_hits, int model_index, acgm::ShaderStruct intersected_point, glm::vec3 ray_direction, std::shared_ptr<acgm::Image> image) const
{
    acgm::ShaderReturn shader_info = models_.at(model_index)->GetShader()->IdentifyColor(intersected_point);
    std::optional<acgm::HitResult> ray_hitresult, min_hitresult;
    int index;

    cogs::Color3f finalized_color = cogs::Color3f(0.0f, 0.0f, 0.0f);

    // ========== TRANSPARENCY ========== //
    //! if transparency is 0 or reached max number of hits, dont calculate transparency
    if (shader_info.transparency > 0.0f && number_of_transparency_hits <= transparency_number_)
    {
        std::shared_ptr <acgm::Ray> transparency_ray;
        min_hitresult->distance = INFINITY;
        index = -1;
        glm::vec3 T;
        int intern_reflection = 0;

        //! calculate new ray direction
        std::optional<glm::vec3> T_refact = refract(ray_direction, intersected_point.normal, index_of_refraction_, shader_info.refractive_index);

        //! check if there is complete internal reflection
        if (T_refact == std::nullopt)
        {
            intern_reflection = 1;
            T = reflect(ray_direction, intersected_point.normal);
        }
        else
            T = *T_refact;

        transparency_ray = std::make_shared<acgm::Ray>(intersected_point.point + bias * T, T, bias);

        //! find nearest object intersect
        for (int i = 0; i < models_.size(); i++)
        {
            ray_hitresult = models_.at(i)->Intersect(transparency_ray);

            if (ray_hitresult->distance > 0 && ray_hitresult->distance < min_hitresult->distance)
            {
                min_hitresult->distance = ray_hitresult->distance;
                min_hitresult->normal = ray_hitresult->normal;
                min_hitresult->point = ray_hitresult->point;
                index = i;
            }
        }

        //! if ray hit an object, calculate colors
        if (index != -1)
        {
            if (intern_reflection == 1 && number_of_reflection_hits <= reflection_number_)
            {
                intersected_point = CalculateIntersectPoint(min_hitresult);
                return (1 - shader_info.transparency) * shader_info.color
                        + (shader_info.transparency * CalculateColorRecursion(number_of_reflection_hits + 1, number_of_transparency_hits, index, intersected_point, T, image));
            }
            else if (intern_reflection == 0)
            {
                intersected_point = CalculateIntersectPoint(min_hitresult);
                finalized_color += (1 - shader_info.transparency) * shader_info.color
                                    + (shader_info.transparency * CalculateColorRecursion(number_of_reflection_hits, number_of_transparency_hits + 1, index, intersected_point, T, image));
            }
        }
        else //! if ray didnt hit anything, calculate background color
        {
            if (enviro_image_file_.compare("") != 0)
            {
                return CalculateImageColor(T, image);
            }
        }
    }

    // ========== REFLECTION ========== //
    //! if glossiness is 0 or reached max hits, dont calculate reflection
    if (shader_info.glossiness < 0.1f || number_of_reflection_hits >= reflection_number_)
    {
        if (transparency_number_ == 0)
            return shader_info.color;
        else
            return shader_info.color * (1 - shader_info.transparency) + shader_info.transparency * finalized_color;
    }

    min_hitresult->distance = INFINITY;
    index = -1;

    //! calculate new ray direction
    glm::vec3 R = reflect(ray_direction, intersected_point.normal);

    //! find nearest object intersect
    std::shared_ptr <acgm::Ray> reflection_ray = std::make_shared<acgm::Ray>(intersected_point.point + bias * R, R, bias);
    for (int i = 0; i < models_.size(); i++)
    {
        ray_hitresult = models_.at(i)->Intersect(reflection_ray);

        if (ray_hitresult->distance > 0 && ray_hitresult->distance < min_hitresult->distance)
        {
            min_hitresult->distance = ray_hitresult->distance;
            min_hitresult->normal = ray_hitresult->normal;
            min_hitresult->point = ray_hitresult->point;
            index = i;
        }
    }

    //! if ray hit object, calculate reflection color
    if (index != -1)
    {
        intersected_point = CalculateIntersectPoint(min_hitresult);
        return (1 - shader_info.glossiness - shader_info.transparency) * shader_info.color 
                + (shader_info.glossiness * CalculateColorRecursion(number_of_reflection_hits + 1, number_of_transparency_hits, index, intersected_point, R, image) + finalized_color);
    }
    else // if ray didnt hit object, calculate background color
    {
        if (number_of_reflection_hits == 0)
        {
            if (enviro_image_file_.compare("") != 0)
                return shader_info.glossiness * CalculateImageColor(R, image) + shader_info.color * (1 - shader_info.glossiness);
            else
                return shader_info.color * (1 - shader_info.transparency);
        }
        else
        {
            if (enviro_image_file_.compare("") != 0)
                return CalculateImageColor(R, image) + finalized_color;
            else
                return cogs::Color3f(0.0f, 0.0f, 0.0f);
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
                    renderer.SetPixel(column, renderer.GetResolution().y - row - 1, CalculateImageColor(ray_direction, image));

                x = ((column + 1) * dx) + base_x;
                continue;
            }

            //! calculate intersected point
            intersected_point = CalculateIntersectPoint(min_hitresult);

            //! calculate color from reflections and transparency
            final_color = CalculateColorRecursion(0, 0, index, intersected_point, ray_direction, image);

            renderer.SetPixel(column, renderer.GetResolution().y - row - 1, final_color);

            x = ((column + 1) * dx) + base_x;
        }
    }

    if (enviro_image_file_.compare("") != 0)
        image->FreeImageData();
}
