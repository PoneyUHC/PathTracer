
#pragma once

#include "math/vec.hpp"

#include <memory>


class IRenderer {

private:

    std::shared_ptr<RGBColor[]> m_buffer;


public:

    virtual ~IRenderer() {};
    virtual void Init() = 0;
    virtual void Render(int n_steps) = 0;
    virtual std::shared_ptr<RGBColor[]> GetBuffer() = 0;

};