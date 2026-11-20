#pragma once

#include "core/DocumentNumber.hpp"
#include "core/enums.hpp"
#include "production/Product.hpp"

#include <string>

class QualityCheck
{
    int id_;
    int product_id_;
    int sample_size_;
    int defects_found_;
    QualityResult result_;
    std::string inspector_;

public:
    QualityCheck(int id,
                 int product_id,
                 int sample_size,
                 std::string inspector,
                 QualityResult result = QualityResult::Passed);

    double defect_rate() const;
    bool passes(double threshold) const;
    void record_defect(int count);
    std::string summary() const;
    bool needs_rework() const;
};

class DefectReport
{
    int id_;
    int product_id_;
    std::string severity_;
    std::string description_;
    bool resolved_;

public:
    DefectReport(int id,
                 int product_id,
                 std::string severity,
                 std::string description);

    void resolve();
    bool is_critical() const;
    std::string ticket_code(DocumentNumber& counter) const;
    bool matches_product(const Product& product) const;
    bool is_resolved() const;
};
