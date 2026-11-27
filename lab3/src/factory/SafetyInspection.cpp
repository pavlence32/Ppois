#include "factory/SafetyInspection.hpp"

#include "core/exceptions.hpp"

SafetyInspection::SafetyInspection(int id, std::string area, int score, std::string inspector)
    : id_(id),
      area_(std::move(area)),
      score_(score),
      inspector_(std::move(inspector)),
      passed_(score >= 70)
{
    if (score < 0 || score > 100)
        throw SafetyViolationException("Оценка безопасности должна быть в диапазоне 0..100");
}

bool SafetyInspection::is_critical() const
{
    return score_ < 50;
}

void SafetyInspection::recheck(int new_score)
{
    if (new_score < 0 || new_score > 100)
        throw SafetyViolationException("Некорректная повторная оценка");
    score_ = new_score;
    passed_ = score_ >= 70;
}

std::string SafetyInspection::grade() const
{
    if (score_ >= 90)
        return "A";
    if (score_ >= 75)
        return "B";
    if (score_ >= 50)
        return "C";
    return "D";
}

bool SafetyInspection::requires_follow_up() const
{
    return !passed_ || score_ < 75;
}

bool SafetyInspection::passed() const
{
    return passed_;
}
