#ifndef PROCESS_H
#define PROCESS_H

#include "seed.h"
#include "const.h"
#include "bundle.h"

// CHALLENGE TYPE
struct challenge_type
{
    int level;
    int event;
    int difficulty;
    float distance;
    float limit;
};

const float UNKNOWN_DISTANCE = -2;
const float UNKNOWN_LIMIT = -2;

// CHALLENGE LEVEL
enum {  DOJO,
        LOTLD,
        PIT,
        TOWER,
        UNKNOWN_LEVEL };


const std::string LEVEL_LIST[5] = {
    "The Dojo",
    "Land of the Livid Dead",
    "The Neverending Pit",
    "The Infinite Tower",
    "Unknown"};

// CHALLENGE EVENT
enum {  SPEED,
        LUMS,
        DISTANCE,
        LUMS_SPEED,
        UNKNOWN_EVENT };

const std::string EVENT_LIST[5] = {
    "Get there quickly!",
    "Grab them quickly!",
    "As far as you can!",
    "Grab them quickly! (60\")",
    "Unknown"};

// CHALLENGE DIFFICULTY
enum {  NORMAL,
        EXPERT,
        UNKNOWN_DIFFICULTY };

const std::string DIFFICULTY_LIST[5] = {
    "Normal",
    "Extreme",
    "Unknown"};

/****************************************/
/// class process
/****************************************/

class process
{
    public:
        process(const std::string& process_name);
        process(void);
        bool open(const std::string& process_name);
        seed get_seed(int level);
        challenge_type get_type(seed cur_seed);
        float get_float(uint32_t address);
        uint32_t float_to_uint32(float f);
        bool change_seed(seed new_seed);
        bool change_seed_dojo(seed cur_seed, seed new_seed);
        bool change_distance(float distance);
        bool change_limit(float limit);

        operator bool() const;
        std::string get_last_error(void);

        static HANDLE get_handle(const std::string& process_name);

    private:

        HANDLE phandle;
        bool success;
        uint32_t location; // location of the seed
        uint32_t location_2; // lcation of the second seed (+ distance and type)

        std::string last_error;
};

#endif // PROCESS_H
