#include <iostream>
#include <string>
#include <sstream>
#include <array>

// Table size defined for 26 characters
constexpr int TABLE_SIZE = 26;

// enum to indicate status
enum Status {NEVER_USED, TOMBSTONE, OCCUPIED};

// Each slot has structure key, status
struct Slot
{
    std::string key;
    Status status = NEVER_USED;
};

// Helper function for indexing
int hash_idx(const std::string &s)
{
    return s.back() - 'a';
}

// Search function to find key
int search_key(const std::array<Slot, TABLE_SIZE> &table, const std::string &key)
{
    int i = hash_idx(key);
    for (int step = 0; step < TABLE_SIZE; ++step)
    {
        const Slot &slot = table[i];
        if (slot.status == NEVER_USED) return -1; // Nothing of that key is stored
        if (slot.status == OCCUPIED && slot.key == key) return i; // Value found at i
        i = (i + 1) % TABLE_SIZE; // Include wraparound
    }
    return -1; // No value found in the whole table
}

int main()
{
    std::array<Slot, TABLE_SIZE> table; // Setup table

    std::string line;
    std::getline(std::cin, line);

    std::stringstream ss(line);
    std::string token;

    while (ss >> token)
    {
        char op = token[0]; // First character A or D
        std::string key = token.substr(1); // Keyword

        if (op == 'A') // Insertion
        { 
            if (search_key(table, key) != -1) continue; // Already exists
            int i = hash_idx(key);
            for (int step = 0; step < TABLE_SIZE; ++step)
            {
                if (table[i].status != OCCUPIED)
                {
                    table[i].key = key;
                    table[i].status = OCCUPIED;
                    break;
                }
                i = (i + 1) % TABLE_SIZE;
            }
        }
        else if (op == 'D') // Deletion
        {
            int pos = search_key(table, key);
            if (pos != -1)
            {
                table[pos].key.clear();
                table[pos].status = TOMBSTONE; // Mark as "dead" data
            }
        }
    }

    // Output all occupied slots in order
    bool first = true;
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        if (table[i].status == OCCUPIED)
        {
            if (!first) std::cout << ' ';
            std::cout << table[i].key;
            first = false;
        }
    }
    std::cout << '\n';

    return 0;
}
