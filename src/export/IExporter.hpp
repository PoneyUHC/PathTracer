
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
    
};
