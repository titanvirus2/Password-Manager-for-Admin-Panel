#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>
#include <iomanip>
#include <ctime>

using namespace std;

// Simple XOR encryption/decryption function
string encryptDecrypt(const string & data, char key = 'K') {
    string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] ^= key;
    }
    return result;
}

// Save data to a file
void saveToFile(const map < string, string > & data, const string & filename) {
    ofstream file(filename, ios::out);
    if (!file) {
        cerr << "Error: Unable to open file for saving." << endl;
        return;
    }
    for (const auto& entry : data) {
        file << encryptDecrypt(entry.first) << " " << encryptDecrypt(entry.second) << endl;
    }
    file.close();
}

// Load data from a file
void loadFromFile(map < string , string > & data, const string & filename) {
    ifstream file(filename, ios::in);
    if (!file) {
        cerr << "Error: Unable to open file for loading." << endl;
        return;
    }
    data.clear();
    string key, value;
    while (file >> key >> value) {
        data[encryptDecrypt(key)] = encryptDecrypt(value);
    }
    file.close();
}

// Sleep function without threads
void customSleep(int seconds) {
    time_t start = time(0);
    while (time(0) - start < seconds) {
        // Busy waiting
    }
}

// Authenticate user
bool authenticateUser(map < string, string > & users, const string & dbFile) {
    string username, password;
    int attempt = 0;
    int waitTime = 1; // Initial wait time in minutes

    while (true) {
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter password: ";
        /*
        for(int i =0 ; i<7; i++)
        {
        cin>> password
        if ("admin" == passwprd)
        cout << access granted;
        else {customSleep(waitTime * 60);
                attempt = 0;
                waitTime *= 2;}
                }}
                */
        getline(cin, password);

        if (users.find(username) != users.end() && users[username] == password) {
            cout << "Access granted!\n";
            return true;
        } else {
            attempt++;
            if (attempt == 7) {
                cout << "Too many failed attempts. System locked for " << waitTime << " minute(s).\n";
                customSleep(waitTime * 60);
                attempt = 0;
                waitTime *= 2; // Double the wait time
            } else {
                cout << "Access denied. Try again.\n";
            }
        }
    }
}

// Admin login
bool adminLogin() {
    string adminUsername = "titan";
    string adminPassword = "admin";
    string username, password;

    cout << "Admin Login:\n";
    cout << "Enter username: ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);

    if (username == adminUsername && password == adminPassword) {
        cout << "Admin access granted!\n";
        return true;
    } else {
        cout << "Access denied.\n";
        return false;
    }
}

// Add new user
void addNewUser(map < string, string > & users, const string & dbFile) {
    string username, password;
    cout << "Enter a new username: ";
    getline(cin, username);
    if (users.find(username) != users.end()) {
        cout << "Username already exists.\n";
        return;
    }
    cout << "Enter a new password: ";
    getline(cin, password);
    users[username] = password;
    saveToFile(users, dbFile);
    cout << "User added successfully!\n";
}

// Add, retrieve, delete site-specific credentials
void manageSiteCredentials(map < string, string > & siteCredentials, const string & siteFile) {
    int option;
    string site, username, password;
    
    do {
        cout << "\nManage Site Credentials:\n";
        cout << "1. Add credentials\n";
        cout << "2. Retrieve credentials\n";
        cout << "3. Delete credentials\n";
        cout << "4. Show all saved sites\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> option;
        cin.ignore();

        switch (option) {
            case 1:
                cout << "Enter site (e.g., Facebook, WhatsApp or other sites you need): ";
                getline(cin, site);
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                siteCredentials[site] = username + " " + password;
                saveToFile(siteCredentials, siteFile);
                cout << "Credentials added successfully!\n";
                break;

            case 2:
                cout << "Enter site (e.g., Facebook, WhatsApp): ";
                getline(cin, site);
                if (siteCredentials.find(site) != siteCredentials.end()) {
                    cout << "Credentials for " << site << ": " << siteCredentials[site] << endl;
                } else {
                    cout << "Site not found!\n";
                }
                break;

            case 3:
                cout << "Enter site to delete: ";
                getline(cin, site);
                if (siteCredentials.erase(site)) {
                    saveToFile(siteCredentials, siteFile);
                    cout << "Credentials deleted successfully!\n";
                } else {
                    cout << "Site not found!\n";
                }
                break;

            case 4:
                if (siteCredentials.empty()) {
                    cout << "No sites saved.\n";
                } else {
                    cout << "\nSaved Sites and Credentials:\n";
                    for (const auto& entry : siteCredentials) {
                        cout << "- " << entry.first << ": " << entry.second << endl;
                    }
                }
                break;

            case 5:
                cout << "Exiting Site Credentials Management.\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (option != 5);
}

int main() {
    const string dbFile = "users.dat";
    const string passwordFile = "passwords.dat";
    const string siteFile = "site_credentials.dat";
    map<string, string> users;
    map<string, string> passwords;
    map<string, string> siteCredentials;

    // Load user database, passwords, and site credentials
    loadFromFile(users, dbFile);
    loadFromFile(passwords, passwordFile);
    loadFromFile(siteCredentials, siteFile);

    cout << "Welcome to the Secure Password Manager!\n";
    cout << "1. Login\n";
    cout << "2. Register\n";
    cout << "3. Admin Panel\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        if (!authenticateUser(users, dbFile)) {
            return 0;
        }
    } else if (choice == 2) {
        addNewUser(users, dbFile);
        return 0;
    } else if (choice == 3) {
        if (!adminLogin()) {
            return 0;
        }
    } else {
        cout << "Invalid choice. Exiting.\n";
        return 0;
    }

    // Password Manager functionality
    int option;
    string account, password;
    do {
        cout << "\nPassword Manager:\n";
        cout << "1. Add a new password\n";
        cout << "2. Retrieve a password\n";
        cout << "3. Delete a password\n";
        cout << "4. Show all accounts\n";
        cout << "5. Manage Site Credentials\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> option;
        cin.ignore();

        switch (option) {
            case 1:
                cout << "Enter account name: ";
                getline(cin, account);
                cout << "Enter password: ";
                getline(cin, password);
                passwords[account] = password;
                saveToFile(passwords, passwordFile);
                cout << "Password added successfully!\n";
                break;

            case 2:
                cout << "Enter account name: ";
                getline(cin, account);
                if (passwords.find(account) != passwords.end()) {
                    cout << "Password for " << account << ": " << passwords[account] << endl;
                } else {
                    cout << "Account not found!\n";
                }
                break;

            case 3:
                cout << "Enter account name to delete: ";
                getline(cin, account);
                if (passwords.erase(account)) {
                    saveToFile(passwords, passwordFile);
                    cout << "Password deleted successfully!\n";
                } else {
                    cout << "Account not found!\n";
                }
                break;

            case 4:
                if (passwords.empty()) {
                    cout << "No accounts saved.\n";
                } else {
                    cout << "\nSaved Accounts:\n";
                    for (const auto& entry : passwords) {
                        cout << "- " << entry.first << endl;
                    }
                }
                break;

            case 5:
                manageSiteCredentials(siteCredentials, siteFile);
                break;

            case 6:
                cout << "Exiting Password Manager. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (option != 6);

    return 0;
}
