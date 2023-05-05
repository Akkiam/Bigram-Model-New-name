#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

// Read names from file
std::vector<std::string> readNamesFromFile(const std::string& file_path) {
    std::vector<std::string> names;
    std::ifstream file(file_path);
    std::string line;
  
    while (std::getline(file, line)) {
        names.push_back(line);
    }
    return names;
}

// Count bigram in the line

std::unordered_map<std::string, int> countBigrams(const std::string& str)
{
    std::unordered_map<std::string, int> bigram_counts;
    std::string strr = "^" + str + "$";
    for (int i = 0; i < strr.size() - 1; i++){
        std::string bigram = strr.substr(i, 2);
        bigram_counts[bigram]++;
    }
    return bigram_counts;
}



// generate a new name
string generateName(const unordered_map<string, double>& bigramProb) {
    default_random_engine generator(time(0));
    uniform_real_distribution<double> distribution(0.0, 1.0);

    string name = "^";
    char prevChar = '^';
    while (prevChar != '$') {
        vector<char> nextChars;
        for (auto& [bigram, prob] : bigramProb) {
            if (bigram[0] == prevChar) {
                nextChars.push_back(bigram[1]);
            }
        }
        if (nextChars.empty()) {
            break;
        }
        char nextChar = nextChars[0];
        double probSum = 0.0;
        for (char c : nextChars) {
            string bigram = string(1, prevChar) + string(1, c);
            probSum += bigramProb.at(bigram);
        }
        double r = distribution(generator);
        double probAccum = 0.0;
        for (char c : nextChars) {
            string bigram = string(1, prevChar) + string(1, c);
            double prob = bigramProb.at(bigram) / probSum;
            probAccum += prob;
            if (r <= probAccum) {
                nextChar = c;
                break;
            }
        }
        name += nextChar;
        prevChar = nextChar;
    }
    return name.substr(1, name.length() - 2);  // Remove start and end tokens
}

int main()
{
    // Read names 
    std::vector<std::string> names = readNamesFromFile("names.txt");
    //Check if the file is open 
    ifstream file("names.txt");
  
    if (!file.is_open()) {
      cerr << "Cannot open file!" << endl;
      return 1;
    }

    // Count bigrams and freq
    std::unordered_map<std::string, int> bigram_counts;
  
    for (const auto& name : names){
        std::unordered_map<std::string, int> name_bigram_counts = countBigrams(name);
        for (const auto& [bigram, count] : name_bigram_counts){
            bigram_counts[bigram] += count;
        }
    }

    // Count bigram's probabilities
    std::unordered_map<std::string, double> bigram_probabilities;
    int totalBigrams = 0;
  
    for (const auto& [bigram, count] : bigram_counts){
        totalBigrams += count;
    }
    for (const auto& [bigram, count] : bigram_counts){
        bigram_probabilities[bigram] = static_cast<double>(count) / totalBigrams;
    }
  
    //count total bigrams
    //std::cout << totalBigrams << "\t" << "\n";
    // Bigram probability table 
  
    for (const auto& [bigram, probability] : bigram_probabilities){
        std::cout << bigram << "\t" << probability << "\n";
    }

    string new_name = generateName(bigram_probabilities);
    cout << "Generated name: " << new_name << endl;
    return 0;
}


