
#include "image_texture.hpp"

#include <algorithm>


RGBColor ImageTexture::GetColor(double u, double v, const Point3 &point) const
{
    u = std::clamp(0.0, 1.0, u);
    v = 1.0 - std::clamp(0.0, 1.0, v);

    int i = int(u * m_image->Width());
    int j = int(v * m_image->Height());
    float* pixel_data = m_image->GetPixelData(i, j);

    return RGBColor(pixel_data[0], pixel_data[1], pixel_data[2]);
}