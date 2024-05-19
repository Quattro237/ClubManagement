#include "Parser.h"
#include "Time.h"
#include <iostream>
#include <sstream>

Parser::Parser(const char *path) : file(path) {
    if (!file.is_open()) {
        std::cerr << "Cannot open file" << '\n';
        return;
    }

    std::string buffer;

    file >> buffer;
    size_t tables_cnt = 0;
    try {
        tables_cnt = std::stoull(buffer);
    } catch (...) {
        std::cerr << "Number of tables should be a positive number" << '\n';
        std::cout << buffer;
        return;
    }

    file >> buffer;
    uint16_t opening_time = 0;
    try {
        opening_time = Time::TimeToUINT16T(buffer);
    } catch (...) {
        std::cerr << "Opening time should be in HH:MM format" << '\n';
        std::cout << buffer;
        return;
    }

    file >> buffer;
    uint16_t closing_time = 0;
    try {
        closing_time = Time::TimeToUINT16T(buffer);
    } catch (...) {
        std::cerr << "Opening time should be in HH:MM format" << '\n';
        std::cout << buffer;
        return;
    }

    file >> buffer;
    size_t hour_cost = 0;
    try {
        hour_cost = std::stoull(buffer);
    } catch (...) {
        std::cerr << "Hour cost should be a positive number" << '\n';
        std::cout << buffer;
        return;
    }

    while (std::isspace(file.peek())) {
        file.get();
    }

    club = new Club(opening_time, closing_time, tables_cnt, hour_cost);
}

Parser::~Parser() {
    file.close();
    if (club) {
        delete club;
    }
}

bool Parser::Ready() const {
    return club;
}

void
Parser::EventHandler(uint16_t event_time, uint16_t event_id, const std::string &client_name, size_t table_num) {
    if (event_id == 1) {
        club->AddNewClient(event_time, client_name);
    } else if (event_id == 2) {
        club->TakeTable(event_time, client_name, table_num);
    } else if (event_id == 3) {
        club->AddClientToQueue(event_time, client_name);
    } else if (event_id == 4) {
        club->DeleteClient(event_time, client_name);
    }
}

bool Parser::Run() {
    std::string line;
    std::string buffer;

    std::cout << Time::UINT16TToTime(club->GetOpeningTime()) << '\n';

    while (!file.eof()) {
        std::getline(file, line);
        if (std::isspace(line.back())) {
            line.pop_back();
        }
        std::istringstream is(line);

        std::cout << line << '\n';

        is >> buffer;
        uint16_t event_time = 0;
        try {
            event_time = Time::TimeToUINT16T(buffer);
        } catch (...) {
            std::cerr << "Event time should be in HH:MM format" << '\n';
            return false;
        }

        is >> buffer;
        uint16_t event_id = 0;
        try {
            event_id = std::stoul(buffer);
        } catch (...) {
            std::cerr << "Event ID should be a positive number" << '\n';
            return false;
        }

        is >> buffer;
        std::string client_name = buffer;

        size_t table_num = 0;
        if (event_id == 2) {
            is >> buffer;
            try {
                table_num = std::stoull(buffer);
            } catch (...) {
                std::cerr << "Table number should be a positive number" << '\n';
                return false;
            }
        }

        EventHandler(event_time, event_id, client_name, table_num);
    }

    club->EndDay();

    std::cout << Time::UINT16TToTime(club->GetClosingTime()) << '\n';

    club->PrintStatistic();

    return true;
}
