#include "camera.hpp"

Camera::Camera(const Point3& camera_center, double aspect_ratio, int width, double focal_length) 
    : m_camera_center{camera_center}, m_aspect_ratio{aspect_ratio}, 
        m_image_width{width}, m_focal_length{focal_length} 
{
    m_image_height = int(m_image_width / m_aspect_ratio);
    m_image_height = (m_image_height < 1) ? 1 : m_image_height;
    m_focal_length = 1.0;

    m_viewport_height = 2.0;
    m_viewport_width = m_viewport_height * (double(m_image_width) / m_image_height);

    m_viewport_u = Vec3(m_viewport_width, 0, 0);
    m_viewport_v = Vec3(0, -m_viewport_height, 0);

    m_pixel_delta_u = m_viewport_u / m_image_width;
    m_pixel_delta_v = m_viewport_v / m_image_height;

    m_viewport_upper_left = camera_center 
        - Vec3(0,0,focal_length) - m_viewport_u/2 - m_viewport_v/2;
    m_pixel00_loc = m_viewport_upper_left + (m_pixel_delta_u + m_pixel_delta_v) * 0.5;
}


std::optional<Vec3> Camera::GetPixelPosition(int i, int j) 
{
    if(i < 0 || i >= m_image_width || j < 0 || j >= m_image_width) {
        return std::nullopt;
    }

    return m_pixel00_loc + m_pixel_delta_u * i + m_pixel_delta_v * j;
}