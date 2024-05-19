#include "Club.h"

#include <iostream>
#include "Time.h"

Club::Club(uint16_t open_time, uint16_t closing_time, size_t tables_cnt, size_t hour_cost) : opening_time_(open_time),
                                                                                             closing_time_(
                                                                                                     closing_time),
                                                                                             hour_cost_(hour_cost),
                                                                                             tables_(tables_cnt + 1) {
    for (size_t i = 1; i <= tables_cnt; ++i) {
        tables_[i].SetNumber(i);
        free_tables_.insert(i);
    }
}

void Club::AddNewClient(uint16_t coming_time, const std::string &client_name) {
    if (coming_time < opening_time_ || coming_time > closing_time_) {
        HandleNotOpenYetError(coming_time);
        return;
    }

    if (auto iter = name_to_id_.find(client_name); iter != name_to_id_.end()) {
        HandleYouShallNotPassError(coming_time);
        return;
    }

    name_to_id_[client_name] = free_id;
    id_to_name_[free_id] = client_name;
    ++free_id;
}

void Club::TakeTable(uint16_t taking_time, const std::string &client_name, size_t table_num) {
    auto client_iter = name_to_id_.find(client_name);
    if (client_iter == name_to_id_.end()) {
        HandleClientUnknownError(taking_time);
        return;
    }

    size_t client_id = client_iter->second;

    if (!tables_[table_num].TakeTable(client_id, taking_time)) {
        HandlePlaceIsBusyError(taking_time);
        return;
    }

    free_tables_.erase(table_num);

    if (auto table_iter = id_to_table_.find(client_id); table_iter != id_to_table_.end()) {
        Table *table = table_iter->second;
        if (!table->FreeTable(client_id, taking_time)) {
            std::cerr << "Error in table management" << '\n';
        }
        free_tables_.insert(table->GetNumber());
        id_to_table_.erase(table_iter);
    }

    id_to_table_[client_id] = &tables_[table_num];
}

void Club::ClientLeave(uint16_t leaving_time, size_t client_id) {
    if (auto table_iter = id_to_table_.find(client_id); table_iter != id_to_table_.end()) {
        Table *table = table_iter->second;
        if (!table->FreeTable(client_id, leaving_time)) {
            std::cerr << "Error in table management" << '\n';
        }
        free_tables_.insert(table->GetNumber());
        id_to_table_.erase(table_iter);
    }

    std::string client_name = id_to_name_[client_id];
    id_to_name_.erase(client_id);
    name_to_id_.erase(client_name);

    std::cout << Time::UINT16TToTime(leaving_time) << " 11 " << client_name << '\n';
}

void Club::AddClientToQueue(uint16_t start_time, const std::string &client_name) {
    auto client_iter = name_to_id_.find(client_name);
    if (client_iter == name_to_id_.end()) {
        HandleClientUnknownError(start_time);
        return;
    }

    size_t client_id = client_iter->second;

    if (!free_tables_.empty()) {
        HandleICanWaitNoLongerError(start_time);
        return;
    }

    if (queue_.size() + 1 >= tables_.size()) {
        ClientLeave(start_time, client_id);
        return;
    }

    queue_.push(client_id);
}

void Club::TakeFreedTable(uint16_t taking_time, size_t client_id, size_t table_num) {
    tables_[table_num].TakeTable(client_id, taking_time);
    free_tables_.erase(table_num);
    id_to_table_[client_id] = &tables_[table_num];

    std::cout << Time::UINT16TToTime(taking_time) << " 12 " << id_to_name_[client_id] << ' ' << table_num << '\n';
}

void Club::DeleteClient(uint16_t leaving_time, const std::string &client_name, bool reseat) {
    auto client_iter = name_to_id_.find(client_name);
    if (client_iter == name_to_id_.end()) {
        HandleClientUnknownError(leaving_time);
        return;
    }

    size_t client_id = client_iter->second;

    if (auto table_iter = id_to_table_.find(client_id); table_iter != id_to_table_.end()) {
        Table *table = table_iter->second;
        if (!table->FreeTable(client_id, leaving_time)) {
            std::cerr << "Error in table management" << '\n';
        }
        free_tables_.insert(table->GetNumber());
        id_to_table_.erase(table_iter);
    }

    if (!queue_.empty() && reseat) {
        size_t freed_client_id = queue_.front();
        queue_.pop();
        auto table_num = free_tables_.begin();
        TakeFreedTable(leaving_time, freed_client_id, *table_num);
    }

    id_to_name_.erase(client_id);
    name_to_id_.erase(client_name);
}

void Club::EndDay() {
    std::vector<std::string> leaving_clients;
    leaving_clients.reserve(name_to_id_.size());

    while (!id_to_table_.empty()) {
        auto iter = id_to_table_.begin();
        leaving_clients.emplace_back(id_to_name_[iter->first]);
        DeleteClient(closing_time_, leaving_clients.back(), false);
    }

    while (!queue_.empty()) {
        size_t client_id = queue_.front();
        queue_.pop();
        DeleteClient(closing_time_, id_to_name_[client_id], false);
    }

    std::sort(leaving_clients.begin(), leaving_clients.end());

    std::string closing_time_str = Time::UINT16TToTime(closing_time_);
    for (std::string &client_name: leaving_clients) {
        std::cout << closing_time_str << " 11 " << client_name << '\n';
    }
}

void Club::PrintStatistic() const {
    for (size_t i = 1; i < tables_.size(); ++i) {
        size_t income = tables_[i].GetWorkingHours() * hour_cost_;
        std::cout << i << ' ' << income << ' ' << Time::UINT16TToTime(tables_[i].GetWorkingTime()) << '\n';
    }
}

uint16_t Club::GetClosingTime() const {
    return closing_time_;
}

uint16_t Club::GetOpeningTime() const {
    return opening_time_;
}

void Club::HandleClientUnknownError(uint16_t error_time) {
    std::cout << Time::UINT16TToTime(error_time) << " 13 " << "ClientUnknown" << '\n';
}

void Club::HandleICanWaitNoLongerError(uint16_t error_time) {
    std::cout << Time::UINT16TToTime(error_time) << " 13 " << "ICanWaitNoLonger" << '\n';
}

void Club::HandlePlaceIsBusyError(uint16_t error_time) {
    std::cout << Time::UINT16TToTime(error_time) << " 13 " << "PlaceIsBusy" << '\n';
}

void Club::HandleYouShallNotPassError(uint16_t error_time) {
    std::cout << Time::UINT16TToTime(error_time) << " 13 " << "YouShallNotPass" << '\n';
}

void Club::HandleNotOpenYetError(uint16_t error_time) {
    std::cout << Time::UINT16TToTime(error_time) << " 13 " << "NotOpenYet" << '\n';
}