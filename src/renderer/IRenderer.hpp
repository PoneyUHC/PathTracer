
#pragma once

#include "math/vec.hpp"

#include <memory>


class IRenderer {

private:

    std::shared_ptr<RGBColor[]> m_buffer;


public:

    virtual ~IRenderer() {};
    virtual void Render() = 0;
    virtual std::shared_ptr<RGBColor[]> GetBuffer() = 0;

};