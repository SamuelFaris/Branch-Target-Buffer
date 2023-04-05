
#include "BTB.h"

BTB::BTB(bool machine_sel = 0)
    : machine_sel(machine_sel)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table[i].pc = 0;
        table[i].target = 0;
        table[i].prediction = 0;
    }
}

//Simulates indexing into the BTB and returning the prediction
void BTB::run(uint32_t current_pc)
{
    //remove 20 MSBs and lsr 2 to obtain 10bit indexing value
    instruction_count++;
    indexed_pc = current_pc;
    index = (indexed_pc & 0x00000FFF) >> 2; 

    //check if entry exists, if it does then assign predicted pc based on table entry
    if (table[index].pc != 0)
    {
        //check for collision
        if (table[index].pc != indexed_pc)
        {
            table[index].pc = indexed_pc;
            collisions++;
        }

        //update predicted_pc: target if taken, pc+4 if not taken
        if (table[index].prediction == 0 || table[index].prediction == 1)
           predicted_pc = table[index].target;
        else
            predicted_pc = current_pc + 4;
        return;
    }

    //no prediction if entry does not exist
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
    
    //If the entry exists already, update BTB prediction data
    //TODO: check back on the if condition here
    if (table[index].pc == indexed_pc)
    {
        hits++;

        //Update prediction counts
        if (predicted_pc == actual_pc)
            correct_predictions++;
        else
            incorrect_predictions++;
        
        //Update table values
        update_prediction(taken);
        if (taken)
        {
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
            case 0b00:
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
        cout << "updating prediction using machine A\n";
        switch(prediction)
        {
            case 0b00:
                if (taken)
                    prediction = 0b00;
                else
                    prediction = 0b01;
                break;

            case 0b01:
                if (taken)
                    prediction = 0b00;
                else
                    prediction = 0b11;
                break;

            case 0b10:
                if (taken)
                    prediction = 0b01;
                else
                    prediction = 0b11;
                break;
            
            case 0b11:
                if (taken)
                    prediction = 0b10;
                else
                    prediction = 0b11;
                break;

            default:
                prediction = prediction;
        }
    }

    table[index].prediction = prediction;
}

void BTB::print_results()
{
    cout << endl << endl;
    cout << "          BTB Table            \n";
    cout << "+-------+----------+----------+\n";
    cout << "| Entry |    PC    |  Target  |\n";
    cout << "+-------+----------+----------+\n";

    for (int i = 0; i < 1024; i++)
        if (table[i].pc != 0)
            cout << dec << "|" << setw(7) << i << hex << "| 0x" << table[i].pc << " | 0x" << table[i].target << " |\n";
    cout << "+-------+----------+----------+\n";

    double hit_rate = (double)hits/(hits+misses);
    double accuracy = (double)correct_predictions/hits;
    double wrong_addr = (double)incorrect_addr_count/incorrect_predictions;
    string state_machine = machine_sel ? "CLASS_STATE_MACHINE" : "STATE_MACHINE_A";

    cout << dec << endl << endl;
    cout << "          BTB Results          \n";
    cout << "+-----------------------------+\n";
    cout << "   Using " << state_machine << "\n\n";
    cout << "   Instruction count: " << instruction_count << "\n";
    cout << "   Hits: " << hits << "\n";
    cout << "   Misses: " << misses << "\n";
    cout << "   Correct: " << correct_predictions << "\n";
    cout << "   Incorrect: " << incorrect_predictions << "\n"; 
    cout << "   Branches Taken: " << branches_taken << "\n";
    cout << "   Collisions: " << collisions << "\n\n";
    cout << "   Hit rate: " << hit_rate*100 << "%\n";
    cout << "   Accuracy: " << accuracy*100 << "%\n";
    cout << "   Wrong address: " << wrong_addr*100 << "%\n";
    cout << "+-----------------------------+\n\n";
}