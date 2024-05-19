#include "Table.h"

void Table::SetNumber(size_t number) {
    number_ = number;
}

size_t Table::GetNumber() const {
    return number_;
}

bool Table::TakeTable(size_t client_id, uint16_t take_time) {
    if (is_busy_) {
        return false;
    }

    is_busy_ = true;
    start_time_ = take_time;
    client_id_ = client_id;

    return true;
}

bool Table::FreeTable(size_t client_id, uint16_t free_time) {
    if (client_id != client_id_) {
        return false;
    }

    uint16_t busy_time = free_time - start_time_;
    busy_time_ += busy_time;
    busy_hours_ += busy_time / 60 + (busy_time % 60 == 0 ? 0 : 1);
    client_id_ = 0;
    is_busy_ = false;
    start_time_ = 0;

    return true;
}

uint16_t Table::GetWorkingHours() const {
    return busy_hours_;
}

uint16_t Table::GetWorkingTime() const {
    return busy_time_;
}