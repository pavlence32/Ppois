#include "complaint.hpp"

Complaint::Complaint(std::string text, int priority)
    : text_(text), resolved_(false), priority_(priority)
{}

void Complaint::resolve() { resolved_ = true; }
bool Complaint::is_open() const { return !resolved_; }
bool Complaint::is_urgent() const { return priority_ >= 8; }
