
#include "BTB.h"

BTB::BTB(bool machine_sel = 0)
    : machine_sel(machine_sel)
{
}

//Simulates indexing into the BTB and returning the prediction
void BTB::run(uint32_t current_pc)
{
    //remove 20 MSBs and lsr 2 to obtain 10bit indexing value
    indexed_pc = current_pc;
    index = (indexed_pc && 0x00000FFF) >> 2; 

    //check if entry exists, if it does then assign predicted pc based on table entry
    if (table[index].pc != 0 || table[index].pc == indexed_pc)
    {
        //check for collision
        if (table[index].pc != indexed_pc)
        {
            table[index].pc = indexed_pc;
            collisions++;
        }

        //update predicted_pc
        if (table[index].prediction == 0 || table[index].prediction == 1)
           predicted_pc = table[index].target;
        else
            predicted_pc = current_pc + 4;
        return;
    }

    //else no prediction
    predicted_pc = 0;
    return;
}

//Simulates the comparison between predicted and actual pcs and updates table as needed
void BTB::compare(uint32_t actual_pc)
{
    //Check if branch was taken or not
    if (actual_pc != indexed_pc + 4)
    {
        taken = true;
        branches_taken++;
    }
    else
        taken = false;

    //Update prediction counts
    if (predicted_pc == actual_pc)
        correct_predictions++;
    else
        incorrect_predictions++;
    
    //If the entry exists already, update BTB prediction data
    //TODO: check back on the if condition here
    if (table[index].pc == indexed_pc)
    {
        update_prediction(taken);
        if (taken)
        {
            hits++;
            if (table[index].target != actual_pc)
            {   
                table[index].target = actual_pc;
                incorrect_addr_count++;
            }
        }
    }

    //Else add this entry to the BTB if branch was taken
    else if (taken)
    {
        table[index].pc = indexed_pc;
        table[index].target = actual_pc;
        table[index].prediction = 0;
        misses++;
    }
}


//Uses state machines to update prediction value
void BTB::update_prediction(bool taken)
{
    //Guide:
    // 0b'00 = strongly taken
    // 0b'01 = weakly taken
    // 0b'10 = weakly not taken
    // 0b'11 = strongly not taken
    uint32_t prediction = table[index].prediction;

    //Using state machine from lecture slides
    if (machine_sel)
    {
        switch(prediction)
        {
            case 0:
                if (taken)
                    prediction = 0;
                else
                    prediction = 1;
                break;

            case 1:
                if (taken)
                    prediction = 0;
                else
                    prediction = 2;
                break;

            case 2:
                if (taken)
                    prediction = 1;
                else
                    prediction = 3;
                break;
            
            case 3:
                if (taken)
                    prediction = 2;
                else
                    prediction = 3;
                break;

            default:
                prediction = prediction;
        }
    }

    //Using state machine A
    //If not taken when predicted to be weakly taken, jump to strongly not taken
    else
    {
        switch(prediction)
        {
            case 0:
                if (taken)
                    prediction = 0;
                else
                    prediction = 1;
                break;

            case 1:
                if (taken)
                    prediction = 0;
                else
                    prediction = 3;
                break;

            case 2:
                if (taken)
                    prediction = 1;
                else
                    prediction = 3;
                break;
            
            case 3:
                if (taken)
                    prediction = 2;
                else
                    prediction = 3;
                break;

            default:
                prediction = prediction;
        }
    }

    table[index].prediction = prediction;
}

void BTB::print_results(void)
{
    for (int i = 0; i < 1024; i++)
    {
        cout << "table[" << i << "] = " << table[i].pc << ", " << table[i].target << ", " << table[i].prediction << endl;
    }

    double hit_rate = (double)hits/(hits+misses);
    double accuracy = (double)correct_predictions/hits;
    double wrong_addr = (double)incorrect_addr_count/incorrect_predictions;

    cout << "\n------- BTB Results -------\n";
    cout << "Hits: " << hits << "\n";
    cout << "Misses: " << misses << "\n";
    cout << "Correct: " << correct_predictions << "\n";
    cout << "Incorrect: " << incorrect_predictions << "\n"; 
    cout << "Branches Taken: " << branches_taken << "\n";
    cout << "Collisions: " << collisions << "\n\n";
    cout << "Hit rate: " << hit_rate*100 << "%\n";
    cout << "Accuracy: " << accuracy*100 << "%\n";
    cout << "Wrong address: " << wrong_addr*100 << "%\n";
}