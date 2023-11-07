// MMKLogixCompiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Developed by:  Mehrbod Molla Kazemi.
// Professor:  Dr. Safaei Mehrabani.

#include "MMKLogixCompiler.h"

int main()
{
    // Open program.txt.
    ifstream file_Program("program.txt");
    if (!file_Program.is_open())
    {
        cout << "File program.txt not found!\b" << endl;
        return -2;
    }
}
