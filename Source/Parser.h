#pragma once

#include "Club.h"
#include <fstream>

class Parser {
public:
    explicit Parser(const char *path);

    ~Parser();

    // Indicates whether initialization of Parser done successfully, and it is ready for parsing.
    bool Ready() const;

    // Start reading events. Returns status of run: false if an error occurred, true in other case.
    bool Run();

private:
    void
    EventHandler(uint16_t event_time, uint16_t event_id, const std::string &client_name, size_t table_num);


    std::fstream file;
    Club *club = nullptr;
};
