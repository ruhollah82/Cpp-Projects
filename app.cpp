#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <iomanip>

#define ANSI_RESET "\x1B[0m"
#define ANSI_YELLOW "\x1B[33m"
#define ANSI_RED "\x1B[31m"
#define ANSI_BLUE "\x1B[34m"
#define ANSI_GREEN "\x1B[32m"
#define ANSI_CYAN "\x1B[36m"
#define ANSI_BOLD "\x1B[1m"

using namespace std;

class User
{
private:
    int ID;
    std::string Name;
    std::string Username;
    std::string Password;
    std::string UserType;
    bool Allow;
    long long int Balance;

public:
    User() : ID(1), Name("null"), Username("null"), Password("null"), UserType("Customer"), Allow(true), Balance(0) {}
    User(int _id, std::string _name, std::string _username, std::string _password, std::string _userType, bool _allow, long long int _balance)
        : ID(_id), Name(_name), Username(_username), Password(_password), UserType(_userType), Allow(_allow), Balance(_balance) {}

    void setID(int _id);
    void setName(std::string _name);
    void setUsername(std::string _username);
    void setPassword(std::string _password);
    void setUserType(std::string _usertype);
    void setAllow(bool _allow);
    void setBalance(long long int _balance);

    int getID() const { return ID; }
    std::string getName() const { return Name; }
    std::string getUsername() const { return Username; }
    std::string getPassword() const { return Password; }
    std::string getUserType() const { return UserType; }
    long long int getBalance() const { return Balance; }
    bool getAllow() const { return Allow; }
    virtual void Menu(vector<User *> &users) = 0;
};

class Employee : public User
{
public:
    Employee() : User(0, "null", "null", "null", "Employee", true, 0) {}
    Employee(int _id, std::string _name, std::string _username, std::string _password, bool _allow, long long int _balance)
        : User(_id, _name, _username, _password, "Employee", _allow, _balance) {}

    void Menu(vector<User *> &users) override;
    void displayUsers(const vector<User *> &users);
    void displayEmployees(const vector<User *> &users);

    void AllowUser(vector<User *> &users, string status);
};

class Customer : public User
{
public:
    Customer() : User(0, "null", "null", "null", "Customer", true, 0) {}

    Customer(int _id, std::string _name, std::string _username, std::string _password, bool _allow, long long int _balance)
        : User(_id, _name, _username, _password, "Customer", _allow, _balance) {}

    void Menu(vector<User *> &users) override;

    void transfer(vector<User *> &users, int targetIndex, int Amount);
};

int searchByUN(const vector<User *> &users, const string &username);
bool matchPassword(const vector<User *> &users, int index, const string &password);
void menu();
void SignUp();
void append(User &_user);
void LogIn(vector<User *> &users);
void printHeader(const string &title);
void read(vector<User *> &users, const string &filename);

int main()
{
    menu();
}

void menu()
{
    vector<User *> _users;
    read(_users, "users.txt");

    system("clear");
    char choice;

    printHeader("MAIN MENU");
    cout << ANSI_BOLD << ANSI_CYAN << " 1. Log in\n";
    cout << " 2. Sign Up\n";
    cout << " 3. Help\n";
    cout << " 4. Exit\n"
         << ANSI_RESET;

    cout << "Enter your choice: ";
    cin >> choice;
    switch (choice)
    {
    case '1':
        LogIn(_users);
        break;
    case '2':
        SignUp();
        break;
    case '3':
        cout << "Help !\n";
        break;
    case '4':
        exit(0);

    default:
        cout << ANSI_RED << ANSI_BOLD << "Invalid choice!\n";
        sleep(2);
        menu();
        break;
    }
}

void LogIn(vector<User *> &users)
{
    string username;
    string password;
    system("clear");

    printHeader("Log In");
    cout << ANSI_BOLD << ANSI_CYAN << "Enter Username: " << ANSI_RESET;
    cin >> username;

    int index = searchByUN(users, username);

    if (index != -1)
    {
        cout << ANSI_BOLD << ANSI_CYAN << "Enter Password: " << ANSI_RESET;
        cin >> password;

        if (matchPassword(users, index, password))
        {
            cout << ANSI_BOLD << ANSI_GREEN << "Login successful!\n"
                 << ANSI_RESET;
            sleep(1);
            users[index]->Menu(users);
        }
        else
        {
            cout << ANSI_RED << ANSI_BOLD << "Incorrect password. Login failed.\n"
                 << ANSI_RESET;
            sleep(1);
            menu();
        }
    }
    else
    {
        cout << ANSI_BOLD << ANSI_RED << "User with username '" << username << "' not found.\n"
             << ANSI_RESET;
        sleep(1);
        menu();
    }
}

void SignUp()
{
    system("clear");
    printHeader("SignUP");

    string userType;
    cout << ANSI_BOLD << ANSI_CYAN << "Enter user type (Customer/Employee): " << ANSI_RESET;
    cin >> userType;

    string name, username, password;
    cout << "Enter name: ";
    cin >> name;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    User *newUser;
    if (userType == "Customer")
    {
        newUser = new Customer(0, name, username, password, true, 0);
    }
    else if (userType == "Employee")
    {
        newUser = new Employee(0, name, username, password, true, 0);
    }
    else
    {
        cout << ANSI_RED << ANSI_BOLD << "Invalid user type!\n"
             << ANSI_RESET;
        sleep(1);
        menu();
    }

    append(*newUser);
    delete newUser;
    sleep(1);
    menu();
}

void append(User &_user)
{
    ofstream file("users.txt", ios::out | ios::app);
    if (!file.is_open())
    {
        cout << ANSI_BOLD << ANSI_RED "Cannot open the file for saving!\n"
             << ANSI_RESET;
        exit(0);
    }

    file << _user.getID() << " " << _user.getName() << " " << _user.getUsername() << " " << _user.getPassword() << " " << _user.getUserType() << " " << _user.getAllow() << " " << _user.getBalance() << endl;

    file.close();
    cout << ANSI_BOLD << ANSI_GREEN << "User saved!\n"
         << ANSI_RESET;
}

void save(vector<User *> &users)
{
    ofstream file("users.txt", ios::out | ios::trunc);
    if (!file.is_open())
    {
        std::cout << ANSI_RED << ANSI_BOLD << "Cannot open the file for saving!\n"
                  << ANSI_RESET;
        exit(0);
    }

    for (const User *user : users)
    {
        file << user->getID() << " " << user->getName() << " " << user->getUsername() << " " << user->getPassword() << " " << user->getUserType() << " " << user->getAllow() << " " << user->getBalance() << endl;
    }

    file.close();
}

void read(vector<User *> &users, const string &filename = "users.txt")
{
    ifstream file(filename, ios::in);
    if (!file.is_open())
    {
        cout << ANSI_BOLD << ANSI_RED << "Cannot open the file for reading!\n"
             << ANSI_RESET;
        exit(0);
    }

    users.clear();
    int id;
    string name, username, password, userType;
    bool allow;
    long long int balance;

    while (file >> id >> name >> username >> password >> userType >> allow >> balance)
    {
        User *temp;
        if (userType == "Customer")
        {
            temp = new Customer(id, name, username, password, allow, balance);
        }
        else if (userType == "Employee")
        {
            temp = new Employee(id, name, username, password, allow, balance);
        }
        else
        {
            cout << ANSI_BOLD << ANSI_RED << "Invalid user type in the file!\n"
                 << ANSI_RESET;
            exit(1);
        }

        users.push_back(temp);
    }

    file.close();
}

void Employee::displayUsers(const vector<User *> &users)
{
    system("clear");
    printHeader("Users List");

    for (int i = 0; i < users.size(); i++)
    {
        if (users[i]->getUserType() == "Employee")
        {
            continue;
        }
        else
        {
            string name = users[i]->getName();
            int namePadding = (40 - name.length()) / 2;
            cout << ANSI_BOLD << ANSI_CYAN << setfill('-') << setw(namePadding) << "" << name << setw(namePadding) << setfill('-') << "" << ANSI_RESET << endl;
        }

        cout << ANSI_CYAN << "| Name: " << users[i]->getName() << " | Username: " << users[i]->getUsername() << (users[i]->getAllow() ? " | Allowed |" : " | Blocked |") << ANSI_RESET << endl;
        cout << ANSI_BLUE << ANSI_BOLD << setw(40) << setfill('*') << "" << ANSI_RESET << endl;
    }
}

void Employee::displayEmployees(const vector<User *> &users)
{
    system("clear");
    printHeader("Users List");

    for (int i = 0; i < users.size(); i++)
    {
        if (users[i]->getUserType() == "Customer")
        {
            continue;
        }
        else
        {
            string name = users[i]->getName();
            int namePadding = (40 - name.length()) / 2;
            cout << ANSI_BOLD << ANSI_CYAN << setfill('-') << setw(namePadding) << "" << name << setw(namePadding) << setfill('-') << "" << ANSI_RESET << endl;
        }

        cout << ANSI_CYAN << "| Name: " << users[i]->getName() << " | Username: " << users[i]->getUsername() << " | password: " << users[i]->getPassword() << ANSI_RESET << endl;
        cout << ANSI_BLUE << ANSI_BOLD << setw(40) << setfill('*') << "" << ANSI_RESET << endl;
    }
}

void Employee::AllowUser(vector<User *> &users, string status)
{
    system("clear");
    printHeader(status + " user");
    cout << ANSI_CYAN << ANSI_BOLD << "Enter cutomer username : " << ANSI_RESET;
    string targetuser;
    cin >> targetuser;
    int index = searchByUN(users, targetuser);

    if (index < 0)
    {
        cout << ANSI_BOLD << ANSI_RED << "Customer not found!" << ANSI_RESET << endl;
        return;
    }
    else if (users[index]->getUserType() == "Employee")
    {
        cout << ANSI_BOLD << ANSI_RED << "You cannot block employees" << ANSI_RESET << endl;
        return;
    }
    else
    {
        if (status == "Block")
        {
            users[index]->setAllow(0);
            save(users);
            cout << ANSI_BOLD << ANSI_RED << users[index]->getName() << " Blocked !" << ANSI_RESET << endl;
            return;
        }
        else if (status == "Allow")
        {
            users[index]->setAllow(1);
            save(users);
            cout << ANSI_BOLD << ANSI_GREEN << users[index]->getName() << " Allowed !" << ANSI_RESET << endl;
            return;
        }
    }
}

int searchByUN(const vector<User *> &users, const string &username)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i]->getUsername() == username)
        {
            return i;
        }
    }

    return -1;
}

bool matchPassword(const vector<User *> &users, int index, const string &password)
{
    return users[index]->getPassword() == password;
}

void User::setID(int _id)
{
    while (_id <= 0)
    {
        cin.clear();
        cin.ignore();
        cout << ANSI_BOLD << ANSI_RED << "Try again! The ID must be a positive number: " << ANSI_RED;
        cin >> _id;
    }

    ID = _id;
}

void User::setName(std::string _name) { Name = _name; }
void User::setUsername(std::string _username) { Username = _username; }
void User::setPassword(std::string _password) { Password = _password; }
void User::setAllow(bool _allow) { Allow = _allow; }
void User::setUserType(std::string _usertype) { UserType = _usertype; }
void User::setBalance(long long int _balance)
{
    while (_balance < 0)
    {
        cin.clear();
        cin.ignore();
        cout << ANSI_BOLD << ANSI_RED << "Try again! Account balance cannot be negative: " << ANSI_BOLD;
        cin >> _balance;
    }

    Balance = _balance;
}

void printHeader(const string &title)
{
    int width = 40;      // Adjust this value based on your desired width
    char fillChar = '='; // You can customize the fill character

    cout << ANSI_BLUE << ANSI_BOLD << setw(width) << setfill(fillChar) << "" << ANSI_RESET << endl;

    // Print title with proper formatting
    int titlePadding = (width - title.length()) / 2;
    cout << setfill(' ') << setw(titlePadding) << "" << ANSI_BOLD << ANSI_BLUE << title << ANSI_RESET << endl;

    // Print bottom border
    cout << ANSI_BLUE << ANSI_BOLD << setw(width) << setfill(fillChar) << "" << ANSI_RESET << endl;
}

void Employee::Menu(vector<User *> &users)
{
    system("clear");
    printHeader("Employee Panel");
    cout << ANSI_BOLD << ANSI_CYAN << " 1. Show Customers\n";
    cout << " 2. Show Employees\n";
    cout << " 3. Block Customer\n";
    cout << " 4. Unbolck Customer\n";
    cout << " 5. Log out" << ANSI_RESET << endl;
    char choice;
    cout << "Enter your choice: ";
    cin >> choice;
    switch (choice)
    {
    case '1':
        this->displayUsers(users);
        cout << ANSI_YELLOW << ANSI_BOLD << "Press ENTER key to return " << ANSI_RESET;
        getchar();
        getchar();
        Menu(users);
        break;
    case '2':
        this->displayEmployees(users);
        cout << ANSI_YELLOW << ANSI_BOLD << "Press ENTER key to return " << ANSI_RESET;
        getchar();
        getchar();
        Menu(users);
    case '3':
        this->AllowUser(users, "Block");
        sleep(2);
        Menu(users);

        break;
    case '4':
        this->AllowUser(users, "Allow");
        sleep(2);
        Menu(users);
        break;
    case '5':
        cout << ANSI_BOLD << ANSI_GREEN << "Log out successful\n"
             << endl
             << ANSI_RESET;
        sleep(2);
        menu();
        break;
    default:
        cout << ANSI_RED << ANSI_BOLD << "Invalid choice!\n";
        sleep(2);
        this->Menu(users);

        break;
    }
}

void Customer::Menu(vector<User *> &users)
{
    system("clear");
    if (!getAllow())
    {
        printHeader("Sorry, you have been blocked !");
        cout << ANSI_YELLOW << ANSI_BOLD << "Press ENTER key to return " << ANSI_RESET;
        getchar();
        getchar();
        menu();
    }
    else
    {
        printHeader("Customer Panel");
        cout << ANSI_BOLD << ANSI_CYAN << " 1. Deposit\n";
        cout << " 2. Withdraw Money\n";
        cout << " 3. Account Balance\n";
        cout << " 4. Money Transfer\n";
        cout << " 5. Log out\n"
             << ANSI_RESET;
        char choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case '1':
        {
            system("clear");
            printHeader("Deposit");
            int money;
            cout << ANSI_CYAN << ANSI_BOLD << "Enter the amount of money you want to deposit: " << ANSI_RESET << endl;
            cin >> money;
            while (money <= 0)
            {
                cin.clear();
                cin.ignore();
                cout << ANSI_BOLD << ANSI_RED << "Try again! The amount of money must be a positive number: " << ANSI_RED << endl;
                cin >> money;
            }
            this->setBalance(this->getBalance() + money);
            save(users);
            cout << ANSI_GREEN << ANSI_BOLD << "Deposit successful!" << ANSI_RESET << endl;
            sleep(2);
            this->Menu(users);
            break;
        }
        case '2':
        {
            system("clear");
            printHeader("Withdraw");
            cout << ANSI_CYAN << ANSI_BOLD << "Enter the amount of money you want to withdraw: " << ANSI_RESET << endl;
            int money;
            cin >> money;
            while (money <= 0)
            {
                cin.clear();
                cin.ignore();
                cout << ANSI_BOLD << ANSI_RED << "Try again! The amount of money must be a positive number: " << ANSI_RED << endl;
                sleep(2);
                cin >> money;
            }
            if (money > this->getBalance())
            {
                cout << ANSI_BOLD << ANSI_RED << "Sorry, You don't have enough money !" << ANSI_RED << endl;
                sleep(2);
                this->Menu(users);
            }

            this->setBalance(this->getBalance() - money);
            save(users);
            cout << ANSI_GREEN << ANSI_BOLD << "Withdrawal successful!" << ANSI_RESET << endl;
            sleep(2);
            this->Menu(users);
            break;
        }
        case '3':
            system("clear");
            printHeader("Account Balance");
            cout << ANSI_BOLD << ANSI_YELLOW << "Your current account balance is: $" << this->getBalance() << ANSI_RESET << endl;
            sleep(2);
            this->Menu(users);
            break;
        case '4':
        {
            system("clear");
            printHeader("Money Transfer");
            string targetuser;
            cout << ANSI_CYAN << ANSI_BOLD "Enter the username of the recipient: " << ANSI_RESET << endl;
            cin >> targetuser;

            int Amount;
            cout << ANSI_CYAN << ANSI_BOLD << "Enter the amount of money to transfer(0 for cancel): " << ANSI_RESET << endl;
            cin >> Amount;
            if (Amount == 0)
            {
                cout << ANSI_YELLOW << ANSI_BOLD << "Operation canceled !" << ANSI_RESET << endl;
                sleep(2);
                Menu(users);
            }

            while (Amount < 0 || Amount > this->getBalance())
            {
                cin.clear();
                cout << ANSI_BOLD << ANSI_RED << "Try again! Invalid amount or insufficient funds. Enter a valid amount: " << ANSI_RESET;
                sleep(2);
                cin >> Amount;
                if (Amount == 0)
                {
                    cout << ANSI_YELLOW << ANSI_BOLD << "Operation canceled !" << ANSI_RESET << endl;
                    sleep(2);
                    Menu(users);
                }
            }

            int targetIndex = searchByUN(users, targetuser);

            if (targetIndex != -1 || users[targetIndex]->getUserType() != "Employee")
            {
                this->transfer(users, targetIndex, Amount);
                cout << ANSI_BOLD << ANSI_GREEN << "Money transfer successful\n"
                     << endl
                     << ANSI_RESET;
            }
            else
            {
                cout << ANSI_BOLD << ANSI_RED << "Customer with username '" << targetuser << "' not found. Money transfer failed." << ANSI_RESET << endl;
            }
            sleep(2);
            this->Menu(users);
            break;
        }
        case '5':
            cout << ANSI_BOLD << ANSI_GREEN << "Log out successful\n"
                 << endl
                 << ANSI_RESET;
            sleep(2);
            menu();
            break;
        default:
            cout << ANSI_RED << ANSI_BOLD << "Invalid choice!\n";
            sleep(2);
            this->Menu(users);
            break;
        }
    }
}
void Customer::transfer(vector<User *> &users, int targetIndex, int Amount)
{
    this->setBalance(this->getBalance() - Amount);
    users[targetIndex]->setBalance(users[targetIndex]->getBalance() + Amount);
    save(users);
}
