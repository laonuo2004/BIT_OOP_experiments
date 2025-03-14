/*
Create a program that opens a text file, Chapter5InJourneyToWest.txt .
    1.1 Define a function to count the words in the file;
    1.2 Question: Who are the very important person in {大王,玉帝,七仙女,大圣}?
Please give two names and tell WHY.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

class TextAnalyzer 
{
private:
    string fileName;
    string content;
    map<string, int> characterCount; // Counts for each character
    // Aliases for characters (different names for the same character)
    map<string, set<string>> characterAliases;
    vector<string> characters = {"大王", "玉帝", "七仙女", "大圣"};

public:
    TextAnalyzer(const string& fileName); // Constructor
    bool readFile(); // Read file and store content
    int countWords();
    void countCharacterFrequency();
    void analyzeImportantCharacters();
    void initializeAliases(); // Initialize character aliases
    void displayResults();
};

TextAnalyzer::TextAnalyzer(const string& fileName) 
{
    this->fileName = fileName;
    initializeAliases();
}

void TextAnalyzer::initializeAliases() 
{
    // Initialize character aliases (different names for the same character)
    // We omitted some aliases because they can be matched by their substrings, e.g. "大圣" vs "齐天大圣"
    characterAliases["大圣"].insert({"大圣", "猴王", "老孙", "弼马温",
                                    "妖猴", "猴子", "猴精", "爷爷"});
    characterAliases["玉帝"].insert({"玉帝", "万岁", "陛下", "上帝"});
    characterAliases["大王"].insert({"大王", "独角鬼王", "妖王"});
    characterAliases["七仙女"].insert({"仙娥", "仙女"});
}

bool TextAnalyzer::readFile() 
{
    ifstream file(fileName);
    if (!file.is_open()) 
    {
        cout << "Failed to open file: " << fileName << endl;
        return false;
    }

    string line;
    while (getline(file, line)) 
    {
        // Skip lines that are file path comments or empty
        if (line.find("// filepath:") != string::npos || line.empty()) 
        {
            continue;
        }
        content += line + "\n";
    }
    file.close();
    return true;
}

int TextAnalyzer::countWords() 
{
    // For Chinese text, we consider each character as a word
    // This is a simplified approach - more sophisticated NLP would be better
    int charCount = 0;
    
    for (size_t i = 0; i < content.length(); i++) 
    {
        // Check if this byte is the start of a UTF-8 character
        // Chinese characters in UTF-8 typically start with bytes matching 11100000 or higher
        if ((content[i] & 0xC0) != 0x80) 
        {
            charCount++;
        }
    }
    return charCount;
}

void TextAnalyzer::countCharacterFrequency() 
{
    // Initialize all character counts to zero
    for (const auto& character : characters) 
    {
        characterCount[character] = 0;
    }
    
    // Count characters and their aliases
    for (const auto& character : characters) 
    {
        for (const auto& alias : characterAliases[character]) 
        {
            size_t pos = 0;
            while ((pos = content.find(alias, pos)) != string::npos) 
            {
                characterCount[character]++;
                pos += alias.length();
            }
        }
    }
}

void TextAnalyzer::analyzeImportantCharacters() 
{
    // Sort characters by frequency
    vector<pair<string, int>> sortedCharacters;
    for (const auto& pair : characterCount) 
    {
        sortedCharacters.push_back(pair);
    }
    
    // Sort in descending order based on frequency
    sort(sortedCharacters.begin(), sortedCharacters.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    cout << "Character importance analysis:" << endl;
    for (const auto& pair : sortedCharacters) 
    {
        cout << pair.first << ": appeared " << pair.second << " times" << endl;
    }
    
    // Analyze the two most important characters
    if (sortedCharacters.size() >= 2) 
    {
        cout << "\nThe two most important characters are:" << endl;
        
        string char1 = sortedCharacters[0].first;
        string char2 = sortedCharacters[1].first;
        
        cout << "1. " << char1 << " (appeared " << sortedCharacters[0].second << " times)" << endl;
        cout << "2. " << char2 << " (appeared " << sortedCharacters[1].second << " times)" << endl;

        cout << "as they appeared the most times." << endl;
    }
}

void TextAnalyzer::displayResults() 
{
    cout << "File analysis for: " << fileName << endl;
    cout << "Total characters in the text: " << countWords() << endl << endl;
    countCharacterFrequency();
    analyzeImportantCharacters();
}

int main() 
{
    TextAnalyzer analyzer("Chapter5InJourneyToWest.txt");
    
    if (!analyzer.readFile()) 
    {
        cerr << "Error: Failed to read the file." << endl;
        return 1;
    }
    
    analyzer.displayResults();
    
    return 0;
}