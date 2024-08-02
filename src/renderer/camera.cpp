
#include "renderer/camera.hpp"

#include "math/ray.hpp"


Camera::Camera(CameraParams&& params) 
{
    m_aspect_ratio = params.aspect_ratio;
    m_image_width = params.image_width;
    m_vfov = params.vfov;
    m_lookfrom = params.lookfrom;
    m_lookat = params.lookat;
    m_vup = params.vup;
    m_defocus_angle = params.defocus_angle;
    m_focus_dist = params.focus_dist;

    m_camera_center = m_lookfrom;

    m_image_height = int(m_image_width / m_aspect_ratio);
    m_image_height = (m_image_height < 1) ? 1 : m_image_height;

    double theta = degrees_to_radians(m_vfov);
    double h = tan(theta/2.0);
    m_viewport_height = 2.0 * h * m_focus_dist;
    m_viewport_width = m_viewport_height * (double(m_image_width) / m_image_height);

    m_w = (m_lookfrom - m_lookat).Normalized();
    m_u = m_vup.Cross(m_w).Normalized();
    m_v = m_w.Cross(m_u);

    m_viewport_u = m_viewport_width * m_u;
    m_viewport_v = m_viewport_height * (-m_v);

    m_pixel_delta_u = m_viewport_u / m_image_width;
    m_pixel_delta_v = m_viewport_v / m_image_height;

    m_viewport_upper_left = m_camera_center 
        - (m_focus_dist * m_w) - m_viewport_u/2 - m_viewport_v/2;
    m_pixel00_loc = m_viewport_upper_left + (m_pixel_delta_u + m_pixel_delta_v) * 0.5;

    double defocus_radius = m_focus_dist * tan(degrees_to_radians(m_defocus_angle / 2));
    m_defocus_disk_u = m_u * defocus_radius;
    m_defocus_disk_v = m_v * defocus_radius;
}


Vec3 Camera::GetPixelPosition(int i, int j) const
{
    return m_pixel00_loc + m_pixel_delta_u * i + m_pixel_delta_v * j;
}


Ray Camera::SampleRayForPixel(int i, int j) const
{
    Vec3 offset = sample_in_unit_square();
    
    Vec3 sample_position = GetPixelPosition(i, j)
        + offset.X() * m_pixel_delta_u
        + offset.Y() * m_pixel_delta_v;

    Point3 ray_origin = (m_defocus_angle <= 0) ? m_camera_center : DefocusDiskSample();
    return Ray(ray_origin, sample_position - ray_origin);;
    
}


Point3 Camera::DefocusDiskSample() const
{
    Point3 p = Vec3::RandomUnitVector();
    return m_camera_center + (p.X() * m_defocus_disk_u) + (p.Y() * m_defocus_disk_v);
}
