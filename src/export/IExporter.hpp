
#pragma once

#include "math/vec.hpp"
#include "math/interval.hpp"

#include <memory>


class IExporter {

public:

    virtual ~IExporter() {};
    virtual int Export(int width, int height, std::shared_ptr<RGBColor[]> buffer) = 0;

    // Image viewers expect gamma corrected images most of the time
    // We approximate the gamme correction with a gamma 2 transform.
    // Going from linear space to gamma space, it means 1 / gamma, ie sqrt()
    static double linear_to_gamma(double linearComponent){
        if(linearComponent > 0){
            return sqrt(linearComponent);
        }

        return 0;
    }


    static void rgb_normalized_to_8bits(const RGBColor& color, uint8_t *dest){
        double r = color[0];
        double g = color[1];
        double b = color[2];

        r = linear_to_gamma(r);
        g = linear_to_gamma(g);
        b = linear_to_gamma(b);
        
        static const Interval intensity(0.000, 0.999);
        dest[0] = uint8_t(256 * intensity.Clamp(r));
        dest[1] = uint8_t(256 * intensity.Clamp(g));
        dest[2] = uint8_t(256 * intensity.Clamp(b));
    }
    
};
