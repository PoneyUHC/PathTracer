
#include "camera.hpp"


Camera::Camera(const Point3& camera_center, CameraParams&& params) 
{
    m_aspect_ratio = params.aspect_ratio;
    m_image_width = params.image_width;
    m_focal_length = params.focal_length;
    m_vfov = params.vfov;

    m_image_height = int(m_image_width / m_aspect_ratio);
    m_image_height = (m_image_height < 1) ? 1 : m_image_height;

    double theta = degrees_to_radians(m_vfov);
    double h = tan(theta/2.0);
    m_viewport_height = 2.0 * h * m_focal_length;
    m_viewport_width = m_viewport_height * (double(m_image_width) / m_image_height);

    m_viewport_u = Vec3(m_viewport_width, 0, 0);
    m_viewport_v = Vec3(0, -m_viewport_height, 0);

    m_pixel_delta_u = m_viewport_u / m_image_width;
    m_pixel_delta_v = m_viewport_v / m_image_height;

    m_viewport_upper_left = camera_center 
        - Vec3(0, 0, m_focal_length) - m_viewport_u/2 - m_viewport_v/2;
    m_pixel00_loc = m_viewport_upper_left + (m_pixel_delta_u + m_pixel_delta_v) * 0.5;
}


Vec3 Camera::GetPixelPosition(int i, int j) const
{
    return m_pixel00_loc + m_pixel_delta_u * i + m_pixel_delta_v * j;
}


Vec3 Camera::SamplePositionAroundPixel(int i, int j) const
{
    Vec3 offset = sample_in_unit_square();
    
    Vec3 samplePosition = GetPixelPosition(i, j)
        + offset.X() * m_pixel_delta_u
        + offset.Y() * m_pixel_delta_v;

    return samplePosition;
}