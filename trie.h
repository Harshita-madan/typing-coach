#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <string>
#include <vector>
using namespace std;

struct TrieNode {
    bool isEndOfWord;
    int mistakeCount;
    int totalAttempts;
    unordered_map<char, TrieNode*> children;

    TrieNode() : isEndOfWord(false), mistakeCount(0), totalAttempts(0) {}
};

class Trie {
private:
    TrieNode* root;
    
    // Helper function to recursively delete trie nodes
    void deleteNode(TrieNode* node);

public:
    Trie();
    ~Trie(); // Destructor
    void insert(const string& word);
    bool search(const string& word);
    void incrementMistake(const string& word);
    int getMistakeCount(const string& word);
    void incrementTotalAttempts(const string& word);
    double getAccuracy(const string& word);          
    vector<string> getSuggestionsForWord(const string& word, int maxDistance);

private:
    void getSuggestions(TrieNode* node, string currentWord, const string& targetWord, int maxDistance, vector<string>& suggestions);
    int calculateLevenshteinDistance(const string& a, const string& b);
};

#endif
