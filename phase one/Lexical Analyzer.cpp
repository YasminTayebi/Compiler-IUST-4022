#include <iostream>
#include <string>
#include <regex>
#include <vector>

using namespace std;

// Token structure
struct Token {
    string type;
    string lexeme;
    int line;
};

vector<Token> tokenize(const string& code) {
    vector<Token> tokens;
    // Define regular expressions for tokens
    string reservedword = "\\b(int|float|void|cin|cout|continue|break|#include|using|namespace|std|main|if|else|while|for|return)\\b";
    string identifier = "\\b[a-zA-Z][a-zA-Z0-9]*\\b";
    string digit = "\\b[0-9]\\b";
    string number = "\\b[0-9]+(?:\\.[0-9]+)?\\b";
    string stringsPattern = R"(".*?")";
    string symbol = "\\b[(|)|{|}|\\[|\\]|,|;|+|-|*|/|==|!=|>|>=|<|<=|=|&&|\\|\\||<<|>>]\\b";
    string whitespacePattern = "\\s+";

    // Combine regular expressions into one
    string tokenRegexStr = "(" + reservedword + "|" + identifier + "|" + digit + "|" + number + "|" + stringsPattern + "|" + symbol + "|" + whitespacePattern + ")";
    regex tokenRegex(tokenRegexStr);

    // Tokenization process
    smatch match;
    int lineNumber = 1;
    string::const_iterator start = code.begin();
    string::const_iterator end = code.end();
    while (regex_search(start, end, match, tokenRegex)) {
        if (!regex_match(match[0].str(), regex(whitespacePattern))) {
            Token token;
            token.lexeme = match[0];
            token.line = lineNumber;
            // Determine token type
            if (regex_match(token.lexeme, regex(reservedword)))
                token.type = "Reservedword";
            else if (regex_match(token.lexeme, regex(identifier)))
                token.type = "Identifier";
            else if (regex_match(token.lexeme, regex(digit)))
                token.type = "Digit";
            else if (regex_match(token.lexeme, regex(number)))
                token.type = "Number";
            else if (regex_match(token.lexeme, regex(stringsPattern)))
                token.type = "String";
            else if (regex_match(token.lexeme, regex(symbol)))
                token.type = "Symbol";
            tokens.push_back(token);
        }

        start = match.suffix().first;
        // Count lines
        while (start != end && (*start == '\n' || *start == '\r')) {
            if (*start == '\n') lineNumber++;
            ++start;
        }
    }
    return tokens;
}

int main() {
    string code = R"(
        #include <iostream>
        using namespace std;
        int main(){
          int x;
          int s=0, t=10;
          while (t >= 0){
           cin>>x;
           t = t - 1;
           s = s + x;
        }
        cout<<"sum="<<s;
        return 0;
    }
    )";
    vector<Token> tokens = tokenize(code);
    for (const auto& token : tokens) {
        cout << "Type: " << token.type << ", Lexeme: " << token.lexeme << ", Line: " << token.line << endl;
    }
    return 0;
}