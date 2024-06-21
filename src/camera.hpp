
#pragma once

#include "vec.hpp"
#include "ray.hpp"

#include <optional>


class Camera {

private:

    Vec3 m_camera_center;

    double m_aspect_ratio;
    int m_image_width;
    int m_image_height;

    double m_focal_length;

    double m_viewport_width;
    double m_viewport_height;
    
    Vec3 m_viewport_u;
    Vec3 m_viewport_v;
    Point3 m_viewport_upper_left;

    Vec3 m_pixel_delta_u;
    Vec3 m_pixel_delta_v;
    Point3 m_pixel00_loc;


public:
    
    Camera(const Point3& camera_center, double aspect_ratio, int width, double focal_length);

    inline double ImageHeight() const { return m_image_height; }
    inline double ImageWidth() const { return m_image_width; }
    inline Point3 CameraCenter() const { return m_camera_center; }

    std::optional<Point3> GetPixelPosition(int i, int j);

};