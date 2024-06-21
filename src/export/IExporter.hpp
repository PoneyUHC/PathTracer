
#pragma once

#include "vec.hpp"

#include <memory>


class IExporter {

public:

    virtual ~IExporter() {};
    virtual int Export(int width, int height, std::shared_ptr<RGBColor[]> buffer) const = 0;

};
