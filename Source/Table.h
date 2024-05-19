#include <cstddef>
#include <cstdint>

class Table {
public:
    Table() = default;

    void SetNumber(size_t number);

    // Returns number of table
    size_t GetNumber() const;

    // Attempt to take the table from client with client_id on take_time. Returns true if attempt successful,
    // false if attempt failed (table already busy)
    bool TakeTable(size_t client_id, uint16_t take_time);

    // Attempt to free the table from client with client_id on free_time. Returns true if attempt successful,
    // false if attempt failed (client_id is not equal to client_id_)
    bool FreeTable(size_t client_id, uint16_t free_time);

    // Returns working hours.
    uint16_t GetWorkingHours() const;

    // Returns working time.
    uint16_t GetWorkingTime() const;

private:
    size_t number_ = 0;
    bool is_busy_ = false;
    size_t client_id_ = 0;
    uint16_t start_time_ = 0;
    uint16_t busy_hours_ = 0;
    uint16_t busy_time_ = 0;
};
