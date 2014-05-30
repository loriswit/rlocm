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
        MURPHY,
        UNKNOWN_LEVEL };


const std::string LEVEL_LIST[6] = {
    "Level: The Dojo",
    "Level: Land of the Livid Dead",
    "Level: The Neverending Pit",
    "Level: The Infinite Tower",
    "Level: Murphy's Dungeon",
    "Level: Unknown"};

/*const char* LEVEL_LIST[6] = {
    "Level: The Dojo",
    "Level: Land of the Livid Dead",
    "Level: The Neverending Pit",
    "Level: The Infinite Tower",
    "Level: Murphy's Dungeon",
    "Level: Unknown"};*/

// CHALLENGE EVENT
enum {  SPEED,
        LUMS,
        DISTANCE,
        LUMS_SPEED,
        UNKNOWN_EVENT };

const std::string EVENT_LIST[5] = {
    "Event: Get there quickly!",
    "Event: Grab them quickly!",
    "Event: As far as you can!",
    "Event: Grab them quickly! (60\")",
    "Event: Unknown"};

/*const char* EVENT_LIST[5] = {
    "Event: Get there quickly!",
    "Event: Grab them quickly!",
    "Event: As far as you can!",
    "Event: Grab them quickly! (60\")",
    "Event: Unknown"};*/

// CHALLENGE DIFFICULTY
enum {  NORMAL,
        EXPERT,
        UNKNOWN_DIFFICULTY };

const std::string DIFFICULTY_LIST[5] = {
    "Difficulty: Normal",
    "Difficulty: Extreme",
    "Difficulty: Unknown"};

/*const char* DIFFICULTY_LIST[5] = {
    "Difficulty: Normal",
    "Difficulty: Extreme",
    "Difficulty: Unknown"};*/

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
