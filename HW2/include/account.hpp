#ifndef ACC
#define ACC

struct transaction
{
    bool deposit = false;
    bool withdraw = false;
    bool profits = false;
    std::string origin;
    std::string destination;
    long double amount;
    std::time_t date;
    long double remained;
};

class account
{
public:
    account(std::string, std::string, unsigned int);            
    ~account() {}                                               

    std::string getUsername() const;

    void setIP(std::string);
    std::vector<std::string> getIP() const;
    void printIP();
    
    unsigned int getCardNumber() const;

    void changeBalance(long double);
    long double getBalance() const;
        
    std::time_t getOpeningDate() const;

    void setExpDate();
    std::time_t getExpDate() const;

    void addTransaction(transaction);
    std::vector<transaction> getTransaction() const;

    void changeDebt(long double);
    long double getDebt() const;

private:
    std::string userName;
    void setUsername(std::string);

    std::vector<std::string> ip;

    unsigned int cardNumber;
    void setCardNumber(unsigned int);

    long double balance = 0;

    std::time_t openingDate;
    void setOpeningDate();

    std::time_t expDate;
    
    std::vector<transaction> transactions;

    long double debt = 0;
};

#endif 
