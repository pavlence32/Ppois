#include "transport_request.hpp"
TransportRequest::TransportRequest(int guests, Money price)
    : guests_(guests), price_(price), confirmed_(false) {}
void TransportRequest::confirm() { confirmed_ = true; }
bool TransportRequest::is_confirmed() const { return confirmed_; }
Money TransportRequest::price() const { return price_; }
