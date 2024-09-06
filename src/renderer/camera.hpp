
#pragma once

#include "math/vec.hpp"


class Ray;


struct CameraParams {
    double aspect_ratio;
    int image_width;
    double vfov;
    Point3 lookfrom;
    Point3 lookat;
    Vec3 vup;
    double defocus_angle;
    double focus_dist;
    RGBColor background_color;
};


class Camera {

private:

    CameraParams m_params;

    Vec3 m_camera_center;

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

    Vec3 m_u, m_v ,m_w;

    // Defocus disk horizontal radius
    Vec3 m_defocus_disk_u;       
    Vec3 m_defocus_disk_v;


public:
    
    Camera(CameraParams&& params);

    double ImageHeight() const { return m_image_height; }
    double ImageWidth() const { return m_params.image_width; }
    Point3 CameraCenter() const { return m_camera_center; }
    CameraParams GetParams() const { return m_params; }

    Point3 GetPixelPosition(int i, int j) const;

    Ray SampleRayForPixel(int i, int j) const;

    Point3 DefocusDiskSample() const;

};