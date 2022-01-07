#include <iostream>
#include <vector>
#include "account.hpp"
#include "function.hpp"

using namespace std;

int main()
{
    vector<account> accounts;
    bool bankruptcy = false;
    string strCommand;
    cout << "Enter your command:\n>>> ";
    getline(cin, strCommand);

    while (strCommand != "exit")
    {
        if (strCommand.size() != 0 && (int)strCommand[0] != 32)
        {
            char* chCommand = new char[strCommand.length()+1];  
            strcpy(chCommand, strCommand.c_str());              
            splitCommand(chCommand, accounts, bankruptcy);
            delete[] chCommand;
        }
        else
        {
            cout << "vared nashod" << endl;
        }
        cout << "*****************************" << endl;
        cout << "Enter your command:\n>>> ";
        getline(cin, strCommand);       
    }

    return 0;
}