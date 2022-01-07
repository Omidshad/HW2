#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cctype>
#include <random>
#include "account.hpp"
#include "function.hpp"

using namespace std;

unsigned int cardNumberGenerator(const vector<account>& accounts)   
{
    default_random_engine engine(static_cast<unsigned int>(time(0)));
    uniform_int_distribution<unsigned int> randNum(1000, 9999);
    bool find = false;
    unsigned int cardNumber;
    while (!find)
    {
        find = true;
        cardNumber = randNum(engine);
        for (size_t i = 0; i < accounts.size(); i++)
        {
            if (accounts[i].getCardNumber() == cardNumber)
            {
                find = false;
                break;
            }
        }
    }
    return cardNumber;
}

bool usernameValidator(const string& userName)            
{
    if (isdigit(userName[0]))
    {
        return false;
    }
    for (size_t i = 0; i < userName.length(); i++)
    {
        if (ispunct(userName[i]))
        {
            return false;
        }
    }
    return true;
}

bool ipValidator(const string& strIP)                     
{
    char* ip = new char[strIP.length() + 1];
    strcpy(ip, strIP.c_str());
    char* token = strtok(ip, ".");
    if (token == NULL)
    {
        delete[] ip;
        return false;
    }
    int counter = 0;
    while (token != NULL)
    {
        counter++;
        string temp = token;
        for (size_t i = 0; i < temp.length(); i++)
        {
            if (!isdigit(temp[i]))
            {
                delete[] ip;
                return false;
            }
        }
        int tempNum = stoi(temp);
        if (tempNum > 255 || tempNum < 0)
        {
            delete[] ip;
            return false;
        }
        token = strtok(NULL, ".");
    }
    if (counter != 4)
    {
        delete[] ip;
        return false;
    }
    delete[] ip;
    return true;
}


int findUsername(const string& username, const vector<account> accounts) 
{
    for (size_t i = 0; i < accounts.size() ; i++)
    {
        if (accounts[i].getUsername() == username)
        {
            return i;
        }
    }
    return -1;
}

int findIP(const string& ip, const vector<account>& accounts)      
{
    vector<string> temp;
    for (size_t i = 0; i < accounts.size() ; i++)
    {
        temp = accounts[i].getIP();
        for (size_t j = 0; j < temp.size(); j++)
        {
            if (temp[j] == ip)
            {
                return i;
            }
        }
    }
    return -1;
}

void addTransaction(bool deposit, string origin, string destination, long double amount, int index, vector<account>& accounts, bool profits = false)
{
    transaction temp;
    if (deposit == true)
    {
        temp.deposit = true;
    }
    else 
    {
        temp.withdraw = true;
    }
    if (profits)
    {
        temp.profits = true;
    }
    temp.origin = origin;
    temp.destination = destination;
    temp.amount = amount;
    temp.date = time(0);
    temp.remained = accounts[index].getBalance();
    accounts[index].addTransaction(temp);
}

bool printInfo(string token, vector<account>& accounts)
{
    int index = findUsername(token, accounts);
    bool find = false;
    for (size_t i = 0; i < accounts.size() && !find; i++)
    {
        if (to_string(accounts[i].getCardNumber()) == token)
        {
            index = i;
            find = true;
        }

        vector<string> temp = accounts[i].getIP();
        for (size_t j = 0; j < temp.size() && !find; j++)
        {
            if (temp[j] == token)
            {
                index = i;
                find = true;
            }
        }
    }
    if (index != -1)
    {    
        cout << "----------------------------------" << endl;
        cout << "Username: " << accounts[index].getUsername() << endl;
        cout << "--------" << endl;
        cout << "IPs:" << endl;
        accounts[index].printIP();
        cout << "--------" << endl;
        cout << "Card number:" << accounts[index].getCardNumber() << endl;
        cout << "--------" << endl;
        cout << "Balance: " << accounts[index].getBalance() << "$" << endl;
        cout << "--------" << endl;
        time_t tempOpen = accounts[index].getOpeningDate();
        tm* tempTime = localtime(&tempOpen);
        cout << "Opening date: " << (tempTime->tm_year + 1900) << "-" << (tempTime->tm_mon + 1) << "-" << (tempTime->tm_mday) << endl;
        tempOpen = accounts[index].getExpDate();
        tempTime = localtime(&tempOpen);
        cout << "Expired time: " << (tempTime->tm_year + 1900) << "-" << (tempTime->tm_mon + 1) << "-" << (tempTime->tm_mday) << endl;
        cout << "Debt: " << accounts[index].getDebt() << endl;    
        return true;
    }
    return false;
}

bool checkBankruptcy(const long double& amount, const vector<account>& accounts)
{
    long double loans = 0;
    long double allBalance = 0;
    long double availableBalance = 0;
    for (size_t i = 0; i < accounts.size(); i++)
    {
        loans += accounts[i].getDebt();
        allBalance += accounts[i].getBalance();
    }
    availableBalance = allBalance - loans;
    if (availableBalance < amount)
    {
        return true;        
    }
    return false;
}

void payLoan(const string& username, const long double& amount, vector<account>& accounts)
{

    int index = findUsername(username, accounts);
    if (index == -1)
    {
        throw invalid_argument("Error: username not found");
    }
    if (amount < 0)
    {
        throw invalid_argument("Error: invalid amount of money");
    }
    
    if (accounts[index].getDebt() == 0)
    {
        accounts[index].changeBalance(amount);
        cout << "you have no debts and money added to your account. your balance is: " << accounts[index].getBalance() << "$" << endl;
    }
    else if (accounts[index].getDebt() < amount)
    {
        long double debt = accounts[index].getDebt();
        long double extra = amount - accounts[index].getDebt();
        accounts[index].changeDebt((-1) * debt);
        accounts[index].changeBalance(extra);
        cout << "debt successfully paid. current debt is: " << accounts[index].getDebt() << endl;
        cout << extra << "$ was extra and added to your account. your balance is: " << accounts[index].getBalance() << "$" << endl;
    }
    else 
    {
        accounts[index].changeDebt((-1) * amount);
        cout << "debt successfully paid. current debt is: " << accounts[index].getDebt() << endl;
    }
}

void getLoan(const string& username, const string& ip, const long double& amount, vector<account>& accounts)
{
    int index = findUsername(username, accounts);
    if (index == -1)
    {
        throw invalid_argument("Error: username not found");
    }
    int ipIndex = findIP(ip, accounts);
    if (ipIndex == -1 || ipIndex != index)
    {
        throw invalid_argument("Error: ip not found");
    }
    if (time(0) >= accounts[index].getExpDate())
    {
        throw invalid_argument("Error: your account has expired");
    }
    if (amount < 0)
    {
        throw invalid_argument("invalid amount");
    }
    if (accounts[index].getBalance() < (amount * 0.75))
    {
        throw invalid_argument("you can\'t get the loan. you must have at least 0.75 of the money you requested");
    }
    if (accounts[index].getDebt() > 0)
    {
        throw invalid_argument("you have unpaid loan");
    }
    
    long double loans = 0;
    long double allbalance = 0;
    for (size_t i = 0; i < accounts.size(); i++)
    {
        loans += accounts[i].getDebt();
        allbalance += accounts[i].getBalance();
    }
    long double availableBalance = allbalance - loans;
    if (availableBalance < amount)
    {
        throw invalid_argument("Bank can\'t give a loan in this amount");
    }
    accounts[index].changeDebt(amount + (amount * 0.25));
    cout << "The loan successfully gave" << endl;
}

void addProfits(const string& username, vector<account>& accounts)
{
    int index = findUsername(username, accounts);
    if (index == -1)
    {
        throw invalid_argument("Error: Username not found");
    }
    if (time(0) >= accounts[index].getExpDate())
    {
        throw invalid_argument("Error: your account has expired");
    }
    if (accounts[index].getOpeningDate() > (time(0) - 2592000))  
    {
        throw invalid_argument("you can\'t get the profits. account must created for more than 30 days");
    }
    vector<transaction> temp = accounts[index].getTransaction();
    for (size_t i = 0; i < temp.size(); i++)
    {
        if ((temp[i].date > (time(0) - 2592000)) && temp[i].profits)
        {
            throw invalid_argument("you can\'t get the profits. you have got the profits in last 30 days");
        }
    }

    time_t now = time(0);
    int counter = 1;
    bool hasWithdraw = false;
    long double balance = accounts[index].getBalance();
    for (size_t i = 0; i < temp.size(); i++)
    {
        if ((temp[i].date > now - 604800) && (temp[i].date < now))
        {
            if (temp[i].withdraw)
            {
                hasWithdraw = true;
            }
            
            counter++;
            balance += temp[i].remained;
        }
    }
    long double profit;
    if (hasWithdraw)
    {
        profit = ((balance/counter) * 0.18 ) / 12;          
    }
    else
    {
        profit = ((balance/counter) * 0.20 ) / 12;     
    }
    accounts[index].changeBalance(profit);
    cout << profit << "$ added" << endl;
    addTransaction(true, "Bank", username, profit, index, accounts, true);
}

void transfer(const string& senderUsername, const string& senderIP, string& receiver, const long double& amount, vector<account>& accounts)
{
    int senderIndex = findUsername(senderUsername, accounts);
    if (senderIndex == -1)
    {
        throw invalid_argument("Error: Sender username not found");
    }
    int senderIpIndex = findIP(senderIP, accounts);
    if (senderIpIndex == -1 || senderIpIndex != senderIndex)
    {
        throw invalid_argument("Error: Sender IP not found");
    }

    int receiverIndex = findUsername(receiver, accounts);
    int receiverIpIndex = findIP(receiver, accounts);
    if (receiverIndex == -1 && receiverIpIndex == -1)
    {
        throw invalid_argument("Error: Receiver username or ip not found");
    }
    if (amount <= 0)
    {
        throw invalid_argument("Error: invalid amount of money");
    }
    if (time(0) >= accounts[senderIndex].getExpDate())     
    {
        cout << "Your account has expired" << endl;
        renewal(senderUsername, senderIP, accounts);
        return;
    }
    if (accounts[senderIndex].getBalance() < amount)
    {
        throw invalid_argument("Error: your money is not enough");
    }

    accounts[senderIndex].changeBalance((-1) * amount);

    if (receiverIndex != -1)    
    {
        accounts[receiverIndex].changeBalance(amount);     
        addTransaction(true, senderUsername, receiver, amount, receiverIndex, accounts);      
    }
    else    
    {
        accounts[receiverIpIndex].changeBalance(amount);
        receiver = accounts[receiverIpIndex].getUsername();
        addTransaction(true, senderUsername, receiver, amount, receiverIpIndex, accounts);    
    }

    addTransaction(false, senderUsername, receiver, amount, senderIndex, accounts);  
    cout << "Transfer compeleted" << endl;
}

void depositAndWithdraw(bool deposit, const string& username, const string& ip, const long double& amount, vector<account>& accounts, bool& bankruptcy)
{
    int index = findUsername(username, accounts);
    if (index == -1)
    {
        throw invalid_argument("Error: Username not found");
    }

    int ipIndex = findIP(ip, accounts);
    if (ipIndex == -1 || ipIndex != index)
    {
        throw invalid_argument("Error: IP not found");
    }
    if (amount <= 0)
    {
        throw invalid_argument("Error: Invalid amount of money");
    }
    if (time(0) >= accounts[index].getExpDate())    
    {
        cout << "Your account has expired" << endl;
        renewal(username, ip, accounts);
        return;
    }
    
    if (deposit == true)
    {
        accounts[index].changeBalance(amount);
        addTransaction(true, "cash", username, amount, index, accounts);
        cout << "Money successfully added. current balance is: " << accounts[index].getBalance() << "$" << endl;
    }
    else if (deposit == false)
    {
        if (accounts[index].getDebt() > 0)
        {
            throw invalid_argument("you have unpaid loan. you can\'t withdraw money");
        }
        if (accounts[index].getBalance() < amount)
        {
            throw invalid_argument("Your balance is not enough");
        }
        if (checkBankruptcy(amount, accounts))
        {
            bankruptcy = true;
            throw invalid_argument("The bank went bankrupt. we can\'t pay the money");
        }
        accounts[index].changeBalance((-1) * amount);
        addTransaction(false, username, "cash", amount, index, accounts);
        cout << "Money successfully withdrew. current balance is: " << accounts[index].getBalance() << "$" << endl;
    }
    
}


void renewal(const string& userName, const string& ip, vector<account>& accounts)
{
    int index = findUsername(userName, accounts);
    if (index == -1)
    {
        throw invalid_argument("Error: Username not found");
    }

    int ipIndex = findIP(ip, accounts);
    if (ipIndex == -1 || ipIndex != index)
    {
        throw invalid_argument("Error: IP not found");
    }
    
    cout << "The cost of renwal is 2$. Do you want to renew your account? (yes or no)" << endl;
    string decision;
    bool correct = false;
    while (!correct)
    {
        getline(cin, decision);
        if (decision == "yes")
        {
            correct = true;
            if (accounts[index].getBalance() < 2)
            {
                cout << "your account balance is not enough" << endl;
                return;
            }
            else
            {
                accounts[index].changeBalance(-2);
                accounts[index].setExpDate();               
                addTransaction(false, userName, "Bank", 2, index, accounts);
                cout << "Account renewed successfully. New expire time is: ";
                time_t newExp = accounts[index].getExpDate();
                tm* exp = localtime(&newExp);
                cout << (exp->tm_year + 1900) << "-" << (exp->tm_mon + 1) << "-" << (exp->tm_mday) << endl;
                
            }
        }
        else if (decision == "no")
        {
            correct = true;
            cout << "Renewal canceled" << endl;
            return;
        }
        else
        {
            cout << "invalid command... Do you want to renew your account? (yes or no)" << endl;
        }
    }
}

void addIP(const string& userName, const string& ip, vector<account>& accounts)  
{
    int index = findUsername(userName, accounts);
    if(index == -1)
    {
        throw invalid_argument("Error: Username not found");
    }
    if (findIP(ip, accounts) != -1)
    {
        throw invalid_argument("Error: Ip used before");
    }
    if (!ipValidator(ip))
    {
        throw invalid_argument("Error: IP is not valid");
    }
    accounts[index].setIP(ip);
    cout << "IP successfully added" << endl;
}

void addAccount(string& userName, char*& ip, vector<account>& accounts)
{
    string strIP = ip;
    if (!usernameValidator(userName) || !ipValidator(ip))
    {
        throw invalid_argument("Error: invalid username or ip");
    }
    int index = findUsername(userName, accounts);
    if (index > -1)
    {
        throw invalid_argument("Error: Username used before");
    }
    if (findIP(ip, accounts) != -1)
    {
        throw invalid_argument("Error: IP used before");
    }
    unsigned int cardNumber = cardNumberGenerator(accounts);
    account temp(userName, strIP, cardNumber);
    accounts.push_back(temp);
    cout << "Account successfully added" << endl;
}


void splitCommand(char*& command, vector<account>& accounts, bool& bankruptcy)
{
    char* token = strtok(command, " ");
    try
    {
        if (bankruptcy)
        {
            throw invalid_argument("bank has gone bankrupt. No services are provided");
        }
        else if ((string)token == "create")
        {
            token = strtok(NULL, ":");
            string userName = token;
            token = strtok(NULL, " ");
            if (token == NULL)
            {
                throw invalid_argument("Error: invalid command");
            }   
            char* ip = token;
            addAccount(userName, ip, accounts);
        }
        else if ((string)token == "add_ip")
        {
            token = strtok(NULL, ":");
            string username = token;
            token = strtok(NULL, " ");
            if (token == NULL)
            {
                throw invalid_argument("Error: invalid command");
            }
            string ip = token;
            addIP(username , ip, accounts);
        }
        else if((string)token == "renewal")
        {
            token = strtok(NULL, ":");
            string userName = token;
            token = strtok(NULL, " ");
            if (token == NULL)
            {
                throw invalid_argument("Error: invalid command");
            }
            string ip = token;
            renewal(userName, ip, accounts);
        }
        else if ((string)token == "deposit")
        {
            token = strtok(NULL, ":");
            string username = token;
            token = strtok(NULL, ":");
            string ip = token;
            token = strtok(NULL, " ");
            if (token == NULL)
            {
                throw invalid_argument("Error: invalid command");
            }
            string strAmount = token;
            long double amount = stold(strAmount);
            depositAndWithdraw(true, username, ip, amount, accounts, bankruptcy);
        }
        else if ((string)token == "transfer")
        {
            token = strtok(NULL, ":");
            string senderUsername = token;
            token = strtok(NULL, ":");
            string senderIP = token;
            token = strtok(NULL, ":");
            string receiverUsername = token;
            token = strtok(NULL, " ");
            if (token == NULL)
            {
                throw invalid_argument("Error: invalid command");
            }
            string strAmount = token;
            long double amount = stold(strAmount);
            transfer(senderUsername, senderIP, receiverUsername, amount, accounts);
        }
        else if ((string)token == "withdraw")
        {
            token = strtok(NULL, ":");
            string username = token;
            token = strtok(NULL, ":");
            string ip = token;
            token = strtok(NULL, " ");
            if (token == NULL)
            {
                throw invalid_argument("Error: invalid command");
            }
            string strAmount = token;
            long double amount = stold(strAmount);
            depositAndWithdraw(false, username, ip, amount, accounts, bankruptcy);
        }
        else if ((string)token == "add_profits")
        {
            token = strtok(NULL, " ");
            if (token == NULL)
            {
                throw invalid_argument("Error: invalid command");
            }
            string username = token;
            addProfits(username, accounts);
        }
        else if ((string)token == "get_loan")
        {
            token = strtok(NULL, ":");
            string username = token;
            token = strtok(NULL, ":");
            string ip = token;
            token = strtok(NULL, " ");
            if (token == NULL)
            {
                throw invalid_argument("Error: invalid argument");
            }
            string strAmount = token;
            long double amount = stold(strAmount);
            getLoan(username, ip, amount, accounts);
        }
        else if ((string)token == "pay_loan")
        {
            token = strtok(NULL, ":");
            string username = token;
            token = strtok(NULL, " ");
            if (token == NULL)
            {
                throw invalid_argument("Error: invalid command");
            }
            string strAmount = token;
            long double amount = stold(strAmount);
            payLoan(username, amount, accounts);
        }
        else if ((string)token == "bank")
        {
            cout << "Number of accounts: " << accounts.size() << endl;
            long double balance = 0;
            long double loans = 0;
            for (size_t i = 0; i < accounts.size(); i++)
            {
                balance += accounts[i].getBalance();
                loans += accounts[i].getDebt();
            }
            cout << "accounts balances: " << balance << endl;
            cout << "loans balances: " << loans << endl;
        }
        else if ((string)token == "help")
        {
            cout << "defined commands:\n";
            cout << "opening new account    ---> create username:ip\n";
            cout << "adding new ip          ---> add_ip username:newIp\n";
            cout << "renewing account       ---> renewal username:ip\n";
            cout << "deposit money          ---> deposit username:ip:amount\n";
            cout << "withdraw money         ---> withdraw username:ip:amount\n";
            cout << "transfering money      ---> transfer senderUsername:senderIp:receiverUsername:amount\n";
            cout << "adding account profits ---> add_profits username\n";
            cout << "getting loan           ---> get_loan username:ip:amount\n";
            cout << "pay back the loan      ---> pay_loan username:ip:amount\n";
            cout << "print account info     ---> username or ip or card number\n";
            cout << "print bank info        ---> bank\n";
        }
        
        else
        {
            if (!printInfo(token, accounts))
            {
                cout << "didint get it" << endl;
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}