#pragma once

#include <string>

class Tool
{
    int id_;
    std::string name_;
    std::string condition_;
    int usage_hours_;
    int max_hours_;

public:
    Tool(int id, std::string name, std::string condition, int max_hours);

    void use(int hours);
    void sharpen();
    bool needs_replacement() const;
    double wear_percent() const;
    bool is_available() const;
};
