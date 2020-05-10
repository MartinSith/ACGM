#pragma once
#include <glm/glm.hpp>
#include <COGS/Color.h>

namespace acgm
{
    //! Model  - abstract base class for scene models
    class Image
    {
    public:
        //! Constructors
        explicit Image(const std::string file_path);
        virtual ~Image() = default;

        //! Functions
        // function for image free
        void FreeImageData();

        // function returning color at point in parameter
        cogs::Color3f GetColorAt(const glm::vec2& uvs) const;

    private:
        uint8_t* image_;
        int width_;
        int height_;
        int bpp_;
    };
}