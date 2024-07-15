
#include "camera.hpp"


Camera::Camera(const Point3& camera_center, CameraParams&& params) 
{
    m_aspect_ratio = params.aspect_ratio;
    m_image_width = params.image_width;
    m_vfov = params.vfov;
    m_lookfrom = params.lookfrom;
    m_lookat = params.lookat;
    m_vup = params.vup;

    m_camera_center = m_lookfrom;
    m_focal_length = (m_lookfrom - m_lookat).Magnitude();

    m_image_height = int(m_image_width / m_aspect_ratio);
    m_image_height = (m_image_height < 1) ? 1 : m_image_height;

    double theta = degrees_to_radians(m_vfov);
    double h = tan(theta/2.0);
    m_viewport_height = 2.0 * h * m_focal_length;
    m_viewport_width = m_viewport_height * (double(m_image_width) / m_image_height);

    m_w = (m_lookfrom - m_lookat).Normalized();
    m_u = m_vup.Cross(m_w).Normalized();
    m_v = m_w.Cross(m_u);

    m_viewport_u = m_viewport_width * m_u;
    m_viewport_v = m_viewport_height * (-m_v);

    m_pixel_delta_u = m_viewport_u / m_image_width;
    m_pixel_delta_v = m_viewport_v / m_image_height;

    m_viewport_upper_left = m_camera_center 
        - (m_focal_length * m_w) - m_viewport_u/2 - m_viewport_v/2;
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