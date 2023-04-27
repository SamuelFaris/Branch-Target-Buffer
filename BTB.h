
#ifndef _BTB
#define _BTB

#include "Entry.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <bitset>
using namespace std;

#define TABLE_SIZE 1024

class BTB
{
private:
    //BTB values
    Entry table [TABLE_SIZE];
    uint32_t indexed_pc;
    uint32_t predicted_pc;
    uint32_t index;
    bool taken;
    bool machine_sel; //selects state machine implementation
    bool collision_on_last; //used to update BTB after a collision in the decoding stage
    
    //Benchmark variables
    int instruction_count = 1;
    int hits;
    int misses;
    int correct_predictions;
    int incorrect_predictions;
    int incorrect_addr_count;
    int collisions;
    int branches_taken;
    int stalls;

public:
    BTB(bool machine_sel);

    void run(uint32_t current_pc);
    void compare(uint32_t actual_pc);
    void update_prediction(bool taken);

    void print_results(fstream& logOut);

};

#endif