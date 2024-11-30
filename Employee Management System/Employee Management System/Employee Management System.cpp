#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

// Define Employee structure
struct Employee {
    int id;
    string name;
    string department;
    string position;
    string contact;
    string gender;
    string email;
    string joinDate;
    string attendance; // Represent attendance as "P" for Present, "A" for Absent
};

// Function to generate random 5-digit ID
int generateRandomID() {
    return rand() % 90000 + 10000;  // Generate random 5-digit number
}

class EmployeeManagementSystem {
private:
    vector<Employee> employees;
    unordered_map<string, string> adminAccounts;  // For Admin account
    unordered_map<string, string> employeeAccounts;  // For Employee account
    unordered_map<int, string> employeePasswords;  // For Employee passwords
    unordered_map<int, string> employeePhoneNumbers;  // To reset password based on phone number
    unordered_map<int, string> employeeEmails;  // To verify email during reset

public:
    // Helper function to get current date in YYYY-MM-DD format
    string getCurrentDate() {
        time_t t = time(0);  // Get time now
        tm* now = localtime(&t);
        return to_string(now->tm_year + 1900) + "-" +
            to_string(now->tm_mon + 1) + "-" +
            to_string(now->tm_mday);
    }

    // Function to create admin account
    void createAdminAccount() {
        string username, password;
        string name, gender, contact, department, email;
        cout << "Enter Admin Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Admin Gender: ";
        getline(cin, gender);
        cout << "Enter Admin Phone Number: ";
        getline(cin, contact);
        cout << "Enter Admin Department: ";
        getline(cin, department);
        cout << "Enter Admin Email: ";
        getline(cin, email);

        int id = generateRandomID();  // Generate random 5-digit Admin ID
        cout << "Generated Admin ID: " << id << endl;

        cout << "Enter Admin Username: ";
        cin >> username;
        cout << "Enter Admin Password: ";
        cin >> password;

        adminAccounts[username] = password;

        // Save admin data to CSV file
        ofstream outFile("user_data.csv", ios::app);
        outFile << id << "," << name << "," << department << "," << contact << ","
            << gender << "," << email << "," << "Admin" << endl;
        outFile.close();

        cout << "Admin account created successfully!" << endl;
    }

    // Function to create employee account
    void createEmployeeAccount(int id) {
        string username, password, confirmPassword;
        string name, gender, contact, department, email;

        bool passwordsMatch = false;

        while (!passwordsMatch) {
            cout << "Enter Employee Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Employee Gender: ";
            getline(cin, gender);
            cout << "Enter Employee Phone Number: ";
            getline(cin, contact);
            cout << "Enter Employee Department: ";
            getline(cin, department);
            cout << "Enter Employee Email: ";
            getline(cin, email);

            int empID = generateRandomID();  // Generate random 5-digit Employee ID
            cout << "Generated Employee ID: " << empID << endl;

            cout << "Enter Employee Username: ";
            cin >> username;
            cout << "Enter Employee Password: ";
            cin >> password;
            cout << "Confirm Employee Password: ";
            cin >> confirmPassword;

            if (password == confirmPassword) {
                passwordsMatch = true;
                employeeAccounts[username] = password;
                employeePasswords[empID] = password;
                employeePhoneNumbers[empID] = contact;  // Store employee's phone number for password reset
                employeeEmails[empID] = email;  // Store email for password reset

                // Save employee data to CSV file
                ofstream outFile("user_data.csv", ios::app);
                outFile << empID << "," << name << "," << department << "," << contact << ","
                    << gender << "," << email << "," << "Employee" << endl;
                outFile.close();

                cout << "Employee account created successfully!" << endl;
            }
            else {
                cout << "Passwords do not match! Please re-enter your details." << endl;
            }
        }
    }

    // Function to login (admin or employee)
    bool login(string username, string password, bool isAdmin) {
        if (isAdmin) {
            return (adminAccounts.find(username) != adminAccounts.end() && adminAccounts[username] == password);
        }
        else {
            return (employeeAccounts.find(username) != employeeAccounts.end() && employeeAccounts[username] == password);
        }
    }

    // Reset password using phone number
    bool resetPassword(int id, string newPassword) {
        if (employeePhoneNumbers.find(id) != employeePhoneNumbers.end()) {
            employeePasswords[id] = newPassword;
            cout << "Password reset successfully!" << endl;
            return true;
        }
        cout << "Phone number not found!" << endl;
        return false;
    }

    // Reset password using email address
    bool resetPasswordByEmail(int id, string newPassword) {
        if (employeeEmails.find(id) != employeeEmails.end()) {
            employeePasswords[id] = newPassword;
            cout << "Password reset successfully using email!" << endl;
            return true;
        }
        cout << "Email not found!" << endl;
        return false;
    }

    // Other employee management functions
    void addEmployee(int id, const string& name, const string& department, const string& position, const string& contact) {
        Employee newEmployee = { id, name, department, position, contact, getCurrentDate(), "A" };
        employees.push_back(newEmployee);
        cout << "Employee added successfully!" << endl;
    }

    void editEmployee(int id, const string& name, const string& department, const string& position, const string& contact) {
        for (auto& emp : employees) {
            if (emp.id == id) {
                emp.name = name;
                emp.department = department;
                emp.position = position;
                emp.contact = contact;
                cout << "Employee details updated successfully!" << endl;
                return;
            }
        }
        cout << "Employee not found!" << endl;
    }

    void deleteEmployee(int id) {
        for (auto it = employees.begin(); it != employees.end(); ++it) {
            if (it->id == id) {
                employees.erase(it);
                cout << "Employee deleted successfully!" << endl;
                return;
            }
        }
        cout << "Employee not found!" << endl;
    }

    void viewEmployee(int id) {
        for (const auto& emp : employees) {
            if (emp.id == id) {
                cout << "ID: " << emp.id << endl
                    << "Name: " << emp.name << endl
                    << "Department: " << emp.department << endl
                    << "Position: " << emp.position << endl
                    << "Contact: " << emp.contact << endl
                    << "Joining Date: " << emp.joinDate << endl
                    << "Attendance: " << emp.attendance << endl;
                return;
            }
        }
        cout << "Employee not found!" << endl;
    }

    void markAttendance(int id, const string& attendance) {
        for (auto& emp : employees) {
            if (emp.id == id) {
                emp.attendance = attendance;
                cout << "Attendance for employee " << id << " marked as " << attendance << endl;
                return;
            }
        }
        cout << "Employee not found!" << endl;
    }

    void viewAllEmployees() {
        for (const auto& emp : employees) {
            cout << "ID: " << emp.id << " Name: " << emp.name << " Department: " << emp.department << endl;
        }
    }

    void generateAttendanceReport() {
        cout << "Employee Attendance Report:" << endl;
        for (const auto& emp : employees) {
            cout << "ID: " << emp.id << " Name: " << emp.name << " Attendance: " << emp.attendance << endl;
        }
    }
};

int main() {
    EmployeeManagementSystem ems;
    string username, password;
    int choice;
    bool isAdminLoggedIn = false;

    srand(time(0)); // Initialize random seed

    // Allow admin account creation if not existing
    cout << "Would you like to create an Admin account? (y/n): ";
    char createAdmin;
    cin >> createAdmin;
    if (createAdmin == 'y' || createAdmin == 'Y') {
        ems.createAdminAccount();
    }

    do {
        cout << "\n*** Employee Management System ***\n";
        cout << "1. Login as Admin\n";
        cout << "2. Login as Employee\n";
        cout << "3. Create Admin Account\n";
        cout << "4. Create Employee Account\n";
        cout << "5. Reset Password\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter Admin Username: ";
            cin >> username;
            cout << "Enter Admin Password: ";
            cin >> password;

            if (ems.login(username, password, true)) {
                isAdminLoggedIn = true;
                cout << "Admin logged in successfully!" << endl;
            }
            else {
                cout << "Invalid Admin credentials!" << endl;
            }
        }
        else if (choice == 2) {
            cout << "Enter Employee Username: ";
            cin >> username;
            cout << "Enter Employee Password: ";
            cin >> password;

            if (ems.login(username, password, false)) {
                isAdminLoggedIn = false;
                cout << "Employee logged in successfully!" << endl;
            }
            else {
                cout << "Invalid Employee credentials!" << endl;
            }
        }
        else if (choice == 3) {
            ems.createAdminAccount();
        }
        else if (choice == 4) {
            int id;
            cout << "Enter Employee ID to create account: ";
            cin >> id;
            ems.createEmployeeAccount(id);
        }
        else if (choice == 5) {
            int id;
            string newPassword;
            cout << "Enter Employee ID for password reset: ";
            cin >> id;
            cout << "Enter new password: ";
            cin >> newPassword;
            ems.resetPassword(id, newPassword);
        }
        else if (choice == 0) {
            cout << "Exiting the system..." << endl;
        }
        else {
            cout << "Invalid choice!" << endl;
        }

    } while (choice != 0);

    return 0;
}
