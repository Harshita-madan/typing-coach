#include "trie.h"
#include<vector>
#include<algorithm>
Trie::Trie(){
    root = new TrieNode();
}

void Trie::deleteNode(TrieNode* node) {
    if (node == nullptr) return;
    
    // Recursively delete all children
    for (auto& child : node->children) {
        deleteNode(child.second);
    }
    
    // Delete current node
    delete node;
}

Trie::~Trie() {
    // Clean up all allocated memory
    deleteNode(root);
}
void Trie:: insert(const string& word){
    TrieNode* node = root;
    for(char ch: word){
        if(!node->children[ch]){
        node->children[ch] = new TrieNode;
    }
    node= node->children[ch];
    }
    node->isEndOfWord = true;
}
bool Trie:: search(const string& word){
    TrieNode* node = root;
    for(char ch:word){
        if(node->children.find(ch) == node->children.end()) {
            return false; // Character not found in trie
        }
        node = node->children[ch];
    }
    return node->isEndOfWord; // Must be end of word marker
}
void Trie:: incrementMistake(const string& word){
    TrieNode* node = root;
    for(char ch:word){
        if(node->children.find(ch) == node->children.end()) {
            // Word doesn't exist in dictionary
            return;
        }
        node = node->children[ch];
    }
    
    // Only increment for valid dictionary words
    if(node->isEndOfWord) {
        node->mistakeCount++;
    }
}
int Trie :: getMistakeCount(const string& word){
    TrieNode* node = root;
    for(char ch:word){
        if(!node->children[ch]) return 0;
        node= node->children[ch];
    }
    return node->mistakeCount;
    
}
void Trie:: getSuggestions(TrieNode* node, string currentWord,const string& targetWord,int maxDistance,vector<string>& suggestions){
    if(node ==nullptr) return;
int distance = calculateLevenshteinDistance(currentWord,targetWord);
if(distance<=maxDistance && node->isEndOfWord) 
{
    suggestions.push_back(currentWord);
}
for(auto & child: node->children){
    getSuggestions(child.second,currentWord+ child.first,targetWord,maxDistance,suggestions);
}
}
int Trie:: calculateLevenshteinDistance(const string& a, const string& b){
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n+1,vector<int>(m+1));
    for(int i = 0; i<=n;i++) dp[i][0]=i;
    for(int j = 0; j<=m;++j) dp[0][j]=j;
    for(int i = 1; i<=n;i++){
        for(int j = 1; j<=m; j++){
            if(a[i-1]==b[j-1])
            dp[i][j]= dp[i-1][j-1];
            else{
                dp[i][j] = 1+min({dp[i-1][j],dp[i][j-1],dp[i-1][j-1]});
            }
        }
    }
return dp[n][m];
}
vector<string> Trie::getSuggestionsForWord(const string& word, int maxDistance) {
    vector<string> suggestions;
    getSuggestions(root, "", word, maxDistance, suggestions);

    vector<pair<string, pair<int, int>>> rankedSuggestions;

    for (const string& suggestion : suggestions) {
        int distance = calculateLevenshteinDistance(suggestion, word);
        int mistake = getMistakeCount(suggestion);
        rankedSuggestions.push_back({suggestion, {distance, -mistake}});
    }

    // Sort by lowest distance first, then by highest mistake count
    sort(rankedSuggestions.begin(), rankedSuggestions.end(), [](const auto& a, const auto& b) {
        if (a.second.first != b.second.first)
            return a.second.first < b.second.first;  // smaller distance first
        return a.second.second < b.second.second;    // larger mistake count first (negated)
    });

    // Limit to top 3 suggestions
    vector<string> topSuggestions;
    for (int i = 0; i < min(3, (int)rankedSuggestions.size()); ++i) {
        topSuggestions.push_back(rankedSuggestions[i].first);
    }

    return topSuggestions;
}
void Trie::incrementTotalAttempts(const string& word) {
    TrieNode* node = root;
    for (char ch : word) {
        if (node->children.find(ch) == node->children.end()) {
            // Node doesn't exist in dictionary, so don't track stats for non-dictionary words
            return;
        }
        node = node->children[ch];
    }
    
    // Only increment for valid dictionary words
    if (node->isEndOfWord) {
        node->totalAttempts++;
    }
}
double Trie::getAccuracy(const string& word) {
    TrieNode* node = root;
    for (char ch : word) {
        if (!node->children[ch]) return 0.0;
        node = node->children[ch];
    }
    if (node->totalAttempts == 0) return 0.0;
    return 100.0 * (1.0 - (double)node->mistakeCount / node->totalAttempts);
}
