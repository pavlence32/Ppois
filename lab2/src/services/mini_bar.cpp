#include "services/mini_bar.hpp"
MiniBar::MiniBar(int products, Money total_price)
    : products_(products), total_price_(total_price), sealed_(false) {}
void MiniBar::seal() { sealed_ = true; }
bool MiniBar::is_sealed() const { return sealed_; }
Money MiniBar::price() const { return total_price_; }
