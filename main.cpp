#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std;

// Transaction class to store transaction details
class Transaction {
public:
    string type;        // Deposit, Withdrawal, Transfer
    double amount;
    string timestamp;
    string description;

    Transaction(string t, double a, string desc)
        : type(t), amount(a), description(desc) {
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp.pop_back(); // Remove newline
    }

    void display() const {
        cout << left << setw(15) << type
             << setw(12) << fixed << setprecision(2) << amount
             << setw(30) << description
             << setw(25) << timestamp << endl;
    }
};

// Account class
class Account {
private:
    string accountNumber;
    string accountHolder;
    double balance;
    string accountType;
    vector<Transaction> transactionHistory;

public:
    Account(string accNum, string holder, string type, double initialBalance = 0.0)
        : accountNumber(accNum), accountHolder(holder), accountType(type), balance(initialBalance) {
        if (initialBalance > 0) {
            transactionHistory.push_back(Transaction("Deposit", initialBalance, "Initial deposit"));
        }
    }

    // Getters
    string getAccountNumber() const { return accountNumber; }
    string getAccountHolder() const { return accountHolder; }
    double getBalance() const { return balance; }
    string getAccountType() const { return accountType; }

    // Display account details
    void displayAccountInfo() const {
        cout << "\n========== ACCOUNT INFORMATION ==========" << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Holder Name:    " << accountHolder << endl;
        cout << "Account Type:   " << accountType << endl;
        cout << "Current Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "========================================\n" << endl;
    }

    // Deposit money
    bool deposit(double amount, string description = "") {
        if (amount <= 0) {
            cout << "Error: Deposit amount must be positive!" << endl;
            return false;
        }
        balance += amount;
        if (description.empty()) {
            description = "Deposit";
        }
        transactionHistory.push_back(Transaction("Deposit", amount, description));
        cout << "Successfully deposited $" << fixed << setprecision(2) << amount << endl;
        return true;
    }

    // Withdraw money
    bool withdraw(double amount, string description = "") {
        if (amount <= 0) {
            cout << "Error: Withdrawal amount must be positive!" << endl;
            return false;
        }
        if (amount > balance) {
            cout << "Error: Insufficient balance! Current balance: $" << fixed << setprecision(2) << balance << endl;
            return false;
        }
        balance -= amount;
        if (description.empty()) {
            description = "Withdrawal";
        }
        transactionHistory.push_back(Transaction("Withdrawal", amount, description));
        cout << "Successfully withdrawn $" << fixed << setprecision(2) << amount << endl;
        return true;
    }

    // Transfer money to another account
    bool transfer(Account& recipient, double amount, string description = "") {
        if (amount <= 0) {
            cout << "Error: Transfer amount must be positive!" << endl;
            return false;
        }
        if (amount > balance) {
            cout << "Error: Insufficient balance!" << endl;
            return false;
        }
        
        balance -= amount;
        recipient.balance += amount;
        
        if (description.empty()) {
            description = "Transfer to " + recipient.getAccountHolder();
        }
        
        transactionHistory.push_back(Transaction("Transfer", amount, description));
        recipient.transactionHistory.push_back(
            Transaction("Transfer", amount, "Transfer from " + accountHolder)
        );
        
        cout << "Successfully transferred $" << fixed << setprecision(2) << amount 
             << " to " << recipient.getAccountHolder() << endl;
        return true;
    }

    // Display transaction history
    void displayTransactionHistory() const {
        if (transactionHistory.empty()) {
            cout << "\nNo transactions yet." << endl;
            return;
        }
        
        cout << "\n========== TRANSACTION HISTORY ==========" << endl;
        cout << left << setw(15) << "Type"
             << setw(12) << "Amount"
             << setw(30) << "Description"
             << setw(25) << "Timestamp" << endl;
        cout << "==========================================" << endl;
        
        for (const auto& transaction : transactionHistory) {
            transaction.display();
        }
        cout << "==========================================" << endl;
    }

    // Get balance
    double checkBalance() const {
        return balance;
    }
};

// Bank class to manage accounts
class Bank {
private:
    map<string, Account> accounts;
    int accountCounter = 1000;

public:
    // Create new account
    bool createAccount(string holderName, string accountType, double initialBalance = 0.0) {
        accountCounter++;
        string accountNum = "ACC" + to_string(accountCounter);
        
        if (accounts.find(accountNum) != accounts.end()) {
            cout << "Error: Account already exists!" << endl;
            return false;
        }
        
        accounts[accountNum] = Account(accountNum, holderName, accountType, initialBalance);
        cout << "Account created successfully!" << endl;
        cout << "Account Number: " << accountNum << endl;
        cout << "Account Holder: " << holderName << endl;
        return true;
    }

    // Find account by number
    Account* findAccount(string accountNum) {
        auto it = accounts.find(accountNum);
        if (it != accounts.end()) {
            return &it->second;
        }
        return nullptr;
    }

    // Display all accounts
    void displayAllAccounts() const {
        if (accounts.empty()) {
            cout << "\nNo accounts available." << endl;
            return;
        }
        
        cout << "\n========== ALL ACCOUNTS ==========" << endl;
        cout << left << setw(12) << "Account #"
             << setw(20) << "Holder Name"
             << setw(12) << "Type"
             << setw(15) << "Balance" << endl;
        cout << "===================================" << endl;
        
        for (const auto& pair : accounts) {
            cout << left << setw(12) << pair.first
                 << setw(20) << pair.second.getAccountHolder()
                 << setw(12) << pair.second.getAccountType()
                 << "$" << fixed << setprecision(2) << pair.second.getBalance() << endl;
        }
        cout << "===================================" << endl;
    }

    // Delete account
    bool deleteAccount(string accountNum) {
        if (accounts.find(accountNum) != accounts.end()) {
            accounts.erase(accountNum);
            cout << "Account deleted successfully." << endl;
            return true;
        }
        cout << "Error: Account not found!" << endl;
        return false;
    }
};

// Main menu function
void displayMenu() {
    cout << "\n========== BANK MANAGEMENT SYSTEM ==========" << endl;
    cout << "1. Create Account" << endl;
    cout << "2. View Account Information" << endl;
    cout << "3. Deposit Money" << endl;
    cout << "4. Withdraw Money" << endl;
    cout << "5. Transfer Money" << endl;
    cout << "6. View Transaction History" << endl;
    cout << "7. View All Accounts" << endl;
    cout << "8. Delete Account" << endl;
    cout << "9. Exit" << endl;
    cout << "===========================================" << endl;
}

int main() {
    Bank bank;
    int choice;
    
    cout << "========== WELCOME TO BANK MANAGEMENT SYSTEM ==========" << endl;

    while (true) {
        displayMenu();
        cout << "Enter your choice (1-9): ";
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice) {
            case 1: {
                // Create Account
                string name, type;
                double initialBalance;
                
                cout << "\nEnter account holder name: ";
                getline(cin, name);
                cout << "Enter account type (Savings/Checking/Business): ";
                getline(cin, type);
                cout << "Enter initial balance (press 0 for no initial deposit): ";
                cin >> initialBalance;
                cin.ignore();
                
                bank.createAccount(name, type, initialBalance);
                break;
            }

            case 2: {
                // View Account Information
                string accountNum;
                cout << "\nEnter account number: ";
                getline(cin, accountNum);
                
                Account* acc = bank.findAccount(accountNum);
                if (acc != nullptr) {
                    acc->displayAccountInfo();
                } else {
                    cout << "Error: Account not found!" << endl;
                }
                break;
            }

            case 3: {
                // Deposit Money
                string accountNum;
                double amount;
                
                cout << "\nEnter account number: ";
                getline(cin, accountNum);
                cout << "Enter deposit amount: $";
                cin >> amount;
                cin.ignore();
                
                Account* acc = bank.findAccount(accountNum);
                if (acc != nullptr) {
                    acc->deposit(amount);
                } else {
                    cout << "Error: Account not found!" << endl;
                }
                break;
            }

            case 4: {
                // Withdraw Money
                string accountNum;
                double amount;
                
                cout << "\nEnter account number: ";
                getline(cin, accountNum);
                cout << "Enter withdrawal amount: $";
                cin >> amount;
                cin.ignore();
                
                Account* acc = bank.findAccount(accountNum);
                if (acc != nullptr) {
                    acc->withdraw(amount);
                } else {
                    cout << "Error: Account not found!" << endl;
                }
                break;
            }

            case 5: {
                // Transfer Money
                string fromAccNum, toAccNum;
                double amount;
                
                cout << "\nEnter source account number: ";
                getline(cin, fromAccNum);
                cout << "Enter destination account number: ";
                getline(cin, toAccNum);
                cout << "Enter transfer amount: $";
                cin >> amount;
                cin.ignore();
                
                Account* fromAcc = bank.findAccount(fromAccNum);
                Account* toAcc = bank.findAccount(toAccNum);
                
                if (fromAcc != nullptr && toAcc != nullptr) {
                    fromAcc->transfer(*toAcc, amount);
                } else {
                    cout << "Error: One or both accounts not found!" << endl;
                }
                break;
            }

            case 6: {
                // View Transaction History
                string accountNum;
                cout << "\nEnter account number: ";
                getline(cin, accountNum);
                
                Account* acc = bank.findAccount(accountNum);
                if (acc != nullptr) {
                    acc->displayTransactionHistory();
                } else {
                    cout << "Error: Account not found!" << endl;
                }
                break;
            }

            case 7: {
                // View All Accounts
                bank.displayAllAccounts();
                break;
            }

            case 8: {
                // Delete Account
                string accountNum;
                cout << "\nEnter account number to delete: ";
                getline(cin, accountNum);
                bank.deleteAccount(accountNum);
                break;
            }

            case 9: {
                // Exit
                cout << "\nThank you for using Bank Management System. Goodbye!" << endl;
                return 0;
            }

            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}
