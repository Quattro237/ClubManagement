#pragma once

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <string>
#include "Table.h"

class Club {
public:
    Club() = default;

    // Creates Club object
    Club(uint16_t open_time, uint16_t closing_time, size_t tables_cnt, size_t hour_cost);

    // Adds new client to the club. If client is already in the club YouShallNotPass error is generating.
    // If client came at non-working time NotOpenYet error is generating.
    void AddNewClient(uint16_t coming_time, const std::string& client_name);

    // Attempt to take the table from client with client_id on take_time. If client is not in the club
    // ClientUnknown error is generating, if table table_num is already busy PlaceIsBusy error is generating.
    void TakeTable(uint16_t taking_time, const std::string& client_name, size_t table_num);

    // Adds client with client_name to queue. If queue already full client leaves. If client is not
    // in the club ClientUnknown error is generating. If there are available tables ICanWaitNoLonger error
    // is generating.
    void AddClientToQueue(uint16_t start_time, const std::string& client_name);

    // Deletes client from the club. If client is not in the club ClientUnknown error is generating.
    // If reseat set to true then clients from queue will take freed tables.
    void DeleteClient(uint16_t leaving_time, const std::string& client_name, bool reseat = true);

    // Ends the day and deletes clients that still left in the club.
    void EndDay();

    // Prints statistic for each table.
    void PrintStatistic() const;

    // Returns opening and closing time.
    uint16_t GetOpeningTime() const;
    uint16_t GetClosingTime() const;

private:
    void ClientLeave(uint16_t leaving_time, size_t client_id);
    void TakeFreedTable(uint16_t taking_time, size_t client_id, size_t table_num);

    void HandleYouShallNotPassError(uint16_t error_time);
    void HandleNotOpenYetError(uint16_t error_time);
    void HandleClientUnknownError(uint16_t error_time);
    void HandlePlaceIsBusyError(uint16_t error_time);
    void HandleICanWaitNoLongerError(uint16_t error_time);

    uint16_t opening_time_ = 0;
    uint16_t closing_time_ = 0;
    size_t hour_cost_ = 0;

    std::vector<Table> tables_{};
    std::unordered_set<size_t> free_tables_{};

    std::queue<size_t> queue_{};
    size_t free_id = 1;
    std::unordered_map<std::string, size_t> name_to_id_{};
    std::unordered_map<size_t, Table*> id_to_table_{};
    std::unordered_map<size_t, std::string> id_to_name_{};
};
