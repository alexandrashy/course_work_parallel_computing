#pragma once
#include <string>

struct USER_DATA {
    std::string Path;
    std::string USER_NAME;
    USER_DATA(const std::string path, const std::string& name) : Path(Path), USER_NAME(name) {}
};