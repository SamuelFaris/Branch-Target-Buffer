
#include "BTB.h"

#define CLASS_STATE_MACHINE 1
#define STATE_MACHINE_A 0

int main(void)
{
    BTB btb(CLASS_STATE_MACHINE);

    fstream program_trace("program_trace.txt");
    if(!program_trace.is_open())
        cout << "\nCould not open trace file\n";
    else
    {
        int count = 1;
        cout << "\nOpened file. Beginning simulation.\n";
        string buffer;
        getline(program_trace, buffer);
        cout << "first stoi\n";
        uint32_t curr = stoi(buffer);
        uint32_t next = 0;

        while(!program_trace.eof())
        {
            count++;
            //Iterate through pairs of addresses
            getline(program_trace, buffer);
            cout << "line " << count << endl;
            next = stoi(buffer);

            //Send current pc to BTB and have it predict target pc
            btb.run(curr);

            //Send actual target pc to BTB and have it update stats accordingly
            btb.compare(next);

            curr = next;
        }

        btb.print_results();
    }

    return 0;
}