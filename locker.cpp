#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <algorithm>
#include <json.hpp>
#include "locker.h"
using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;
bool is_prime(int num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}
bool check_password(const string &password, const vector<vector<int>> &clues) {
    for (const auto &clue : clues) {
        if (clue.empty()) continue;
        if (clue.size() == 2 && clue[0] == -1 && clue[1] == -1) {
            set<int> prime_digits;
            for (char c : password) {
                int digit = c - '0';
                if (!is_prime(digit) || prime_digits.count(digit)) {
                    return false;
                }
                prime_digits.insert(digit);
            }
            continue; // This clue checked, move to next
        }
        // Handle position+condition type clues [position, condition]
        if (clue.size() == 2) {
            int pos = clue[0] - 1;  // Convert to 0-based index
            if (pos < 0 || pos >= password.length()) return false;

            int digit = password[pos] - '0';
            int condition = clue[1];

            switch (condition) {
                case -1: { // All digits are unique primes
                    set<int> prime_digits;
                    for (char c : password) {
                        int d = c - '0';
                        if (!is_prime(d) || prime_digits.count(d)) {
                            return false;
                        }
                        prime_digits.insert(d);
                    }
                    break;
                }
                case 0:  // Even
                    if (digit % 2 != 0) return false;
                    break;
                case 1:  // Odd
                    if (digit % 2 != 1) return false;
                    break;
                default:
                    return false;
            }
        }
        else if (clue.size() == 3) {
            for (int i = 0; i < 3; ++i) {
                if (clue[i] != -1 && (password[i] - '0') != clue[i]) {
                    return false;
                }
            }
        }
    }
    return true;
}
vector<string> generate_all_passwords() {
    vector<string> passwords;
    for (int i = 0; i <= 999; ++i) {
        string password = to_string(i);
        while (password.length() < 3) {
            password = "0" + password;
        }
        passwords.push_back(password);
    }
    return passwords;
}

vector<string> find_valid_passwords(const vector<vector<int>>& clues) {
    vector<string> valid_passwords;
    auto all_passwords = generate_all_passwords();

    copy_if(all_passwords.begin(), all_passwords.end(), back_inserter(valid_passwords),
            [&clues](const string& pwd) { return check_password(pwd, clues); });

    return valid_passwords;
}

string find_password(const vector<vector<int>>& clues, const string& target_hash, int& total_attempts) {
    PasswordLock lock;
    auto valid_passwords = find_valid_passwords(clues);
    total_attempts = 0; // Reset attempt counter
    for (const auto& password : valid_passwords) {
        total_attempts++; // Count before each attempt
        string hash = lock.hashPassword(password);
        if (hash == target_hash) {
            return password;
        }
    }

    cout << "\nWarning: No password matched target hash" << endl;
    cout << "Total attempts: " << total_attempts << endl;
    return "";
}
void password_lock(int &value) {
    std::cout<<std::endl;
    // Test file path (adjust according to project structure)
    std::string test_file = "D:\\code\\GAME\\maze.json";
    // Statistics variables initialization
    int passed = 0;         // Success count
    int failed = 0;         // Failure count
    // Check if file exists
    if (!fs::exists(test_file)) {
        cerr << "Error: Test file does not exist - " << test_file << endl;
        return ;
    }
    // Open file stream
    ifstream input(test_file);
    if (!input.is_open()) {
        cerr << "Error: Unable to open file " << test_file << endl;
        return ;
    }
    try {
        // Parse JSON data
        json data;
        input >> data;
        input.close();  // Close file promptly
        // Extract test data
        vector<vector<int>> clues;  // Password clues
        string target_hash;         // Target hash value
        try {
            // Get clue array and target hash from JSON
            clues = data["C"].get<vector<vector<int>>>();
            target_hash = data["L"].get<string>();
            // Validate required fields
            if (clues.empty() || target_hash.empty()) {
                throw runtime_error("Required fields C(clues) or L(target hash) missing/empty");
            }
            int current_attempts = 0;
            string found_password = find_password(clues, target_hash, current_attempts);
            // Output results
            if (!found_password.empty()) {
                cout<<"Successfully found password!: "<<found_password<<endl;
                cout<<"Attempt count:"<<current_attempts<<" Wrong attempts: " << current_attempts-1<< endl;
                value-=current_attempts-1;
                passed++;
                return;
            } else {
                failed++;
                return ;
            }
        } catch (const json::exception &e) {
            cerr << "JSON format error: " << e.what() << endl;
            failed++;
        }

    } catch (const exception &e) {
        cerr << "Processing error: " << e.what() << endl;
        failed++;
        if (input.is_open()) input.close();  // Ensure file is closed
    }
}