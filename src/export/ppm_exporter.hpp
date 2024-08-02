
#pragma once

#include "export/IExporter.hpp"
#include "math/vec.hpp"

#include <string>
#include <memory>


class PpmExporter : public IExporter {

private:

    std::string m_filepath;


public:

    PpmExporter(const std::string& filepath) : m_filepath{filepath} {};
    int Export(int width, int height, std::shared_ptr<RGBColor[]> buffer) const override;

};