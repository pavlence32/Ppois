#include "production/QualityCheck.hpp"

#include "core/exceptions.hpp"

#include <sstream>

QualityCheck::QualityCheck(int id,
                           int product_id,
                           int sample_size,
                           std::string inspector,
                           QualityResult result)
    : id_(id),
      product_id_(product_id),
      sample_size_(sample_size),
      defects_found_(0),
      result_(result),
      inspector_(std::move(inspector))
{
    if (sample_size <= 0)
        throw FactoryException("Размер выборки должен быть положительным");
}

double QualityCheck::defect_rate() const
{
    return 100.0 * static_cast<double>(defects_found_) / sample_size_;
}

bool QualityCheck::passes(double threshold) const
{
    return defect_rate() <= threshold && result_ == QualityResult::Passed;
}

void QualityCheck::record_defect(int count)
{
    if (count <= 0)
        throw FactoryException("Количество дефектов должно быть положительным");
    defects_found_ += count;
    if (defects_found_ > sample_size_ * 0.1)
        result_ = QualityResult::Failed;
}

std::string QualityCheck::summary() const
{
    std::ostringstream stream;
    stream << "Проверка #" << id_ << " продукта " << product_id_ << ": "
           << defects_found_ << " дефектов из " << sample_size_ << " ("
           << defect_rate() << "%), инспектор " << inspector_;
    return stream.str();
}

bool QualityCheck::needs_rework() const
{
    return result_ == QualityResult::Rework || result_ == QualityResult::Failed;
}

DefectReport::DefectReport(int id,
                           int product_id,
                           std::string severity,
                           std::string description)
    : id_(id),
      product_id_(product_id),
      severity_(std::move(severity)),
      description_(std::move(description)),
      resolved_(false)
{
}

void DefectReport::resolve()
{
    if (description_.empty())
        throw FactoryException("Нельзя закрыть дефект без описания");
    resolved_ = true;
}

bool DefectReport::is_critical() const
{
    return severity_ == "critical" || severity_ == "high";
}

std::string DefectReport::ticket_code(DocumentNumber& counter) const
{
    return counter.next();
}

bool DefectReport::matches_product(const Product& product) const
{
    return product.id() == product_id_;
}

bool DefectReport::is_resolved() const
{
    return resolved_;
}
