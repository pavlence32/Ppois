#include "maintenance_task.hpp"

MaintenanceTask::MaintenanceTask(int room_number, std::string description)
    : room_number_(room_number), description_(description), completed_(false)
{}

void MaintenanceTask::complete() { completed_ = true; }
bool MaintenanceTask::is_completed() const { return completed_; }
bool MaintenanceTask::concerns_room(int number) const { return room_number_ == number; }
