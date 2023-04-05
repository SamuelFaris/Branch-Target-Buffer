
#ifndef _BTB
#define _BTB

#include "Entry.h"
#include <fstream>
#include <iostream>
using namespace std;

class BTB
{
private:
    //BTB values
    Entry table [1024];
    uint32_t indexed_pc;
    uint32_t predicted_pc;
    uint32_t index;
    bool taken;
    bool machine_sel; //selects state machine implementation
    
    //Benchmark variables
    int hits;
    int misses;
    int correct_predictions;
    int incorrect_predictions;
    int incorrect_addr_count;
    int collisions;
    int branches_taken;

public:
    BTB(bool machine_sel);

    void run(uint32_t current_pc);
    void compare(uint32_t actual_pc);
    void insert(uint32_t current_pc, uint32_t target_pc);
    void update_prediction(bool taken);

    void print_results(void);

};

#endif