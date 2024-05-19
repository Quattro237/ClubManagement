#include "Time.h"

std::string Time::UINT16TToTime(uint16_t time) {
    uint16_t hours = time / minutes_;
    uint16_t minutes = time % minutes_;

    std::string hours_str = std::to_string(hours);
    if (hours < 10) {
        hours_str = '0' + hours_str;
    }

    std::string minutes_str = std::to_string(minutes);
    if (minutes < 10) {
        minutes_str = '0' + minutes_str;
    }

    return hours_str + ':' + minutes_str;
}

uint16_t Time::TimeToUINT16T(const std::string &time) {
    std::string hours_str;
    hours_str += time[0];
    hours_str += time[1];

    std::string minutes_str;
    minutes_str += time[3];
    minutes_str += time[4];

    uint16_t hours = std::stoul(hours_str);
    uint16_t minutes = std::stoul(minutes_str);

    return hours * minutes_ + minutes;
}