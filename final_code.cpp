#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <unordered_map>
#include <numeric>
#include<iomanip>
#include <conio.h> // For getch()

using namespace std;

// Travel struct to hold travel details
struct Travel {
    string username;
    string date;
    string destination;
    double distance;
    string notes;
};

// A vector to hold travel data
vector<Travel> travels;

// User struct to hold user details
struct User {
    string username;
    string password;
    string email;
    string phone;
    string fullName;
    string lastLogin; // Last login date in YYYY-MM-DD format
};

// Declare a global vector to hold registered users
vector<User> users;
string loggedInUsername;

// Function prototypes
void loadUsersFromFile();
void saveUsersToFile();
int findUserIndex(const string& username);
void displayUserDetails();
bool deleteUserAccount();
void registerUser();
bool loginUser();
void updateUserDetails();
void addSchedule();
void updateSchedule();
void deleteSchedule();
void viewSchedules();
void searchScheduleByDate();
void addExpense();
void updateExpense();
void deleteExpense();
void viewExpenses();
void generateExpenseReport();
bool adminLogin();
void viewAllUsers();
void deleteInactiveUsers();
void backupData();
void restoreData();
void sortSchedulesByDate();
void sortExpensesByCategory();
void searchExpenseByKeyword();
void findMaxExpense();
void findMinExpense();
void generateBudgetSummary();
void showMonthlyExpenseGraph();
void compareExpensesByCategory();
void generateTravelStatistics();
void notifyOverBudget();





void loadUsersFromFile() {
    users.clear(); // Clear any existing users
    ifstream file("users.txt");

    if (file.is_open()) {
        string username,password, fullName, email, phone;
        while (file >> username >> fullName >> email >> phone) {
            User newUser = {username, fullName, email, phone};
            users.push_back(newUser);
        }
        file.close();
    } else {
        cout << "Unable to open file." << endl;
    }
}


void saveUsersToFile() {
    ofstream file("users.txt");

    if (file.is_open()) {
        for (const auto& user : users) {
            file << user.username<<" " <<user.password<< " " << user.fullName << " "
                 << user.email << " " << user.phone << endl;
        }
        file.close();
    } else {
        cout << "Unable to open file for saving." << endl;
    }
}


// Function to find the index of the user
int findUserIndex(const string& username) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i].username == username) {
            return i;
        }
    }
    return -1; // User not found
}

string getPasswordInput() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // '\r' is the carriage return character (Enter key)
        if (ch == '\b') { // '\b' is the backspace character
            if (!password.empty()) {
                cout << "\b \b"; // Erase the last character from console
                password.pop_back();
            }
        } else {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

void registerUser() {
    User newUser;
    string input;
    regex emailRegex(R"((\w+)(\.{1}\w+)*@(\w+)(\.\w+)+)");
    regex phoneRegex(R"(\d{10})");

    cout << "Register a New User" << endl;

    // Get username
    while (true) {
        cout << "Enter username: ";
        cin >> input;
        bool usernameExists = false;
        for (const auto& user : users) {
            if (user.username == input) {
                usernameExists = true;
                break;
            }
        }
        if (usernameExists) {
            cout << "Username already exists. Please choose another." << endl;
        } else {
            newUser.username = input;
            break;
        }
    }

    // Get password
    while (true) {
        cout << "Enter password: ";
        input = getPasswordInput();
        if (input.length() >= 6) {
            newUser.password = input;
            break;
        } else {
            cout << "Password must be at least 6 characters long." << endl;
        }
    }

    // Confirm password
    while (true) {
        cout << "Confirm password: ";
        input = getPasswordInput();
        if (input == newUser.password) {
            break;
        } else {
            cout << "Passwords do not match. Please try again." << endl;
        }
    }

    // Get email
    while (true) {
        cout << "Enter email: ";
        cin >> input;
        if (regex_match(input, emailRegex)) {
            newUser.email = input;
            break;
        } else {
            cout << "Invalid email format. Please enter a valid email." << endl;
        }
    }

    // Get phone number
    while (true) {
        cout << "Enter phone number (10 digits): ";
        cin >> input;
        if (regex_match(input, phoneRegex)) {
            newUser.phone = input;
            break;
        } else {
            cout << "Invalid phone number. Please enter a valid 10-digit phone number." << endl;
        }
    }

    // Get full name
    cout << "Enter full name: ";
    cin.ignore();
    getline(cin, input);
    newUser.fullName = input;

    // Add the new user to the users vector and save to file
    users.push_back(newUser);
    saveUsersToFile();

    cout << "User registered successfully!" << endl;
}


bool loginUser() {
    loadUsersFromFile();
    string username, password;

    cout << "Login User" << endl;

    // Prompt user for username
    cout << "Enter username: ";
    cin >> username;

    // Prompt user for password
    cout << "Enter password: ";
    password = getPasswordInput();

    // Check if the entered credentials match any registered user
    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            cout << "Login successful!" << endl;
            return true;
        }
    }

    // If no match is found, login fails
    cout << "Invalid username or password. Please try again." << endl;
    return false;
}



extern vector<User> users;


void updateUserDetails() {
    string username;
    cout << "Enter your username to update details: ";
    cin >> username;

    int userIndex = findUserIndex(username);
    if (userIndex == -1) {
        cout << "User not found. Please check your username." << endl;
        return;
    }

    User& user = users[userIndex];
    string input;
    regex emailRegex(R"((\w+)(\.{1}\w+)*@(\w+)(\.\w+)+)");
    regex phoneRegex(R"(\d{10})");

    cout << "Update User Details" << endl;

    while (true) {
        int choice;
        cout << "Select the detail you want to update:" << endl;
        cout << "1. Email" << endl;
        cout << "2. Phone Number" << endl;
        cout << "3. Full Name" << endl;
        cout << "4. Password" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                // Update email
                while (true) {
                    cout << "Enter new email (leave blank to keep current): ";
                    cin.ignore();
                    getline(cin, input);
                    if (input.empty()) {
                        break; // Keep current email
                    }
                    if (regex_match(input, emailRegex)) {
                        user.email = input;
                        cout << "Email updated successfully!" << endl;
                        break;
                    } else {
                        cout << "Invalid email format. Please enter a valid email." << endl;
                    }
                }
                break;
            case 2:
                // Update phone number
                while (true) {
                    cout << "Enter new phone number (10 digits, leave blank to keep current): ";
                    cin.ignore();
                    getline(cin, input);
                    if (input.empty()) {
                        break; // Keep current phone number
                    }
                    if (regex_match(input, phoneRegex)) {
                        user.phone = input;
                        cout << "Phone number updated successfully!" << endl;
                        break;
                    } else {
                        cout << "Invalid phone number. Please enter a valid 10-digit phone number." << endl;
                    }
                }
                break;
            case 3:
                // Update full name
                cout << "Enter new full name (leave blank to keep current): ";
                cin.ignore();
                getline(cin, input);
                if (!input.empty()) {
                    user.fullName = input;
                    cout << "Full name updated successfully!" << endl;
                }
                break;
            case 4:
                // Update password
                while (true) {
                    cout << "Enter new password (leave blank to keep current): ";
                    cin.ignore();
                    getline(cin, input);
                    if (input.empty()) {
                        break; // Keep current password
                    }
                    if (input.length() >= 6) {
                        user.password = input;
                        cout << "Password updated successfully!" << endl;
                        break;
                    } else {
                        cout << "Password must be at least 6 characters long." << endl;
                    }
                }
                break;
            case 5:
                // Exit
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}



// Function to display user details
void displayUserDetails() {
    loadUsersFromFile(); // Ensure the latest data is loaded from the file

    string username;
    cout << "Enter your username to display details: ";
    cin >> username;

    int userIndex = findUserIndex(username);
    if (userIndex == -1) {
        cout << "User not found. Please check your username." << endl;
        return;
    }

    User user = users[userIndex];

    cout << "User Details:" << endl;
    cout << "Username: " << user.username << endl;
    cout << "Full Name: " << user.fullName << endl;
    cout << "Email: " << user.email << endl;
    cout << "Phone: " << user.phone << endl;
}

bool deleteUserAccount() {
    loadUsersFromFile(); // Ensure the latest data is loaded from the file

    string username, password;
    cout << "Enter your username to delete your account: ";
    cin >> username;

    int userIndex = findUserIndex(username);
    if (userIndex == -1) {
        cout << "User not found. Please check your username." << endl;
        return false;
    }

    cout << "Enter your password: ";
    cin >> password;

    if (users[userIndex].password != password) {
        cout << "Incorrect password. Account deletion failed." << endl;
        return false;
    }

    // Confirm deletion
    char confirm;
    cout << "Are you sure you want to delete your account? This action cannot be undone. (y/n): ";
    cin >> confirm;
    if (confirm != 'y' && confirm != 'Y') {
        cout << "Account deletion canceled." << endl;
        return false;
    }

    // Remove user from vector
    users.erase(users.begin() + userIndex);
    saveUsersToFile(); // Update the file with the remaining users

    cout << "Account deleted successfully!" << endl;
    return true;
}

struct Schedule {
    string username;  // To associate the schedule with the user
    string date;
    string time;
    string description;
    string location;
};

// A vector to hold schedules
vector<Schedule> schedules;

// Function to save schedules to a file
void saveSchedulesToFile() {
    ofstream outFile("schedules.txt");
    for (const auto& schedule : schedules) {
        outFile << schedule.username << ' ' << schedule.date << ' ' << schedule.time << ' ' << schedule.description << '|' << schedule.location << '\n';
    }
    outFile.close();
}

// Function to load schedules from a file
void loadSchedulesFromFile() {
    ifstream inFile("schedules.txt");
    if (!inFile) {
        return; // If the file does not exist, return
    }

    Schedule schedule;
    while (inFile >> schedule.username >> schedule.date >> schedule.time) {
        inFile.ignore(); // Ignore the whitespace before reading description and location
        getline(inFile, schedule.description, '|');
        getline(inFile, schedule.location);
        if (schedule.username == loggedInUsername) {
            schedules.push_back(schedule);
        }
    }
    inFile.close();
}

void addSchedule() {
    loadSchedulesFromFile(); // Ensure the latest data is loaded from the file

    Schedule newSchedule;
    newSchedule.username = loggedInUsername; // Associate the schedule with the logged-in user
    string input;
    regex dateRegex(R"(\d{4}-\d{2}-\d{2})"); // Simple date format YYYY-MM-DD
    regex timeRegex(R"(\d{2}:\d{2})"); // Simple time format HH:MM

    cout << "Add a New Schedule" << endl;

    // Get date
    while (true) {
        cout << "Enter date (YYYY-MM-DD): ";
        cin >> input;
        if (regex_match(input, dateRegex)) {
            newSchedule.date = input;
            break;
        } else {
            cout << "Invalid date format. Please enter a valid date." << endl;
        }
    }

    // Get time
    while (true) {
        cout << "Enter time (HH:MM): ";
        cin >> input;
        if (regex_match(input, timeRegex)) {
            newSchedule.time = input;
            break;
        } else {
            cout << "Invalid time format. Please enter a valid time." << endl;
        }
    }

    // Get description
    cout << "Enter description: ";
    cin.ignore(); // Clear the newline character from the input buffer
    getline(cin, input);
    newSchedule.description = input;

    // Get location
    cout << "Enter location: ";
    getline(cin, input);
    newSchedule.location = input;

    // Add the new schedule to the schedules vector and save to file
    schedules.push_back(newSchedule);
    saveSchedulesToFile();
    cout << "Schedule added successfully!" << endl;
}

void updateSchedule() {
    loadSchedulesFromFile(); // Ensure the latest data is loaded from the file

    string date, time;
    cout << "Enter the date (YYYY-MM-DD) of the schedule you want to update: ";
    cin >> date;
    cout << "Enter the time (HH:MM) of the schedule you want to update: ";
    cin >> time;

    // Find the schedule to update
    bool scheduleFound = false;
    for (auto& schedule : schedules) {
        if (schedule.username == loggedInUsername && schedule.date == date && schedule.time == time) {
            scheduleFound = true;

            string input;
            regex dateRegex(R"(\d{4}-\d{2}-\d{2})"); // Simple date format YYYY-MM-DD
            regex timeRegex(R"(\d{2}:\d{2})"); // Simple time format HH:MM

            cout << "Updating Schedule for " << date << " at " << time << ":\n";

            // Update date
            while (true) {
                cout << "Enter new date (YYYY-MM-DD, leave blank to keep current): ";
                cin.ignore();
                getline(cin, input);
                if (input.empty()) {
                    break; // Keep current date
                }
                if (regex_match(input, dateRegex)) {
                    schedule.date = input;
                    break;
                } else {
                    cout << "Invalid date format. Please enter a valid date." << endl;
                }
            }

            // Update time
            while (true) {
                cout << "Enter new time (HH:MM, leave blank to keep current): ";
                getline(cin, input);
                if (input.empty()) {
                    break; // Keep current time
                }
                if (regex_match(input, timeRegex)) {
                    schedule.time = input;
                    break;
                } else {
                    cout << "Invalid time format. Please enter a valid time." << endl;
                }
            }

            // Update description
            cout << "Enter new description (leave blank to keep current): ";
            getline(cin, input);
            if (!input.empty()) {
                schedule.description = input;
            }

            // Update location
            cout << "Enter new location (leave blank to keep current): ";
            getline(cin, input);
            if (!input.empty()) {
                schedule.location = input;
            }

            cout << "Schedule updated successfully!" << endl;
            saveSchedulesToFile(); // Save the updated schedules to the file
            return;
        }
    }

    if (!scheduleFound) {
        cout << "Schedule not found. Please check the date and time." << endl;
    }
}

void deleteSchedule() {
    loadSchedulesFromFile(); // Ensure the latest data is loaded from the file

    string date, time;
    cout << "Enter the date (YYYY-MM-DD) of the schedule you want to delete: ";
    cin >> date;
    cout << "Enter the time (HH:MM) of the schedule you want to delete: ";
    cin >> time;

    // Find the schedule to delete
    bool scheduleFound = false;
    for (auto it = schedules.begin(); it != schedules.end(); ++it) {
        if (it->username == loggedInUsername && it->date == date && it->time == time) {
            scheduleFound = true;
            // Confirm deletion
            char confirm;
            cout << "Are you sure you want to delete this schedule? (y/n): ";
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                // Delete the schedule
                schedules.erase(it);
                saveSchedulesToFile(); // Save the updated schedules to the file
                cout << "Schedule deleted successfully!" << endl;
            } else {
                cout << "Schedule deletion canceled." << endl;
            }
            return;
        }
    }

    if (!scheduleFound) {
        cout << "Schedule not found. Please check the date and time." << endl;
    }
}

void viewSchedules() {
    loadSchedulesFromFile(); // Ensure the latest data is loaded from the file

    cout << "Your Schedules:\n";
    cout << left << setw(12) << "Date" << setw(8) << "Time" << setw(30) << "Description" << setw(20) << "Location" << endl;
    cout << string(70, '-') << endl;

    bool hasSchedules = false;
    for (const auto& schedule : schedules) {
        if (schedule.username == loggedInUsername) {
            cout << left << setw(12) << schedule.date
                 << setw(8) << schedule.time
                 << setw(30) << schedule.description
                 << setw(20) << schedule.location << endl;
            hasSchedules = true;
        }
    }

    if (!hasSchedules) {
        cout << "No schedules found for the logged-in user." << endl;
    }
}

void searchScheduleByDate() {
    loadSchedulesFromFile(); // Ensure the latest data is loaded from the file

    string date;
    regex dateRegex(R"(\d{4}-\d{2}-\d{2})"); // Simple date format YYYY-MM-DD

    // Get date input
    while (true) {
        cout << "Enter the date to search schedules (YYYY-MM-DD): ";
        cin >> date;
        if (regex_match(date, dateRegex)) {
            break;
        } else {
            cout << "Invalid date format. Please enter a valid date." << endl;
        }
    }

    // Print header
    cout << "Schedules on " << date << ":\n";
    cout << left << setw(8) << "Time" << setw(30) << "Description" << setw(20) << "Location" << endl;
    cout << string(58, '-') << endl;

    bool hasSchedules = false;
    for (const auto& schedule : schedules) {
        if (schedule.username == loggedInUsername && schedule.date == date) {
            cout << left << setw(8) << schedule.time
                 << setw(30) << schedule.description
                 << setw(20) << schedule.location << endl;
            hasSchedules = true;
        }
    }

    if (!hasSchedules) {
        cout << "No schedules found for the specified date." << endl;
    }
}

// Expense struct to hold expense details
struct Expense {
    string username; // To associate the expense with the user
    string date;
    string category;
    double amount;
    string description;
};

// A vector to hold expenses
vector<Expense> expenses;

// Function to save expenses to a file
void saveExpensesToFile() {
    ofstream outFile("expenses.txt");
    for (const auto& expense : expenses) {
        outFile << expense.username << ' ' << expense.date << ' ' << expense.category << ' ' << expense.amount << ' ' << expense.description << '\n';
    }
    outFile.close();
}

// Function to load expenses from a file
void loadExpensesFromFile() {
    ifstream inFile("expenses.txt");
    if (!inFile) {
        return; // If the file does not exist, return
    }

    Expense expense;
    while (inFile >> expense.username >> expense.date >> expense.category >> expense.amount) {
        inFile.ignore(); // Ignore the whitespace before reading description
        getline(inFile, expense.description);
        expenses.push_back(expense);
    }
    inFile.close();
}

void addExpense() {
    loadExpensesFromFile(); // Ensure the latest data is loaded from the file

    Expense newExpense;
    newExpense.username = loggedInUsername; // Associate the expense with the logged-in user
    string input;
    regex dateRegex(R"(\d{4}-\d{2}-\d{2})"); // Simple date format YYYY-MM-DD

    cout << "Add a New Expense" << endl;

    // Get date
    while (true) {
        cout << "Enter date (YYYY-MM-DD): ";
        cin >> input;
        if (regex_match(input, dateRegex)) {
            newExpense.date = input;
            break;
        } else {
            cout << "Invalid date format. Please enter a valid date." << endl;
        }
    }

    // Get category
    cout << "Enter category (e.g., Food, Travel, Accommodation): ";
    cin >> newExpense.category;

    // Get amount
    while (true) {
        cout << "Enter amount: ";
        cin >> input;
        try {
            newExpense.amount = stod(input);
            break;
        } catch (invalid_argument&) {
            cout << "Invalid amount format. Please enter a valid number." << endl;
        }
    }

    // Get description
    cout << "Enter description: ";
    cin.ignore(); // Clear the newline character from the input buffer
    getline(cin, input);
    newExpense.description = input;

    // Add the new expense to the expenses vector and save to file
    expenses.push_back(newExpense);
    saveExpensesToFile();

    cout << "Expense added successfully!" << endl;
}

void updateExpense() {
    loadExpensesFromFile(); // Ensure the latest data is loaded from the file

    string date, description;
    cout << "Enter the date (YYYY-MM-DD) of the expense you want to update: ";
    cin >> date;
    cout << "Enter the description of the expense you want to update: ";
    cin.ignore();
    getline(cin, description);

    // Find the expense to update
    bool expenseFound = false;
    for (auto& expense : expenses) {
        if (expense.username == loggedInUsername && expense.date == date && expense.description == description) {
            expenseFound = true;

            string input;
            regex dateRegex(R"(\d{4}-\d{2}-\d{2})"); // Simple date format YYYY-MM-DD

            cout << "Updating Expense for " << date << " with description: " << description << ":\n";

            // Update date
            while (true) {
                cout << "Enter new date (YYYY-MM-DD, leave blank to keep current): ";
                getline(cin, input);
                if (input.empty()) {
                    break; // Keep current date
                }
                if (regex_match(input, dateRegex)) {
                    expense.date = input;
                    break;
                } else {
                    cout << "Invalid date format. Please enter a valid date." << endl;
                }
            }

            // Update category
            cout << "Enter new category (leave blank to keep current): ";
            getline(cin, input);
            if (!input.empty()) {
                expense.category = input;
            }

            // Update amount
            while (true) {
                cout << "Enter new amount (leave blank to keep current): ";
                getline(cin, input);
                if (input.empty()) {
                    break; // Keep current amount
                }
                try {
                    expense.amount = stod(input);
                    break;
                } catch (invalid_argument&) {
                    cout << "Invalid amount format. Please enter a valid number." << endl;
                }
            }

            // Update description
            cout << "Enter new description (leave blank to keep current): ";
            getline(cin, input);
            if (!input.empty()) {
                expense.description = input;
            }

            cout << "Expense updated successfully!" << endl;
            saveExpensesToFile(); // Save the updated expenses to the file
            return;
        }
    }

    if (!expenseFound) {
        cout << "Expense not found. Please check the date and description." << endl;
    }
}

void deleteExpense() {
    loadExpensesFromFile(); // Ensure the latest data is loaded from the file

    string date, description;
    cout << "Enter the date (YYYY-MM-DD) of the expense you want to delete: ";
    cin >> date;
    cout << "Enter the description of the expense you want to delete: ";
    cin.ignore();
    getline(cin, description);

    // Find the expense to delete
    bool expenseFound = false;
    for (auto it = expenses.begin(); it != expenses.end(); ++it) {
        if (it->username == loggedInUsername && it->date == date && it->description == description) {
            expenseFound = true;
            // Confirm deletion
            char confirm;
            cout << "Are you sure you want to delete this expense? (y/n): ";
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                // Delete the expense
                expenses.erase(it);
                saveExpensesToFile(); // Save the updated expenses to the file
                cout << "Expense deleted successfully!" << endl;
            } else {
                cout << "Expense deletion canceled." << endl;
            }
            return;
        }
    }

    if (!expenseFound) {
        cout << "Expense not found. Please check the date and description." << endl;
    }
}

void viewExpenses() {
    loadExpensesFromFile(); // Ensure the latest data is loaded from the file

    cout << "Your Expenses:\n";
    cout << left << setw(12) << "Date" << setw(15) << "Category" << setw(10) << "Amount" << setw(30) << "Description" << endl;
    cout << string(67, '-') << endl;

    bool hasExpenses = false;
    for (const auto& expense : expenses) {
        if (expense.username == loggedInUsername) {
            cout << left << setw(12) << expense.date
                 << setw(15) << expense.category
                 << setw(10) << expense.amount
                 << setw(30) << expense.description << endl;
            hasExpenses = true;
        }
    }

    if (!hasExpenses) {
        cout << "No expenses found for the logged-in user." << endl;
    }
}

void generateExpenseReport() {
    loadExpensesFromFile(); // Ensure the latest data is loaded from the file

    cout << "Expense Report:\n";
    cout << left << setw(12) << "Date" << setw(15) << "Category" << setw(10) << "Amount" << setw(30) << "Description" << endl;
    cout << string(67, '-') << endl;

    double totalExpenses = 0.0;
    map<string, double> categoryExpenses; // Map to store total expenses by category

    for (const auto& expense : expenses) {
        if (expense.username == loggedInUsername) {
            cout << left << setw(12) << expense.date
                 << setw(15) << expense.category
                 << setw(10) << expense.amount
                 << setw(30) << expense.description << endl;

            totalExpenses += expense.amount;
            categoryExpenses[expense.category] += expense.amount;
        }
    }

    cout << string(67, '-') << endl;
    cout << "Total Expenses: $" << totalExpenses << endl;

    cout << "\nExpenses by Category:\n";
    cout << left << setw(15) << "Category" << setw(10) << "Amount" << endl;
    cout << string(25, '-') << endl;

    for (const auto& category : categoryExpenses) {
        cout << left << setw(15) << category.first << setw(10) << category.second << endl;
    }
}

// Predefined admin credentials
const string ADMIN_USERNAME = "admin";
const string ADMIN_PASSWORD = "admin123";

bool adminLogin() {
    string username, password;

    cout << "Enter admin username: ";
    cin >> username;
    cout << "Enter admin password: ";
    password = getPasswordInput();

    if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
        cout << "Admin login successful!" << endl;
        return true;
    } else {
        cout << "Invalid admin credentials!" << endl;
        return false;
    }
}

void viewAllUsers() {
    loadUsersFromFile(); // Ensure the latest data is loaded from the file

    cout << "Registered Users:\n";
    cout << left << setw(15) << "Username" << setw(15) << "Full Name"
         << setw(25) << "Email" << setw(15) << "Phone" << endl;
    cout << string(70, '-') << endl;

    if (users.empty()) {
        cout << "No registered users found." << endl;
    } else {
        for (const auto& user : users) {
            cout << left << setw(15) << user.username
                 << setw(15) << user.fullName
                 << setw(25) << user.email
                 << setw(15) << user.phone << endl;
        }
    }
}


// Helper function to get the current date in YYYY-MM-DD format
string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;

    char buffer[11];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", year, month, day);
    return string(buffer);
}

// Helper function to check if a user is inactive for more than a year
bool isInactive(const string& lastLoginDate, const string& currentDate) {
    int lastYear, lastMonth, lastDay;
    int currYear, currMonth, currDay;

    sscanf(lastLoginDate.c_str(), "%d-%d-%d", &lastYear, &lastMonth, &lastDay);
    sscanf(currentDate.c_str(), "%d-%d-%d", &currYear, &currMonth, &currDay);

    if ((currYear - lastYear) > 1) {
        return true;
    } else if ((currYear - lastYear) == 1) {
        if (currMonth > lastMonth || (currMonth == lastMonth && currDay >= lastDay)) {
            return true;
        }
    }
    return false;
}

void deleteInactiveUsers() {
    loadUsersFromFile(); // Ensure the latest data is loaded from the file

    string currentDate = getCurrentDate();
    vector<User> activeUsers;

    for (const auto& user : users) {
        if (!isInactive(user.lastLogin, currentDate)) {
            activeUsers.push_back(user);
        }
    }

    users = activeUsers;
    saveUsersToFile(); // Save the updated user list back to the file

    cout << "Inactive users deleted successfully!" << endl;
}

// Helper function to get the current date and time for the backup file names
string getCurrentDateTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;
    int hour = ltm->tm_hour;
    int min = ltm->tm_min;
    int sec = ltm->tm_sec;

    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d_%02d-%02d-%02d", year, month, day, hour, min, sec);
    return string(buffer);
}

void backupFile(const string& sourceFile, const string& backupFile) {
    ifstream src(sourceFile, ios::binary);
    ofstream dst(backupFile, ios::binary);

    if (!src.is_open() || !dst.is_open()) {
        cout << "Error opening file for backup: " << sourceFile << endl;
        return;
    }

    dst << src.rdbuf();

    src.close();
    dst.close();
}

void backupData() {
    string currentDateTime = getCurrentDateTime();

    // Backup users data
    string usersBackupFile = "users_backup_" + currentDateTime + ".txt";
    backupFile("users.txt", usersBackupFile);

    // Backup expenses data
    string expensesBackupFile = "expenses_backup_" + currentDateTime + ".txt";
    backupFile("expenses.txt", expensesBackupFile);

    cout << "Data backup completed successfully!" << endl;
}

// Helper function to copy data from a backup file to the original file
void restoreFile(const string& backupFile, const string& destinationFile) {
    ifstream src(backupFile, ios::binary);
    ofstream dst(destinationFile, ios::binary);

    if (!src.is_open()) {
        cout << "Error opening backup file: " << backupFile << endl;
        return;
    }
    if (!dst.is_open()) {
        cout << "Error opening destination file: " << destinationFile << endl;
        return;
    }

    dst << src.rdbuf();

    src.close();
    dst.close();

    cout << "Restored data from " << backupFile << " to " << destinationFile << " successfully!" << endl;
}

void restoreData() {
    string usersBackupFile, expensesBackupFile;

    cout << "Enter the backup file name for users data: ";
    cin >> usersBackupFile;
    cout << "Enter the backup file name for expenses data: ";
    cin >> expensesBackupFile;

    // Restore users data
    restoreFile(usersBackupFile, "users.txt");

    // Restore expenses data
    restoreFile(expensesBackupFile, "expenses.txt");

    cout << "Data restoration process completed!" << endl;
}

// Function to sort schedules by date
void sortSchedulesByDate() {
    loadSchedulesFromFile(); // Ensure the latest data is loaded from the file

    // Sort the schedules by date
    sort(schedules.begin(), schedules.end(), [](const Schedule& a, const Schedule& b) {
        return a.date < b.date;
    });

    // Save the sorted schedules back to the file
    saveSchedulesToFile();

    // Display the sorted schedules
    cout << "Schedules sorted by date:\n";
    for (const auto& schedule : schedules) {
        cout << schedule.date << " - " << schedule.description << endl;
    }
}

// Partition function for QuickSort
int partition(vector<Expense>& arr, int low, int high) {
    string pivot = arr[high].category;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].category < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// QuickSort function
void quickSort(vector<Expense>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void sortExpensesByCategory() {
    loadExpensesFromFile(); // Ensure the latest data is loaded from the file

    // Sort the expenses by category using QuickSort
    quickSort(expenses, 0, expenses.size() - 1);

    // Save the sorted expenses back to the file
    saveExpensesToFile();

    // Display the sorted expenses
    cout << "Expenses sorted by category:\n";
    for (const auto& expense : expenses) {
        cout << "Username: " << expense.username
             << ", Date: " << expense.date
             << ", Category: " << expense.category
             << ", Amount: " << expense.amount
             << ", Description: " << expense.description << endl;
    }
}

void searchExpenseByKeyword() {
    loadExpensesFromFile(); // Ensure the latest data is loaded from the file

    string keyword;
    cout << "Enter keyword to search in expense descriptions: ";
    cin >> keyword;

    cout << "Expenses matching the keyword '" << keyword << "':\n";
    bool found = false;
    for (const auto& expense : expenses) {
        if (expense.description.find(keyword) != string::npos) {
            cout << "Username: " << expense.username
                 << ", Date: " << expense.date
                 << ", Category: " << expense.category
                 << ", Amount: " << expense.amount
                 << ", Description: " << expense.description << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No expenses found with the keyword '" << keyword << "'." << endl;
    }
}

void findMaxExpense() {
    loadExpensesFromFile(); // Ensure the latest data is loaded from the file

    if (expenses.empty()) {
        cout << "No expenses found." << endl;
        return;
    }

    Expense maxExpense = expenses[0]; // Initialize maxExpense with the first expense

    for (const auto& expense : expenses) {
        if (expense.amount > maxExpense.amount) {
            maxExpense = expense;
        }
    }

    // Display the expense with the maximum amount
    cout << "Expense with the maximum amount:\n";
    cout << "Username: " << maxExpense.username << endl;
    cout << "Date: " << maxExpense.date << endl;
    cout << "Category: " << maxExpense.category << endl;
    cout << "Amount: " << maxExpense.amount << endl;
    cout << "Description: " << maxExpense.description << endl;
}

void findMinExpense() {
    loadExpensesFromFile(); // Ensure the latest data is loaded from the file

    if (expenses.empty()) {
        cout << "No expenses found." << endl;
        return;
    }

    Expense minExpense = expenses[0]; // Initialize minExpense with the first expense

    for (const auto& expense : expenses) {
        if (expense.amount < minExpense.amount) {
            minExpense = expense;
        }
    }

    // Display the expense with the minimum amount
    cout << "Expense with the minimum amount:\n";
    cout << "Username: " << minExpense.username << endl;
    cout << "Date: " << minExpense.date << endl;
    cout << "Category: " << minExpense.category << endl;
    cout << "Amount: " << minExpense.amount << endl;
    cout << "Description: " << minExpense.description << endl;
}

// Function to generate budget summary
void generateBudgetSummary() {
    loadExpensesFromFile(); // Ensure the latest data is loaded from the file

    if (expenses.empty()) {
        cout << "No expenses found." << endl;
        return;
    }

    unordered_map<string, double> categoryTotals;

    for (const auto& expense : expenses) {
        categoryTotals[expense.category] += expense.amount;
    }

    // Display the budget summary
    cout << "Budget Summary by Category:\n";
    cout << setw(20) << left << "Category" << setw(10) << right << "Total Amount" << endl;
    cout << string(30, '-') << endl;

    for (const auto& entry : categoryTotals) {
        cout << setw(20) << left << entry.first << setw(10) << right << entry.second << endl;
    }
}

// Function to aggregate expenses by month
map<string, double> aggregateExpensesByMonth() {
    map<string, double> monthlyExpenses;

    for (const auto& expense : expenses) {
        string month = expense.date.substr(0, 7); // Extract YYYY-MM
        monthlyExpenses[month] += expense.amount;
    }

    return monthlyExpenses;
}

// Function to display a text-based bar chart for monthly expenses
void showMonthlyExpenseGraph() {
    loadExpensesFromFile(); // Ensure the latest data is loaded from the file

    if (expenses.empty()) {
        cout << "No expenses found." << endl;
        return;
    }

    map<string, double> monthlyExpenses = aggregateExpensesByMonth();

    cout << "Monthly Expense Graph:\n";
    for (const auto& entry : monthlyExpenses) {
        cout << entry.first << " : " << string(entry.second/10, '*') << " (" << entry.second << ")" << endl;
    }
}

// Function to compare expenses by category
void compareExpensesByCategory() {
    loadExpensesFromFile(); // Ensure the latest data is loaded from the file

    if (expenses.empty()) {
        cout << "No expenses found." << endl;
        return;
    }

    unordered_map<string, double> categoryTotals;

    // Aggregate expenses by category
    for (const auto& expense : expenses) {
        categoryTotals[expense.category] += expense.amount;
    }

    // Display the comparison of expenses by category
    cout << "Comparison of Expenses by Category:\n";
    cout << setw(20) << left << "Category" << setw(10) << right << "Total Amount" << endl;
    cout << string(30, '-') << endl;

    for (const auto& entry : categoryTotals) {
        cout << setw(20) << left << entry.first << setw(10) << right << entry.second << endl;
    }
}

// Function to load travel data from a file
void loadTravelsFromFile() {
    ifstream inFile("travel.txt");
    if (!inFile) {
        cerr << "Unable to open file travel.txt" << endl;
        return; // If the file does not exist, return
    }

    Travel travel;
    while (inFile >> travel.username >> travel.date >> travel.destination >> travel.distance) {
        inFile.ignore(); // Ignore the whitespace before reading notes
        getline(inFile, travel.notes);
        travels.push_back(travel);
    }
    inFile.close();
}

// Function to generate and display travel statistics
void generateTravelStatistics() {
    loadTravelsFromFile(); // Ensure the latest data is loaded from the file

    if (travels.empty()) {
        cout << "No travel data found." << endl;
        return;
    }

    double totalDistance = 0;
    for (const auto& travel : travels) {
        totalDistance += travel.distance;
    }

    double averageDistance = totalDistance / travels.size();
    int numberOfTrips = travels.size();

    // Display the travel statistics
    cout << "Travel Statistics:\n";
    cout << "Total Distance Traveled: " << totalDistance << " km" << endl;
    cout << "Average Distance per Trip: " << fixed << setprecision(2) << averageDistance << " km" << endl;
    cout << "Number of Trips: " << numberOfTrips << endl;
}

// A map to hold budget data
unordered_map<string, double> budget;

// Function to load budget from a file
void loadBudgetFromFile() {
    ifstream inFile("budget.txt");
    if (!inFile) {
        cerr << "Unable to open file budget.txt" << endl;
        return; // If the file does not exist, return
    }

   string category;
    double amount;
    while (inFile >> category >> amount) {
        budget[category] = amount;
    }
    inFile.close();
}

// Function to notify if any category exceeds the budget
void notifyOverBudget() {
    loadExpensesFromFile(); // Ensure the latest data is loaded from the file
    loadBudgetFromFile(); // Ensure the latest budget is loaded from the file

    if (expenses.empty()) {
        cout << "No expenses found." << endl;
        return;
    }

    if (budget.empty()) {
        cout << "No budget data found." << endl;
        return;
    }

    unordered_map<string, double> categoryTotals;

    // Aggregate expenses by category
    for (const auto& expense : expenses) {
        categoryTotals[expense.category] += expense.amount;
    }

    // Notify if any category exceeds the budget
    bool overBudget = false;
    for (const auto& entry : categoryTotals) {
        const string& category = entry.first;
        double totalExpense = entry.second;

        if (budget.find(category) != budget.end() && totalExpense > budget[category]) {
            cout << "Over budget in category " << category << ": spent " << totalExpense << " (budget " << budget[category] << ")" << endl;
            overBudget = true;
        }
    }

    if (!overBudget) {
        cout << "All expenses are within the budget." << endl;
    }
}



#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits> // Added for numeric_limits
#include <cstdlib> // Added for exit

// Data Structures for the System

// Data Structure for Room // Data Structure for Room
struct Room
{
    string roomType;
    int price;
    bool isAvailable;

    // Constructor for Room
    Room(string type, int p, bool available)
        : roomType(type), price(p), isAvailable(available) {}
};

// Data Structure for Hotel

struct Hotel
{
    string name;
    double distanceFromPlace;
    int pricePerNight;  // This value is not used, but kept for the structure's consistency
    vector<Room> rooms;
    string description;
    string contactNumber;
    double rating;  // New field for rating

    // Constructor with rating
    Hotel(string name, double distanceFromPlace, int pricePerNight, vector<Room> rooms, string description, string contactNumber, double rating)
        : name(name), distanceFromPlace(distanceFromPlace), pricePerNight(pricePerNight), rooms(rooms), description(description), contactNumber(contactNumber), rating(rating) {}
};


// Data Structure for Tourist places
struct TouristPlace
{
    string name;
    string location;
    vector<string> facilities;
    vector<Hotel> hotels;

    // Constructor for TouristPlace
    TouristPlace(string n, string loc, vector<string> fac, vector<Hotel> h)
        : name(n), location(loc), facilities(fac), hotels(h) {}
};



// Function to write booking details to detail.txt
void writeBookingToFile(const string &bookingDetails)
{
    ofstream file("detail.txt", ios::app);

    // Open in append mode
    if (!file.is_open())
    {
        cout << "Error opening file: detail.txt" << endl;
        return;
    }
    file << bookingDetails << endl;

    // Write booking details
    file.close();
}



// Function to read booking details from detail.txt
vector<string> readBookingsFromFile()
{
    vector<string> bookings;
    ifstream file("detail.txt");
    if (!file.is_open())
    {
        cout << "Error opening file: detail.txt" << endl;
        return bookings;
    }

    string line;
    while (getline(file, line))
    {
        bookings.push_back(line);

        // Read each line into the vector
    }
    file.close();
    return bookings;
}



// Function to write canceled booking details to cancel.txt
void writeCanceledToFile(const string &canceledDetails)
{
    ofstream file("cancel.txt", ios::app);

    // Open in append mode
    if (!file.is_open())
    {
        cout << "Error opening file: cancel.txt" << endl;
        return;
    }
    file << canceledDetails << endl;

    // Write canceled details
    file.close();
}


// File Handling Functions
vector<string> readFile(const string &filename)
{
    vector<string> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return data;
    }
    string line;
    while (getline (file, line)) {
        data.push_back(line);
    }
    file.close();
    return data;
}



// Function to write data to a file
void writeFile(const string &filename, const vector<string> &data)
{

    ofstream file(filename);

    if (!file.is_open())
    {
        cout << "Error opening file: " << filename << endl;
        return; // Exit if the file couldn't be opened
    }

    for (const string &line : data)
    {
        file << line << endl;  // Write each line followed by a newline
    }

    // Close the file after writing is complete
    file.close();
}


// Utility Functions
void displayLine(char c = '-', int width = 58)
{
    cout << string(width, c) << endl;
}


// Display Main menu for accommodation
void displayMenu()
{
    displayLine();
    cout << "\n  Accommodation Management System \n";
    displayLine();
    cout << "1. View Tourist Places  -";
    cout << "   Discover top attractions.\n";

    cout << "2. Find Accommodation  \t-";
    cout << "   Search for unique stays.\n";

    cout << "3. Hotels by Rating  \t-";
    cout << "   View Hotels Sorted by Rating\n";

    cout << "4. Book Your Stay  \t-";
    cout << "   Reserve the perfect room.\n";

    cout << "5. Cancel Your Booking  -";
    cout << "   Modify your plans.\n";

    cout << "6. View Trip Details  \t-";
    cout << "   Check your bookings.\n";

    cout << "7. Feedback \t\t-";
    cout << "   Give Feedback.\n";

    cout << "8. Exit \t\t-";
    cout << "   Close the system.\n";
    displayLine();

    cout << "\nPlease select an option (1-7): ";
}



// Function to display canceled booking details from cancel.txt
void cancelDetails()
{
    cout << "\n";
    cout << "              Cancelled Booking Details              \n";
    cout << "==========================================================\n";
    ifstream file("cancel.txt"); // Open cancel.txt in read mode
    if (!file.is_open())
    {
        cout << "Error opening file: cancel.txt" << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        cout << line << endl; // Print the booking details (Tourist Place, Hotel, etc.)
    }

    file.close(); // Close the file after reading
}



// Booked room details
void bookingDetails()
{
    cout << "\n";
    cout << "                       Booking Details\n";
    cout << "==========================================================\n";
    vector<string> bookings = readBookingsFromFile(); // Read bookings from file

    if (!bookings.empty())
    {
        for (const auto &booking : bookings) {
            cout << booking << "\n"; // Directly displaying the booking for now
        }
    }
    else
    {
        cout << "No active bookings found.\n";
    }
}



// Function for view details of booked rooms and cancelled bookings
void viewdetails()
{
    cout << "\n==========================================================\n";
    cout << "              View Trip Details\n";
    cout << "==========================================================\n";
    cout << "\n";

    cout << "1. View Booking History\t-";
    cout << "   Check your past bookings.\n";
    cout << "2. View Cancellation History\t-";
    cout << "   Check your past cancellations.\n";

    int ch;
    cout << "\nEnter your choice : ";
    cin >> ch;

    switch(ch)
    {
        case 1:
            bookingDetails();
            break;

        case 2:
            cancelDetails();
            break;

        default:
            cout << "Invalid choice! Please select again.\n";
            break;
    }
}


// Function to display the hotels and their respective rooms and their prices
// Function to display hotel details with available rooms
void displayHotels(const vector<Hotel> &hotels)
{
    cout << "\nAccommodation Details" << endl;
    displayLine();  // Assuming displayLine is a helper function that formats output

    // Loop through all hotels to display their details
    for (size_t i = 0; i < hotels.size(); i++)
    {
        // Display basic hotel details
        cout << i + 1 << ". " << hotels[i].name << endl;
        cout << "   Distance: " << hotels[i].distanceFromPlace << " km" << endl;  // Distance from the tourist place
        cout << "   Price per night: Rs. " << hotels[i].pricePerNight << endl;
        cout << "   Rating: " << hotels[i].rating << " stars" << endl;  // Rating of the hotel
        cout << "   Description: " << hotels[i].description << endl;  // Short description of the hotel
        cout << "   Contact: " << hotels[i].contactNumber << endl;  // Contact number of the hotel
        cout << "   Available Rooms:" << endl;

        // Display room details in a formatted manner
        cout << "       Room Type         Price          Availability" << endl;
        displayLine();  // Assuming displayLine is used to separate sections visually

        // Loop through rooms and display availability
        for (const auto &room : hotels[i].rooms)
        {
            cout << "       " << setw(15) << room.roomType << "  "
                 << "Rs. " << setw(10) << room.price << "  "
                 << (room.isAvailable ? "Available" : "Booked") << endl;
        }
        cout << endl;  // Add extra space between hotels for clarity
    }
}




// Function to book room and update the hotel details
void bookRoom(vector<TouristPlace> &places)
{
    // Display the booking menu header
    cout << "\n=====================================================\n";
    cout << "                     Book a Room\n";
    cout << "=====================================================\n";

    // Display the list of available tourist places
    cout << "\nAvailable Tourist Places:\n";

    for (size_t i = 0; i < places.size(); ++i)
    {
        cout << i + 1 << ". " << places[i].name << " (" << places[i].location << ")\n";
    }

    // Prompt user to select a tourist place
    cout << "\nEnter the tourist place : ";
    int placeChoice;

    // Input validation for tourist place choice
    while (!(cin >> placeChoice) || placeChoice < 1 || placeChoice > places.size())
    {
        cout << "\nInvalid input! \nPlease enter a valid tourist place number : ";
        cin.clear(); // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // Discard invalid input
    }

    // Display the list of hotels in the selected tourist place
    cout << "\nAvailable Hotels:\n";

    for (size_t i = 0; i < places[placeChoice - 1].hotels.size(); ++i)
    {
        // Display hotel details (name and distance from the tourist place)
        cout << i + 1 << ". " << places[placeChoice - 1].hotels[i].name << "\t"
             << " (" << places[placeChoice - 1].hotels[i].distanceFromPlace << " km away)\n";
    }

    // Prompt user to select a hotel
    cout << "\nEnter the hotel number to book a room : ";
    int hotelChoice;

    // Input validation for hotel choice
    while (!(cin >> hotelChoice) || hotelChoice < 1 || hotelChoice > places[placeChoice - 1].hotels.size())
    {
        cout << "\nInvalid input! \nPlease enter a valid hotel number : ";
        cin.clear(); // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // Discard invalid input
    }

    // Get the selected hotel
    Hotel &selectedHotel = places[placeChoice - 1].hotels[hotelChoice - 1];

    // Display the selected hotel and its available rooms
    cout << "\n=====================================================\n";
    cout << "      Selected Hotel: " << selectedHotel.name << "\n";
    cout << "=====================================================\n";

    cout << "\nAvailable Rooms:\n";
    bool roomAvailable = false;
    // Check if at least one room is available

    // Display available rooms in the selected hotel
    for (size_t i = 0; i < selectedHotel.rooms.size(); ++i)
    {
        if (selectedHotel.rooms[i].isAvailable)
        {
            roomAvailable = true;
            cout << i + 1 << ". " << selectedHotel.rooms[i].roomType
                 << " - Rs. " << selectedHotel.rooms[i].price << "\n";
        }
    }

    // If no rooms are available, notify the user and return
    if (!roomAvailable)
    {
        cout << "\nNo rooms are available in " << selectedHotel.name << ".\n";
        cout << "Returning to the main menu...\n";
        return;
    }

    // Prompt user to select a room
    cout << "\nEnter the room number to book : ";
    int roomChoice;

    // Input validation for room choice
    while (!(cin >> roomChoice) || roomChoice < 1 || roomChoice > selectedHotel.rooms.size() || !selectedHotel.rooms[roomChoice - 1].isAvailable)
    {
        cout << "\nInvalid room choice or the room is already booked! \nPlease enter a valid room number : ";
        cin.clear(); // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // Discard invalid input
    }

    // Display confirmation message for the selected room
    cout << "\nYou have selected:\n";
    cout << "Room Type: " << selectedHotel.rooms[roomChoice - 1].roomType << "\n";
    cout << "Price: Rs. " << selectedHotel.rooms[roomChoice - 1].price << "\n";
    cout << "Hotel: " << selectedHotel.name << "\n";
    cout << "-----------------------------------------------------\n";
    cout << "Confirm booking? (yes/no): ";
    string confirmation;
    cin >> confirmation;

    // Normalize user input to lowercase
    transform(confirmation.begin(), confirmation.end(), confirmation.begin(), ::tolower);

    // Process booking confirmation
    if (confirmation == "yes")
    {
        // Ask for payment method
        cout << "How would you like to pay?(cash/upi) : ";
        string paymentMethod;
        cin >> paymentMethod;

        // Normalize payment method input
        transform(paymentMethod.begin(), paymentMethod.end(), paymentMethod.begin(), ::tolower);

        if (paymentMethod == "cash" || paymentMethod == "upi")
        {
            // Mark room as booked
            selectedHotel.rooms[roomChoice - 1].isAvailable = false;
            cout << "\n=====================================================\n";
            cout << "  Payment of Rs. " << selectedHotel.rooms[roomChoice - 1].price
                 << " received via " << paymentMethod << ".\n";
            cout << "  Room booked successfully at " << selectedHotel.name << "!\n";
            cout << "=====================================================\n";

            // Save booking details to file
            string bookingDetails = "Booking Confirmation\n" +
                                    string("==============================\n") +
                                    "Tourist Place: " + places[placeChoice - 1].name + "\n" +
                                    "Hotel: " + selectedHotel.name + "\n" +
                                    "Room Type: " + selectedHotel.rooms[roomChoice - 1].roomType + "\n" +
                                    "Price: Rs. " + to_string(selectedHotel.rooms[roomChoice - 1].price) + "\n" +
                                    "Payment Method: " + paymentMethod + "\n" +
                                    string("==============================\n");

            writeBookingToFile(bookingDetails); // Save booking details to file
        }

        else
        {
            cout << "\nInvalid payment method! Booking process aborted.\n";
        }
    }

    else
    {
        cout << "\nBooking process aborted. Returning to the main menu...\n";
    }
}



// Function to display tourist places and their hotels
void displayTouristPlaces(const vector<TouristPlace> &places)
{
    cout << "\nExplore the Beauty of Mysuru - Tourist Places" << endl;
    displayLine();

    for (size_t i = 0; i < places.size(); i++) {
        cout << i + 1 << ". " << places[i].name << endl;
    }
    displayLine();

    cout << "Choose a place to explore (1-" << places.size() << "): ";
}



// Function to display hotel names for the selected tourist place
void displayHotelNames(const TouristPlace &place)
{
    // Display the header with the name of the selected tourist place
    cout << "\nHotels near " << place.name << ":" << endl;

    // Loop through each hotel in the tourist place and display its name
    for (size_t i = 0; i < place.hotels.size(); i++)
    {
        // Display hotel number (1-based) and hotel name
        cout << i + 1 << ". " << place.hotels[i].name << endl;
    }

    // Prompt the user to choose a hotel by number
    cout << "\nChoose a hotel (1-" << place.hotels.size() << "): ";
}





// Function to display details of the selected hotel
// Function to display details of a selected hotel
void displaySelectedHotelDetails(const Hotel &hotel)
{
    // Display the hotel header and name
    cout << "\n==========================================================\n";
    cout << "Hotel Details: " << hotel.name << endl;
    cout << "==========================================================\n";

    // Display location details
    cout << "Location Details:\n";
    cout << "  Distance from tourist place: " << hotel.distanceFromPlace << " km\n";
    cout << "  Price per night:             Rs. " << hotel.pricePerNight << endl;
    cout << "  Rating:                     " << hotel.rating << " stars" << endl;
    // Added rating
    cout << "  Description:\n    " << hotel.description << endl;
    cout << "  Contact Information:         " << hotel.contactNumber << endl;
     // Fixed typo 'contact' to 'contactNumber'

    // Function to display a separator line
    displayLine();

    // Display room availability section
    cout << "Room Availability:\n";
    // Set column widths for better alignment of room details
    cout << setw(15) << "Room Type";
    cout << setw(20);
    cout << "Price per Night" ;
    cout << setw(20);
    cout << "Availability";
    cout << endl;

    // Function to display a separator line
    displayLine();

    // Loop through each room in the hotel and display its details
    for (const auto &room : hotel.rooms)
    {
        // Display room type, price, and availability status
        cout << setw(15) << room.roomType
             << setw(20) << "Rs. " + to_string(room.price)
             << setw(20) << (room.isAvailable ? "Available" : "Booked") << endl;
    }

    // Display a separator line at the end
    displayLine();
}



// Function to search for accommodation based on
// user's preferences and display the results
void searchAccommodation(const vector<TouristPlace> &places)

{
    // Prompt the user to enter the name of the hotel they are searching for
    cout << "\nEnter the name of the hotel to search: ";
    string searchQuery;

    cin.ignore();  // Ignore any leftover newline characters
    getline(cin, searchQuery);  // Get the full line input from the user

    // Convert the search query to lowercase for case-insensitive comparison
    transform(searchQuery.begin(), searchQuery.end(), searchQuery.begin(), ::tolower);

    bool found = false;  // Flag to track whether any matching hotels are found

    // Loop through all the tourist places
    for (const auto &place : places)
    {
        // Loop through all the hotels in the current place
        for (const auto &hotel : place.hotels)
        {
            // Convert the hotel name to lowercase for comparison
            string hotelNameLower = hotel.name;
            transform(hotelNameLower.begin(), hotelNameLower.end(), hotelNameLower.begin(), ::tolower);

            // Check if the lowercase hotel name contains the search query
            if (hotelNameLower.find(searchQuery) != string::npos)
            {
                // If a match is found, display the hotel and the associated tourist place
                cout << "Found: " << hotel.name << " near " << place.name << endl;
                found = true;
            }
        }
    }

    // If no matching accommodation was found, display a message
    if (!found) {
        cout << "\nNo accommodation found with the name: " << searchQuery << endl;
    }
}




// Function to cancel a booking
void cancelBooking(vector<TouristPlace>& places)

{
    // Display a separator and the cancel booking title
    cout << "\n=====================================================\n";
    cout << "              Cancel a Booking\n";
    cout << "=====================================================\n";

    // Read the bookings from the "detail.txt" file
    vector<string> bookings = readBookingsFromFile();

    // If there are no bookings
    if (bookings.empty())
    {
        cout << "\nNo bookings found! You don't have any rooms booked.\n";
        return;
    }

    // Display the list of bookings with indexing every 8 lines
    cout << "\nYour Bookings:\n";
    int bookingIndex = 1;

    for (size_t i = 0; i < bookings.size(); i += 8)
    {
        // Step 8 because each booking is 8 lines
        cout << "Booking " << bookingIndex++ << ":\n";

        for (size_t j = i; j < i + 8 && j < bookings.size(); ++j) {

            cout << bookings[j] << endl;
        }
        cout << "\n";
        // Add a blank line between different bookings
    }

    // Prompt user to select a booking to cancel
    cout << "\nEnter the booking number to cancel (1-" << (bookingIndex - 1) << ") : ";
    int bookingChoice;

    // Input loop to ensure a valid booking number is selected
    while (!(cin >> bookingChoice) || bookingChoice < 1 || bookingChoice >= bookingIndex)
    {
        cout << "\nInvalid input! Please enter a valid booking number : ";
        cin.clear(); // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // Discard invalid input
    }

    // Calculate the starting index of the selected booking in the bookings vector
    int bookingStartIndex = (bookingChoice - 1) * 8;

    // Extract the canceled booking details
    string canceledBookingDetails = "";
    for (int i = bookingStartIndex; i < bookingStartIndex + 8 && i < bookings.size(); ++i)
    {
        canceledBookingDetails += bookings[i] + "\n";
    }

    // Extract relevant booking details from the canceled booking (e.g., Tourist Place, Hotel, Room)
    size_t touristPlaceStart = canceledBookingDetails.find("Tourist Place: ") + 15;
    size_t hotelStart = canceledBookingDetails.find("Hotel: ") + 7;
    size_t roomStart = canceledBookingDetails.find("Room Type: ") + 11;
    size_t priceStart = canceledBookingDetails.find("Price: Rs. ") + 11;

    // Get the specific details (Tourist Place, Hotel, Room Type, Price) from the booking
    string touristPlaceName = canceledBookingDetails.substr(touristPlaceStart, canceledBookingDetails.find("\n", touristPlaceStart) - touristPlaceStart);
    string hotelName = canceledBookingDetails.substr(hotelStart, canceledBookingDetails.find("\n", hotelStart) - hotelStart);
    string roomType = canceledBookingDetails.substr(roomStart, canceledBookingDetails.find("\n", roomStart) - roomStart);
    string price = canceledBookingDetails.substr(priceStart, canceledBookingDetails.find("\n", priceStart) - priceStart);

    // Find the booking details in the places vector and set the room availability back to true
    bool roomFound = false;
    for (auto &place : places)
    {
        for (auto &hotel : place.hotels)
        {
            if (hotel.name == hotelName)
            {
                for (auto &room : hotel.rooms)
                {
                    if (room.roomType == roomType && !room.isAvailable)
                    {
                        room.isAvailable = true;
                        roomFound = true;
                        break;
                    }
                }
            }
            if (roomFound) break;
        }
        if (roomFound) break;
    }

    // If the room was found and successfully marked as available
    if (roomFound) {
        // Display confirmation that the booking has been canceled and the room is available
        cout << "\n=====================================================";
        cout << "\n";
        cout << "  The booking for room type '" << roomType << "' at " << hotelName << " has been canceled.";
        cout << "\n";
        cout << "  The room is now available for booking again.";
        cout << "\n";
        cout << "=====================================================";
        cout << "\n";

        // Save canceled booking details to cancel.txt
        string canceledDetails = string("==============================\n") +
                                 "Canceled Booking Details\n" +
                                 string("==============================\n") +
                                 "Tourist Place: " + touristPlaceName + "\n" +
                                 "Hotel: " + hotelName + "\n" +
                                 "Room Type: " + roomType + "\n" +
                                 "Price: Rs. " + price + "\n" +
                                 string("==============================\n");

        writeCanceledToFile(canceledDetails);
        // Save canceled booking details to file

        // Remove the canceled booking from the bookings list
        bookings.erase(bookings.begin() + bookingStartIndex, bookings.begin() + bookingStartIndex + 8);

        // Write the updated bookings list back to the detail.txt file
        writeFile("detail.txt", bookings);
        // Correct function to rewrite the full list
    }
    else
    {
        // Display an error if the booking was not found or the room was already available
        cout << "\nError: Booking not found or room was already available!";
        cout << "\n";
    }
}




// Function to collect feedback from the user
void collectFeedback()

{
    // Declare variables to store feedback text and rating
    string feedbackText;
    int rating;

    // Print a separator line and prompt for feedback text
    cout << "\n============================== FeedBACK ==============================" << endl;
    cout << "We value your feedback! Please share your thoughts about our service:" << endl;
    cout << "-----------------------------------------------------------------------" << endl;

    // Prompt the user to enter their feedback
    cout << "Enter your feedback: ";

    // Clear the input buffer before taking the input
    cin.ignore();

    // Get the full line of feedback text
    getline(cin, feedbackText);

    // Prompt for a rating from 1 to 5
    cout << "Please rate your experience from 1 to 5 \n(1 being the lowest and 5 being the highest): ";

    // Read the rating input
    cin >> rating;

    // Validate the rating input to ensure it's between 1 and 5
    while (rating < 1 || rating > 5)
    {
        cout << "Invalid rating! Please enter a number between 1 and 5: ";
        cin >> rating;
    }

    // Print a separator line after the rating input
    cout << "-----------------------------------------------------------------------" << endl;

    // Thank the user for their feedback
    cout << "Thank you for your feedback!" << endl;

    // Respond based on the rating provided by the user
    switch (rating)
    {
        case 1:
            cout << "\nWe're sorry to hear that you had a poor experience. \nWe will strive to improve!" << endl;
            break;

        case 2:
            cout << "\nWe appreciate your input and will work on making things better." << endl;
            break;

        case 3:
            cout << "\nWe're glad you had an average experience, \nbut we aim to do better." << endl;
            break;

        case 4:
            cout << "\nWe're happy to hear you had a good experience. \nYour feedback motivates us!" << endl;
            break;

        case 5:
            cout << "\nWe're thrilled you had an excellent experience!" << endl;
            break;
    }

    // Open a file in append mode to save the feedback
    ofstream feedbackFile("feedback.txt", ios::app); // Open file in append mode

    // Check if the file was opened successfully
    if (feedbackFile.is_open())
    {
        // Write feedback details to the file
        feedbackFile << "Accommodation Management System Feedback:" << endl;
        feedbackFile << "Feedback: " << feedbackText << endl;
        feedbackFile << "Rating: " << rating << endl;
        feedbackFile << "---------------------------------------------------" << endl;

        // Close the file after writing
        feedbackFile.close();
        cout << "Your feedback has been saved successfully!" << endl;
    }
    else
    {
        // Print an error message if unable to open the file
        cout << "Error: Unable to save feedback to file." << endl;
    }

    // Display a closing message thanking the user for their feedback
    cout << "\nYour feedback is important to us, \nand we will use it to enhance our services." << endl;
    cout << "=======================================================================" << endl;

}




// Thank you function contains the statements for thanking
// Function to display the thank you message to the user
void thankyou()

{
    // Print a separator line for clarity
    cout << "\n=========================================================";

    // Print a blank line for spacing
    cout << "\n";

    // Print the first part of the thank you message
    cout << "             Thank You for Choosing Mysuru's";

    // Print a blank line after the first message
    cout << "\n";

    // Print the second part of the thank you message
    cout << "       Accommodation Management System for Your Stay";

    // Print a blank line after the second message
    cout << "\n";

    // Print the final part of the thank you message
    cout << "   We look forward to welcoming you back in the future!";

    // Print the closing separator line
    cout << "\n=========================================================";

    // Print a couple of blank lines for extra spacing
    cout << "\n";
    cout << "\n";
}




// Function to display the welcome message to the user
void displayWelcomeMessage()

{
    // Print a separator line for clarity
    cout << "\n=========================================================\n";

    // Print the welcome message
    cout << "     Welcome to the Karnataka Tourism Management System!\n";

    // Print the tagline for the system
    cout << "        Your Trusted Partner for Travel and Stays         \n";

    // Print the closing separator line
    cout << "=========================================================\n";
}



// Function to handle tourist place selection
// Function to handle the selection of a tourist place and hotel
void handleTouristPlaceSelection(const vector<TouristPlace>& places) {

    // Display the list of tourist places
    displayTouristPlaces(places);

    // Variable to store the user's choice of tourist place
    int placeChoice;

    // Input loop for valid tourist place selection
    while (!(cin >> placeChoice) || placeChoice < 1 || placeChoice > places.size()) {
        // Prompt the user to enter a valid place number if the input is invalid
        cout << "Invalid place selection! Please enter a valid number: ";

        // Clear the error flag and ignore invalid input
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Display the list of hotels for the selected tourist place
    displayHotelNames(places[placeChoice - 1]);

    // Variable to store the user's choice of hotel
    int hotelChoice;

    // Input loop for valid hotel selection
    while (!(cin >> hotelChoice) || hotelChoice < 1 || hotelChoice > places[placeChoice - 1].hotels.size()) {
        // Prompt the user to enter a valid hotel number if the input is invalid
        cout << "Invalid hotel selection! Please enter a valid number: ";

        // Clear the error flag and ignore invalid input
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Display the details of the selected hotel
    displaySelectedHotelDetails(places[placeChoice - 1].hotels[hotelChoice - 1]);
}





// Function to handle booking
void bookRoom(Hotel& selectedHotel)
{
    // Display available rooms in the selected hotel
    cout << "\n=====================================================\n";
    cout << "      Selected Hotel: " << selectedHotel.name << "\n";
    cout << "=====================================================\n";

    cout << "\nAvailable Rooms:\n";
    bool roomAvailable = false;
    for (size_t i = 0; i < selectedHotel.rooms.size(); ++i)
    {
        //  Display room details
        if (selectedHotel.rooms[i].isAvailable)
        {
            //  Display room details
            roomAvailable = true;
            cout << i + 1 << ". " << selectedHotel.rooms[i].roomType
                 << " - Rs. " << selectedHotel.rooms[i].price << "\n";
        }
    }

    if (!roomAvailable)
    {
        //  Display message if no rooms are available
        cout << "\nNo rooms are available in " << selectedHotel.name << ".\n";
        return;
    }

    // Prompt user to select a room
    cout << "\nEnter the room number to book: ";
    int roomChoice;
    cin >> roomChoice;

    while (roomChoice < 1 || roomChoice > selectedHotel.rooms.size() || !selectedHotel.rooms[roomChoice - 1].isAvailable)
    {
        //  Clear input buffer if invalid input
        cout << "\nInvalid room choice or the room is already booked! Please enter a valid room number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> roomChoice;
    }

    // Display confirmation message
    cout << "\nYou have selected:\n";
    cout << "Room Type: " << selectedHotel.rooms[roomChoice - 1].roomType << "\n";
    cout << "Price: Rs. " << selectedHotel.rooms[roomChoice - 1].price << "\n";
    cout << "Hotel: " << selectedHotel.name << "\n";
    cout << "-----------------------------------------------------\n";
    cout << "Confirm booking? (yes/no): ";
    string confirmation;
    cin >> confirmation;

    // Normalize user input to lowercase
    transform(confirmation.begin(), confirmation.end(), confirmation.begin(), ::tolower);

    // Process confirmation
    if (confirmation == "yes")
    {
        //  Update room status
        // Ask for payment method
        cout << "How would you like to pay? (cash/upi): ";
        string paymentMethod;
        cin >> paymentMethod;

        // Normalize payment method input
        transform(paymentMethod.begin(), paymentMethod.end(), paymentMethod.begin(), ::tolower);

        if (paymentMethod == "cash" || paymentMethod == "upi")
        {
            //  Update room status
            selectedHotel.rooms[roomChoice - 1].isAvailable = false;
            cout << "\n=====================================================\n";
            cout << "  Payment of Rs. " << selectedHotel.rooms[roomChoice - 1].price
                 << " received via " << paymentMethod << ".\n";
            cout << "  Room booked successfully at " << selectedHotel.name << "!\n";
            cout << "=====================================================\n";

            // Save comprehensive booking details to file
            string bookingDetails = "Booking Confirmation\n" +
                                    string("==============================\n") +
                                    "Hotel: " + selectedHotel.name + "\n" +
                                    "Room Type: " + selectedHotel.rooms[roomChoice - 1].roomType + "\n" +
                                    "Price: Rs. " + to_string(selectedHotel.rooms[roomChoice - 1].price) + "\n" +
                                    "Payment Method: " + paymentMethod + "\n" +
                                    string("==============================\n");

            // Assuming writeBookingToFile function exists
            writeBookingToFile(bookingDetails); // Save booking details to file
        }
        else
        {
            //  Update room status
            cout << "\nInvalid payment method! Booking process aborted.\n";
        }
    }
    else
    {
        //  Update room status
        cout << "\nBooking process aborted. Returning to the main menu...\n";
    }
}




// Main function for displaying hotels by
// tourist place and handling user selection
void displayHotelsByRating(vector<TouristPlace>& places)
{
    //  Display tourist places
    // Ask user to select a tourist place
    cout << "\nSelect a Tourist Place:\n";

    displayLine();

    for (size_t i = 0; i < places.size(); ++i)
    {
        //  Display tourist places
        cout << i + 1 << ". " << places[i].name << "\n";
    }
    cout << "\nEnter the tourist place number : ";
    int placeChoice;
    cin >> placeChoice;

    while (placeChoice < 1 || placeChoice > places.size())
    {
        //  Input validation
        cout << "\nInvalid input! \nPlease enter a valid tourist place number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> placeChoice;
    }

    TouristPlace& selectedPlace = places[placeChoice - 1];

    cout << "\nPlease choose a hotel category:\n";
    displayLine();
    cout << "1. Premium (Rating: 4.5 and above)\n";
    cout << "2. Standard (Rating: 3.5 to 4.5)\n";
    cout << "3. Budget (Rating below: 3.5)\n";
    cout << "\nEnter your choice: ";

    int choice;
    cin >> choice;

    vector<Hotel> premiumHotels;
    vector<Hotel> standardHotels;
    vector<Hotel> budgetHotels;

    // Categorize hotels based on rating
    for (const auto& hotel : selectedPlace.hotels)
    {
        //  Assuming hotel.rating is a float value
        // between 0 and 5
        if (hotel.rating >= 4.5)
        {
            //  Add to premium hotels list
            premiumHotels.push_back(hotel);
        }
        else if (hotel.rating >= 3.5)
        {
            //  Add to standard hotels list
            standardHotels.push_back(hotel);
        }
        else
        {
            //   Add to budget hotels list
            budgetHotels.push_back(hotel);
        }
    }

    switch (choice) {
        case 1: {
            // Premium hotels
            if (premiumHotels.empty())
            {
                //  Display message if no premium hotels found
                cout << "\nNo Premium hotels are available for booking.\n";
                break;
            }

            cout << "\nPremium Hotels:\n";
            for (size_t i = 0; i < premiumHotels.size(); ++i)
            {
                //  Display hotel details
                cout << i + 1 << ". " << premiumHotels[i].name;
                cout << " \t(Rating: " << premiumHotels[i].rating << ")"<< "\n";
            }

            // Ask the user if they want to book a room
            cout << "\nWould you like to book a room? (yes/no): ";
            string bookChoice;
            cin >> bookChoice;
            transform(bookChoice.begin(), bookChoice.end(), bookChoice.begin(), ::tolower);

            if (bookChoice == "yes") {
                // Ask the user to choose a hotel to book

                cout << "\nProceeding to Booking...\n";
                cout << "\nEnter the hotel number to book a room : ";
                int hotelChoice;
                cin >> hotelChoice;

                while (hotelChoice < 1 || hotelChoice > premiumHotels.size())
                {
                    //  Input validation
                    cout << "\nInvalid input! \nPlease enter a valid hotel number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> hotelChoice;
                }

                Hotel& selectedHotel = premiumHotels[hotelChoice - 1];
                // Display available rooms in the selected hotel
    cout << "\n=====================================================\n";
    cout << "      Selected Hotel: " << selectedHotel.name << "\n";
    cout << "=====================================================\n";

    cout << "\nAvailable Rooms:\n";
    bool roomAvailable = false;
    for (size_t i = 0; i < selectedHotel.rooms.size(); ++i)
    {
        //  Check if the room is available

        if (selectedHotel.rooms[i].isAvailable)
        {
            //  Check if the room is available

            roomAvailable = true;
            cout << i + 1 << ". " << selectedHotel.rooms[i].roomType
                 << " - Rs. " << selectedHotel.rooms[i].price << "\n";
        }
    }

    if (!roomAvailable)
    {
        //  Check if the room is available

        cout << "\nNo rooms are available in " << selectedHotel.name << ".\n";
        return;
    }

    // Prompt user to select a room
    cout << "\nEnter the room number to book : ";
    int roomChoice;
    cin >> roomChoice;

    while (roomChoice < 1 || roomChoice > selectedHotel.rooms.size() || !selectedHotel.rooms[roomChoice - 1].isAvailable)
    {
        //  Input validation
        cout << "\nInvalid room choice or the room is already booked! Please enter a valid room number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> roomChoice;
    }

    // Display confirmation message
    cout << "\nYou have selected:\n";
    cout << "Room Type: " << selectedHotel.rooms[roomChoice - 1].roomType << "\n";
    cout << "Price: Rs. " << selectedHotel.rooms[roomChoice - 1].price << "\n";
    cout << "Hotel: " << selectedHotel.name << "\n";
    cout << "Tourist Place: " << selectedPlace.name << "\n";
    cout << "-----------------------------------------------------\n";
    cout << "Confirm booking? (yes/no): ";
    string confirmation;
    cin >> confirmation;

    // Normalize user input to lowercase
    transform(confirmation.begin(), confirmation.end(), confirmation.begin(), ::tolower);

    // Process confirmation

    if (confirmation == "yes")
    {
        //  Update room status
        // Ask for payment method
        cout << "How would you like to pay? (cash/upi): ";
        string paymentMethod;
        cin >> paymentMethod;

        // Normalize payment method input
        transform(paymentMethod.begin(), paymentMethod.end(), paymentMethod.begin(), ::tolower);

        if (paymentMethod == "cash" || paymentMethod == "upi")
        {
            //  Process payment
            selectedHotel.rooms[roomChoice - 1].isAvailable = false;

            cout << "\n=====================================================\n";
            cout << "  Payment of Rs. " << selectedHotel.rooms[roomChoice - 1].price
                 << " received via " << paymentMethod << ".\n";
            cout << "  Room booked successfully at " << selectedHotel.name << "!\n";
            cout << "=====================================================\n";

            // Save comprehensive booking details to file

            string bookingDetails = "Booking Confirmation\n" +
                                    string("==============================\n") +
                                    "Tourist Place: " + selectedPlace.name + "\n" +
                                    "Hotel: " + selectedHotel.name + "\n" +
                                    "Room Type: " + selectedHotel.rooms[roomChoice - 1].roomType + "\n" +
                                    "Price: Rs. " + to_string(selectedHotel.rooms[roomChoice - 1].price) + "\n" +
                                    "Payment Method: " + paymentMethod + "\n" +
                                    string("==============================\n");

            // Write to file

            ofstream outFile("booking_details.txt", ios::app); // Open the file in append mode
            outFile << bookingDetails << endl;
            outFile.close();
        }

        else
        {
            //  Process payment
            cout << "\nInvalid payment method! Booking process aborted.\n";
        }
    }

    else
    {
        //  Process payment
        cout << "\nBooking process aborted. Returning to the main menu...\n";
    }
            }
            else {
                cout << "\nExiting to the main menu...\n";
            }
            break;
        }

        case 2: {
            // Standard hotels
            if (standardHotels.empty())
            {
                //  If no standard hotels are available,
                // display a message and break the switch
                cout << "\nNo Standard hotels are available for booking.\n";
                break;
            }

            cout << "\nStandard Hotels:\n";
            for (size_t i = 0; i < standardHotels.size(); ++i)
            {
                //  Display the hotel name and rating
                cout << i + 1 << ". " << standardHotels[i].name;
                cout << " \t(Rating: " << standardHotels[i].rating << ")"<< "\n";
            }

            // Ask the user if they want to book a room
            cout << "\nWould you like to book a room? (yes/no): ";
            string bookChoice;

            cin >> bookChoice;
            transform(bookChoice.begin(), bookChoice.end(), bookChoice.begin(), ::tolower);

            if (bookChoice == "yes") {
                // Ask the user to choose a hotel to book

                cout << "\nProceeding to Booking...\n";
                cout << "\nEnter the hotel number to book a room : ";
                int hotelChoice;
                cin >> hotelChoice;

                while (hotelChoice < 1 || hotelChoice > standardHotels.size())
                {
                    //  Clear the input buffer to handle invalid inputs
                    cout << "\nInvalid input! \nPlease enter a valid hotel number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> hotelChoice;
                }

                Hotel& selectedHotel = standardHotels[hotelChoice - 1];
                // Display available rooms in the selected hotel
    cout << "\n=====================================================\n";
    cout << "      Selected Hotel: " << selectedHotel.name << "\n";
    cout << "=====================================================\n";

    cout << "\nAvailable Rooms:\n";
    bool roomAvailable = false;
    for (size_t i = 0; i < selectedHotel.rooms.size(); ++i)
    {
        //  Check if the room is available

        if (selectedHotel.rooms[i].isAvailable)
        {
            //  Check if the room is available

            roomAvailable = true;

            cout << i + 1 << ". " << selectedHotel.rooms[i].roomType
                 << " - Rs. " << selectedHotel.rooms[i].price << "\n";
        }
    }

    if (!roomAvailable)
    {
        //  Check if the room is available

        cout << "\nNo rooms are available in " << selectedHotel.name << ".\n";
        return;
    }

    // Prompt user to select a room
    cout << "\nEnter the room number to book : ";

    int roomChoice;
    cin >> roomChoice;

    while (roomChoice < 1 || roomChoice > selectedHotel.rooms.size() || !selectedHotel.rooms[roomChoice - 1].isAvailable)
    {
        //  Input validation
        cout << "\nInvalid room choice or the room is already booked! Please enter a valid room number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> roomChoice;
    }

    // Display confirmation message
    cout << "\nYou have selected:\n";
    cout << "Room Type: " << selectedHotel.rooms[roomChoice - 1].roomType << "\n";
    cout << "Price: Rs. " << selectedHotel.rooms[roomChoice - 1].price << "\n";
    cout << "Hotel: " << selectedHotel.name << "\n";
    cout << "Tourist Place: " << selectedPlace.name << "\n";
    cout << "-----------------------------------------------------\n";
    cout << "Confirm booking? (yes/no): ";
    string confirmation;
    cin >> confirmation;

    // Normalize user input to lowercase
    transform(confirmation.begin(), confirmation.end(), confirmation.begin(), ::tolower);

    // Process confirmation
    if (confirmation == "yes")
    {
        //  Update room status
        // Ask for payment method
        cout << "How would you like to pay? (cash/upi): ";
        string paymentMethod;
        cin >> paymentMethod;

        // Normalize payment method input
        transform(paymentMethod.begin(), paymentMethod.end(), paymentMethod.begin(), ::tolower);

        if (paymentMethod == "cash" || paymentMethod == "upi")
        {
            //  Process payment
            selectedHotel.rooms[roomChoice - 1].isAvailable = false;
            cout << "\n=====================================================\n";
            cout << "  Payment of Rs. " << selectedHotel.rooms[roomChoice - 1].price
                 << " received via " << paymentMethod << ".\n";
            cout << "  Room booked successfully at " << selectedHotel.name << "!\n";
            cout << "=====================================================\n";

            // Save comprehensive booking details to file
            string bookingDetails = "Booking Confirmation\n" +
                                    string("==============================\n") +
                                    "Tourist Place: " + selectedPlace.name + "\n" +
                                    "Hotel: " + selectedHotel.name + "\n" +
                                    "Room Type: " + selectedHotel.rooms[roomChoice - 1].roomType + "\n" +
                                    "Price: Rs. " + to_string(selectedHotel.rooms[roomChoice - 1].price) + "\n" +
                                    "Payment Method: " + paymentMethod + "\n" +
                                    string("==============================\n");

            // Write to file
            ofstream outFile("booking_details.txt", ios::app); // Open the file in append mode
            outFile << bookingDetails << endl;
            outFile.close();
        }
        else
        {
            //  Process payment
            cout << "\nInvalid payment method! Booking process aborted.\n";
        }
    }
    else
    {
        //  Process payment
        cout << "\nBooking process aborted. Returning to the main menu...\n";
    }
            }
            else
            {
                cout << "\nExiting to the main menu...\n";
            }
            break;
        }

        case 3: { // Budget hotels
            if (budgetHotels.empty())
            {
                cout << "\nNo Budget hotels are available for booking.\n";
                break;
            }

            cout << "\nBudget Hotels:\n";
            for (size_t i = 0; i < budgetHotels.size(); ++i) {
                cout << i + 1 << ". " << budgetHotels[i].name;
                cout << " \t(Rating: " << budgetHotels[i].rating << ")"<< "\n";
            }

            // Ask the user if they want to book a room
            cout << "\nWould you like to book a room? (yes/no): ";
            string bookChoice;
            cin >> bookChoice;
            transform(bookChoice.begin(), bookChoice.end(), bookChoice.begin(), ::tolower);

            if (bookChoice == "yes") {
                // Ask the user to choose a hotel to book

                cout << "\nProceeding to Booking...\n";
                cout << "\nEnter the hotel number to book a room : ";
                int hotelChoice;
                cin >> hotelChoice;

                while (hotelChoice < 1 || hotelChoice > budgetHotels.size())
                {
                    cout << "\nInvalid input! \nPlease enter a valid hotel number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> hotelChoice;
                }

                Hotel& selectedHotel = budgetHotels[hotelChoice - 1];
                // Display available rooms in the selected hotel
    cout << "\n=====================================================\n";
    cout << "      Selected Hotel: " << selectedHotel.name << "\n";
    cout << "=====================================================\n";

    cout << "\nAvailable Rooms:\n";
    bool roomAvailable = false;
    for (size_t i = 0; i < selectedHotel.rooms.size(); ++i)
    {
        //  Check if the room is available

        if (selectedHotel.rooms[i].isAvailable)
        {
            //  Check if the room is available

            roomAvailable = true;
            cout << i + 1 << ". " << selectedHotel.rooms[i].roomType
                 << " - Rs. " << selectedHotel.rooms[i].price << "\n";
        }
    }

    if (!roomAvailable)
    {
        //  Check if the room is available

        cout << "\nNo rooms are available in " << selectedHotel.name << ".\n";
        return;
    }

    // Prompt user to select a room

    cout << "\nEnter the room number to book : ";
    int roomChoice;
    cin >> roomChoice;

    while (roomChoice < 1 || roomChoice > selectedHotel.rooms.size() || !selectedHotel.rooms[roomChoice - 1].isAvailable)
    {
        //  Input validation
        cout << "\nInvalid room choice or the room is already booked! Please enter a valid room number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> roomChoice;
    }

    // Display confirmation message

    cout << "\nYou have selected:\n";
    cout << "Room Type: " << selectedHotel.rooms[roomChoice - 1].roomType << "\n";
    cout << "Price: Rs. " << selectedHotel.rooms[roomChoice - 1].price << "\n";
    cout << "Hotel: " << selectedHotel.name << "\n";
    cout << "Tourist Place: " << selectedPlace.name << "\n";
    cout << "-----------------------------------------------------\n";
    cout << "Confirm booking? (yes/no): ";
    string confirmation;
    cin >> confirmation;

    // Normalize user input to lowercase
    transform(confirmation.begin(), confirmation.end(), confirmation.begin(), ::tolower);

    // Process confirmation
    if (confirmation == "yes")
    {

        //  Update room status
        // Ask for payment method
        cout << "How would you like to pay? (cash/upi): ";
        string paymentMethod;
        cin >> paymentMethod;

        // Normalize payment method input
        transform(paymentMethod.begin(), paymentMethod.end(), paymentMethod.begin(), ::tolower);


        if (paymentMethod == "cash" || paymentMethod == "upi")
        {
            //  Process payment
            selectedHotel.rooms[roomChoice - 1].isAvailable = false;
            cout << "\n=====================================================\n";
            cout << "  Payment of Rs. " << selectedHotel.rooms[roomChoice - 1].price
                 << " received via " << paymentMethod << ".\n";
            cout << "  Room booked successfully at " << selectedHotel.name << "!\n";
            cout << "=====================================================\n";

            // Save comprehensive booking details to file
            string bookingDetails = "Booking Confirmation\n" +
                                    string("==============================\n") +
                                    "Tourist Place: " + selectedPlace.name + "\n" +
                                    "Hotel: " + selectedHotel.name + "\n" +
                                    "Room Type: " + selectedHotel.rooms[roomChoice - 1].roomType + "\n" +
                                    "Price: Rs. " + to_string(selectedHotel.rooms[roomChoice - 1].price) + "\n" +
                                    "Payment Method: " + paymentMethod + "\n" +
                                    string("==============================\n");

            // Write to file
            ofstream outFile("booking_details.txt", ios::app); // Open the file in append mode
            outFile << bookingDetails << endl;
            outFile.close();
        }
        else
        {
            //  Process payment
            cout << "\nInvalid payment method! Booking process aborted.\n";
        }
    }
    else
    {
        //  Process payment
        cout << "\nBooking process aborted. Returning to the main menu...\n";
    }
            }
            else
            {
                cout << "\nExiting to the main menu...\n";
            }
            break;
        }

        default:
        {

            // Invalid choice
            cout << "\nInvalid choice! Please choose a valid option (1, 2, or 3).\n";
            break;
        }
    }
}



// Function to handle main menu option
void handleMainMenuOption(int choice, vector<TouristPlace>& places)
{

    switch (choice)
    {
        case 1:
            handleTouristPlaceSelection(places);
            break;

        case 2:
            searchAccommodation(places);
            break;

        case 3:
            displayHotelsByRating(places);
            break;

        case 4:
            bookRoom(places);
            break;

        case 5:
            cancelBooking(places);
            break;

        case 6:
            viewdetails();
            break;

        case 7:
            collectFeedback();
            break;

       // case 8:
          //  thankyou();
          //     loginUser();


        default:
            cout << "\nInvalid option! \nPlease choose a valid option.\n";
            break;
    }

}





  void manageAccommodation()
{
    // Sample Data with More Hotels

    vector<TouristPlace> places = {
        TouristPlace(
            "Hampi",
            "Vijayanagar",
            {
                "Temple visits",
                "Exploring ruins",
                "Coracle Ride on Tungabhadra River",
                "Sunset View from Hemakuta Hill",
                "Boulder climbing",
                "Archaeological Museum Visit"
            },
            {
                Hotel(
                    "Vijaya Residency",
                    2.0,
                    0, // pricePerNight (not used in the code)
                    {
                        Room("Single Room", 2000, true),
                        Room("Double Room", 3500, true),
                        Room("Suite", 6000, false)
                    },
                    "Located near the Hampi Bazaar, with easy access to temples.",
                    "+91-8395225001",
                    3.8 // Rating for Vijaya Residency
                ),
                Hotel(
                    "Hampi Heritage Lodge",
                    1.2,
                    0, // pricePerNight (not used in the code)
                    {
                        Room("Single Room", 2200, true),
                        Room("Double Room", 3800, true),
                        Room("Suite", 6500, false)
                    },
                    "Traditional heritage stay with village vibes.",
                    "+91-8395201011",
                    4.0 // Rating for Hampi Heritage Lodge
                ),
                Hotel(
                    "Royal Hampi Resort",
                    3.5,
                    0, // pricePerNight (not used in the code)
                    {
                        Room("Single Room", 3200, true),
                        Room("Double Room", 5000, true),
                        Room("Suite", 9000, true)
                    },
                    "Luxurious resort with a view of the ruins.",
                    "+91-8395222062",
                    4.5 // Rating for Royal Hampi Resort
                ),
                Hotel(
                    "Coracle Inn",
                    4.0,
                    0, // pricePerNight (not used in the code)
                    {
                        Room("Single Room", 2700, false),
                        Room("Double Room", 4500, true),
                        Room("Suite", 7200, true)
                    },
                    "Great for nature lovers, with a riverside view.",
                    "+91-8395226060",
                    4.2 // Rating for Coracle Inn
                ),
                Hotel(
                    "Hampi Guest House",
                    1.5,
                    0, // pricePerNight (not used in the code)
                    {
                        Room("Single Room", 1800, true),
                        Room("Double Room", 3000, false),
                        Room("Suite", 5000, false)
                    },
                    "Budget-friendly, walking distance from the Virupaksha Temple.",
                    "+91-8395222311",
                    3.6 // Rating for Hampi Guest House
                )
            }
        ),
        TouristPlace(
    "Bhadra Wildlife Sanctuary",
    "Chikmagalur, Karnataka",
    {
        "Safari ride to spot tigers and elephants",
        "Bird watching in the sanctuary",
        "Trekking to the Bhadra River",
        "Visit the Honnemaradu Reservoir",
        "Jungle walks with guides",
        "Camping near the river bank"
    },
    {
        Hotel(
            "Bhadra River Resort",
            3.5,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 2500, true),
                Room("Double Room", 4500, true),
                Room("Suite", 8000, false)
            },
            "Located near the river with a serene view and wildlife experience.",
            "+91-8192837456",
            4.6 // Rating for Bhadra River Resort
        ),
        Hotel(
            "Wildlife Haven Lodge",
            3.0,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 2200, true),
                Room("Double Room", 4000, true),
                Room("Suite", 7000, false)
            },
            "A lodge near the sanctuary offering wildlife safaris and bird watching.",
            "+91-9441035792",
            4.4 // Rating for Wildlife Haven Lodge
        ),
        Hotel(
            "Jungle View Resort",
            2.7,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 2100, true),
                Room("Double Room", 3700, true),
                Room("Suite", 6500, false)
            },
            "A nature-inspired resort located deep within the jungle.",
            "+91-9635742824",
            4.2 // Rating for Jungle View Resort
        ),
        Hotel(
            "Bhadra Guest House",
            1.5,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 1700, true),
                Room("Double Room", 3200, false),
                Room("Suite", 5800, false)
            },
            "Budget guest house located near the sanctuary entrance.",
            "+91-9741356548",
            3.5 // Rating for Bhadra Guest House
        ),
        Hotel(
            "Bhadra Sanctuary Resort",
            4.2,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 2900, true),
                Room("Double Room", 4800, true),
                Room("Suite", 8500, true)
            },
            "A luxury resort located within the sanctuary, offering guided safaris.",
            "+91-9443202376",
            4.8 // Rating for Bhadra Sanctuary Resort
        )
    }
),

    TouristPlace(
        "Mysore",
        "Mysuru",
        {
            "Mysore Palace",
            "Chamundeshwari Temple",
            "Brindavan Gardens",
            "St. Philomena's Church",
            "KRS Dam"
        },
        {
            Hotel(
                "Radisson Blu Plaza Hotel",
                2.5,
                0,
                {
                    Room("Single Room", 3500, true),
                    Room("Double Room", 5000, true),
                    Room("Suite", 8500, false)
                },
                "Luxurious hotel with great amenities and proximity to Mysore Palace.",
                "+91-821-4255555",
                4.3
            ),
            Hotel(
                "Royal Orchid Metropole",
                1.8,
                0,
                {
                    Room("Single Room", 3000, true),
                    Room("Double Room", 4800, true),
                    Room("Suite", 7500, false)
                },
                "Classic heritage hotel located near the city center.",
                "+91-821-2425600",
                4.1
            ),
            Hotel(
                "Lalitha Mahal Palace Hotel",
                3.0,
                0,
                {
                    Room("Single Room", 2800, true),
                    Room("Double Room", 4500, true),
                    Room("Suite", 7000, true)
                },
                "Palatial hotel with a grand experience and vintage vibes.",
                "+91-821-2515151",
                4.5
            )
        }
    ),
    TouristPlace(
    "Agumbe",
    "Western Ghats, Karnataka",
    {
        "Visit the Agumbe Rainforest Research Station",
        "Trek to Sunset Point",
        "Explore the Barkana Waterfalls",
        "Nature walk in the rainforests",
        "Spot the famous King Cobra",
        "Photography at the Agumbe village"
    },
    {
        Hotel(
            "Agumbe Rainforest Resort",
            3.0,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 2500, true),
                Room("Double Room", 4500, true),
                Room("Suite", 8000, false)
            },
            "A beautiful rainforest resort with a serene view.",
            "+91-9742886766",
            4.3 // Rating for Agumbe Rainforest Resort
        ),
        Hotel(
            "Monsoon Bliss Resort",
            4.0,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 3000, true),
                Room("Double Room", 5000, true),
                Room("Suite", 9000, true)
            },
            "A luxury resort with views of the dense forest and misty hills.",
            "+91-9745922334",
            4.7 // Rating for Monsoon Bliss Resort
        ),
        Hotel(
            "Gokanra Retreat",
            3.5,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 2200, true),
                Room("Double Room", 4000, true),
                Room("Suite", 7500, false)
            },
            "A peaceful retreat located near the Gokanra forest.",
            "+91-9483487624",
            4.1 // Rating for Gokanra Retreat
        ),
        Hotel(
            "Agumbe Guest House",
            2.0,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 1800, true),
                Room("Double Room", 3500, false),
                Room("Suite", 6000, false)
            },
            "A budget-friendly option located near the village.",
            "+91-9976234581",
            3.6 // Rating for Agumbe Guest House
        )
    }
),
TouristPlace(
    "Gokarna",
    "Coastal Karnataka",
    {
        "Visit the Om Beach",
        "Relax at Kudle Beach",
        "Explore the Mirjan Fort",
        "Visit the Gokarna Temple",
        "Trekking along the beaches",
        "Camp under the stars at Half Moon Beach"
    },
    {
        Hotel(
            "Om Beach Resort",
            4.2,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 2800, true),
                Room("Double Room", 4500, true),
                Room("Suite", 8500, false)
            },
            "Located on Om Beach with a perfect view of the sunset.",
            "+91-9449320105",
            4.5 // Rating for Om Beach Resort
        ),
        Hotel(
            "Kudle Beach View Resort",
            3.5,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 2500, true),
                Room("Double Room", 4200, true),
                Room("Suite", 7500, false)
            },
            "A peaceful resort with views of the beautiful Kudle Beach.",
            "+91-9448267342",
            4.3 // Rating for Kudle Beach View Resort
        ),
        Hotel(
            "Namaste Cafe and Beach Resort",
            2.8,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 1800, true),
                Room("Double Room", 3200, true),
                Room("Suite", 6000, false)
            },
            "A beachfront cafe and resort offering an amazing stay experience.",
            "+91-8383007607",
            3.9 // Rating for Namaste Cafe and Beach Resort
        ),
        Hotel(
            "Gokarna International Beach Resort",
            3.8,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 3500, true),
                Room("Double Room", 5500, true),
                Room("Suite", 9500, true)
            },
            "Luxurious beach resort offering stunning views and top-notch amenities.",
            "+91-9343832087",
            4.6 // Rating for Gokarna International Beach Resort
        ),
        Hotel(
            "Gokarna Guest House",
            1.5,
            0, // pricePerNight (not used in the code)
            {
                Room("Single Room", 1200, true),
                Room("Double Room", 2500, false),
                Room("Suite", 4500, false)
            },
            "Budget-friendly guest house, walking distance from the beach.",
            "+91-9612345780",
            3.4 // Rating for Gokarna Guest House
        )
    }
),
    TouristPlace(
        "Bangalore",
        "Bangalore Urban",
        {
            "Bangalore Palace",
            "Lalbagh Botanical Garden",
            "Vidhana Soudha",
            "Nandi Hills",
            "ISKCON Temple",
            "Cubbon Park",
            "Bannerghatta Biological Park"
        },
        {
            Hotel(
                "Taj West End",
                3.0,
                0,
                {
                    Room("Single Room", 7000, true),
                    Room("Double Room", 9500, true),
                    Room("Suite", 15000, false)
                },
                "Luxury hotel offering a peaceful getaway in the heart of Bangalore.",
                "+91-80-66605656",
                4.7
            ),
            Hotel(
                "The Leela Palace",
                4.0,
                0,
                {
                    Room("Single Room", 8000, true),
                    Room("Double Room", 11000, true),
                    Room("Suite", 18000, false)
                },
                "One of Bangalore's finest, known for its impeccable service and elegant design.",
                "+91-80-25211234",
                4.8
            ),
            Hotel(
                "Sheraton Grand Bangalore Hotel",
                2.5,
                0,
                {
                    Room("Single Room", 5500, true),
                    Room("Double Room", 7800, true),
                    Room("Suite", 13000, true)
                },
                "A modern hotel with great facilities and a central location.",
                "+91-80-42521000",
                4.3
            )
        }
    ),
    TouristPlace(
        "Coorg",
        "Kodagu",
        {
            "Abbey Falls",
            "Raja's Seat",
            "Dubare Elephant Camp",
            "Namdroling Monastery",
            "Coffee Plantations"
        },
        {
            Hotel(
                "The Tamara Coorg",
                5.0,
                0,
                {
                    Room("Single Room", 11000, true),
                    Room("Double Room", 15000, true),
                    Room("Suite", 25000, true)
                },
                "Luxury resort surrounded by nature, offering great views of the coffee plantations.",
                "+91-8272-281999",
                4.7
            ),
            Hotel(
                "Eagle Ridge Resort",
                3.0,
                0,
                {
                    Room("Single Room", 4500, true),
                    Room("Double Room", 7500, true),
                    Room("Suite", 12000, false)
                },
                "Nestled in the hills, perfect for a peaceful retreat.",
                "+91-8272-276600",
                4.1
            ),
            Hotel(
                "Coorg Wilderness Resort",
                4.5,
                0,
                {
                    Room("Single Room", 9000, true),
                    Room("Double Room", 13000, true),
                    Room("Suite", 20000, true)
                },
                "Exclusive eco-friendly resort with a private atmosphere.",
                "+91-8272-225555",
                4.6
            )
        }
    ),
    TouristPlace(
        "Chikmagalur",
        "Chikmagalur",
        {
            "Mullayanagiri",
            "Baba Budangiri",
            "Hebbe Falls",
            "Coffee Plantations",
            "Kudremukh National Park"
        },
        {
            Hotel(
                "The Serai Chikmagalur",
                4.5,
                0,
                {
                    Room("Single Room", 9500, true),
                    Room("Double Room", 13500, true),
                    Room("Suite", 20000, true)
                },
                "A serene resort offering an unforgettable experience amid lush green landscapes.",
                "+91-8262-226000",
                4.6
            ),
            Hotel(
                "Gateway Chikmagalur",
                3.0,
                0,
                {
                    Room("Single Room", 5500, true),
                    Room("Double Room", 8000, true),
                    Room("Suite", 13000, false)
                },
                "Peaceful hotel with great views and a cozy ambiance.",
                "+91-8262-232040",
                4.2
            ),
            Hotel(
                "Java Rain Resorts",
                3.5,
                0,
                {
                    Room("Single Room", 8000, true),
                    Room("Double Room", 12000, true),
                    Room("Suite", 18000, true)
                },
                "Exclusive resort offering a luxurious experience with breathtaking views.",
                "+91-8262-260000",
                4.7
            )
        }
    ),
    TouristPlace(
        "Udupi",
        "Udupi",
        {
            "Sri Krishna Temple",
            "St. Mary's Island",
            "Malpe Beach",
            "Udupi Manipal"
        },
        {
            Hotel(
                "Hotel Udupi Residency",
                2.5,
                0,
                {
                    Room("Single Room", 2000, true),
                    Room("Double Room", 3500, true),
                    Room("Suite", 5000, false)
                },
                "Budget-friendly hotel near the Sri Krishna Temple.",
                "+91-820-2551077",
                3.9
            ),
            Hotel(
                "Paradise Isle Beach Resort",
                1.8,
                0,
                {
                    Room("Single Room", 3500, true),
                    Room("Double Room", 5500, true),
                    Room("Suite", 8000, true)
                },
                "Beachfront resort with a relaxing atmosphere and excellent facilities.",
                "+91-820-2585302",
                4.4
            ),
            Hotel(
                "The Ocean Pearl",
                2.0,
                0,
                {
                    Room("Single Room", 2700, true),
                    Room("Double Room", 4300, true),
                    Room("Suite", 6500, false)
                },
                "Comfortable stay with a close proximity to the beach.",
                "+91-820-2222222",
                4.3
            )
        }
    ),
    TouristPlace(
        "Hassan",
        "Hassan",
        {
            "Hoysaleswara Temple",
            "Chennakesava Temple",
            "Shravanabelagola (Bahubali Statue)"
        },
        {
            Hotel(
                "Hoysala Village Resort",
                3.5,
                0,
                {
                    Room("Single Room", 3000, true),
                    Room("Double Room", 4500, true),
                    Room("Suite", 7000, false)
                },
                "A heritage-style resort offering a peaceful stay amidst nature.",
                "+91-8177-256800",
                4.4
            ),
            Hotel(
                "Hotel S K Regency",
                1.8,
                0,
                {
                    Room("Single Room", 2200, true),
                    Room("Double Room", 3500, true),
                    Room("Suite", 5500, false)
                },
                "Budget-friendly hotel with easy access to the city�s attractions.",
                "+91-8177-227100",
                3.9
            ),
            Hotel(
                "KSRTC Guest House",
                2.0,
                0,
                {
                    Room("Single Room", 1800, true),
                    Room("Double Room", 2800, true),
                    Room("Suite", 4500, false)
                },
                "Basic guest house for budget travelers.",
                "+91-8177-226200",
                3.5
            )
        }
    ),
    TouristPlace(
        "Kabini",
        "Kodagu",
        {
            "Kabini River",
            "Kabini Wildlife Sanctuary",
            "Jungle Safaris"
        },
        {
            Hotel(
                "The Serai Kabini",
                4.5,
                0,
                {
                    Room("Single Room", 12000, true),
                    Room("Double Room", 18000, true),
                    Room("Suite", 25000, true)
                },
                "Luxury resort offering safaris and pristine views of the Kabini River.",
                "+91-8272-273000",
                4.7
            ),
            Hotel(
                "Orange County Kabini",
                4.0,
                0,
                {
                    Room("Single Room", 15000, true),
                    Room("Double Room", 22000, true),
                    Room("Suite", 30000, false)
                },
                "Exclusive resort with nature trails and beautiful riverside views.",
                "+91-8272-236000",
                4.6
            ),
            Hotel(
                "Kabini River Lodge",
                3.8,
                0,
                {
                    Room("Single Room", 8000, true),
                    Room("Double Room", 12000, true),
                    Room("Suite", 20000, true)
                },
                "Government-run lodge with rustic charm and safari options.",
                "+91-8272-271350",
                4.2
            )
        }
    ),
    TouristPlace(
        "Badami",
        "Bagalkot",
        {
            "Badami Caves",
            "Agastya Lake",
            "Bhuthnatha Temple",
            "Fort",
            "Palace"
        },
        {
            Hotel(
                "KSTDC Hotel Mayura Chalukya",
                2.5,
                0,
                {
                    Room("Single Room", 2500, true),
                    Room("Double Room", 4000, true),
                    Room("Suite", 6000, false)
                },
                "Simple government-run hotel with close access to the Badami Caves.",
                "+91-8382-220500",
                3.8
            ),
            Hotel(
                "Badami Court Hotel",
                3.0,
                0,
                {
                    Room("Single Room", 3500, true),
                    Room("Double Room", 5000, true),
                    Room("Suite", 7000, false)
                },
                "Comfortable hotel offering modern amenities and proximity to key attractions.",
                "+91-8382-220233",
                4.2
            ),
            Hotel(
                "Hotel Mayura",
                2.0,
                0,
                {
                    Room("Single Room", 1800, true),
                    Room("Double Room", 2800, true),
                    Room("Suite", 4200, false)
                },
                "Basic stay option, suitable for budget travelers.",
                "+91-8382-220366",
                3.5
            )
        }
    ),
    TouristPlace(
        "Bijapur",
        "Vijayapura",
        {
            "Gol Gumbaz",
            "Bijapur Fort",
            "Ibrahim Rauza",
            "Jami Masjid"
        },
        {
            Hotel(
                "Hotel Basava Residency",
                2.5,
                0,
                {
                    Room("Single Room", 2500, true),
                    Room("Double Room", 3800, true),
                    Room("Suite", 5500, false)
                },
                "Affordable hotel with access to local landmarks.",
                "+91-8382-263344",
                3.7
            ),
            Hotel(
                "KSTDC Hotel Mayura",
                1.8,
                0,
                {
                    Room("Single Room", 2200, true),
                    Room("Double Room", 3500, true),
                    Room("Suite", 5000, false)
                },
                "Basic government-run hotel with clean rooms.",
                "+91-8382-262800",
                3.6
            ),
            Hotel(
                "Kishkinda Heritage Resort",
                3.2,
                0,
                {
                    Room("Single Room", 3000, true),
                    Room("Double Room", 4500, true),
                    Room("Suite", 6500, false)
                },
                "Heritage-style resort near Bijapur Fort, perfect for history lovers.",
                "+91-8382-261208",
                4.0
            )
        }
    ),
    TouristPlace(
        "Dandeli",
        "Uttara Kannada",
        {
            "Dandeli Wildlife Sanctuary",
            "Kali River",
            "Syntheri Rocks",
            "Anshi National Park"
        },
        {
            Hotel(
                "Dandeli Jungle Resort",
                3.5,
                0,
                {
                    Room("Single Room", 3500, true),
                    Room("Double Room", 5000, true),
                    Room("Suite", 7500, true)
                },
                "Eco-friendly resort located near Dandeli Wildlife Sanctuary with various activities.",
                "+91-8382-264409",
                4.5
            ),
            Hotel(
                "The River View Resort",
                3.0,
                0,
                {
                    Room("Single Room", 3000, true),
                    Room("Double Room", 4500, true),
                    Room("Suite", 6500, false)
                },
                "Secluded riverside resort offering serene views and adventure activities.",
                "+91-8382-223344",
                4.1
            ),
            Hotel(
                "Wilderness Resort",
                2.8,
                0,
                {
                    Room("Single Room", 2800, true),
                    Room("Double Room", 4200, true),
                    Room("Suite", 6000, false)
                },
                "Basic resort with options for jungle safaris and nature walks.",
                "+91-8382-228888",
                4.0
            )
        }
    ),
    TouristPlace(
        "Sakleshpur",
        "Hassan",
        {
            "Manjarabad Fort",
            "Bisle Reserve Forest",
            "Coffee Plantations",
            "Sakleshpur Hills"
        },
        {
            Hotel(
                "The Coffee Bean Resort",
                4.0,
                0,
                {
                    Room("Single Room", 7000, true),
                    Room("Double Room", 10000, true),
                    Room("Suite", 15000, true)
                },
                "A cozy resort set amidst coffee plantations and serene hills.",
                "+91-8197-203400",
                4.5
            ),
            Hotel(
                "The Windflower Resort & Spa",
                4.5,
                0,
                {
                    Room("Single Room", 9000, true),
                    Room("Double Room", 13000, true),
                    Room("Suite", 18000, true)
                },
                "Exclusive resort with beautiful views, wellness facilities, and modern amenities.",
                "+91-820-2571100",
                4.6
            ),
            Hotel(
                "Hoysala Village Resort",
                3.0,
                0,
                {
                    Room("Single Room", 4000, true),
                    Room("Double Room", 6000, true),
                    Room("Suite", 9000, false)
                },
                "Classic resort offering traditional ambiance and a relaxing environment.",
                "+91-820-2506099",
                4.2
            )
        }
    ),
    TouristPlace(
        "Tungabhadra Dam",
        "Hospet, Ballari",
        {
            "Tungabhadra Dam",
            "Anjaneya Hill",
            "Hampi"
        },
        {
            Hotel(
                "Vijaya Residency",
                2.0,
                0,
                {
                    Room("Single Room", 2000, true),
                    Room("Double Room", 3500, true),
                    Room("Suite", 6000, false)
                },
                "Located near the Hampi Bazaar, with easy access to temples.",
                "+91-8395225001",
                3.8
            ),
            Hotel(
                "Hampi Heritage Lodge",
                1.2,
                0,
                {
                    Room("Single Room", 2200, true),
                    Room("Double Room", 3800, true),
                    Room("Suite", 6500, false)
                },
                "Traditional heritage stay with village vibes.",
                "+91-8395201011",
                4.0
            ),
            Hotel(
                "Royal Hampi Resort",
                3.5,
                0,
                {
                    Room("Single Room", 3200, true),
                    Room("Double Room", 5000, true),
                    Room("Suite", 9000, true)
                },
                "Luxurious resort with a view of the ruins.",
                "+91-8395222062",
                4.5
            )
        }
    ),
    TouristPlace(
        "Ranganathittu Bird Sanctuary",
        "Mandya",
        {
            "Bird Watching",
            "Crocodile Observation",
            "Nature Walks"
        },
        {
            Hotel(
                "KSTDC Mayura Ranganathittu",
                2.0,
                0,
                {
                    Room("Single Room", 2500, true),
                    Room("Double Room", 4000, true),
                    Room("Suite", 5500, false)
                },
                "Affordable stay with proximity to the bird sanctuary.",
                "+91-8272-256200",
                3.8
            ),
            Hotel(
                "Hotel Kadamba",
                2.5,
                0,
                {
                    Room("Single Room", 2000, true),
                    Room("Double Room", 3500, true),
                    Room("Suite", 5000, false)
                },
                "Budget hotel ideal for short stays with basic facilities.",
                "+91-8272-240500",
                3.6
            ),
            Hotel(
                "Srirangapatna Resort",
                3.2,
                0,
                {
                    Room("Single Room", 3500, true),
                    Room("Double Room", 5000, true),
                    Room("Suite", 7000, true)
                },
                "Eco-resort with river views and bird watching activities.",
                "+91-8272-243400",
                4.0
            )
        }
    ),
    TouristPlace(
        "Channarayana Durga",
        "Tumkur",
        {
            "Hill Trekking",
            "Channarayana Durga Fort"
        },
        {
            Hotel(
                "Hotel Sai Prakash",
                2.0,
                0,
                {
                    Room("Single Room", 2200, true),
                    Room("Double Room", 3500, true),
                    Room("Suite", 5000, false)
                },
                "Simple and affordable hotel located in Tumkur.",
                "+91-8162-251433",
                3.7
            ),
            Hotel(
                "Channarayana Durga Resort",
                3.5,
                0,
                {
                    Room("Single Room", 3000, true),
                    Room("Double Room", 4500, true),
                    Room("Suite", 6500, false)
                },
                "Resort offering a peaceful stay with views of the Channarayana Durga hills.",
                "+91-8162-239800",
                4.0
            ),
            Hotel(
                "Tumkur Viceroy",
                2.5,
                0,
                {
                    Room("Single Room", 1800, true),
                    Room("Double Room", 2800, true),
                    Room("Suite", 4200, false)
                },
                "Budget hotel with basic amenities.",
                "+91-8162-252221",
                3.5
            )
        }
    ),
    TouristPlace(
        "Shravanabelagola",
        "Hassan",
        {
            "Gommateshwara (Bahubali Statue)",
            "Shravanabelagola Hill",
            "Jain Temples"
        },
        {
            Hotel(
                "Shravanabelagola Resort",
                3.0,
                0,
                {
                    Room("Single Room", 2500, true),
                    Room("Double Room", 4000, true),
                    Room("Suite", 6000, false)
                },
                "Simple stay near the famous Bahubali statue.",
                "+91-8177-258300",
                3.9
            ),
            Hotel(
                "Hotel Mayura",
                2.5,
                0,
                {
                    Room("Single Room", 2000, true),
                    Room("Double Room", 3500, true),
                    Room("Suite", 5000, false)
                },
                "Basic and budget-friendly stay option.",
                "+91-8177-258500",
                3.6
            ),
            Hotel(
                "Jain Heritage Stay",
                3.2,
                0,
                {
                    Room("Single Room", 3000, true),
                    Room("Double Room", 4500, true),
                    Room("Suite", 7000, false)
                },
                "A serene and quiet stay with a heritage touch.",
                "+91-8177-259000",
                4.1
            )
        }
    ),
    TouristPlace(
        "Mangaluru",
        "Dakshina Kannada",
        {
            "Panambur Beach",
            "Tannirbhavi Beach",
            "St. Aloysius Chapel",
            "Kudroli Gokarnath Temple"
        },
        {
            Hotel(
                "The Gateway Hotel",
                3.5,
                0,
                {
                    Room("Single Room", 4000, true),
                    Room("Double Room", 6000, true),
                    Room("Suite", 8000, true)
                },
                "A luxury hotel with modern amenities and excellent service.",
                "+91-824-2222200",
                4.3
            ),
            Hotel(
                "Hotel Moti Mahal",
                2.0,
                0,
                {
                    Room("Single Room", 2500, true),
                    Room("Double Room", 4000, true),
                    Room("Suite", 5500, false)
                },
                "Affordable hotel with a convenient location in Mangaluru.",
                "+91-824-2225600",
                3.8
            ),
            Hotel(
                "Ocean Pearl",
                2.8,
                0,
                {
                    Room("Single Room", 3500, true),
                    Room("Double Room", 5000, true),
                    Room("Suite", 7000, false)
                },
                "Beachside hotel with a relaxing atmosphere and top-notch services.",
                "+91-824-6667777",
                4.1
            )
        }
    ),
    TouristPlace(
        "Kudremukh",
        "Chikmagalur",
        {
            "Kudremukh National Park",
            "Kudremukh Peak",
            "Trekking"
        },
        {
            Hotel(
                "Kudremukh Wildlife Resort",
                3.5,
                0,
                {
                    Room("Single Room", 3500, true),
                    Room("Double Room", 5000, true),
                    Room("Suite", 7000, true)
                },
                "A tranquil resort offering trekking and nature walks.",
                "+91-8262-277555",
                4.5
            ),
            Hotel(
                "Trekking Trails Resort",
                3.0,
                0,
                {
                    Room("Single Room", 2500, true),
                    Room("Double Room", 4000, true),
                    Room("Suite", 6000, false)
                },
                "Budget-friendly stay with trekking options to explore the natural beauty.",
                "+91-8262-273300",
                4.0
            ),
            Hotel(
                "Chikmagalur Coffee Estate Resort",
                4.2,
                0,
                {
                    Room("Single Room", 5000, true),
                    Room("Double Room", 7000, true),
                    Room("Suite", 9000, true)
                },
                "A resort located within coffee plantations offering serene views and hiking trails.",
                "+91-8262-290000",
                4.6
            )
        }
    ),
    TouristPlace(
        "Kalburgi",
        "Kalaburagi",
        {
            "Gulbarga Fort",
            "Jama Masjid",
            "Sharana Basaveshwara Temple"
        },
        {
            Hotel(
                "Hotel KSTDC Mayura",
                2.5,
                0,
                {
                    Room("Single Room", 2000, true),
                    Room("Double Room", 3500, true),
                    Room("Suite", 5000, false)
                },
                "Located near the Gulbarga Fort, this hotel offers basic amenities.",
                "+91-8482-240800",
                3.6
            ),
            Hotel(
                "Hotel Ashoka",
                3.0,
                0,
                {
                    Room("Single Room", 2500, true),
                    Room("Double Room", 4000, true),
                    Room("Suite", 5500, false)
                },
                "Budget-friendly stay with simple comforts, located centrally in Kalaburagi.",
                "+91-8482-256990",
                3.7
            ),
            Hotel(
                "Siri Hotel",
                2.8,
                0,
                {
                    Room("Single Room", 2200, true),
                    Room("Double Room", 3800, true),
                    Room("Suite", 6000, false)
                },
                "Ideal for short stays with comfortable rooms and basic services.",
                "+91-8482-258000",
                3.5
            )
        }
    ),
    TouristPlace(
        "Yellapur",
        "Uttara Kannada",
        {
            "Magod Falls",
            "Sathodi Falls",
            "Unchalli Falls",
            "Suwarna River"
        },
        {
            Hotel(
                "Sathodi Resort",
                3.2,
                0,
                {
                    Room("Single Room", 2500, true),
                    Room("Double Room", 4000, true),
                    Room("Suite", 6000, false)
                },
                "Peaceful resort surrounded by waterfalls and lush greenery.",
                "+91-8392-279200",
                4.0
            ),
            Hotel(
                "Unchalli Falls Eco Retreat",
                3.8,
                0,
                {
                    Room("Single Room", 3000, true),
                    Room("Double Room", 4500, true),
                    Room("Suite", 7000, false)
                },
                "Eco-friendly retreat with river views and trekking opportunities.",
                "+91-8392-280500",
                4.2
            ),
            Hotel(
                "Yellapur Jungle Lodge",
                4.0,
                0,
                {
                    Room("Single Room", 2200, true),
                    Room("Double Room", 3500, true),
                    Room("Suite", 5500, false)
                },
                "Jungle lodge offering an adventurous stay with wildlife experiences.",
                "+91-8392-285600",
                4.1
            )
        }
    ),
    TouristPlace(
        "Chamundi Hill",
        "Mysuru",
        {
            "Chamundeshwari Temple",
            "Chamundi Hill Viewpoint",
            "Nandi Statue"
        },
        {
            Hotel(
                "Hotel Royal Orchid",
                3.5,
                0,
                {
                    Room("Single Room", 4000, true),
                    Room("Double Room", 6000, true),
                    Room("Suite", 8000, true)
                },
                "A luxury hotel with breathtaking views of the Chamundi Hill.",
                "+91-821-2423030",
                4.2
            ),
            Hotel(
                "Hotel Southern Star",
                2.8,
                0,
                {
                    Room("Single Room", 2500, true),
                    Room("Double Room", 4000, true),
                    Room("Suite", 5500, false)
                },
                "Comfortable mid-range hotel offering convenient access to the Chamundi Hill temple.",
                "+91-821-2425225",
                3.9
            ),
            Hotel(
                "KSTDC Mayura",
                2.0,
                0,
                {
                    Room("Single Room", 2000, true),
                    Room("Double Room", 3500, true),
                    Room("Suite", 5000, false)
                },
                "Budget-friendly hotel with basic amenities and a good location.",
                "+91-821-2412300",
                3.5
            ),
            Hotel(
                "Ramada by Wyndham",
                3.0,
                0,
                {
                    Room("Single Room", 3500, true),
                    Room("Double Room", 5000, true),
                    Room("Suite", 7000, false)
                },
                "Upscale hotel with excellent amenities, located near Chamundi Hill.",
                "+91-821-2446000",
                4.3
            )
        }
    )
};

    // Display Welcome Message
   // displayWelcomeMessage();

    // Main Menu Loop
    while (true) {
        displayMenu();
        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 8)
        {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number (1-7): ";
        }

        handleMainMenuOption(choice, places);

        if (choice == 8)
        break;
    }
}



// Function declarations (assuming these functions are defined elsewhere in your code)
void registerUser();
bool loginUser();
bool adminLogin();
void addSchedule();
void updateSchedule();
void deleteSchedule();
void viewSchedules();
void searchScheduleByDate();
void addExpense();
void updateExpense();
void deleteExpense();
void viewExpenses();
void generateExpenseReport();
void updateUserDetails();
void displayUserDetails();
//void deleteUserAccount();
void viewAllUsers();
void deleteInactiveUsers();
void backupData();
void restoreData();
void sortSchedulesByDate();
void sortExpensesByCategory();
void searchExpenseByKeyword();
void findMaxExpense();
void findMinExpense();
void generateBudgetSummary();
void showMonthlyExpenseGraph();
void compareExpensesByCategory();
void generateTravelStatistics();
void notifyOverBudget();



// Structure to store destination details
struct Destination
{

    string name;
    string district;
    vector<string> tourist_spots;
    string category;
    // E.g., Historical, Hill Station, Beach, etc.
    double rating;
    // Ratings for destinations
};

// Booking structure

struct Booking
{
    string name;
    string from;
    string to;
    string transport_type;
    int people_count;
    string vehicle_type;
    double estimated_budget;

};

// Vector to store destinations
vector<Destination> destinations;
vector<Booking> bookings; // Vector to store bookings

// Function prototypes
void listAllDestinations();
void viewDestinationDetails();
void bookTrip();
void viewBookingDetails();
void filterByCategory();
void planItinerary();
void displayRatingsAndReviews();
void showMenu();

// List all destinations
void listAllDestinations()
{

    cout << "\nAvailable Destinations:" << endl;
    for (const auto& dest : destinations) {
        cout << "- " << dest.name << " (" << dest.category << ", " << dest.district << ")" << endl;

    }
}

// View details for a specific destination

void viewDestinationDetails()
{
    string destination_name;
    cout << "Enter destination name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, destination_name);
    bool found = false;

    for (const auto& dest : destinations)
    {
        if (dest.name == destination_name)
        {
            cout << "\nDetails of " << destination_name << ":\n"
                 << "District: " << dest.district << "\nCategory: " << dest.category
                 << "\nRating: " << dest.rating << "/5\nTourist Spots:\n";
            for (const string& spot : dest.tourist_spots)
            {
                cout << "- " << spot << endl;
            }
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "Destination not found!" << endl;
    }
}
void bookTrip() {
    Booking booking;
    cout << "\n--- Book a Trip ---" << endl;

    cout << "Enter your name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, booking.name);

    string phoneNumber, gender;
    int age;

    // Additional fields for booking details
    cout << "Enter your phone number: ";
    getline(cin, phoneNumber);

    cout << "Enter your gender (Male/Female/Other): ";
    getline(cin, gender);

    cout << "Enter your age: ";
    while (!(cin >> age) || age <= 0) {
        cout << "Invalid input. Enter a valid age: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter your starting location: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, booking.from);

    listAllDestinations();
    cout << "Enter the destination you want to visit: ";
    getline(cin, booking.to);

    // Check if the destination is valid
    bool found = false;
    for (const auto& dest : destinations) {
        if (dest.name == booking.to) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Invalid destination!" << endl;
        return;
    }

    cout << "Enter transport type (Cab, Bus, Train): ";
    cin >> booking.transport_type;

    cout << "Enter the number of people traveling: ";
    while (!(cin >> booking.people_count) || booking.people_count <= 0) {
        cout << "Invalid input. Enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Simplified vehicle type selection
    if (booking.transport_type == "Cab") {
        booking.vehicle_type = "Private Cab";
    } else if (booking.transport_type == "Bus") {
        booking.vehicle_type = "Shared Bus";
    } else if (booking.transport_type == "Train") {
        booking.vehicle_type = "Train Seat";
    } else {
        cout << "Invalid transport type. Booking canceled." << endl;
        return;
    }

    // Placeholder budget estimation
    booking.estimated_budget = booking.people_count * 500;

    // Store the booking
    bookings.push_back(booking);

    // Display a confirmation message with all details
    cout << "\n--- Booking Confirmed! ---\n";
    cout << "Name: " << booking.name << "\nPhone: " << phoneNumber
         << "\nGender: " << gender << "\nAge: " << age
         << "\nFrom: " << booking.from << "\nTo: " << booking.to
         << "\nTransport Type: " << booking.transport_type
         << "\nVehicle Type: " << booking.vehicle_type
         << "\nNumber of People: " << booking.people_count
         << "\nEstimated Budget: " << booking.estimated_budget << " INR\n";
}
// Book a trip
//View all booking details
void viewBookingDetails() {
    if (bookings.empty()) {
        cout << "\nNo bookings found!" << endl;
        return;
    }
    cout << "\n--- Booking Details ---" << endl;
    for (const auto& booking : bookings) {
        cout << "Name: " << booking.name << "\nFrom: " << booking.from << "\nTo: " << booking.to
             << "\nTransport Type: " << booking.transport_type << "\nVehicle Type: " << booking.vehicle_type
             << "\nPeople: " << booking.people_count << "\nEstimated Budget: " << booking.estimated_budget << " INR\n" << endl;
    }
}

// Filter destinations by category
void filterByCategory() {
    string category;
    cout << "Enter category (e.g., Historical, Hill Station, Beach): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, category);
    cout << "\nDestinations in category '" << category << "':" << endl;
    for (const auto& dest : destinations) {
        if (dest.category == category) {
            cout << "- " << dest.name << " (" << dest.district << ")" << endl;
        }
    }
}

// View ratings and reviews of a destination

void displayRatingsAndReviews()
{
    string destination_name;
    cout << "Enter the destination name to view its ratings: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, destination_name);

    bool found = false;
    for (const auto& dest : destinations)
    {
        if (dest.name == destination_name)
        {
            cout << "\n" << destination_name << " has a rating of " << dest.rating << "/5." << endl;
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "Destination not found!" << endl;
    }
}
void planItinerarys() {
    cout << "\n--- Plan Your Itinerary ---" << endl;
    listAllDestinations();

    vector<string> itinerary;
    string destination;

    cout << "\nSelect multiple destinations (type 'done' to finish):" << endl;

    while (true) {
        cout << "Enter destination name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear newline
        getline(cin, destination);

        if (destination == "done") break;

        bool found = false;
        for (const auto& dest : destinations) {
            if (dest.name == destination) {
                itinerary.push_back(destination);
                found = true;
                cout << "Added '" << destination << "' to your itinerary.\n";
                break;
            }
        }
        if (!found) {
            cout << "Invalid destination. Try again!" << endl;
        }
    }

    if (itinerary.empty()) {
        cout << "No destinations selected. Exiting itinerary planning.\n";
        return;
    }

    cout << "\nYour Itinerary:" << endl;
    for (const string& place : itinerary) {
        cout << "- " << place << endl;
    }

    cout << "Do you want to confirm this itinerary? (yes/no): ";
    string confirm;
    cin >> confirm;

    if (confirm == "no") {
        cout << "Restarting itinerary planning...\n";
        planItinerary();
        return;
    }

    cout << "\n--- Final Itinerary ---" << endl;
    for (const string& place : itinerary) {
        for (const auto& dest : destinations) {
            if (dest.name == place) {
                cout << "- " << dest.name << " (" << dest.category << ", " << dest.district << ")" << endl;
            }
        }
    }
    cout << "Enjoy your trip!\n";
}

// Plan an itinerary by selecting multiple destinations
void planItinerary()
{
    cout << "\n--- Plan Your Itinerary ---" << endl;
    listAllDestinations();
    cout << "Select multiple destinations (type 'done' to finish):" << endl;
    vector<string> itinerary;
    string destination;
    while (true)
    {
        cin >> destination;
        if (destination == "done") break;
        bool found = false;
        for (const auto& dest : destinations)
        {
            if (dest.name == destination) {
                itinerary.push_back(destination);
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "Invalid destination. Try again!" << endl;
        }
    }

    cout << "\nYour Itinerary:" << endl;
    for (const string& place : itinerary)
    {
        cout << "- " << place << endl;
    }
}

// Show the main menu
void showMenu()
{
    displayLine();
    cout << "\nTransportation Management System" << endl;
     displayLine();

    cout << "1. List all destinations\n2. View destination details\n3. Book a trip\n"
         << "4. View booking details\n5. Filter by category\n6. Plan itinerary\n7. View ratings\n8. Exit\n";
}
void addDestination()
{
    Destination new_dest;
    cout << "\nEnter destination name: ";
    cin.ignore();
    getline(cin, new_dest.name);

    cout << "Enter district: ";
    getline(cin, new_dest.district);

    cout << "Enter category (Historical, Beach, Hill Station, etc.): ";
    getline(cin, new_dest.category);

    cout << "Enter rating (out of 5): ";
    cin >> new_dest.rating;

    cout << "Enter tourist spots (type 'done' when finished):\n";
    string spot;
    while (true)
    {
        cin.ignore();
        getline(cin, spot);
        if (spot == "done") break;
        new_dest.tourist_spots.push_back(spot);
    }

    destinations.push_back(new_dest);
    cout << "New destination added successfully!" << endl;
}
void updateBookingDetails()
{
    int booking_id;
    cout << "Enter booking ID to update: ";
    cin >> booking_id;

    if (booking_id <= 0 || booking_id > bookings.size())
    {
        cout << "Invalid booking ID." << endl;
        return;
    }

    Booking &booking = bookings[booking_id - 1];
    cout << "Updating booking for " << booking.name << endl;
    cout << "Enter new transport type (Cab, Bus, Train): ";
    cin >> booking.transport_type;

    cout << "Enter the number of people traveling: ";
    while (!(cin >> booking.people_count) || booking.people_count <= 0)
    {
        cout << "Invalid input. Enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Simple budget update calculation (For the sake of simplicity)
    booking.estimated_budget = booking.people_count * 500;

    cout << "Booking updated! New estimated budget: " << booking.estimated_budget << " INR." << endl;
}

void deleteBooking()
{
    int booking_id;
    cout << "Enter booking ID to delete: ";
    cin >> booking_id;

    if (booking_id <= 0 || booking_id > bookings.size())
    {
        cout << "Invalid booking ID." << endl;
        return;
    }

    bookings.erase(bookings.begin() + booking_id - 1);
    cout << "Booking ID " << booking_id << " has been deleted successfully." << endl;
}

void calculateTotalEstimatedBudget()
{
    double total_budget = 0;
    for (const auto& booking : bookings) {
        total_budget += booking.estimated_budget;
    }

    cout << "Total estimated budget for all bookings: " << total_budget << " INR." << endl;
}

void showBookingHistory()
{
    string name;
    cout << "Enter your name to view your booking history: ";
    cin.ignore();
    getline(cin, name);

    cout << "\nBooking history for " << name << ":\n";
    bool found = false;

    for (const auto& booking : bookings)
    {
        if (booking.name == name) {
            cout << "From: " << booking.from << "\nTo: " << booking.to
                 << "\nTransport Type: " << booking.transport_type
                 << "\nPeople Count: " << booking.people_count
                 << "\nEstimated Budget: " << booking.estimated_budget << " INR\n\n";
            found = true;
        }
    }

    if (!found)
    {
        cout << "No bookings found for " << name << "!" << endl;
    }
}
void addNewDestination()
{
    Destination new_dest;
    cout << "\nEnter destination name: ";
    cin.ignore();
    getline(cin, new_dest.name);

    cout << "Enter district: ";
    getline(cin, new_dest.district);

    cout << "Enter category (Historical, Beach, Hill Station, etc.): ";
    getline(cin, new_dest.category);

    cout << "Enter rating (out of 5): ";
    cin >> new_dest.rating;

    cout << "Enter tourist spots (type 'done' when finished):\n";
    string spot;
    while (true)
    {
        cin.ignore();
        getline(cin, spot);
        if (spot == "done") break;
        new_dest.tourist_spots.push_back(spot);
    }

    destinations.push_back(new_dest);
    cout << "New destination added successfully!" << endl;
}
void bookaTrip() {
    Booking booking;
    cout << "\n--- Book a Trip ---" << endl;

    cout << "Enter your name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, booking.name);

    string phoneNumber, gender;
    int age;

    // Additional fields for booking details
    cout << "Enter your phone number: ";
    getline(cin, phoneNumber);

    cout << "Enter your gender (Male/Female/Other): ";
    getline(cin, gender);

    cout << "Enter your age: ";
    while (!(cin >> age) || age <= 0) {
        cout << "Invalid input. Enter a valid age: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter your starting location: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, booking.from);

    listAllDestinations();
    cout << "Enter the destination you want to visit: ";
    getline(cin, booking.to);

    // Check if the destination is valid
    bool found = false;
    for (const auto& dest : destinations) {
        if (dest.name == booking.to) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Invalid destination!" << endl;
        return;
    }

    cout << "Enter transport type (Cab, Bus, Train): ";
    cin >> booking.transport_type;

    cout << "Enter the number of people traveling: ";
    while (!(cin >> booking.people_count) || booking.people_count <= 0) {
        cout << "Invalid input. Enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Simplified vehicle type selection
    if (booking.transport_type == "Cab") {
        booking.vehicle_type = "Private Cab";
    } else if (booking.transport_type == "Bus") {
        booking.vehicle_type = "Shared Bus";
    } else if (booking.transport_type == "Train") {
        booking.vehicle_type = "Train Seat";
    } else {
        cout << "Invalid transport type. Booking canceled." << endl;
        return;
    }

    // Placeholder budget estimation
    booking.estimated_budget = booking.people_count * 500;

    // Store the booking
    bookings.push_back(booking);

    // Display a confirmation message with all details
    cout << "\n--- Booking Confirmed! ---\n";
    cout << "Name: " << booking.name << "\nPhone: " << phoneNumber
         << "\nGender: " << gender << "\nAge: " << age
         << "\nFrom: " << booking.from << "\nTo: " << booking.to
         << "\nTransport Type: " << booking.transport_type
         << "\nVehicle Type: " << booking.vehicle_type
         << "\nNumber of People: " << booking.people_count
         << "\nEstimated Budget: " << booking.estimated_budget << " INR\n";
}
void viewDestinationDetail()
{
    string destination_name;
    cout << "Enter destination name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, destination_name);
    bool found = false;

    for (const auto& dest : destinations)
    {
        if (dest.name == destination_name)
        {
            cout << "\nDetails of " << destination_name << ":\n"
                 << "District: " << dest.district << "\nCategory: " << dest.category
                 << "\nRating: " << dest.rating << "/5\nTourist Spots:\n";
            for (const string& spot : dest.tourist_spots)
            {
                cout << "- " << spot << endl;
            }
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "Destination not found!" << endl;
    }
}
void updateDestinationRatings()
{
    string destination_name;
    cout << "Enter the destination name to update its rating: ";
    cin.ignore();
    getline(cin, destination_name);

    bool found = false;
    for (auto& dest : destinations)
    {
        if (dest.name == destination_name)
        {
            cout << "Current Rating of " << dest.name << ": " << dest.rating << endl;
            cout << "Enter new rating (out of 5): ";
            cin >> dest.rating;
            cout << "Rating updated successfully!" << endl;
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "Destination not found!" << endl;
    }
}
void deleteBookingss()
{
    int booking_id;
    cout << "Enter booking ID to delete: ";
    cin >> booking_id;

    if (booking_id <= 0 || booking_id > bookings.size())
    {
        cout << "Invalid booking ID." << endl;
        return;
    }

    bookings.erase(bookings.begin() + booking_id - 1);
    cout << "Booking ID " << booking_id << " has been deleted successfully." << endl;
}

void filterDestinationsByRatings()
{
    double rating_threshold;
    cout << "Enter the minimum rating (out of 5): ";
    cin >> rating_threshold;

    cout << "\nDestinations with a rating of " << rating_threshold << " and above:\n";
    bool found = false;

    for (const auto& dest : destinations)
    {
        if (dest.rating >= rating_threshold)
        {
            cout << "- " << dest.name << " (" << dest.rating << "/5)" << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "No destinations meet the rating requirement!" << endl;
    }
}
void listAllDestination()
{

    cout << "\nAvailable Destinations:" << endl;
    for (const auto& dest : destinations) {
        cout << "- " << dest.name << " (" << dest.category << ", " << dest.district << ")" << endl;

    }
}
void searchDestinationByName()
{
    string name;
    cout << "Enter the name of the destination: ";
    cin.ignore();
    getline(cin, name);

    bool found = false;
    for (const auto& dest : destinations)
    {
        if (dest.name == name)
        {
            cout << "\nDestination: " << dest.name
                 << "\nDistrict: " << dest.district
                 << "\nCategory: " << dest.category
                 << "\nRating: " << dest.rating << "/5\n"
                 << "Tourist spots:\n";
            for (const auto& spot : dest.tourist_spots)
            {
                cout << "- " << spot << endl;
            }
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "Destination not found!" << endl;
    }
}


void filterDestinationsByDistrict()
{
    string district;
    cout << "Enter the district name: ";
    cin.ignore();
    getline(cin, district);

    bool found = false;
    cout << "\nDestinations in " << district << " district:\n";
    for (const auto& dest : destinations)
    {
        if (dest.district == district)
        {
            cout << "- " << dest.name << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "No destinations found in the district: " << district << endl;
    }
}

void updateDestinationRating()
{
    string destination_name;
    cout << "Enter the destination name to update its rating: ";
    cin.ignore();
    getline(cin, destination_name);

    bool found = false;
    for (auto& dest : destinations)
    {
        if (dest.name == destination_name)
        {
            cout << "Current Rating of " << dest.name << ": " << dest.rating << endl;
            cout << "Enter new rating (out of 5): ";
            cin >> dest.rating;
            cout << "Rating updated successfully!" << endl;
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "Destination not found!" << endl;
    }
}

void filterDestinationsByRating()
{
    double rating_threshold;
    cout << "Enter the minimum rating (out of 5): ";
    cin >> rating_threshold;

    cout << "\nDestinations with a rating of " << rating_threshold << " and above:\n";
    bool found = false;

    for (const auto& dest : destinations)
    {
        if (dest.rating >= rating_threshold)
        {
            cout << "- " << dest.name << " (" << dest.rating << "/5)" << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "No destinations meet the rating requirement!" << endl;
    }
}

void addReviewForDestination()
{
    string destination_name;
    cout << "Enter the destination name to add a review: ";
    cin.ignore();
    getline(cin, destination_name);

    bool found = false;
    for (auto& dest : destinations) {
        if (dest.name == destination_name) {
            string review;
            cout << "Enter your review: ";
            getline(cin, review);
            cout << "Review for " << dest.name << " added successfully!" << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Destination not found!" << endl;
    }
}

void deleteBookings()
{
    int booking_id;
    cout << "Enter booking ID to delete: ";
    cin >> booking_id;

    if (booking_id <= 0 || booking_id > bookings.size())
    {
        cout << "Invalid booking ID." << endl;
        return;
    }

    bookings.erase(bookings.begin() + booking_id - 1);
    cout << "Booking ID " << booking_id << " has been deleted successfully." << endl;
}
void calculateBudgetDetails(const Booking& booking)
 {
    cout << "\n--- Detailed Budget Breakdown ---\n";
    double transport_cost = booking.people_count * 200; // Example calculation
    double accommodation_cost = booking.people_count * 300; // Example calculation
    double food_cost = booking.people_count * 100; // Example calculation
    double total = transport_cost + accommodation_cost + food_cost;

    cout << "Transport Cost: " << transport_cost << " INR\n";
    cout << "Accommodation Cost: " << accommodation_cost << " INR\n";
    cout << "Food Cost: " << food_cost << " INR\n";
    cout << "Total Estimated Cost: " << total << " INR\n";
}

void manageTransportation() {

destinations.push_back({"Hampi", "Ballari", {"Virupaksha Temple", "Vittala Temple", "Matanga Hill", "Hampi Bazaar", "Lotus Mahal"}, "Historical", 4.8});
destinations.push_back({"Mysore", "Mysore", {"Mysore Palace", "Chamundeshwari Temple", "Brindavan Gardens", "St. Philomena's Church", "KRS Dam"}, "City", 4.7});
destinations.push_back({"Bangalore", "Bangalore ", {"Bangalore Palace", "Lalbagh Botanical Garden", "Vidhana Soudha", "Nandi Hills", "ISKCON Temple", "Cubbon Park", "Bannerghatta Biological Park"}, "Urban", 4.5});
destinations.push_back({"Coorg", "Kodagu", {"Abbey Falls", "Rajas Seat", "Dubare Elephant Camp", "Namdroling Monastery", "Coffee Plantations"}, "Hill Station", 4.6});
destinations.push_back({"Chikmagalur", "Chikmagalur", {"Mullayanagiri", "Baba Budangiri", "Hebbe Falls", "Coffee Plantations", "Kudremukh National Park"}, "Hill Station", 4.4});
destinations.push_back({"Udupi", "Udupi", {"Sri Krishna Temple", "St. Marys Island", "Malpe Beach", "Udupi Manipal"}, "Religious", 4.3});
destinations.push_back({"Gokarna", "Uttara Kannada", {"Om Beach", "Kudle Beach", "Gokarna Temple", "Mirjan Fort"}, "Beach", 4.5});
destinations.push_back({"Hassan", "Hassan", {"Hoysaleswara Temple", "Chennakesava Temple", "Shravanabelagola (Bahubali Statue)"}, "Religious", 4.6});
destinations.push_back({"Badami", "Bagalkot", {"Badami Caves", "Agastya Lake", "Bhuthnatha Temple", "Fort", "Palace"}, "Historical", 4.7});
destinations.push_back({"Bijapur", "Bijapur", {"Gol Gumbaz", "Bijapur Fort", "Ibrahim Rauza", "Jami Masjid"}, "Historical", 4.3});
destinations.push_back({"Kabini", "Kodagu", {"Kabini River", "Kabini Wildlife Sanctuary", "Jungle Safaris"}, "Wildlife", 4.8});
destinations.push_back({"Dandeli", "Uttara Kannada", {"Dandeli Wildlife Sanctuary", "Kali River", "Syntheri Rocks", "Anshi National Park"}, "Wildlife", 4.4});
destinations.push_back({"Sakleshpur", "Hassan", {"Manjarabad Fort", "Bisle Reserve Forest", "Coffee Plantations", "Sakleshpur Hills"}, "Hill Station", 4.2});
destinations.push_back({"Tungabhadra Dam", "Hospet, Ballari", {"Tungabhadra Dam", "Anjaneya Hill", "Hampi"}, "Dam", 4.7});
destinations.push_back({"Ranganathittu Bird Sanctuary", "Mandya", {"Bird Watching", "Crocodile Observation", "Nature Walks"}, "Wildlife", 4.5});
destinations.push_back({"Channarayana Durga", "Tumkur", {"Hill Trekking", "Channarayana Durga Fort"}, "Hill", 4.2});
destinations.push_back({"Shravanabelagola", "Hassan", {"Gommateshwara (Bahubali Statue)", "Shravanabelagola Hill", "Jain Temples"}, "Religious", 4.6});
destinations.push_back({"Mangalore", "Dakshina Kannada", {"Panambur Beach", "Tannirbhavi Beach", "St. Aloysius Chapel", "Kudroli Gokarnath Temple"}, "City", 4.5});
destinations.push_back({"Kudremukh", "Chikmagalur", {"Kudremukh National Park", "Kudremukh Peak", "Trekking"}, "Hill Station", 4.4});
destinations.push_back({"Kalaburagi", "Kalaburagi", {"Gulbarga Fort", "Jama Masjid", "Sharana Basaveshwara Temple"}, "Historical", 4.1});
destinations.push_back({"Agumbe", "Shimoga", {"Agumbe Rainforest", "Sunset Point", "Someshwara Wildlife Sanctuary"}, "Rainforest", 4.6});
destinations.push_back({"Bhadra", "Chikmagalur", {"Bhadra River", "Jungle Safari", "Wildlife Observation"}, "Wildlife", 4.7});
destinations.push_back({"Yellapur", "Uttara Kannada", {"Magod Falls", "Sathodi Falls", "Unchalli Falls", "Suwarna River"}, "Waterfalls", 4.3});
destinations.push_back({"Koppal", "Koppal", {"Koppal Fort", "Almatti Dam", "Shivgiri Temple", "Historical Sites"}, "Historical", 4.3});
destinations.push_back({"Kudligi", "Ballari", {"Kudligi Fort", "Veerabhadra Temple", "Vijaya Durga Hill", "Shivana Temples"}, "Historical", 4.5});
destinations.push_back({"Narsipur", "Mysore", {"Sri Lakshmi Venkateshwara Temple", "Barigala Falls", "Silent Valley", "Teak Tree Walk"}, "Religious", 4.5});
destinations.push_back({"Mandya", "Mandya", {"Krishna Raja Sagar Dam", "Ranganathittu Bird Sanctuary", "Srirangapatna", "Amma's Shrine"}, "Dam", 4.8});
destinations.push_back({"Raichur", "Raichur", {"Raichur Fort", "Koppal Lake", "Sri Malkhed Temples", "Historic Markets"}, "Historical", 4.2});
destinations.push_back({"Dharwad", "Dharwad", {"Dharwad Lake", "Karnataka High Court", "Sri Channakesava Temple", "Jain Temples"}, "Urban", 4.4});
destinations.push_back({"Hosanagara", "Shimoga", {"Gajanur Dam", "Singimala Falls", "Bhadravathi", "Joga Falls"}, "Waterfall", 4.5});
destinations.push_back({"Kanakapura", "Ramanagaram", {"Kanakpura Lake", "Ramanagaram Fort", "Ramanagaram Rocks", "Nature Trails"}, "Hill", 4.6});
destinations.push_back({"Mangalore", "Dakshina Kannada", {"St. Aloysius Chapel", "Tannirbhavi Beach", "Manjunatha Temple", "Panambur Beach"}, "Urban", 4.7});
destinations.push_back({"Bagalkot", "Bagalkot", {"Badami Caves", "Bela Fort", "Bhootnath Temple", "Rock Formations"}, "Historical", 4.4});
destinations.push_back({"Nanjangud", "Mysore", {"Sri Nanjundeshwara Temple", "Satyamangala Reserve", "Nalvadi Ramaswamy"}, "Religious", 4.3});
destinations.push_back({"Srirangapatna", "Mandya", {"Ranganathaswamy Temple", "Tipu Sultan's Summer Palace", "Gumbaz Tombs", "Gaddige Mausoleum"}, "Religious", 4.5});
destinations.push_back({"Bellary Fort", "Ballari", {"Bellary Fort", "Old Market", "Chandramouleshwara Temple"}, "Historical", 4.3});
destinations.push_back({"Iruppu Falls", "Kodagu", {"Iruppu Waterfall", "Nadi Tirtha", "Ramalya Wildlife Sanctuary"}, "Waterfall", 4.6});
destinations.push_back({"Chikkamagaluru", "Chikmagalur", {"Mullayanagiri", "Baba Budangiri", "Hebbe Falls", "Kudremukh National Park"}, "Hill Station", 4.8});
destinations.push_back({"Kudligi", "Ballari", {"Banda Anjaneya Temple", "Kudligi Reservoir", "Sundermanya Temple"}, "Religious", 4.2});
destinations.push_back({"Manjarabad Fort", "Hassan", {"Manjarabad Fort", "Bisle Reserve Forest", "Coffee Plantations"}, "Historical", 4.5});
destinations.push_back({"Badami Caves", "Bagalkot", {"Badami Caves", "Agastya Lake", "Bhuthnatha Temple", "Fort"}, "Historical", 4.4});
destinations.push_back({"Mysore Zoo", "Mysore", {"Mysore Zoo", "Animal Conservation", "Animal Watching"}, "Wildlife", 4.3});
destinations.push_back({"Agumbe Rainforest", "Shimoga", {"Agumbe Rainforest", "Sunset Point", "Someshwara Wildlife Sanctuary"}, "Rainforest", 4.6});
destinations.push_back({"Hampi", "Ballari", {"Virupaksha Temple", "Vittala Temple", "Matanga Hill", "Hampi Bazaar", "Lotus Mahal"}, "Historical", 4.8});
destinations.push_back({"Kabini", "Kodagu", {"Kabini River", "Kabini Wildlife Sanctuary", "Bamboo Grove", "Jungle Safari"}, "Wildlife", 4.9});
destinations.push_back({"Karkala", "Udupi", {"Karkala Statue", "Chaturmukha Basadi", "St. Lawrence Church", "Kudumala Temple"}, "Religious", 4.2});
destinations.push_back({"Sagara", "Shivamogga", {"Jog Falls", "Sagara Fort", "Scenic Hiking Trails", "Suragi Falls"}, "Hill Station", 4.4});
destinations.push_back({"Channarayana Durga", "Tumkur", {"Hill Trekking", "Channarayana Durga Fort", "View of Devarayanadurga"}, "Hill", 4.2});
destinations.push_back({"Yagachi Dam", "Hassan", {"Yagachi Dam", "Water Sports", "Boating", "Nature Walks"}, "Dam", 4.5});
destinations.push_back({"Devagundi Falls", "Tirthahalli", {"Devagundi Falls", "Nature Trails", "Waterfall Trekking"}, "Waterfall", 4.3});
destinations.push_back({"Ramanagaram", "Ramanagaram", {"Ramadevara Betta", "Rock Climbing", "Silk City", "Nature Trails"}, "Adventure", 4.5});
destinations.push_back({"Bhadra Wildlife Sanctuary", "Chikmagalur", {"Bhadra River", "Wildlife Watching", "Bhadra Tiger Reserve"}, "Wildlife", 4.7});
destinations.push_back({"Chitradurga Fort", "Chitradurga", {"Chitradurga Fort", "Beggars' Rock", "Obavva's Fort", "Yellur Fort"}, "Historical", 4.6});
destinations.push_back({"Nandi Hills", "Bangalore Rural", {"Nandi Temple", "Paragliding", "Nandi Hills Viewpoint", "Bhoga Nandeeshwara Temple"}, "Hill", 4.5});
destinations.push_back({"Kabini", "Kodagu", {"Kabini River", "Kabini Backwaters", "Kabini Wildlife Safari", "Bamboo Grove", "Wayanad Wildlife Sanctuary"}, "Wildlife", 4.7});
destinations.push_back({"Agrahar Daasanur", "Hassan", {"Historical Temples", "Eco-tourism", "Hiking Trails"}, "Religious", 4.3});
destinations.push_back({"Muthyala Maduvu", "Bangalore Rural", {"Muthyala Maduvu Waterfall", "Rocky Terrain", "Temple of Goddess"}, "Waterfall", 4.4});
destinations.push_back({"Bangalore Palace", "Bangalore", {"Palace Grounds", "Royal Events", "Museum"}, "Palace", 4.2});
destinations.push_back({"Kabini River", "Kodagu", {"River Safari", "Elephant Camp", "Kabini Forest Reserve", "Kabini Backwaters"}, "Nature", 4.6});
destinations.push_back({"Jog Falls", "Shivamogga", {"Waterfall Viewing", "Nature Walks", "Boat Ride"}, "Waterfall", 4.7});
destinations.push_back({"Somnathpur", "Mysore", {"Keshava Temple", "Sculptures", "Temple Architecture"}, "Historical", 4.5});
destinations.push_back({"Murudeshwar", "Uttara Kannada", {"Murudeshwar Temple", "Shiva Statue", "Narasimha Temple", "Beach"}, "Religious", 4.4});
destinations.push_back({"Gokarna", "Uttara Kannada", {"Om Beach", "Kudle Beach", "Mirjan Fort", "Gokarna Temple", "Hippie culture"}, "Beach", 4.6});
destinations.push_back({"Chitradurga", "Chitradurga", {"Chitradurga Fort", "Ankalappa Temple", "Hill Treks", "Historic Monuments"}, "Historical", 4.5});
destinations.push_back({"Ramanagaram", "Ramanagaram", {"Rock Climbing", "Silk City", "Ramanagaram Lake", "Channarayana Durga"}, "Adventure", 4.5});
destinations.push_back({"Srirangapatna", "Mandya", {"Ranganathaswamy Temple", "Dariya Daulat Bagh", "Srirangapatna Fort", "Gumbaz", "Brindavan Gardens"}, "Historical", 4.7});
destinations.push_back({"Madhugiri", "Tumkur", {"Madhugiri Fort", "Trekking", "Stone Steps", "Madhugiri Hill"}, "Hill", 4.2});
destinations.push_back({"Tadiandamol", "Kodagu", {"Tadiandamol Peak", "Trekking Trails", "Scenic Views"}, "Hill", 4.5});
destinations.push_back({"Kundadri Hill", "Shivamogga", {"Hill Trekking", "Sunset Point", "Kundadri Temple", "Nature Walk"}, "Hill", 4.4});
destinations.push_back({"Yelawala", "Mysore", {"Yelawala Viewpoint", "Mountains and Rivers", "Nature Trails"}, "Hill", 4.2});
destinations.push_back({"Nandi Hills", "Chikkaballapur", {"Nandi Temple", "Tipu Sultan's Summer Palace", "Sunrise Viewpoint", "Nandi Hills Viewpoint"}, "Hill Station", 4.5});
destinations.push_back({"Kolar", "Kolar", {"Kolar Gold Fields", "Kolaramma Temple", "Kolar Market", "Mulbagal Fort"}, "Historical", 4.2});
destinations.push_back({"Bijapur", "Bijapur", {"Gol Gumbaz", "Ibrahim Rauza", "Bara Kaman", "Jami Masjid", "Upli Buruj"}, "Historical", 4.7});
destinations.push_back({"Gokak Falls", "Belagavi", {"Gokak Waterfall", "Gokak Dam", "Hidkal Dam", "Satyagraha Sthal"}, "Waterfall", 4.4});
destinations.push_back({"Coorg", "Kodagu", {"Abbey Falls", "Namdroling Monastery", "Tadiandamol Peak", "Madikeri Fort", "Raja's Seat"}, "Hill Station", 4.8});
destinations.push_back({"Kabini River", "Kodagu", {"Kabini River Viewpoint", "Kabini Wildlife Sanctuary", "Bamboo Grove", "Nature Walks"}, "Wildlife", 4.7});
destinations.push_back({"Ramanagaram", "Ramanagaram", {"Ramadevara Betta", "Rock Climbing", "Silk City", "Nature Trails"}, "Adventure", 4.6});
destinations.push_back({"Bangalore Palace", "Bangalore", {"Bangalore Palace", "Vidhana Soudha", "Cubbon Park", "Lalbagh Botanical Garden"}, "Urban", 4.5});
destinations.push_back({"Gulbarga", "Gulbarga", {"Gulbarga Fort", "Sharana Basaveshwara Temple", "Jama Masjid", "Gulbarga Fort Ruins"}, "Historical", 4.3});
destinations.push_back({"Agumbe", "Shimoga", {"Agumbe Rainforest", "Sunset Point", "Someshwara Wildlife Sanctuary", "The Agumbe Jungle Trek"}, "Rainforest", 4.8});
destinations.push_back({"Shravanabelagola", "Hassan", {"Bahubali Statue", "Jain Temples", "Shravanabelagola Hill", "Kunda Hill Trek"}, "Religious", 4.6});
destinations.push_back({"Chikmagalur", "Chikmagalur", {"Mullayanagiri Peak", "Baba Budangiri", "Hebbe Falls", "Coffee Plantations"}, "Hill Station", 4.7});
destinations.push_back({"Ranganathittu Bird Sanctuary", "Mandya", {"Bird Watching", "Crocodile Observation", "Boat Ride", "Nature Walks"}, "Wildlife", 4.9});
destinations.push_back({"Sakleshpur", "Hassan", {"Bisle Reserve Forest", "Manjarabad Fort", "Coffee Plantations", "Sakleshpur Hills"}, "Hill Station", 4.5});
destinations.push_back({"Dandeli", "Uttara Kannada", {"Dandeli Wildlife Sanctuary", "Kali River", "Syntheri Rocks", "Anshi National Park"}, "Wildlife", 4.6});
destinations.push_back({"Tungabhadra Dam", "Ballari", {"Tungabhadra Dam", "Anjaneya Hill", "Temple of Hazara Rama", "Hampi"}, "Dam", 4.7});
destinations.push_back({"Hassan", "Hassan", {"Hoysaleswara Temple", "Chennakesava Temple", "Shravanabelagola (Bahubali Statue)"}, "Religious", 4.7});
destinations.push_back({"Tirthahalli", "Shimoga", {"Agumbe", "Devagundi Falls", "Kundadri Hill", "Kundapur Beach"}, "Nature", 4.4});
destinations.push_back({"Badami", "Bagalkot", {"Badami Caves", "Agastya Lake", "Bhuthnatha Temple", "Cultural Forts"}, "Historical", 4.7});
destinations.push_back({"Bellary", "Ballari", {"Bellary Fort", "Jain Temples", "Lake Vedavyasa", "Sri Siddheshwar Temple"}, "Historical", 4.4});
destinations.push_back({"Hampi", "Ballari", {"Virupaksha Temple", "Vittala Temple", "Matanga Hill", "Hampi Bazaar"}, "Historical", 4.8});
destinations.push_back({"Mangalore", "Dakshina Kannada", {"Panambur Beach", "St. Aloysius Chapel", "Tannirbhavi Beach", "Kadri Manjunath Temple"}, "Urban", 4.6});
destinations.push_back({"Kudremukh", "Chikmagalur", {"Kudremukh Peak", "Kudremukh National Park", "Trekking", "Baba Budangiri"}, "Hill Station", 4.7});
destinations.push_back({"Yellapur", "Uttara Kannada", {"Magod Falls", "Sathodi Falls", "Unchalli Falls", "Suwarna River"}, "Waterfalls", 4.5});
destinations.push_back({"Mysore Zoo", "Mysore", {"Mysore Zoo", "Mysore Palace", "Chamundeshwari Temple", "KRS Dam"}, "Wildlife", 4.3});
destinations.push_back({"Bhadra Wildlife Sanctuary", "Chikmagalur", {"Bhadra River", "Jungle Safari", "Wildlife Observation"}, "Wildlife", 4.8});
destinations.push_back({"Bannimantap", "Mysore", {"Ranganathaswamy Temple", "Bannimantap Bull Temple", "Bulls and Bears Market"}, "Religious", 4.2});
destinations.push_back({"Thirthahalli", "Shimoga", {"Sakrebyle Elephant Camp", "Sharavathi River", "Banavasi Temple", "Tunga Anicut Dam"}, "Nature", 4.6});
destinations.push_back({"Suttur", "Mysore", {"Suttur Mutt", "Shivarathri Festival", "Mysore Palace", "Suttur Ranganathaswamy Temple"}, "Religious", 4.4});
destinations.push_back({"Chitradurga Fort", "Chitradurga", {"Chitradurga Fort", "Beggars' Rock", "Yellur Fort", "Obavva's Fort"}, "Historical", 4.5});
destinations.push_back({"Karnataka High Court", "Bangalore", {"High Court", "Cubbon Park", "Vidhana Soudha", "Kasturba Gandhi Memorial"}, "Urban", 4.2});
destinations.push_back({"Yediyur", "Tumkur", {"Yediyur Lake", "Devarayanadurga Fort", "Yediyur Temple", "Skylark Hill"}, "Nature", 4.3});
destinations.push_back({"Hosanagara", "Shimoga", {"Sagara Fort", "Gajanur Dam", "Singimala Falls", "Nature Trekking"}, "Hill Station", 4.5});
destinations.push_back({"Halebidu", "Hassan", {"Hoysaleswara Temple", "Chennakesava Temple", "Jain Temples", "Archaeological Sites"}, "Historical", 4.6});
destinations.push_back({"Chandragutti", "Shimoga", {"Chandragutti Temple", "Shri Venkateshwara Temple", "Basava Reserve"}, "Religious", 4.4});
destinations.push_back({"Nandihills", "Chikkaballapur", {"Nandi Temple", "Sunrise Point", "Top of Nandi Hill", "Nandi Hills Trekking"}, "Hill", 4.5});
destinations.push_back({"Bhadra Wildlife Sanctuary", "Chikmagalur", {"Bhadra Tiger Reserve", "Bird Watching", "Safari Tours", "Adventure Activities"}, "Wildlife", 4.8});
destinations.push_back({"Srirangapatna", "Mandya", {"Ranganathaswamy Temple", "Tipu Sultan's Summer Palace", "Srirangapatna Fort", "Gumbaz Tombs"}, "Historical", 4.7});
destinations.push_back({"Channarayana Durga", "Tumkur", {"Channarayana Durga Hill", "Hill Trekking", "Fort Walls", "Viewpoint"}, "Hill", 4.4});
destinations.push_back({"Chikmagalur", "Chikmagalur", {"Mullayanagiri", "Baba Budangiri", "Hebbe Falls", "Kudremukh Peak"}, "Hill Station", 4.8});
destinations.push_back({"Badami Caves", "Bagalkot", {"Badami Caves", "Agastya Lake", "Bhuthnatha Temple", "Rock-cut Temples"}, "Historical", 4.7});
destinations.push_back({"Hampi", "Ballari", {"Virupaksha Temple", "Vittala Temple", "Matanga Hill", "Hampi Bazaar"}, "Historical", 4.9});
destinations.push_back({"Gulbarga", "Gulbarga", {"Gulbarga Fort", "Jama Masjid", "Sharana Basaveshwara Temple", "Gulbarga Fort Ruins"}, "Historical", 4.6});
destinations.push_back({"Agumbe", "Shimoga", {"Agumbe Rainforest", "Sunset Point", "Someshwara Wildlife Sanctuary", "Tropical Rainforest Walks"}, "Rainforest", 4.8});
destinations.push_back({"Kabini", "Kodagu", {"Kabini Wildlife Sanctuary", "Kabini River", "Jungle Safari", "Bamboo Grove"}, "Wildlife", 4.8});
destinations.push_back({"Coorg", "Kodagu", {"Abbey Falls", "Tadiandamol Peak", "Namdroling Monastery", "Raja's Seat"}, "Hill Station", 4.9});
destinations.push_back({"Gokak Falls", "Belagavi", {"Gokak Waterfalls", "Gokak Dam", "Satyagraha Sthal", "Khwaja Banda Nawaz Dargah"}, "Waterfalls", 4.5});
destinations.push_back({"Ranganathittu Bird Sanctuary", "Mandya", {"Bird Watching", "Crocodile Observation", "Boat Ride", "Wildlife Conservation"}, "Wildlife", 4.9});
destinations.push_back({"Ramanagaram", "Ramanagaram", {"Ramadevara Betta", "Rock Climbing", "Vulture Sanctuary", "Natural Rock Formations"}, "Adventure", 4.6});
destinations.push_back({"Hassan", "Hassan", {"Hoysaleswara Temple", "Chennakesava Temple", "Shravanabelagola"}, "Religious", 4.7});
destinations.push_back({"Sakleshpur", "Hassan", {"Bisle Reserve Forest", "Manjarabad Fort", "Coffee Plantations", "Sakleshpur Hills"}, "Hill Station", 4.6});
destinations.push_back({"Tungabhadra Dam", "Ballari", {"Tungabhadra Dam", "Anjaneya Hill", "Cultural Sites", "Temples"}, "Dam", 4.7});
destinations.push_back({"Mysore", "Mysore", {"Mysore Palace", "Chamundeshwari Temple", "St. Philomena's Church", "Brindavan Gardens"}, "City", 4.8});
destinations.push_back({"Chitradurga Fort", "Chitradurga", {"Chitradurga Fort", "Beggars' Rock", "Yellur Fort", "Fort Ruins"}, "Historical", 4.7});
destinations.push_back({"Yellapur", "Uttara Kannada", {"Magod Falls", "Unchalli Falls", "Sathodi Falls", "Nature Walks"}, "Waterfalls", 4.6});
destinations.push_back({"Bijapur", "Bijapur", {"Gol Gumbaz", "Bijapur Fort", "Ibrahim Rauza", "Jami Masjid"}, "Historical", 4.5});
destinations.push_back({"Dandeli", "Uttara Kannada", {"Dandeli Wildlife Sanctuary", "Kali River", "Anshi National Park", "Syntheri Rocks"}, "Wildlife", 4.7});
destinations.push_back({"Mangalore", "Dakshina Kannada", {"Tannirbhavi Beach", "Panambur Beach", "Kadri Manjunath Temple", "Mangalore Port"}, "Urban", 4.5});
destinations.push_back({"Dharwad", "Dharwad", {"Unkal Lake", "Chandramouleshwara Temple", "Savadatti Fort", "Dharwad Nature Camp"}, "City", 4.4});
destinations.push_back({"Kolar Gold Fields", "Kolar", {"Kolar Gold Fields", "Kolaramma Temple", "Mulbagal Fort", "Kolar Railway Station"}, "Historical", 4.2});
destinations.push_back({"Halebidu", "Hassan", {"Hoysaleswara Temple", "Chennakesava Temple", "Heritage Walks", "Archaeological Sites"}, "Historical", 4.6});
destinations.push_back({"Agraharas", "Hassan", {"Sri Channakeshava Temple", "Thuruvana Gardens", "Bhimaneshwara Temple", "Old Manjuguni"}, "Religious", 4.4});
destinations.push_back({"Shravanabelagola", "Hassan", {"Bahubali Statue", "Jain Temples", "Shravanabelagola Hill", "Hill Trekking"}, "Religious", 4.7});
destinations.push_back({"Kudremukh", "Chikmagalur", {"Kudremukh Peak", "Baba Budangiri", "Trekking", "Nature Walks"}, "Hill Station", 4.7});
destinations.push_back({"Bangalore", "Bangalore", {"Cubbon Park", "Bangalore Palace", "Lalbagh Botanical Garden", "Vidhana Soudha"}, "Urban", 4.8});
destinations.push_back({"Nandi Hills", "Chikkaballapur", {"Nandi Hills Viewpoint", "Tipu Sultan's Summer Palace", "Nandi Temple", "Sunrise Views"}, "Hill", 4.8});
destinations.push_back({"Yadgir", "Yadgir", {"Yadgir Fort", "Sangameshwar Temple", "Mangalore Falls", "Raichur Fort"}, "Historical", 4.4});
destinations.push_back({"Hampi", "Ballari", {"Vittala Temple", "Matanga Hill", "Virupaksha Temple", "Hampi Bazaar"}, "Historical", 4.9});
destinations.push_back({"Tirupati", "Chikballapur", {"Tirumala Temple", "Sri Venkateshwara Temple", "Shilparamam", "Azhwar Hill"}, "Religious", 4.7});
destinations.push_back({"Shimoga", "Shimoga", {"Jog Falls", "Kubri Falls", "Bhadravathi", "Gajanur Reservoir"}, "Waterfalls", 4.8});
destinations.push_back({"Channarayana Durga", "Tumkur", {"Channarayana Durga Fort", "Hill Trekking", "Viewpoint", "Nature Trails"}, "Hill", 4.5});
destinations.push_back({"Bagalkot", "Bagalkot", {"Badami Caves", "Bhutha Natha Temple", "Jain Temples", "Agastya Lake"}, "Historical", 4.5});
destinations.push_back({"Devagiri", "Siddapura", {"Devagiri Falls", "Brahmagiri Peak", "Elephant Sanctuaries", "Nature Trekking"}, "Hill Station", 4.6});
destinations.push_back({"Tumkur", "Tumkur", {"Nandi Temple", "Siddhappa's Hill", "Silk Farms", "Tumkur Silk Museum"}, "City", 4.2});
destinations.push_back({"Bellary", "Ballari", {"Sri Siddheshwar Temple", "Siddheshwara Fort", "Hampi", "Brahmagiri Hill"}, "Historical", 4.5});
destinations.push_back({"Aihole", "Bagalkot", {"Aihole Temples", "Historical Sites", "Stone Monuments", "Ancient Architecture"}, "Historical", 4.6});
destinations.push_back({"Kadiri", "Anantapur", {"Kadiri Lakshmi Narayan Temple", "Sankasam Reserve", "Kadiri Fort", "Rayadurg Fort"}, "Religious", 4.4});
destinations.push_back({"Hampi", "Ballari", {"Virupaksha Temple", "Vittala Temple", "Matanga Hill", "Hampi Bazaar", "Lotus Mahal"}, "Historical", 4.8});
destinations.push_back({"Mysore", "Mysore", {"Mysore Palace", "Chamundeshwari Temple", "Brindavan Gardens", "St. Philomena's Church", "KRS Dam"}, "City", 4.7});
destinations.push_back({"Bangalore", "Bangalore ", {"Bangalore Palace", "Lalbagh Botanical Garden", "Vidhana Soudha", "Nandi Hills", "ISKCON Temple", "Cubbon Park", "Bannerghatta Biological Park"}, "Urban", 4.5});
destinations.push_back({"Coorg", "Kodagu", {"Abbey Falls", "Rajas Seat", "Dubare Elephant Camp", "Namdroling Monastery", "Coffee Plantations"}, "Hill Station", 4.6});
destinations.push_back({"Chikmagalur", "Chikmagalur", {"Mullayanagiri", "Baba Budangiri", "Hebbe Falls", "Coffee Plantations", "Kudremukh National Park"}, "Hill Station", 4.4});
destinations.push_back({"Udupi", "Udupi", {"Sri Krishna Temple", "St. Marys Island", "Malpe Beach", "Udupi Manipal"}, "Religious", 4.3});
destinations.push_back({"Gokarna", "Uttara Kannada", {"Om Beach", "Kudle Beach", "Gokarna Temple", "Mirjan Fort"}, "Beach", 4.5});
destinations.push_back({"Hassan", "Hassan", {"Hoysaleswara Temple", "Chennakesava Temple", "Shravanabelagola (Bahubali Statue)"}, "Religious", 4.6});
destinations.push_back({"Badami", "Bagalkot", {"Badami Caves", "Agastya Lake", "Bhuthnatha Temple", "Fort", "Palace"}, "Historical", 4.7});
destinations.push_back({"Bijapur", "Bijapur", {"Gol Gumbaz", "Bijapur Fort", "Ibrahim Rauza", "Jami Masjid"}, "Historical", 4.3});
destinations.push_back({"Kabini", "Kodagu", {"Kabini River", "Kabini Wildlife Sanctuary", "Jungle Safaris"}, "Wildlife", 4.8});
destinations.push_back({"Dandeli", "Uttara Kannada", {"Dandeli Wildlife Sanctuary", "Kali River", "Syntheri Rocks", "Anshi National Park"}, "Wildlife", 4.4});
destinations.push_back({"Sakleshpur", "Hassan", {"Manjarabad Fort", "Bisle Reserve Forest", "Coffee Plantations", "Sakleshpur Hills"}, "Hill Station", 4.2});
destinations.push_back({"Tungabhadra Dam", "Hospet, Ballari", {"Tungabhadra Dam", "Anjaneya Hill", "Hampi"}, "Dam", 4.7});
destinations.push_back({"Ranganathittu Bird Sanctuary", "Mandya", {"Bird Watching", "Crocodile Observation", "Nature Walks"}, "Wildlife", 4.5});
destinations.push_back({"Channarayana Durga", "Tumkur", {"Hill Trekking", "Channarayana Durga Fort"}, "Hill", 4.2});
destinations.push_back({"Shravanabelagola", "Hassan", {"Gommateshwara (Bahubali Statue)", "Shravanabelagola Hill", "Jain Temples"}, "Religious", 4.6});
destinations.push_back({"Mangalore", "Dakshina Kannada", {"Panambur Beach", "Tannirbhavi Beach", "St. Aloysius Chapel", "Kudroli Gokarnath Temple"}, "City", 4.5});
destinations.push_back({"Kudremukh", "Chikmagalur", {"Kudremukh National Park", "Kudremukh Peak", "Trekking"}, "Hill Station", 4.4});
destinations.push_back({"Kalaburagi", "Kalaburagi", {"Gulbarga Fort", "Jama Masjid", "Sharana Basaveshwara Temple"}, "Historical", 4.1});
destinations.push_back({"Agumbe", "Shimoga", {"Agumbe Rainforest", "Sunset Point", "Someshwara Wildlife Sanctuary"}, "Rainforest", 4.6});
destinations.push_back({"Bhadra", "Chikmagalur", {"Bhadra River", "Jungle Safari", "Wildlife Observation"}, "Wildlife", 4.7});
destinations.push_back({"Yellapur", "Uttara Kannada", {"Magod Falls", "Sathodi Falls", "Unchalli Falls", "Suwarna River"}, "Waterfalls", 4.3});
destinations.push_back({"Koppal", "Koppal", {"Koppal Fort", "Almatti Dam", "Shivgiri Temple", "Historical Sites"}, "Historical", 4.3});
destinations.push_back({"Kudligi", "Ballari", {"Kudligi Fort", "Veerabhadra Temple", "Vijaya Durga Hill", "Shivana Temples"}, "Historical", 4.5});
destinations.push_back({"Narsipur", "Mysore", {"Sri Lakshmi Venkateshwara Temple", "Barigala Falls", "Silent Valley", "Teak Tree Walk"}, "Religious", 4.5});
destinations.push_back({"Mandya", "Mandya", {"Krishna Raja Sagar Dam", "Ranganathittu Bird Sanctuary", "Srirangapatna", "Amma's Shrine"}, "Dam", 4.8});
destinations.push_back({"Raichur", "Raichur", {"Raichur Fort", "Koppal Lake", "Sri Malkhed Temples", "Historic Markets"}, "Historical", 4.2});
destinations.push_back({"Dharwad", "Dharwad", {"Dharwad Lake", "Karnataka High Court", "Sri Channakesava Temple", "Jain Temples"}, "Urban", 4.4});
destinations.push_back({"Hosanagara", "Shimoga", {"Gajanur Dam", "Singimala Falls", "Bhadravathi", "Joga Falls"}, "Waterfall", 4.5});
destinations.push_back({"Kanakapura", "Ramanagaram", {"Kanakpura Lake", "Ramanagaram Fort", "Ramanagaram Rocks", "Nature Trails"}, "Hill", 4.6});
destinations.push_back({"Mangalore", "Dakshina Kannada", {"St. Aloysius Chapel", "Tannirbhavi Beach", "Manjunatha Temple", "Panambur Beach"}, "Urban", 4.7});
destinations.push_back({"Bagalkot", "Bagalkot", {"Badami Caves", "Bela Fort", "Bhootnath Temple", "Rock Formations"}, "Historical", 4.4});
destinations.push_back({"Nanjangud", "Mysore", {"Sri Nanjundeshwara Temple", "Satyamangala Reserve", "Nalvadi Ramaswamy"}, "Religious", 4.3});
destinations.push_back({"Srirangapatna", "Mandya", {"Ranganathaswamy Temple", "Tipu Sultan's Summer Palace", "Gumbaz Tombs", "Gaddige Mausoleum"}, "Religious", 4.5});
destinations.push_back({"Bellary Fort", "Ballari", {"Bellary Fort", "Old Market", "Chandramouleshwara Temple"}, "Historical", 4.3});
destinations.push_back({"Iruppu Falls", "Kodagu", {"Iruppu Waterfall", "Nadi Tirtha", "Ramalya Wildlife Sanctuary"}, "Waterfall", 4.6});
destinations.push_back({"Chikkamagaluru", "Chikmagalur", {"Mullayanagiri", "Baba Budangiri", "Hebbe Falls", "Kudremukh National Park"}, "Hill Station", 4.8});
destinations.push_back({"Kudligi", "Ballari", {"Banda Anjaneya Temple", "Kudligi Reservoir", "Sundermanya Temple"}, "Religious", 4.2});
destinations.push_back({"Manjarabad Fort", "Hassan", {"Manjarabad Fort", "Bisle Reserve Forest", "Coffee Plantations"}, "Historical", 4.5});
destinations.push_back({"Badami Caves", "Bagalkot", {"Badami Caves", "Agastya Lake", "Bhuthnatha Temple", "Fort"}, "Historical", 4.4});
destinations.push_back({"Mysore Zoo", "Mysore", {"Mysore Zoo", "Animal Conservation", "Animal Watching"}, "Wildlife", 4.3});
destinations.push_back({"Agumbe Rainforest", "Shimoga", {"Agumbe Rainforest", "Sunset Point", "Someshwara Wildlife Sanctuary"}, "Rainforest", 4.6});
destinations.push_back({"Hampi", "Ballari", {"Virupaksha Temple", "Vittala Temple", "Matanga Hill", "Hampi Bazaar", "Lotus Mahal"}, "Historical", 4.8});
destinations.push_back({"Kabini", "Kodagu", {"Kabini River", "Kabini Wildlife Sanctuary", "Bamboo Grove", "Jungle Safari"}, "Wildlife", 4.9});
destinations.push_back({"Karkala", "Udupi", {"Karkala Statue", "Chaturmukha Basadi", "St. Lawrence Church", "Kudumala Temple"}, "Religious", 4.2});
destinations.push_back({"Sagara", "Shivamogga", {"Jog Falls", "Sagara Fort", "Scenic Hiking Trails", "Suragi Falls"}, "Hill Station", 4.4});
destinations.push_back({"Channarayana Durga", "Tumkur", {"Hill Trekking", "Channarayana Durga Fort", "View of Devarayanadurga"}, "Hill", 4.2});
destinations.push_back({"Yagachi Dam", "Hassan", {"Yagachi Dam", "Water Sports", "Boating", "Nature Walks"}, "Dam", 4.5});
destinations.push_back({"Devagundi Falls", "Tirthahalli", {"Devagundi Falls", "Nature Trails", "Waterfall Trekking"}, "Waterfall", 4.3});
destinations.push_back({"Ramanagaram", "Ramanagaram", {"Ramadevara Betta", "Rock Climbing", "Silk City", "Nature Trails"}, "Adventure", 4.5});
destinations.push_back({"Bhadra Wildlife Sanctuary", "Chikmagalur", {"Bhadra River", "Wildlife Watching", "Bhadra Tiger Reserve"}, "Wildlife", 4.7});
destinations.push_back({"Chitradurga Fort", "Chitradurga", {"Chitradurga Fort", "Beggars' Rock", "Obavva's Fort", "Yellur Fort"}, "Historical", 4.6});
destinations.push_back({"Nandi Hills", "Bangalore Rural", {"Nandi Temple", "Paragliding", "Nandi Hills Viewpoint", "Bhoga Nandeeshwara Temple"}, "Hill", 4.5});
destinations.push_back({"Kabini", "Kodagu", {"Kabini River", "Kabini Backwaters", "Kabini Wildlife Safari", "Bamboo Grove", "Wayanad Wildlife Sanctuary"}, "Wildlife", 4.7});
destinations.push_back({"Agrahar Daasanur", "Hassan", {"Historical Temples", "Eco-tourism", "Hiking Trails"}, "Religious", 4.3});
destinations.push_back({"Muthyala Maduvu", "Bangalore Rural", {"Muthyala Maduvu Waterfall", "Rocky Terrain", "Temple of Goddess"}, "Waterfall", 4.4});
destinations.push_back({"Bangalore Palace", "Bangalore", {"Palace Grounds", "Royal Events", "Museum"}, "Palace", 4.2});
destinations.push_back({"Kabini River", "Kodagu", {"River Safari", "Elephant Camp", "Kabini Forest Reserve", "Kabini Backwaters"}, "Nature", 4.6});
destinations.push_back({"Jog Falls", "Shivamogga", {"Waterfall Viewing", "Nature Walks", "Boat Ride"}, "Waterfall", 4.7});
destinations.push_back({"Somnathpur", "Mysore", {"Keshava Temple", "Sculptures", "Temple Architecture"}, "Historical", 4.5});
destinations.push_back({"Murudeshwar", "Uttara Kannada", {"Murudeshwar Temple", "Shiva Statue", "Narasimha Temple", "Beach"}, "Religious", 4.4});
destinations.push_back({"Gokarna", "Uttara Kannada", {"Om Beach", "Kudle Beach", "Mirjan Fort", "Gokarna Temple", "Hippie culture"}, "Beach", 4.6});
destinations.push_back({"Chitradurga", "Chitradurga", {"Chitradurga Fort", "Ankalappa Temple", "Hill Treks", "Historic Monuments"}, "Historical", 4.5});
destinations.push_back({"Ramanagaram", "Ramanagaram", {"Rock Climbing", "Silk City", "Ramanagaram Lake", "Channarayana Durga"}, "Adventure", 4.5});
destinations.push_back({"Srirangapatna", "Mandya", {"Ranganathaswamy Temple", "Dariya Daulat Bagh", "Srirangapatna Fort", "Gumbaz", "Brindavan Gardens"}, "Historical", 4.7});
destinations.push_back({"Madhugiri", "Tumkur", {"Madhugiri Fort", "Trekking", "Stone Steps", "Madhugiri Hill"}, "Hill", 4.2});
destinations.push_back({"Tadiandamol", "Kodagu", {"Tadiandamol Peak", "Trekking Trails", "Scenic Views"}, "Hill", 4.5});
destinations.push_back({"Kundadri Hill", "Shivamogga", {"Hill Trekking", "Sunset Point", "Kundadri Temple", "Nature Walk"}, "Hill", 4.4});
destinations.push_back({"Yelawala", "Mysore", {"Yelawala Viewpoint", "Mountains and Rivers", "Nature Trails"}, "Hill", 4.2});
destinations.push_back({"Nandi Hills", "Chikkaballapur", {"Nandi Temple", "Tipu Sultan's Summer Palace", "Sunrise Viewpoint", "Nandi Hills Viewpoint"}, "Hill Station", 4.5});
destinations.push_back({"Kolar", "Kolar", {"Kolar Gold Fields", "Kolaramma Temple", "Kolar Market", "Mulbagal Fort"}, "Historical", 4.2});
destinations.push_back({"Bijapur", "Bijapur", {"Gol Gumbaz", "Ibrahim Rauza", "Bara Kaman", "Jami Masjid", "Upli Buruj"}, "Historical", 4.7});
destinations.push_back({"Gokak Falls", "Belagavi", {"Gokak Waterfall", "Gokak Dam", "Hidkal Dam", "Satyagraha Sthal"}, "Waterfall", 4.4});
destinations.push_back({"Coorg", "Kodagu", {"Abbey Falls", "Namdroling Monastery", "Tadiandamol Peak", "Madikeri Fort", "Raja's Seat"}, "Hill Station", 4.8});
destinations.push_back({"Kabini River", "Kodagu", {"Kabini River Viewpoint", "Kabini Wildlife Sanctuary", "Bamboo Grove", "Nature Walks"}, "Wildlife", 4.7});
destinations.push_back({"Ramanagaram", "Ramanagaram", {"Ramadevara Betta", "Rock Climbing", "Silk City", "Nature Trails"}, "Adventure", 4.6});
destinations.push_back({"Bangalore Palace", "Bangalore", {"Bangalore Palace", "Vidhana Soudha", "Cubbon Park", "Lalbagh Botanical Garden"}, "Urban", 4.5});
destinations.push_back({"Gulbarga", "Gulbarga", {"Gulbarga Fort", "Sharana Basaveshwara Temple", "Jama Masjid", "Gulbarga Fort Ruins"}, "Historical", 4.3});
destinations.push_back({"Agumbe", "Shimoga", {"Agumbe Rainforest", "Sunset Point", "Someshwara Wildlife Sanctuary", "The Agumbe Jungle Trek"}, "Rainforest", 4.8});
destinations.push_back({"Shravanabelagola", "Hassan", {"Bahubali Statue", "Jain Temples", "Shravanabelagola Hill", "Kunda Hill Trek"}, "Religious", 4.6});
destinations.push_back({"Chikmagalur", "Chikmagalur", {"Mullayanagiri Peak", "Baba Budangiri", "Hebbe Falls", "Coffee Plantations"}, "Hill Station", 4.7});
destinations.push_back({"Ranganathittu Bird Sanctuary", "Mandya", {"Bird Watching", "Crocodile Observation", "Boat Ride", "Nature Walks"}, "Wildlife", 4.9});
destinations.push_back({"Sakleshpur", "Hassan", {"Bisle Reserve Forest", "Manjarabad Fort", "Coffee Plantations", "Sakleshpur Hills"}, "Hill Station", 4.5});
destinations.push_back({"Dandeli", "Uttara Kannada", {"Dandeli Wildlife Sanctuary", "Kali River", "Syntheri Rocks", "Anshi National Park"}, "Wildlife", 4.6});
destinations.push_back({"Tungabhadra Dam", "Ballari", {"Tungabhadra Dam", "Anjaneya Hill", "Temple of Hazara Rama", "Hampi"}, "Dam", 4.7});
destinations.push_back({"Hassan", "Hassan", {"Hoysaleswara Temple", "Chennakesava Temple", "Shravanabelagola (Bahubali Statue)"}, "Religious", 4.7});
destinations.push_back({"Tirthahalli", "Shimoga", {"Agumbe", "Devagundi Falls", "Kundadri Hill", "Kundapur Beach"}, "Nature", 4.4});
destinations.push_back({"Badami", "Bagalkot", {"Badami Caves", "Agastya Lake", "Bhuthnatha Temple", "Cultural Forts"}, "Historical", 4.7});
destinations.push_back({"Bellary", "Ballari", {"Bellary Fort", "Jain Temples", "Lake Vedavyasa", "Sri Siddheshwar Temple"}, "Historical", 4.4});
destinations.push_back({"Hampi", "Ballari", {"Virupaksha Temple", "Vittala Temple", "Matanga Hill", "Hampi Bazaar"}, "Historical", 4.8});
destinations.push_back({"Mangalore", "Dakshina Kannada", {"Panambur Beach", "St. Aloysius Chapel", "Tannirbhavi Beach", "Kadri Manjunath Temple"}, "Urban", 4.6});
destinations.push_back({"Kudremukh", "Chikmagalur", {"Kudremukh Peak", "Kudremukh National Park", "Trekking", "Baba Budangiri"}, "Hill Station", 4.7});
destinations.push_back({"Yellapur", "Uttara Kannada", {"Magod Falls", "Sathodi Falls", "Unchalli Falls", "Suwarna River"}, "Waterfalls", 4.5});
destinations.push_back({"Mysore Zoo", "Mysore", {"Mysore Zoo", "Mysore Palace", "Chamundeshwari Temple", "KRS Dam"}, "Wildlife", 4.3});
destinations.push_back({"Bhadra Wildlife Sanctuary", "Chikmagalur", {"Bhadra River", "Jungle Safari", "Wildlife Observation"}, "Wildlife", 4.8});
destinations.push_back({"Bannimantap", "Mysore", {"Ranganathaswamy Temple", "Bannimantap Bull Temple", "Bulls and Bears Market"}, "Religious", 4.2});
destinations.push_back({"Thirthahalli", "Shimoga", {"Sakrebyle Elephant Camp", "Sharavathi River", "Banavasi Temple", "Tunga Anicut Dam"}, "Nature", 4.6});
destinations.push_back({"Suttur", "Mysore", {"Suttur Mutt", "Shivarathri Festival", "Mysore Palace", "Suttur Ranganathaswamy Temple"}, "Religious", 4.4});
destinations.push_back({"Chitradurga Fort", "Chitradurga", {"Chitradurga Fort", "Beggars' Rock", "Yellur Fort", "Obavva's Fort"}, "Historical", 4.5});
destinations.push_back({"Karnataka High Court", "Bangalore", {"High Court", "Cubbon Park", "Vidhana Soudha", "Kasturba Gandhi Memorial"}, "Urban", 4.2});
destinations.push_back({"Yediyur", "Tumkur", {"Yediyur Lake", "Devarayanadurga Fort", "Yediyur Temple", "Skylark Hill"}, "Nature", 4.3});
destinations.push_back({"Hosanagara", "Shimoga", {"Sagara Fort", "Gajanur Dam", "Singimala Falls", "Nature Trekking"}, "Hill Station", 4.5});
destinations.push_back({"Halebidu", "Hassan", {"Hoysaleswara Temple", "Chennakesava Temple", "Jain Temples", "Archaeological Sites"}, "Historical", 4.6});
destinations.push_back({"Chandragutti", "Shimoga", {"Chandragutti Temple", "Shri Venkateshwara Temple", "Basava Reserve"}, "Religious", 4.4});
destinations.push_back({"Nandihills", "Chikkaballapur", {"Nandi Temple", "Sunrise Point", "Top of Nandi Hill", "Nandi Hills Trekking"}, "Hill", 4.5});
destinations.push_back({"Bhadra Wildlife Sanctuary", "Chikmagalur", {"Bhadra Tiger Reserve", "Bird Watching", "Safari Tours", "Adventure Activities"}, "Wildlife", 4.8});
destinations.push_back({"Srirangapatna", "Mandya", {"Ranganathaswamy Temple", "Tipu Sultan's Summer Palace", "Srirangapatna Fort", "Gumbaz Tombs"}, "Historical", 4.7});
destinations.push_back({"Channarayana Durga", "Tumkur", {"Channarayana Durga Hill", "Hill Trekking", "Fort Walls", "Viewpoint"}, "Hill", 4.4});
destinations.push_back({"Chikmagalur", "Chikmagalur", {"Mullayanagiri", "Baba Budangiri", "Hebbe Falls", "Kudremukh Peak"}, "Hill Station", 4.8});
destinations.push_back({"Badami Caves", "Bagalkot", {"Badami Caves", "Agastya Lake", "Bhuthnatha Temple", "Rock-cut Temples"}, "Historical", 4.7});
destinations.push_back({"Hampi", "Ballari", {"Virupaksha Temple", "Vittala Temple", "Matanga Hill", "Hampi Bazaar"}, "Historical", 4.9});
destinations.push_back({"Gulbarga", "Gulbarga", {"Gulbarga Fort", "Jama Masjid", "Sharana Basaveshwara Temple", "Gulbarga Fort Ruins"}, "Historical", 4.6});
destinations.push_back({"Agumbe", "Shimoga", {"Agumbe Rainforest", "Sunset Point", "Someshwara Wildlife Sanctuary", "Tropical Rainforest Walks"}, "Rainforest", 4.8});
destinations.push_back({"Kabini", "Kodagu", {"Kabini Wildlife Sanctuary", "Kabini River", "Jungle Safari", "Bamboo Grove"}, "Wildlife", 4.8});
destinations.push_back({"Coorg", "Kodagu", {"Abbey Falls", "Tadiandamol Peak", "Namdroling Monastery", "Raja's Seat"}, "Hill Station", 4.9});
destinations.push_back({"Gokak Falls", "Belagavi", {"Gokak Waterfalls", "Gokak Dam", "Satyagraha Sthal", "Khwaja Banda Nawaz Dargah"}, "Waterfalls", 4.5});
destinations.push_back({"Ranganathittu Bird Sanctuary", "Mandya", {"Bird Watching", "Crocodile Observation", "Boat Ride", "Wildlife Conservation"}, "Wildlife", 4.9});
destinations.push_back({"Ramanagaram", "Ramanagaram", {"Ramadevara Betta", "Rock Climbing", "Vulture Sanctuary", "Natural Rock Formations"}, "Adventure", 4.6});
destinations.push_back({"Hassan", "Hassan", {"Hoysaleswara Temple", "Chennakesava Temple", "Shravanabelagola"}, "Religious", 4.7});
destinations.push_back({"Sakleshpur", "Hassan", {"Bisle Reserve Forest", "Manjarabad Fort", "Coffee Plantations", "Sakleshpur Hills"}, "Hill Station", 4.6});
destinations.push_back({"Tungabhadra Dam", "Ballari", {"Tungabhadra Dam", "Anjaneya Hill", "Cultural Sites", "Temples"}, "Dam", 4.7});
destinations.push_back({"Mysore", "Mysore", {"Mysore Palace", "Chamundeshwari Temple", "St. Philomena's Church", "Brindavan Gardens"}, "City", 4.8});
destinations.push_back({"Chitradurga Fort", "Chitradurga", {"Chitradurga Fort", "Beggars' Rock", "Yellur Fort", "Fort Ruins"}, "Historical", 4.7});
destinations.push_back({"Yellapur", "Uttara Kannada", {"Magod Falls", "Unchalli Falls", "Sathodi Falls", "Nature Walks"}, "Waterfalls", 4.6});
destinations.push_back({"Bijapur", "Bijapur", {"Gol Gumbaz", "Bijapur Fort", "Ibrahim Rauza", "Jami Masjid"}, "Historical", 4.5});
destinations.push_back({"Dandeli", "Uttara Kannada", {"Dandeli Wildlife Sanctuary", "Kali River", "Anshi National Park", "Syntheri Rocks"}, "Wildlife", 4.7});
destinations.push_back({"Mangalore", "Dakshina Kannada", {"Tannirbhavi Beach", "Panambur Beach", "Kadri Manjunath Temple", "Mangalore Port"}, "Urban", 4.5});
destinations.push_back({"Dharwad", "Dharwad", {"Unkal Lake", "Chandramouleshwara Temple", "Savadatti Fort", "Dharwad Nature Camp"}, "City", 4.4});
destinations.push_back({"Kolar Gold Fields", "Kolar", {"Kolar Gold Fields", "Kolaramma Temple", "Mulbagal Fort", "Kolar Railway Station"}, "Historical", 4.2});
destinations.push_back({"Halebidu", "Hassan", {"Hoysaleswara Temple", "Chennakesava Temple", "Heritage Walks", "Archaeological Sites"}, "Historical", 4.6});
destinations.push_back({"Agraharas", "Hassan", {"Sri Channakeshava Temple", "Thuruvana Gardens", "Bhimaneshwara Temple", "Old Manjuguni"}, "Religious", 4.4});
destinations.push_back({"Shravanabelagola", "Hassan", {"Bahubali Statue", "Jain Temples", "Shravanabelagola Hill", "Hill Trekking"}, "Religious", 4.7});
destinations.push_back({"Kudremukh", "Chikmagalur", {"Kudremukh Peak", "Baba Budangiri", "Trekking", "Nature Walks"}, "Hill Station", 4.7});
destinations.push_back({"Bangalore", "Bangalore", {"Cubbon Park", "Bangalore Palace", "Lalbagh Botanical Garden", "Vidhana Soudha"}, "Urban", 4.8});
destinations.push_back({"Nandi Hills", "Chikkaballapur", {"Nandi Hills Viewpoint", "Tipu Sultan's Summer Palace", "Nandi Temple", "Sunrise Views"}, "Hill", 4.8});
destinations.push_back({"Yadgir", "Yadgir", {"Yadgir Fort", "Sangameshwar Temple", "Mangalore Falls", "Raichur Fort"}, "Historical", 4.4});
destinations.push_back({"Hampi", "Ballari", {"Vittala Temple", "Matanga Hill", "Virupaksha Temple", "Hampi Bazaar"}, "Historical", 4.9});
destinations.push_back({"Tirupati", "Chikballapur", {"Tirumala Temple", "Sri Venkateshwara Temple", "Shilparamam", "Azhwar Hill"}, "Religious", 4.7});
destinations.push_back({"Shimoga", "Shimoga", {"Jog Falls", "Kubri Falls", "Bhadravathi", "Gajanur Reservoir"}, "Waterfalls", 4.8});
destinations.push_back({"Channarayana Durga", "Tumkur", {"Channarayana Durga Fort", "Hill Trekking", "Viewpoint", "Nature Trails"}, "Hill", 4.5});
destinations.push_back({"Bagalkot", "Bagalkot", {"Badami Caves", "Bhutha Natha Temple", "Jain Temples", "Agastya Lake"}, "Historical", 4.5});
destinations.push_back({"Devagiri", "Siddapura", {"Devagiri Falls", "Brahmagiri Peak", "Elephant Sanctuaries", "Nature Trekking"}, "Hill Station", 4.6});
destinations.push_back({"Tumkur", "Tumkur", {"Nandi Temple", "Siddhappa's Hill", "Silk Farms", "Tumkur Silk Museum"}, "City", 4.2});
destinations.push_back({"Bellary", "Ballari", {"Sri Siddheshwar Temple", "Siddheshwara Fort", "Hampi", "Brahmagiri Hill"}, "Historical", 4.5});
destinations.push_back({"Aihole", "Bagalkot", {"Aihole Temples", "Historical Sites", "Stone Monuments", "Ancient Architecture"}, "Historical", 4.6});
destinations.push_back({"Kadiri", "Anantapur", {"Kadiri Lakshmi Narayan Temple", "Sankasam Reserve", "Kadiri Fort", "Rayadurg Fort"}, "Religious", 4.4});


    // Add more destinations as needed

    string choice;
    do
    {
        showMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == "1") listAllDestinations();
        else if (choice == "2") viewDestinationDetails();
        else if (choice == "3") bookTrip();
        else if (choice == "4") viewBookingDetails();
        else if (choice == "5") filterByCategory();
        else if (choice == "6") planItinerary();
        else if (choice == "7") displayRatingsAndReviews();
        else if (choice == "8") cout << "Exiting... Thank you for using Karnataka Tourism!" << endl;
        else cout << "Invalid choice. Please try again." << endl;

    } while (choice != "8");

    return ;


void updateBookingDetails();
void deleteBooking();
void calculateTotalEstimatedBudget();
void showBookingHistory();
void addNewDestination();
void searchDestinationByName();
void filterDestinationsByDistrict();
void updateDestinationRating();
void filterDestinationsByRating();
void addReviewForDestination();
void calculateBudgetDetails(const Booking& booking);

}


struct MenuItem
 {
    int id;
    string name;
    double price;
    string type; // Veg or Non-Veg
    string category; // Main category like "Rice",
    //"Gravy", "Rotis", "Chinese", etc.
};

// Customer Order Structure
struct Order {
    int ordernumber;  // The order number
    int itemId;       // The item ID being ordered
    int quantity;     // The quantity of the item ordered
    double totalAmount; // The total amount for the order (calculated field)
};



// Additional Enhancements:
// - Add user authentication before allowing access to the system.
// - Implement detailed error logs for each invalid action.
// - Add customer feedback collection and saving functionality.
// - Introduce discount options for large orders or specific items.
// - Display the estimated preparation time based on the number of items ordered.
// - Allow admins to modify the menu dynamically within the program.

// Notes:
// 1. The main function serves as the entry point for the program and provides an interactive menu for users.
// 2. Each menu option is linked to a specific function (e.g., displayMenu, placeOrder) to ensure modularity and readability.
// 3. Extensive error handling is added to validate user inputs and provide meaningful feedback.
// 4. Additional functions (e.g., calculateBill, saveBillToFile) should be implemented in their respective modules for functionality.



// Function to display all orders


void displayMenu(const vector<MenuItem>& menu)
{
    int choice;

    // Prompt the user to choose the type of menu to display
    cout << "Choose the type of menu to view:" << endl;
    cout << "1. Veg Items" << endl;
    cout << "2. Non-Veg Items" << endl;
    cout << "3. Both Veg and Non-Veg Items" << endl;
    cout << "Enter your choice (1/2/3): ";
    cin >> choice;

    // Process the user's choice based on the input
    switch (choice)
    {
        case 1: // Case for displaying vegetarian items only
        {
            cout << "\n--- VEG ITEMS ---" << endl;

            // Define the categories of items to display
            vector<string> categories = {"Rice", "Gravy", "Rotis", "Chinese", "Noodles"};

            // Loop through each category to display vegetarian items under it
            for (const auto& category : categories)
                {
                cout << "\n" << category << ":" << endl; // Print category name

                bool categoryFound = false; // Flag to check if the category contains any items

                // Loop through the menu to find items that match the category and type
                for (const auto& item : menu)
                    {
                    if (item.type == "Veg" && item.category == category)
                    {
                        if (!categoryFound)
                        {
                            categoryFound = true; // Set the flag if an item is found
                        }

                        // Display the item details in a formatted manner
                        cout << setw(5) << item.id << setw(25) << item.name
                             << setw(10) << fixed << setprecision(2) << item.price << endl;
                    }
                }

                // If no items are found for the category, optionally display a message
                if (!categoryFound)
                    {
                        cout << "  No items available in this category." << endl;
                }
            }
            break;
        }

        case 2: // Case for displaying non-vegetarian items only
        {
            cout << "\n--- NON-VEG ITEMS ---" << endl;

            // Define the categories of items to display
            vector<string> categories = {"Rice", "Gravy", "Rotis", "Chinese", "Noodles"};

            // Loop through each category to display non-vegetarian items under it
            for (const auto& category : categories)
                {
                cout << "\n" << category << ":" << endl; // Print category name

                bool categoryFound = false; // Flag to check if the category contains any items

                // Loop through the menu to find items that match the category and type
                for (const auto& item : menu)
                    {
                    if (item.type == "Non-Veg" && item.category == category)
                    {
                        if (!categoryFound)
                         {
                            categoryFound = true; // Set the flag if an item is found
                        }

                        // Display the item details in a formatted manner
                        cout << setw(5) << item.id << setw(25) << item.name
                             << setw(10) << fixed << setprecision(2) << item.price << endl;
                    }
                }

                // If no items are found for the category, optionally display a message
                if (!categoryFound)
                    {
                    cout << "  No items available in this category." << endl;
                }
            }
            break;
        }

        case 3: // Case for displaying both vegetarian and non-vegetarian items
        {
            cout << "\n--- VEG AND NON-VEG ITEMS ---" << endl;

            // Define the categories of items to display
            vector<string> categories = {"Rice", "Gravy", "Rotis", "Chinese", "Noodles"};

            // Loop through each category to display all items under it
            for (const auto& category : categories)
                {
                cout << "\n" << category << ":" << endl; // Print category name

                // Loop through the menu to find items that match the category
                for (const auto& item : menu)
                    {
                    if (item.category == category)
                    {
                        // Display the item details in a formatted manner
                        cout << setw(5) << item.id << setw(25) << item.name<< setw(10) << fixed << setprecision(2) << item.price << endl;
                    }
                }

                // Optionally, add a message if no items are found in the category
                // (This is not implemented here but can be added similarly to cases 1 and 2)
            }
            break;
        }

        default: // Handle invalid input from the user
            cout << "Invalid choice! Please enter 1, 2, or 3." << endl;
            break;
    }

    // Print a separator line to indicate the end of the menu display
    cout << "===========================" << endl;
}

// Function to place an order
// Parameters:
// - menu: A vector containing all menu items (MenuItem objects)
// - orders: A vector to store the orders placed by the user
// This function allows the user to select items from the menu and specify quantities for each order.


void placeOrder(const vector<MenuItem>& menu, vector<Order>& orders) {
    cout << "\n";
    char choice = 'y';
    // Variable to control the loop for placing multiple orders
    int orderNumber = 1;
    // Initialize the first order number for unique tracking

    while (choice == 'y' || choice == 'Y') {
        int itemId, quantity;

        // Prompt user for item ID
        cout << "Enter the item ID you want to order: ";
        cin >> itemId;

        // Check if the entered item ID exists in the menu
        bool found = false;
        // Flag to check if item is found
        double price = 0.0;
        // To store the price of the selected item
        string itemName;
        // To store the name of the selected item

        for (const auto& item : menu) {
            if (item.id == itemId) {
                // Match the item ID
                found = true;
                // Item found in the menu
                price = item.price;
                // Store the item's price
                itemName = item.name;
                // Store the item's name
                break;
                // Exit the loop after finding the item
            }
        }

        // If the item ID is invalid, prompt the user to try again
        if (!found) {
            cout << "Invalid item ID! Please try again." << endl;
            continue;
            // Skip the rest of the loop and re-prompt
        }

        // Prompt user for quantity of the selected item
        cout << "Enter the quantity: ";
        cin >> quantity;

        // Validate the entered quantity
        if (quantity <= 0) {
            cout << "\nInvalid quantity! Please enter a positive number." << endl;
            continue;
        }

        // Create an order object with the entered details
        Order order;
        order.ordernumber = orderNumber++;
        // Assign a unique order number
        order.itemId = itemId;
        // Store the item ID
        order.quantity = quantity;
        // Store the quantity
        order.totalAmount = price * quantity;
        // Calculate the total amount for this order

        orders.push_back(order); // Add the order to the list of orders

        // Display confirmation message to the user
        cout << "Order placed successfully for item '" << itemName
             << "\n' (Qty: " << quantity << ", Total: " << fixed
             << setprecision(2) << order.totalAmount << ")!" << endl;

        // Ask if the user wants to order another item
        cout << "Do you want to order another item? (y/n): ";
        cin >> choice;
    }

    // After placing orders, provide options to review, edit, or finalize
    char finalizeChoice;
    do {
        cout << "\nYour Current Orders:" << endl;
        cout << setw(10) << "Order No" << setw(15) << "Item ID"
             << setw(10) << "Quantity" << setw(15) << "Total Amount" << endl;


        cout << "\n---------------------------------------------------------" << endl;

        // Display the list of current orders
        for (const auto& order : orders) {
            cout << setw(10) << order.ordernumber << setw(15) << order.itemId
                 << setw(10) << order.quantity << setw(15) << fixed
                 << setprecision(2) << order.totalAmount << endl;
        }

        // Ask the user if they want to edit or finalize the order
        cout << "\nOptions:\n1. Edit an Order\n2. Remove an Order\n3. Finalize Order\n";
        cout << "Enter your choice (1/2/3): ";
        cin >> finalizeChoice;

        if (finalizeChoice == '1') {
            // Edit an existing order
            int editOrderNumber;
            cout << "Enter the order number you want to edit: ";
            cin >> editOrderNumber;

            auto it = find_if(orders.begin(), orders.end(),
                              [editOrderNumber](const Order& o) {
                                  return o.ordernumber == editOrderNumber;
                              });

            if (it != orders.end()) {
                cout << "Enter new quantity for item ID " << it->itemId << ": ";
                cin >> it->quantity;

                // Validate and update the order
                if (it->quantity > 0) {
                    it->totalAmount = it->quantity * menu[it->itemId - 1].price; // Update total
                    cout << "Order updated successfully!" << endl;
                } else {
                    cout << "Invalid quantity! Order not updated." << endl;
                }
            } else {
                cout << "Order number not found!" << endl;
            }
        } else if (finalizeChoice == '2') {
            // Remove an existing order
            int removeOrderNumber;
            cout << "Enter the order number you want to remove: ";
            cin >> removeOrderNumber;

            auto it = remove_if(orders.begin(), orders.end(),
                                [removeOrderNumber](const Order& o) {
                                    return o.ordernumber == removeOrderNumber;
                                });

            if (it != orders.end()) {
                orders.erase(it, orders.end());
                cout << "Order removed successfully!" << endl;
            } else {
                cout << "Order number not found!" << endl;
            }
        } else if (finalizeChoice != '3') {
            cout << "Invalid choice! Try again." << endl;
        }
    } while (finalizeChoice != '3');

    cout << "Order finalized! Proceeding to checkout..." << endl;
}




// Function to calculate the bill




// Function to calculate and display the bill
void calculateBill(const vector<MenuItem>& menu, const vector<Order>& orders) {
    // Check if any orders have been placed
    if (orders.empty()) {
        cout << "No orders placed yet!" << endl;
        cout << "---------------------------------------------------------" << endl;
        cout <<"\n";
        return;
        // Exit the function as there's nothing to process
    }

    // Variables to track the total bill amount
    double totalBill = 0.0;
    double taxRate = 0.07;
    // Example tax rate (7%)
    double discount = 0.0;
     // Discount amount
    double grandTotal = 0.0;
     // Total after tax and discount

    // Display the header of the bill
    cout << "\n====================================== BILL ======================================" << endl;
    cout << left << setw(5) << "No."
         << setw(10) << "Item ID"
         << setw(30) << "Item Name"
         << setw(10) << "Quantity"
         << setw(15) << "Unit Price"
         << setw(15) << "Total Price" << endl;
    cout << "===================================================================================\n";

    int orderNumber = 1;  // To track order numbers
    for (const auto& order : orders) {
        bool itemFound = false; // Flag to check if the item is found in the menu

        // Search for the item in the menu
        for (const auto& item : menu) {
            if (item.id == order.itemId) {
                double totalPrice = item.price * order.quantity; // Calculate total price for the item
                totalBill += totalPrice;  // Add the item's price to the total bill

                // Display the details of the current order item
                cout << left << setw(5) << orderNumber
                     << setw(10) << item.id
                     << setw(30) << item.name
                     << setw(10) << order.quantity
                     << setw(15) << fixed << setprecision(2) << item.price
                     << setw(15) << fixed << setprecision(2) << totalPrice << endl;

                itemFound = true; // Mark item as found
                orderNumber++; // Increment order number for the next item
                break; // Exit the inner loop as the item is found
            }
        }

        // If the item ID was not found in the menu, issue a warning
        if (!itemFound) {
            cout << "Warning: Item with ID " << order.itemId << " not found in the menu!" << endl;
        }
    }

    // Display a separator line for better readability
    cout << "===================================================================================\n";

    // Display the total bill before tax and discounts
    cout << right << setw(75) << "Subtotal: " << fixed << setprecision(2) << totalBill << endl;

    // Calculate tax
    double taxAmount = totalBill * taxRate;
    cout << right << setw(75) << "Tax (7%): " << fixed << setprecision(2) << taxAmount << endl;

    // Example discount application
    if (totalBill > 100.0) {
        discount = totalBill * 0.10;
        // Apply a 10% discount for bills over $100
        cout << right << setw(75) << "Discount (10%): " << fixed << setprecision(2) << discount << endl;
    }

    // Calculate the grand total (subtotal + tax - discount)
    grandTotal = totalBill + taxAmount - discount;
    cout << right << setw(75) << "Grand Total: " << fixed << setprecision(2) << grandTotal << endl;

    // Final decorative footer for the bill
    cout << "===================================================================================\n";
    cout << "                             Thank you for your order!                             " << endl;
    cout << "===================================================================================\n";
    cout << endl;

    // Additional breakdown and feedback instructions
    cout << "Detailed Breakdown:\n";
    cout << "- Items purchased: " << orders.size() << endl;
    cout << "- Tax included: " << fixed << setprecision(2) << taxAmount << endl;
    cout << "- Discount applied: " << fixed << setprecision(2) << discount << endl;
    cout << "- Final amount: " << fixed << setprecision(2) << grandTotal << endl;
    cout << endl;

    // Provide feedback instructions
    cout << "  We value your feedback! Please leave a review or suggestion at our website.\n";
    cout << "                   Visit us again soon! Have a great day.\n";

    // Decorative separator for ending the bill
    cout << "===================================================================================\n";
}



// Function to display the list of orders
// Parameters:
// - menu: A vector containing all menu items (MenuItem objects)
// - orders: A vector containing all orders placed by the user (Order objects)
// Displays detailed information about the orders, including item details, quantity, and cost.




// Function to display all orders placed by the customer
void displayOrders(const vector<MenuItem>& menu, const vector<Order>& orders) {
    // Check if any orders have been placed
    if (orders.empty()) {
        cout << "No orders placed yet!" << endl;
        // Inform the user if no orders exist
        cout << "---------------------------------------------------------" << endl;
        cout <<"\n";
        return;
        // Exit the function as there's nothing to display
    }




    // Header section to display order details
    cout << "\n============================== YOUR ORDERS ==========================" << endl;
    cout << setw(5) << "No." // Serial number of the order
         << setw(10) << "Item ID" // Unique identifier for the menu item
         << setw(30) << "Item Name" // Name of the item
         << setw(10) << "Quantity" // Quantity ordered by the customer
         << endl;
    cout << "=====================================================================\n";

    // Variable to track the order number displayed to the user
    int orderDisplayNumber = 1;

    // Loop through each order placed by the customer
    for (const auto& order : orders) {
        bool found = false; // Flag to check if the item ID exists in the menu

        // Loop through the menu to find the details of the ordered item
        for (const auto& item : menu) {
            if (item.id == order.itemId) {
                // Display order details if the item ID matches
                cout << setw(5) << orderDisplayNumber // Display the current order number
                     << setw(10) << item.id // Display the unique item ID
                     << setw(30) << item.name // Display the item's name from the menu
                     << setw(10) << order.quantity // Display the ordered quantity
                     << endl;
                found = true; // Mark item as found in the menu
                orderDisplayNumber++; // Increment the display number for the next order
                break; // Exit the loop once the matching item is found
            }
        }

        // If the item ID was not found in the menu, display a warning
        if (!found) {
            cout << "Warning: Item with ID " << order.itemId << " not found in the menu!" << endl;
        }
    }

    // Footer section for clarity and better presentation
    cout << "=====================================================================\n";

    // Provide summary details
    cout << "Order Summary:\n";
    cout << "- Total Orders: " << orders.size() << endl; // Display total number of orders

    // Calculate total items ordered
    int totalItems = 0;
    for (const auto& order : orders) {
        totalItems += order.quantity; // Accumulate total quantity of items ordered
    }
    cout << "- Total Items Ordered: " << totalItems << endl; // Display total items ordered

    // Highlight special notes or information
    cout << "\n         Note: Please verify your orders carefully. If there are\n";
    cout << "                 discrepancies, inform the staff.\n";
    cout << "=====================================================================\n";

    // Additional feature: Display decorative section for enhanced readability
    cout << "                      Thank you for your orders!                          \n";
    cout << "=====================================================================\n";

}


// Function to save the bill to a file


void saveBillToFile(const vector<MenuItem>& menu, const vector<Order>& orders) {
    // Check if any orders are placed
    if (orders.empty()) {
        cout << "No orders placed yet! Nothing to save." << endl;
        return;
    }

    // Open the bill file for writing
    ofstream billFile("bill.txt");
    if (!billFile) {
        cout << "Failed to create the bill file!" << endl;
        return;
    }

    // Get the current time to timestamp the bill
    time_t now = time(0);
    char* dt = ctime(&now);
    double total = 0.0;

    // Write the bill to the file
    cout << "\n============================== YOUR ORDERS ==========================" << endl;
    billFile << "Generated On: " << dt << "\n";
    billFile << setw(10) << "Item ID" << setw(25) << "Item Name" << setw(10) << "Qty" << setw(10) << "Unit Price" << setw(10) << "Cost" << "\n";
    billFile << "===============================================================\n";
    bool unmatchedOrder = false;

    // Process each order
    for (const auto& order : orders) {
        bool found = false;
        for (const auto& item : menu) {
            if (item.id == order.itemId) {
                double cost = item.price * order.quantity;
                billFile << setw(10) << item.id << setw(25) << item.name << setw(10) << order.quantity << setw(10) << fixed << setprecision(2) << item.price << setw(10) << cost << "\n";
                total += cost;
                found = true;
                break;
            }
        }

        // Handle items not found in the menu
        if (!found) {
            unmatchedOrder = true;
            billFile << "Unmatched Order: Item ID " << order.itemId << " not found in menu.\n";
        }
    }

    // Write the total bill amount
    billFile << "===============================================================\n";
    billFile << "Total Bill: " << fixed << setprecision(2) << total << "\n";
    billFile << "===============================================================\n";
    billFile << "Thank you for dining with us! Please visit again.\n";
    if (unmatchedOrder) {
        billFile << "\n(Note: Some items in the order could not be matched to the menu.)\n";
    }

    // Close the file after writing
    billFile.close();
    cout << "Bill saved to 'bill.txt' successfully!" << endl;

    // Check file size (optional)
    ifstream checkFile("bill.txt", ios::ate);
    if (checkFile.is_open()) {
        size_t fileSize = checkFile.tellg();
        if (fileSize > 0) {
            cout << "File verification successful. File size: " << fileSize << " bytes." << endl;
        } else {
            cout << "Warning: File saved but seems empty!" << endl;
        }
        checkFile.close();
    }
}



// Function to display the exit message
void displayExitMessage()
{
  displayLine();
    cout << "     Thank You for Visiting Us!        " << endl;
    cout << "   We hope to see you again soon.      " << endl;
  displayLine();
}



// Function to display the exit message
// Function to display the exit message
void displayExitMessages()
{
    // Display an introductory thank-you note
    cout << endl;
    cout << "*" << endl;
    cout << "*                                               *" << endl;
    cout << "*            Thank You for Visiting Us!         *" << endl;
    cout << "*          We hope to see you again soon.       *" << endl;
    cout << "*                                               *" << endl;
    cout << "*" << endl;
    cout << endl;

    // Add a decorative border
    cout << "-------------------------------------------------" << endl;

    // Provide a farewell message with additional detail
    cout << "It was a pleasure serving you today." << endl;
    cout << "Don't forget to leave your feedback to help us improve!" << endl;
    cout << "We aim to bring you the best experience every time." << endl;
    cout << "-------------------------------------------------" << endl;

    // Display a motivational quote to leave a positive impression
    cout << "Quote of the Day:" << endl;
    cout << "\"The secret of success in life is to eat what you like\n"
         << "and let the food fight it out inside.\" - Mark Twain" << endl;
    cout << "-------------------------------------------------" << endl;

    // Add an ASCII art-style decorative message
    cout << "   ____   __   ____  _  _  __  ____  ____  _  _   " << endl;
    cout << "  (  _ \\ / _\\ (  _ \\/ )( \\(  )(  _ \\(  __)( \\/ )  " << endl;
    cout << "   ) _ (/    \\ ) _ (\\ /\\ / )(  ) __/ ) _) / \\/ \\  " << endl;
    cout << "  (/\\/\\/(/(/\\)()()  ()\\)(/  " << endl;

    // Additional aesthetic separation
    cout << endl;
    cout << "-------------------------------------------------" << endl;

    // Suggest next visit ideas
    cout << "Don't miss our upcoming events and seasonal specials!" << endl;
    cout << "We look forward to making your next visit even better." << endl;
    cout << endl;

    // Add instructions for additional services or social media
    cout << "Follow us on social media for the latest updates and offers:" << endl;
    cout << "Facebook: @OurRestaurant | Instagram: @OurRestaurant" << endl;
    cout << "Tag us in your photos using #OurRestaurantExperience" << endl;

    // Decorative separator
    cout << "-------------------------------------------------" << endl;

    // Provide a thank-you message in a different language for inclusivity
    cout << "Merci! Gracias! Danke! Grazie! Arigato! Shukriya!" << endl;
    cout << "No matter how you say it, we appreciate your visit!" << endl;

    // Add a "see you again" message with an ASCII smiley face
    cout << "-------------------------------------------------" << endl;
    cout << "               See you again soon! :)            " << endl;
    cout << "-------------------------------------------------" << endl;

    // Add a concluding decorative message with stars
    cout << "*" << endl;
    cout << "*                                               *" << endl;
    cout << "*                Bon App�tit!                   *" << endl;
    cout << "*       Your satisfaction is our priority!      *" << endl;
    cout << "*                                               *" << endl;
    cout << "*" << endl;
    cout << endl;
}


// It is a function that searches for a menu item by its name,
//  ignoring case sensitivity.
//  This function can be added to your existing code.

// Function to search for a menu item by name (case insensitive)

// Function to search for a menu item by name (case insensitive)

// Function to search for a menu item by name (case insensitive)
void searchMenuItem(const vector<MenuItem>& menu) {
    string searchName;


    // Prompt the user to enter the item name they wish to search for
    cout << "\n                Main Menu - Explore Search Options" << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << "1. Search for Veg Items       - Find healthy vegetarian" << endl;
    cout << "2. Search for Non-Veg Items   - Tasty non-vegetarian dishes" << endl;
    cout << "3. Search All Menu Items      - Browse through all items" << endl;

    cout << "---------------------------------------------------------" << endl;

    // Ask for the search term from the user
    cout << "Enter the name of the item to search: ";
    cin.ignore();
    // Clear the input buffer
    getline(cin, searchName);
    // Get the full line input for item name

    // Convert searchName to lowercase for case-insensitive comparison
    transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

    bool found = false;

    // Display search results header
    cout << "\n--------------------- SEARCH RESULT-----------------------" << endl;
    cout << "  ID   Item Name                   Price" << endl; // Header for the results
    cout << "----------------------------------------------------------" << endl;

    // Loop through the menu and search for matching items
    for (const auto& item : menu) {
        string itemNameLower = item.name;
        // Create a copy of the item name
        transform(itemNameLower.begin(), itemNameLower.end(), itemNameLower.begin(), ::tolower); // Convert to lowercase

        // Check if the lowercase item name contains the search term
        if (itemNameLower.find(searchName) != string::npos) {
            // Display found item details in a formatted way
            cout << setw(5) << item.id << setw(25) << item.name
                 << setw(10) << fixed << setprecision(2) << item.price << endl;
            found = true;
        }
    }

    // If no matching items were found, display a message
    if (!found) {
        cout << "          No items found matching your search." << endl;
    }

    // End the search with a separator for aesthetics
    cout << "---------------------------------------------------------" << endl;
    cout <<"\n";
}



// Function to collect feedback from the user
void collectFeedbacks() {
    string feedbackText;
    int rating;

    // Prompt for feedback text
    cout << "\n============================== FeedBACK ==============================" << endl;
    cout << "We value your feedback! Please share your thoughts about our service:" << endl;
    cout << "-----------------------------------------------------------------------" << endl;
    cout << "Enter your feedback: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, feedbackText); // Get the full line of feedback

    // Prompt for rating
    cout << "Please rate your experience from 1 to 5 \n(1 being the lowest and 5 being the highest): ";
    cin >> rating;

    // Validate rating input
    while (rating < 1 || rating > 5) {
        cout << "Invalid rating! Please enter a number between 1 and 5: ";
        cin >> rating;
    }

    // Respond based on the rating
    cout << "-----------------------------------------------------------------------" << endl;
    cout << "Thank you for your feedback!" << endl;

    switch (rating) {
        case 1:
            cout <<"\n";
            cout << "We're sorry to hear that you had a poor experience. \nWe will strive to improve!" << endl;
            break;
        case 2:
            cout <<"\n";
            cout << "We appreciate your input and will work on making things better." << endl;
            break;
        case 3:
            cout <<"\n";
            cout << "We're glad you had an average experience, \nbut we aim to do better." << endl;
            break;
        case 4:
            cout <<"\n";
            cout << "We're happy to hear you had a good experience. \nYour feedback motivates us!" << endl;
            break;
        case 5:
            cout <<"\n";
            cout << "We're thrilled you had an excellent experience!" << endl;
            break;
    }


    // Save feedback to a file
    ofstream feedbackFile("feedback.txt", ios::app); // Open file in append mode
    if (feedbackFile.is_open()) {
        feedbackFile << "Food and dining Management System Feedback:" << endl;
        feedbackFile << "Feedback: " << feedbackText << endl;
        feedbackFile << "Rating: " << rating << endl;
        feedbackFile << "---------------------------------------------------" << endl;
        feedbackFile.close(); // Close the file
        cout << "Your feedback has been saved successfully!" << endl;
    } else {
        cout << "Error: Unable to save feedback to file." << endl;
    }

    // Display a closing message
    cout << "\nYour feedback is important to us, \nand we will use it to enhance our services." << endl;
    cout << "=======================================================================" << endl;
}



void manageFoodAndDining() {

    vector<MenuItem> menu = {
    // Veg Items (60 items)
    // Rice
    {1, "Veg Fried Rice", 70, "Veg", "Rice"},
    {2, "Veg Biryani", 80, "Veg", "Rice"},
    {3, "Veg Pulao", 60, "Veg", "Rice"},
    {4, "Veg Khichdi", 50, "Veg", "Rice"},
    {5, "Veg Lemon Rice", 55, "Veg", "Rice"},
    {6, "Jeera Rice", 50, "Veg", "Rice"},
    {7, "Veg Pulav", 65, "Veg", "Rice"},
    {8, "Veg Pulao with Peas", 60, "Veg", "Rice"},
    {9, "Coconut Rice", 65, "Veg", "Rice"},
    {10, "Saffron Rice", 75, "Veg", "Rice"},
    {11, "Veg Mushroom Rice", 70, "Veg", "Rice"},
    {12, "Veg Peas Pulao", 60, "Veg", "Rice"},
    {13, "Veg Biryani with Tofu", 85, "Veg", "Rice"},
    {14, "Veg Corn Rice", 60, "Veg", "Rice"},
    {15, "Veg Spinach Rice", 65, "Veg", "Rice"},
    {16, "Veg Curd Rice", 60, "Veg", "Rice"},
    {17, "Veg Tamarind Rice", 70, "Veg", "Rice"},
    {18, "Veg Pulao with Cashews", 75, "Veg", "Rice"},
    {19, "Tomato Rice", 60, "Veg", "Rice"},
    {20, "Veg Saag Rice", 70, "Veg", "Rice"},

    // Gravy
    {21, "Paneer Butter Masala", 90, "Veg", "Gravy"},
    {22, "Dal Tadka", 65, "Veg", "Gravy"},
    {23, "Aloo Gobi", 70, "Veg", "Gravy"},
    {24, "Mixed Vegetable Curry", 75, "Veg", "Gravy"},
    {25, "Palak Paneer", 80, "Veg", "Gravy"},
    {26, "Mushroom Masala", 85, "Veg", "Gravy"},
    {27, "Baingan Bharta", 70, "Veg", "Gravy"},
    {28, "Paneer Korma", 100, "Veg", "Gravy"},
    {29, "Kadhi Pakora", 75, "Veg", "Gravy"},
    {30, "Chana Masala", 65, "Veg", "Gravy"},
    {31, "Tofu Butter Masala", 90, "Veg", "Gravy"},
    {32, "Vegetable Kofta", 85, "Veg", "Gravy"},
    {33, "Zucchini Curry", 80, "Veg", "Gravy"},
    {34, "Pumpkin Curry", 75, "Veg", "Gravy"},
    {35, "Methi Malai Murg", 95, "Veg", "Gravy"},
    {36, "Methi Aloo", 60, "Veg", "Gravy"},
    {37, "Chana Saag", 75, "Veg", "Gravy"},
    {38, "Spinach Dal", 65, "Veg", "Gravy"},
    {39, "Lauki Curry", 70, "Veg", "Gravy"},
    {40, "Dal Makhani", 85, "Veg", "Gravy"},

    // Rotis
    {41, "Tandoori Roti", 20, "Veg", "Rotis"},
    {42, "Garlic Naan", 30, "Veg", "Rotis"},
    {43, "Butter Naan", 30, "Veg", "Rotis"},
    {44, "Cheese Naan", 40, "Veg", "Rotis"},
    {45, "Lachha Paratha", 30, "Veg", "Rotis"},
    {46, "Missi Roti", 30, "Veg", "Rotis"},
    {47, "Methi Paratha", 30, "Veg", "Rotis"},
    {48, "Aloo Paratha", 40, "Veg", "Rotis"},
    {49, "Kulcha", 30, "Veg", "Rotis"},
    {50, "Chapati", 20, "Veg", "Rotis"},
    {51, "Pesto Naan", 40, "Veg", "Rotis"},
    {52, "Garlic Paratha", 35, "Veg", "Rotis"},
    {53, "Methi Naan", 35, "Veg", "Rotis"},
    {54, "Jowar Roti", 30, "Veg", "Rotis"},
    {55, "Multigrain Paratha", 35, "Veg", "Rotis"},
    {56, "Bajra Roti", 30, "Veg", "Rotis"},
    {57, "Cheese Garlic Naan", 45, "Veg", "Rotis"},
    {58, "Pulao Roti", 40, "Veg", "Rotis"},
    {59, "Paneer Paratha", 45, "Veg", "Rotis"},
    {60, "Stuffed Aloo Paratha", 50, "Veg", "Rotis"},

    // Chinese
    {61, "Veg Manchurian", 80, "Veg", "Chinese"},
    {62, "Veg Sweet and Sour", 70, "Veg", "Chinese"},
    {63, "Veg Hakka Noodles", 70, "Veg", "Chinese"},
    {64, "Veg Fried Rice", 70, "Veg", "Chinese"},
    {65, "Veg Chili", 75, "Veg", "Chinese"},
    {66, "Veg Szechuan Noodles", 70, "Veg", "Chinese"},
    {67, "Veg Hot and Sour Soup", 55, "Veg", "Chinese"},
    {68, "Veg Spring Roll", 60, "Veg", "Chinese"},
    {69, "Veg Corn Soup", 50, "Veg", "Chinese"},
    {70, "Veg Manchow Soup", 55, "Veg", "Chinese"},
    {71, "Veg Dumplings", 60, "Veg", "Chinese"},
    {72, "Veg Chilli Paneer", 80, "Veg", "Chinese"},
    {73, "Veg Szechuan Fried Rice", 75, "Veg", "Chinese"},
    {74, "Veg Fried Momos", 65, "Veg", "Chinese"},
    {75, "Veg Hot Pot", 80, "Veg", "Chinese"},
    {76, "Veg Tofu Manchurian", 80, "Veg", "Chinese"},
    {77, "Veg Crispy Corn", 70, "Veg", "Chinese"},
    {78, "Veg Cantonese Noodles", 75, "Veg", "Chinese"},
    {79, "Veg Schezwan Fried Rice", 75, "Veg", "Chinese"},
    {80, "Veg Stir Fried Veggies", 70, "Veg", "Chinese"},

    // Noodles
    {81, "Veg Hakka Noodles", 70, "Veg", "Noodles"},
    {82, "Veg Chopsuey", 80, "Veg", "Noodles"},
    {83, "Veg Noodles with Vegetables", 70, "Veg", "Noodles"},
    {84, "Veg Stir Fry Noodles", 80, "Veg", "Noodles"},
    {85, "Veg Schezwan Noodles", 80, "Veg", "Noodles"},
    {86, "Veg Spaghetti", 90, "Veg", "Noodles"},
    {87, "Veg Udon Noodles", 90, "Veg", "Noodles"},
    {88, "Veg Pad Thai Noodles", 80, "Veg", "Noodles"},
    {89, "Veg Lo Mein", 80, "Veg", "Noodles"},
    {90, "Veg Stir Fry Rice Noodles", 70, "Veg", "Noodles"},
    {91, "Veg Ramen", 85, "Veg", "Noodles"},
    {92, "Veg Pad See Ew", 80, "Veg", "Noodles"},
    {93, "Veg Noodles with Tofu", 90, "Veg", "Noodles"},
    {94, "Veg Stir Fried Udon", 80, "Veg", "Noodles"},
    {95, "Veg Laksa", 90, "Veg", "Noodles"},
    {96, "Veg Bean Sprout Noodles", 80, "Veg", "Noodles"},
    {97, "Veg Chow Mein", 75, "Veg", "Noodles"},
    {98, "Veg Thai Curry Noodles", 85, "Veg", "Noodles"},
    {99, "Veg Garlic Noodles", 70, "Veg", "Noodles"},
    {100, "Veg Udon Stir Fry", 80, "Veg", "Noodles"},

    // Non-Veg Items (60 items)
    // Rice
    {101, "Chicken Fried Rice", 90, "Non-Veg", "Rice"},
    {102, "Chicken Biryani", 100, "Non-Veg", "Rice"},
    {103, "Mutton Biryani", 130, "Non-Veg", "Rice"},
    {104, "Chicken Pulao", 85, "Non-Veg", "Rice"},
    {105, "Fish Biryani", 120, "Non-Veg", "Rice"},
    {106, "Egg Fried Rice", 75, "Non-Veg", "Rice"},
    {107, "Chicken Pulao with Peas", 90, "Non-Veg", "Rice"},
    {108, "Mutton Pulao", 115, "Non-Veg", "Rice"},
    {109, "Fish Pulao", 110, "Non-Veg", "Rice"},
    {110, "Egg Biryani", 80, "Non-Veg", "Rice"},
    {111, "Chicken Pulao with Cashews", 95, "Non-Veg", "Rice"},
    {112, "Chicken Tandoori Biryani", 130, "Non-Veg", "Rice"},
    {113, "Prawn Biryani", 135, "Non-Veg", "Rice"},
    {114, "Mutton Khichdi", 125, "Non-Veg", "Rice"},
    {115, "Fish and Rice", 110, "Non-Veg", "Rice"},
    {116, "Prawn Fried Rice", 125, "Non-Veg", "Rice"},
    {117, "Chicken Lemon Rice", 100, "Non-Veg", "Rice"},
    {118, "Mutton Lemon Rice", 120, "Non-Veg", "Rice"},
    {119, "Egg Pulao", 95, "Non-Veg", "Rice"},

    // Gravy
    {120, "Butter Chicken", 130, "Non-Veg", "Gravy"},
    {121, "Chicken Tikka Masala", 120, "Non-Veg", "Gravy"},
    {122, "Mutton Rogan Josh", 150, "Non-Veg", "Gravy"},
    {123, "Chicken Korma", 120, "Non-Veg", "Gravy"},
    {124, "Fish Curry", 140, "Non-Veg", "Gravy"},
    {125, "Prawn Masala", 150, "Non-Veg", "Gravy"},
    {126, "Chicken Do Pyaza", 130, "Non-Veg", "Gravy"},
    {127, "Chicken Vindaloo", 140, "Non-Veg", "Gravy"},
    {128, "Egg Curry", 100, "Non-Veg", "Gravy"},
    {129, "Mutton Korma", 135, "Non-Veg", "Gravy"},
    {130, "Chicken Malai Tikka", 120, "Non-Veg", "Gravy"},
    {131, "Fish Malai Curry", 140, "Non-Veg", "Gravy"},
    {132, "Mutton Shami Kebab", 145, "Non-Veg", "Gravy"},
    {133, "Egg Bhurji", 100, "Non-Veg", "Gravy"},
    {134, "Tandoori Prawns", 150, "Non-Veg", "Gravy"},
    {135, "Mutton Seekh Kebab", 130, "Non-Veg", "Gravy"},
    {136, "Tandoori Fish", 140, "Non-Veg", "Gravy"},
    {137, "Mutton Nihari", 160, "Non-Veg", "Gravy"},
    {138, "Chili Chicken", 140, "Non-Veg", "Gravy"},

    // Rotis
    {139, "Butter Chicken Naan", 40, "Non-Veg", "Rotis"},
    {140, "Chicken Tikka Naan", 40, "Non-Veg", "Rotis"},
    {141, "Keema Paratha", 40, "Non-Veg", "Rotis"},
    {142, "Mutton Naan", 40, "Non-Veg", "Rotis"},
    {143, "Prawn Paratha", 50, "Non-Veg", "Rotis"},
    {144, "Egg Paratha", 40, "Non-Veg", "Rotis"},
    {145, "Chicken Paratha", 50, "Non-Veg", "Rotis"},
    {146, "Mutton Paratha", 50, "Non-Veg", "Rotis"},
    {147, "Keema Naan", 40, "Non-Veg", "Rotis"},
    {148, "Tandoori Chicken Naan", 50, "Non-Veg", "Rotis"},
    {149, "Chicken Malai Naan", 45, "Non-Veg", "Rotis"},
    {150, "Cheese Chicken Naan", 55, "Non-Veg", "Rotis"},
    {151, "Mutton Seekh Naan", 50, "Non-Veg", "Rotis"},
    {152, "Lamb Kebab Paratha", 55, "Non-Veg", "Rotis"},
    {153, "Prawn Kebab Paratha", 60, "Non-Veg", "Rotis"},
    {154, "Tandoori Mutton Naan", 55, "Non-Veg", "Rotis"},
    {155, "Tandoori Chicken Paratha", 55, "Non-Veg", "Rotis"},
    {156, "Chicken Shawarma Naan", 60, "Non-Veg", "Rotis"},
    {157, "Mutton Bhuna Paratha", 55, "Non-Veg", "Rotis"},
    {158, "Fish Tikka Naan", 50, "Non-Veg", "Rotis"},

    // Chinese
    {159, "Chicken Manchurian", 100, "Non-Veg", "Chinese"},
    {160, "Egg Manchurian", 90, "Non-Veg", "Chinese"},
    {161, "Chicken Sweet and Sour", 95, "Non-Veg", "Chinese"},
    {162, "Chicken Fried Rice", 90, "Non-Veg", "Chinese"},
    {163, "Chicken Hakka Noodles", 85, "Non-Veg", "Chinese"},
    {164, "Fish Sweet and Sour", 110, "Non-Veg", "Chinese"},
    {165, "Egg Fried Rice", 75, "Non-Veg", "Chinese"},
    {166, "Chicken Chili", 95, "Non-Veg", "Chinese"},
    {167, "Chicken Szechuan", 90, "Non-Veg", "Chinese"},
    {168, "Prawn Hakka Noodles", 110, "Non-Veg", "Chinese"},
    {169, "Chicken Tikka Momos", 100, "Non-Veg", "Chinese"},
    {170, "Prawn Manchurian", 120, "Non-Veg", "Chinese"},
    {171, "Fish Hakka Noodles", 120, "Non-Veg", "Chinese"},
    {172, "Chicken Szechuan Noodles", 95, "Non-Veg", "Chinese"},
    {173, "Fish Manchurian", 115, "Non-Veg", "Chinese"},
    {174, "Egg Chow Mein", 80, "Non-Veg", "Chinese"},
    {175, "Prawn Sweet and Sour", 115, "Non-Veg", "Chinese"},
    {176, "Chicken Dumplings", 100, "Non-Veg", "Chinese"},
    {177, "Mutton Sweet and Sour", 120, "Non-Veg", "Chinese"},
    {178, "Tandoori Chicken Wings", 125, "Non-Veg", "Chinese"},

    // Noodles
    {179, "Chicken Hakka Noodles", 85, "Non-Veg", "Noodles"},
    {180, "Egg Stir Fry Noodles", 80, "Non-Veg", "Noodles"},
    {181, "Mutton Noodles", 100, "Non-Veg", "Noodles"},
    {182, "Prawn Noodles", 110, "Non-Veg", "Noodles"},
    {183, "Fish Noodles", 110, "Non-Veg", "Noodles"},
    {184, "Chicken Stir Fry Noodles", 90, "Non-Veg", "Noodles"},
    {185, "Egg Chowmein", 80, "Non-Veg", "Noodles"},
    {186, "Prawn Fried Noodles", 110, "Non-Veg", "Noodles"},
    {187, "Mutton Stir Fry Noodles", 100, "Non-Veg", "Noodles"},
    {188, "Chicken Stir Fry Noodles", 90, "Non-Veg", "Noodles"},
    {189, "Egg Hakka Noodles", 85, "Non-Veg", "Noodles"},
    {190, "Mutton Udon Noodles", 110, "Non-Veg", "Noodles"},
    {191, "Prawn Schezwan Noodles", 120, "Non-Veg", "Noodles"},
    {192, "Chicken Pad Thai Noodles", 100, "Non-Veg", "Noodles"},
    {193, "Mutton Chowmein", 110, "Non-Veg", "Noodles"},
    {194, "Egg Spaghetti", 90, "Non-Veg", "Noodles"},
    {195, "Fish Stir Fry Noodles", 115, "Non-Veg", "Noodles"},
    {196, "Prawn Pad Thai", 120, "Non-Veg", "Noodles"},
    {197, "Mutton Lo Mein", 120, "Non-Veg", "Noodles"},
    {198, "Chicken Ramen", 110, "Non-Veg", "Noodles"},
};

    vector<Order> orders;

    // Welcome message with aesthetic formatting

    displayLine();
    cout << "        FOOD & DINING MANAGEMENT System!\n" << endl;
    displayLine();



    int choice;
    do{        // Display menu options with numbered choices
        cout << "\n      Menu Options:" << endl;
        cout << "-----------------------------------";
        cout <<"\n";
        cout << "1. View Menu" << endl;
        cout << "2. Search Menu Item" << endl;
        cout << "3. Place Order" << endl;
        cout << "4. Display Orders" << endl;
        cout << "5. Calculate Bill" << endl;
        cout << "6. Save Bill to File" << endl;
        cout << "7. Feedback" << endl;
        cout << "8. Exit" << endl;

        // enter your choice
        cout << "Enter your choice : ";
        if (!(cin >> choice)) {
            cout << "\nInvalid input! \nPlease enter a number between 1 and 11." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }


        switch (choice)
        {
            case 1:
                displayMenu(menu);
                break;
            case 2:
                searchMenuItem(menu);
                break;
            case 3:
                placeOrder(menu, orders);
                break;
            case 4:
                displayOrders(menu, orders);
                break;
            case 5:
                calculateBill(menu, orders);
                break;
            case 6:
                saveBillToFile(menu, orders);
                break;
            case 7:
                collectFeedbacks();
                // Call the feedback function
                break;
            case 8:
                displayExitMessage();
                break;
            default:
                cout << "\n";
                cout << "Invalid choice!";
                cout << "\n";
                cout << "Please enter a number between 1 and 6." << endl;
                cout << "\n";

        }

    } while (choice != 8);


    return ;
    // Placeholder for Transportation Management logic
    cout << "Transportation Management Module" << endl;
}

void manageScheduleAndExpense() {
    int userChoice;

    while (true) {
        cout << "\nSchedule and Expense Tracking Menu:\n";
        cout << "1. Add Schedule\n2. Update Schedule\n3. Delete Schedule\n";
        cout << "4. View Schedules\n5. Search Schedule by Date\n";
        cout << "6. Add Expense\n7. Update Expense\n8. Delete Expense\n";
        cout << "9. View Expenses\n10. Generate Expense Report\n";
       // cout << "11. Update Profile\n12. Display Profile\n";
        cout << "11. Logout\n";
        cout << "Enter Your Choice : ";
        cin >> userChoice;

        if (userChoice == 11) break;

        switch (userChoice) {
            case 1: addSchedule(); break;
            case 2: updateSchedule(); break;
            case 3: deleteSchedule(); break;
            case 4: viewSchedules(); break;
            case 5: searchScheduleByDate(); break;
            case 6: addExpense(); break;
            case 7: updateExpense(); break;
            case 8: deleteExpense(); break;
            case 9: viewExpenses(); break;
            case 10: generateExpenseReport(); break;
            default: cout << "Invalid Choice!" << endl;
        }
    }
}

void adminMenu() {
    int adminChoice;
    while (true) {
        cout << "\nAdmin Menu:\n";
        cout << "1. View All Users\n2. Delete Inactive Users\n3. Backup Data\n";
        cout << "4. Restore Data\n5. Sort Schedules by Date\n";
        cout << "6. Sort Expenses by Category\n7. Search Expense by Keyword\n";
        cout << "8. Find Maximum Expense\n9. Find Minimum Expense\n";
        cout << "10. Generate Budget Summary\n11. Show Monthly Expense Graph\n";
        cout << "12. Compare Expenses by Category\n13. Generate Travel Statistics\n";
        cout << "14. Notify Over Budget\n15. Logout\n";

        cout << "\nEnter your Choice : ";
        cin >> adminChoice;

        if (adminChoice == 15) break;

        switch (adminChoice) {
            
            case 1: 
            
            viewAllUsers(); break;
            
            case 2: 
            
            deleteInactiveUsers(); break;
            
            case 3: 
            
            backupData(); break;
            
            case 4: 
            
            restoreData(); break;
            
            case 5: 
            
            sortSchedulesByDate(); break;
            
            case 6: 
            
            sortExpensesByCategory(); break;
            
            case 7: 
            
            searchExpenseByKeyword(); break;
            
            case 8: 
            
            findMaxExpense(); break;
            
            case 9: 
            
            findMinExpense(); break;
            
            case 10: 
            
            generateBudgetSummary(); break;
            
            case 11: 
            
            showMonthlyExpenseGraph(); break;
            
            case 12: 
            
            compareExpensesByCategory(); break;
            
            case 13: 
            
            generateTravelStatistics(); break;
            
            case 14: 
            
            notifyOverBudget(); break;
            default: cout << "Invalid Choice!" << endl;
        }
    }
}






int main() {
      int choice;

    displayWelcomeMessage();

    while (true) {
        cout << "\nMain Menu:\n";
        cout << "1. Register User\n2. Login User\n3. Admin Login\n";
        cout << "4. Update Profile\n5. Display Profile\n6. Delete Account\n7. Exit\n";
        cout << "\nEnter Your Choice : ";
        cin >> choice;

        if (choice == 7) {
            cout << "Exiting... Thank you!" << endl;
            break;
        }

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser()) {
                    int moduleChoice;

                    while (true) {
                        cout << "\nUser Modules:\n";
                        cout << "1. Accommodation Management\n2. Food and Dining Management\n";
                        cout << "3. Transportation Management\n4. Schedule and Expense Tracking\n5. Logout\n";
                        cout << "\nEnter Your Choice : ";
                        cin >> moduleChoice;

                        if (moduleChoice == 5) break;

                        switch (moduleChoice) {
                            case 1: manageAccommodation(); break;
                            case 2: manageFoodAndDining(); break;
                            case 3: manageTransportation(); break;
                            case 4: manageScheduleAndExpense(); break;
                            default: cout << "Invalid Choice!" << endl;
                        }
                    }
                }
                break;
            case 3:
                if (adminLogin()) {
                    adminMenu();
                }
                break;

            case 4: updateUserDetails(); break;
            case 5: displayUserDetails(); break;
            case 6: deleteUserAccount(); break;
            default:
                cout << "Invalid Choice!" << endl;
        }
    }


    return 0;
}
