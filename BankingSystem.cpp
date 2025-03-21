#include<iostream>
#include<vector>
using namespace std;

class BankAccount {
    private:
        int account_num;
        string account_holder;
        double balance;

    protected:
        string accountType; // To store the type of account

    public:
        int getAccountNum() {
            return account_num;
        }
        void setAccountNum(int num) {
            account_num = num;
        }

        string getAccountHolder() {
            return account_holder;
        }
        void setAccountHolder(string holder) {
            account_holder = holder;
        }

        double getBalance(){
            return balance;
        }
        void setBalance(double bal) {
            balance = bal;
        }

        string getAccountType() const { // Getter for accountType
            return accountType;
        }

        virtual void deposit(double amount) {}
        virtual void withdraw(double amount) {}
        virtual double calculate_interest() {
            return 0.0;
        }
        virtual void displayAccount() {
            cout << "Account holder: " << account_holder << endl;
            cout << "Account number: " << account_num << endl;
            cout << "Balance: " << balance << endl;
            cout << "Account type: " << accountType << endl; // Display account type
        }
        virtual void addAccountData() {
            cout << "Enter account number: ";
            cin >> account_num;
            cout << "Enter account holder name: ";
            cin>>account_holder;
            cout << "Enter initial balance: ";
            cin >> balance;
        }
        virtual ~BankAccount() {}
};

class SavingsAccount : public BankAccount {
    private:
        double interest_rate;

    public:
        SavingsAccount() {
            accountType = "Savings Account";
        }

        double calculate_interest() override {
            int days;
            cout << "Enter the number of days to calculate interest: ";
            cin >> days;
            interest_rate = (getBalance() * 0.04 * days) / 365; // 4% annual interest
            setBalance(getBalance() + interest_rate);
            cout << "Interest for " << days << " days: " << interest_rate << endl;
            cout << "New balance: " << getBalance() << endl;
            return interest_rate;
        }

        void withdraw(double amount) override {
            if (getBalance() < amount) {
                cout << "Insufficient balance in Savings Account." << endl;
            } else {
                setBalance(getBalance() - amount);
                cout << "Withdrawal successful. New balance: " << getBalance() << endl;
            }
        }

        void deposit(double amount) override {
            setBalance(getBalance() + amount);
            cout << "Deposit successful. New balance: " << getBalance() << endl;
        }
};

class CheckingAccount : public BankAccount {
    private:
        double overDraft_limit = 100000;

    public:
        CheckingAccount() {
            accountType = "Checking Account";
        }

        void withdraw(double amount) override {
            if (getBalance() + overDraft_limit < amount) {
                cout << "Withdrawal denied: Exceeds overdraft limit." << endl;
            } else {
                setBalance(getBalance() - amount);
                cout << "Withdrawal successful. New balance: " << getBalance() << endl;
            }
        }

        void deposit(double amount) override {
            setBalance(getBalance() + amount);
            cout << "Deposit successful. New balance: " << getBalance() << endl;
        }
};

class FixedDepositAccount : public BankAccount {
    private:
        int term; // Term in months
        double interest_rate;

    public:
        FixedDepositAccount() {
            accountType = "Fixed Deposit Account";
        }

        double calculate_interest() override {
            cout << "Enter the term (in months) for the fixed deposit: ";
            cin >> term;
            interest_rate = (getBalance() * 0.05 * term) / 12; // 5% annual interest
            setBalance(getBalance() + interest_rate);
            cout << "Interest for " << term << " months: " << interest_rate << endl;
            cout << "New balance: " << getBalance() << endl;
            return interest_rate;
        }

        void withdraw(double amount) override {
            cout << "Withdrawals are not allowed from Fixed Deposit Account before maturity." << endl;
        }

        void deposit(double amount) override {
            cout << "Deposits are not allowed in Fixed Deposit Account after creation." << endl;
        }
};

int main() {
    vector<BankAccount*> accounts; // Vector to store multiple accounts
    int choice;
    int accountType;


    do {
        cout << "\n=========================================\n";
        cout << "           BANKING SYSTEM MENU           \n";
        cout << "=========================================\n";
        cout << "1. Add New Account\n";
        cout << "2. Display All Accounts\n";
        cout << "3. Deposit Money\n";
        cout << "4. Withdraw Money\n";
        cout << "5. Calculate Interest\n";
        cout << "6. Exit\n";
        cout << "=========================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: { // Add new account
                cout << "Select Account Type:\n";
                cout << "1. Savings Account\n";
                cout << "2. Checking Account\n";
                cout << "3. Fixed Deposit Account\n";
                cout << "Enter your choice: ";
                cin >> accountType;

                BankAccount* account = nullptr;
                if (accountType == 1) {
                    account = new SavingsAccount();
                } else if (accountType == 2) {
                    account = new CheckingAccount();
                } else if (accountType == 3) {
                    account = new FixedDepositAccount();
                } else {
                    cout << "Invalid account type selected.\n";
                    break;
                }

                account->addAccountData();
                accounts.push_back(account); // Add the account to the vector
                cout << "\nAccount added successfully!\n";
                break;
            }

            case 2: { // Display all accounts
                if (accounts.empty()) {
                    cout << "\nNo accounts available.\n";
                } else {
                    cout << "\nDisplaying all accounts:\n";
                    for (size_t i = 0; i < accounts.size(); ++i) {
                        cout << "\nAccount " << i + 1 << ":\n";
                        accounts[i]->displayAccount();
                    }
                }
                break;
            }

            case 3: { // Deposit money
                int accountNumber;
                double amount;
                cout << "\nEnter account number to deposit into: ";
                cin >> accountNumber;

                bool accountFound = false;
                for (BankAccount* account : accounts) {
                    if (account->getAccountNum() == accountNumber) {
                        cout << "Enter amount to deposit: ";
                        cin >> amount;
                        account->deposit(amount);
                        accountFound = true;
                        break;
                    }
                }

                if (!accountFound) {
                    cout << "Account number not found.\n";
                }
                break;
            }

            case 4: { // Withdraw money
                int accountNumber;
                double amount;
                cout << "\nEnter account number to withdraw from: ";
                cin >> accountNumber;

                bool accountFound = false;
                for (BankAccount* account : accounts) {
                    if (account->getAccountNum() == accountNumber) {
                        cout << "Enter amount to withdraw: ";
                        cin >> amount;
                        account->withdraw(amount);
                        accountFound = true;
                        break;
                    }
                }

                if (!accountFound) {
                    cout << "Account number not found.\n";
                }
                break;
            }

            case 5: { // Calculate interest
                int accountNumber;
                cout << "\nEnter account number to calculate interest: ";
                cin >> accountNumber;

                for (BankAccount* account : accounts) {
                    if (account->getAccountNum() == accountNumber) {
                        account->calculate_interest();
                        break;
                    }
                }
                break;
            }

            case 6:
                cout << "\nExiting the program. Thank you for using the Banking System!\n";
                break;

            default:
                cout << "\nInvalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}