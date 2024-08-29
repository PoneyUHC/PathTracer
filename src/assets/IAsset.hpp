
#pragma once

#include <filesystem>
#include <string>


class IAsset {

public:

    virtual ~IAsset() {};
    virtual std::string GetPath() const = 0;
    virtual int Load() = 0;

};