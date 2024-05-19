#include <string>

class Time {
public:
    // Transfers time in minutes from 00:00 to HH:MM time format
    static std::string UINT16TToTime(uint16_t time);

    // Transfers time in HH:MM format to time in minutes from 00:00
    static uint16_t TimeToUINT16T(const std::string &time);

private:
    const static uint16_t minutes_ = 60;
};
