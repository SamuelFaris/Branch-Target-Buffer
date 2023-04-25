
#include "BTB.h"

#define CLASS_STATE_MACHINE 1
#define STATE_MACHINE_A 0
#define TEST_1 "program_trace.txt"
#define TEST_2 "Espresso_int.txt"
#define TEST_3 "Spice_FP.txt"

int main(void)
{
    string fileName = TEST_3;
    BTB btb(CLASS_STATE_MACHINE);

    fstream program_trace(fileName);
    if(!program_trace.is_open())
        cout << "\nCould not open trace file\n";
    else
    {
        cout << hex << "\nOpened file: " << fileName << "\nBeginning simulation.\n";
        string buffer;
        getline(program_trace, buffer);
        uint32_t curr = stoi(buffer, 0, 16);
        uint32_t next = 0;

        while(!program_trace.eof())
        {
            //Send current pc to BTB and have it predict target pc
            btb.run(curr);

            //Send actual target pc to BTB and have it update table/stats accordingly
            getline(program_trace, buffer);
            next = stoi(buffer, 0, 16);
            btb.compare(next);

            //Iterate addresses
            curr = next;
        }

        //Write results to log file
        fstream logOut("results.log", ios::out | ios::trunc);
        logOut.close();
        logOut.open("results.log");
        logOut << "Trace: " << fileName;
        btb.print_results(logOut);

        logOut.close();
        program_trace.close();
    }

    return 0;
}