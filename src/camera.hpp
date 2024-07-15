
#pragma once

#include "vec.hpp"


struct CameraParams {
    double aspect_ratio;
    int image_width;
    double vfov;
    Point3 lookfrom;
    Point3 lookat;
    Vec3 vup;
};


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

    double m_vfov;

    Point3 m_lookfrom;
    Point3 m_lookat;
    Vec3 m_vup;

    Vec3 m_u, m_v ,m_w;


public:
    
    Camera(const Point3& camera_center, CameraParams&& params);

    double ImageHeight() const { return m_image_height; }
    double ImageWidth() const { return m_image_width; }
    Point3 CameraCenter() const { return m_camera_center; }

    Point3 GetPixelPosition(int i, int j) const;

    Vec3 SamplePositionAroundPixel(int i, int j) const;

};