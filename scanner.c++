#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
using namespace std;

struct TokenRule {
    string name;
    regex pattern;
};

vector<TokenRule> loadTokenRules(const string& filename) {
    vector<TokenRule> rules;
    ifstream file(filename);
    string name, pattern;

    while (file >> name) {
        file >> ws;
        getline(file, pattern);
        rules.push_back({name, regex(pattern)});
    }

    return rules;
}

void scanInput(const string& inputFilename, const vector<TokenRule>& rules, const string& outputFilename) {
    ifstream input(inputFilename);
    ofstream output(outputFilename);
    string line;
    int lineNum = 1;
    bool errorFound = false;

    while (getline(input, line)) {
        size_t i = 0;
        while (i < line.size()) {
            bool matched = false;
            for (const auto& rule : rules) {
                smatch match;
                string substr = line.substr(i);
                if (regex_search(substr, match, rule.pattern) && match.position() == 0) {
                    if (rule.name != "WHITESPACE") {
                        output << rule.name << " " << match.str() << endl;
                    }
                    i += match.length();
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                output << "ERROR: Unknown token at line " << lineNum << " near: " << line[i] << endl;
                errorFound = true;
                break;
            }
        }
        if (errorFound) break;
        lineNum++;
    }

    if (!errorFound) {
        output << "ACCEPTED" << endl;
    }
}

int main() {
    vector<TokenRule> rules = loadTokenRules("tokens.txt");
    scanInput("input.json", rules, "scanner_output.txt");
    return 0;
}
