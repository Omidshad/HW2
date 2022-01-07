#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include "account.hpp"

using namespace std;

account::account(string userName, string ip, unsigned int cardNumber)
{
    setUsername(userName);
    setIP(ip);
    setCardNumber(cardNumber);

    setOpeningDate();
    setExpDate();
}

void account::setUsername(string userName)
{
    this->userName = userName;
}

string account::getUsername() const
{
    return userName;
}

void account::setIP(string newIP)
{
    ip.push_back(newIP);
}

std::vector<std::string> account::getIP() const
{
    return ip;
}

void account::printIP()
{
    for (size_t i = 0; i < ip.size(); i++)
    {
        cout << ip[i] << endl;
    }
}

void account::setCardNumber(unsigned int cardNumber)
{
    this->cardNumber = cardNumber;
}

unsigned int account::getCardNumber() const
{
    return cardNumber;
}

void account::changeBalance(long double money)
{
    balance += money;    
}

long double account::getBalance() const
{
    return balance;
}

void account::setOpeningDate()
{
    this->openingDate = time(0);
}

time_t account::getOpeningDate() const
{
    return openingDate;
}

void account::setExpDate()
{
    expDate = time(0) + 63072000;                  
}

time_t account::getExpDate() const
{
    return expDate;
}

void account::addTransaction(transaction temp)
{
    transactions.push_back(temp);
}

vector<transaction> account::getTransaction() const
{
    return transactions;
}

void account::changeDebt(long double amount)
{
    this->debt += amount;
}

long double account::getDebt() const 
{  
    return debt;
}
