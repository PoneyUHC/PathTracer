
#pragma once

#include "texture/ITexture.hpp"
#include "assets/image_asset.hpp"
#include <memory>


class ImageTexture : public ITexture {

private:

    std::shared_ptr<ImageAsset> m_image;


public:

    ImageTexture(std::shared_ptr<ImageAsset> image): m_image{image} {};

    RGBColor GetColor(double u, double v, const Point3& point) const override;

};