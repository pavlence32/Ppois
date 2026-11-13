#pragma once

#include <string>

class Review
{
    int rating_;
    std::string text_;
    bool published_;

public:
    Review(int rating, std::string text);
    void publish();
    bool is_positive() const;
    bool is_published() const;
};
