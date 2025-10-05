#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<unordered_map>
#include<chrono>
#include<iomanip>
#include<algorithm>
#include"trie.h"


// ANSI color codes
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

using namespace std;

int main()
{
    Trie dictionary;
    int totalWords = 0;
    int correctWords = 0;
    unordered_map<string, int> mistakeMap;

    // Start timing
    auto sessionStart = std::chrono::steady_clock::now();

    // Load dictionary words
    ifstream file("words.txt");
    string word;
    int wordCount = 0;
    while (file >> word) {
        // Convert dictionary word to lowercase
        for (auto& c : word) c = tolower(c);
        dictionary.insert(word);
        wordCount++;
    }
    file.close();
    
    cout << "TypingCoach - Loaded " << wordCount << " words\n";
    if (wordCount == 0) {
        cerr << "Warning: No words loaded from words.txt!\n";
    }

    cout << "TypingCoach\nType a word (type 'exit' to quit):\n";

    while (true) {
        cout << ">";
        string input;
        cin >> input;
        // Convert input to lowercase
        for (auto& c : input) c = tolower(c);

        // Handle commands
        if (input == "exit") break;
        
        // Show stats during session
        if (input == "/stats") {
            auto currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = currentTime - sessionStart;
            double seconds = elapsed.count();
            double minutes = seconds / 60.0;
            
            int mistakes = totalWords - correctWords;
            float accuracy = (totalWords == 0) ? 0.0f : (100.0f * correctWords) / totalWords;
            double wpm = (minutes > 0.0) ? (totalWords / minutes) : 0.0;
            
            cout << "\n--- Current Stats ---\n";
            cout << "Words typed:     " << totalWords << "\n";
            cout << "Correct words:   " << correctWords << "\n";
            cout << "Mistakes:        " << mistakes << "\n";
            cout << "Accuracy:        " << accuracy << "%\n";
            cout << "Time elapsed:    " << seconds << " seconds\n";
            cout << "Words/minute:    " << wpm << "\n";
            cout << "-------------------\n\n";
            continue;
        }
        
        // Special command to debug dictionary
        if (input == "/debug") {
            cout << "Dictionary debug - enter word to check: ";
            string checkWord;
            cin >> checkWord;
            for (auto& c : checkWord) c = tolower(c);
            cout << "Check '" << checkWord << "': " << (dictionary.search(checkWord) ? "Found" : "Not found") << "\n";
            continue;
        }
        
        // Show help
        if (input == "/help") {
            cout << "\nCommands:\n";
            cout << "  exit    - Exit the program\n";
            cout << "  /stats  - Show current typing statistics\n";
            cout << "  /help   - Show this help message\n";
            cout << "  /debug  - Debug dictionary word lookup\n\n";
            continue;
        }
        
        totalWords++;
        dictionary.incrementTotalAttempts(input);

        bool wordExists = dictionary.search(input);
        
        if (wordExists) {
            cout << GREEN << "[+] Correct!" << RESET << "\n";
            correctWords++;
            if (mistakeMap[input] >= 3) {
                cout << YELLOW << "[!] Warning: you have mistyped '" << input << "' "
                     << mistakeMap[input] << " times." << RESET << endl;
            }
        } else {
            cout << RED << "[-] Mistake detected." << RESET << "\n";
            mistakeMap[input]++;
            if (mistakeMap[input] >= 3) {
                cout << YELLOW << "[!] Warning: you have mistyped '" << input << "' " << mistakeMap[input] << " times." << RESET << "\n";
            }

            // Suggest similar words
            vector<string> suggestions = dictionary.getSuggestionsForWord(input, 3);
            if (!suggestions.empty()) {
                cout << "Did you mean:\n";
                for (const string& suggestion : suggestions) {
                    cout << "  - " << suggestion << "\n";
                }
            } else {
                cout << "No suggestions found.\n";
            }

            cout << "\n";
        }
    }

    // End timing
    auto sessionEnd = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = sessionEnd - sessionStart;
    double seconds = elapsed.count();
    double minutes = seconds / 60.0;

    int mistakes = totalWords - correctWords;
    float accuracy = (totalWords == 0) ? 0.0f : (100.0f * correctWords) / totalWords;
    double wpm = (minutes > 0.0) ? (totalWords / minutes) : 0.0;

    cout << "\n+------------------------------+\n";
    cout << "|     TYPING COACH SUMMARY     |\n";
    cout << "+------------------------------+\n";
    cout << "\nSession Stats:\n";
    cout << "------------------------------\n";
    cout << "Total words typed: " << totalWords << "\n";
    cout << "Correct words:     " << correctWords << "\n";
    cout << "Mistakes:          " << mistakes << "\n";
    cout << "Accuracy:          " << fixed << setprecision(1) << accuracy << "%\n";
    cout << "Session time:      " << fixed << setprecision(1) << seconds << " seconds\n";
    cout << "Words per minute:  " << fixed << setprecision(1) << wpm << "\n";

    if (!mistakeMap.empty()) {
        // Sort words by mistake count
        vector<pair<string, int>> sortedMistakes;
        for (const auto& entry : mistakeMap) {
            sortedMistakes.push_back({entry.first, entry.second});
        }
        
        sort(sortedMistakes.begin(), sortedMistakes.end(), 
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second; // Sort by count (descending)
             });
        
        cout << "\nTop Mistyped Words:\n";
        cout << "------------------------------\n";
        
        // Display top 5 or all if less than 5
        int displayCount = min(5, static_cast<int>(sortedMistakes.size()));
        for (int i = 0; i < displayCount; i++) {
            cout << i+1 << ". " << RED << sortedMistakes[i].first << RESET 
                 << ": mistyped " << sortedMistakes[i].second << " times\n";
        }

        // Write to report.txt
        ofstream report("report.txt");
        if (report.is_open()) {
            report << "=================================\n";
            report << "      TYPING COACH REPORT       \n";
            report << "=================================\n\n";
            report << "SESSION SUMMARY\n";
            report << "---------------------------------\n";
            report << "Date:             " << __DATE__ << " " << __TIME__ << "\n";
            report << "Total words:      " << totalWords << "\n";
            report << "Correct words:    " << correctWords << "\n";
            report << "Mistakes:         " << mistakes << "\n";
            report << "Accuracy:         " << fixed << setprecision(1) << accuracy << "%\n";
            report << "Session duration: " << fixed << setprecision(1) << seconds << " seconds\n";
            report << "Words per minute: " << fixed << setprecision(1) << wpm << "\n\n";
            
            report << "ALL MISTYPED WORDS\n";
            report << "---------------------------------\n";
            for (const auto& mistake : sortedMistakes) {
                report << "- " << mistake.first << ": " << mistake.second << " times\n";
            }
            
            report.close();
            cout << "\n" << GREEN << "Report saved to 'report.txt'" << RESET << "\n";
        } else {
            cerr << RED << "Failed to write report to file." << RESET << "\n";
        }
    } else {
        cout << "\n" << GREEN << "No mistakes recorded. Great job!" << RESET << "\n";
    }

    return 0;
}
