#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <utility>
#include <stack>

using namespace std;

enum TokenType {
    RESERVEDWORD, IDENTIFIER, NUMBER, SYMBOL, STRING, INCLUDE
};

const vector<pair<TokenType, regex>> tokenDefinitions = {
    {INCLUDE, regex("^(#include\\s*<\\s*iostream\\s*>)")},
    {RESERVEDWORD, regex("^(int|float|void|return|if|while|cin|cout|continue|break|using|namespace|std|main)\\b")},
    {IDENTIFIER, regex("^[a-zA-Z_][a-zA-Z0-9_]*")},
    {NUMBER, regex("^\\d+")},
    {SYMBOL, regex("^(\\(|\\)|\\{|\\}|\\[|\\]|,|;|\\+|\\-|\\*|/|==|!=|>|>=|<|<=|=|&&|\\|\\||<<|>>)")},
    {STRING, regex("^\".*?\"")}
};

vector<pair<string, string>> tokenize(const string& input) {
    vector<pair<string, string>> tokens;
    string::const_iterator start = input.begin();
    string::const_iterator end = input.end();

    while (start != end) {
        bool matched = false;
        for (const auto& def : tokenDefinitions) {
            smatch match;
            if (regex_search(start, end, match, def.second, regex_constants::match_continuous)) {
                string token(match[0]);
                string type;
                switch (def.first) {
                    case INCLUDE: type = "reservedword"; break;
                    case RESERVEDWORD: type = "reservedword"; break;
                    case IDENTIFIER: type = "identifier"; break;
                    case NUMBER: type = "number"; break;
                    case SYMBOL: type = "symbol"; break;
                    case STRING: type = "string"; break;
                }
                tokens.push_back({type, token});
                start += token.length();
                matched = true;
                break;
            }
        }
        if (!matched) {
            ++start;
        }
    }
    return tokens;
}

// Intermediate Code Generation
class ThreeAddressCodeGenerator {
public:
    void emit(const string& op, const string& arg1, const string& arg2, const string& result) {
        code.push_back(op + " " + arg1 + " " + arg2 + " " + result);
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& line : code) {
                file << line << endl;
            }
            file.close();
        }
    }

private:
    vector<string> code;
};

// Parser
class Parser {
public:
    Parser(const vector<pair<string, string>>& tokens) : tokens(tokens), index(0), tempVarCounter(0) {}

    void parse() {
        Program();
    }

    void saveCode(const string& filename) {
        tac.saveToFile(filename);
    }

private:
    vector<pair<string, string>> tokens;
    size_t index;
    int tempVarCounter;
    ThreeAddressCodeGenerator tac;

    string newTemp() {
        return "t" + to_string(tempVarCounter++);
    }

    void debugCurrentToken() {
        if (index < tokens.size()) {
            cout << "Current token: <" << tokens[index].first << ", " << tokens[index].second << ">" << endl;
        } else {
            cout << "No more tokens" << endl;
        }
    }

    void Program() {
        cout << "Entering Program" << endl;
        Declarations();
        Statements();
        cout << "Exiting Program" << endl;
    }

    void Declarations() {
        cout << "Entering Declarations" << endl;
        while (index < tokens.size() && match("reservedword", "int")) {
            Declaration();
        }
        cout << "Exiting Declarations" << endl;
    }

    void Declaration() {
        cout << "Entering Declaration" << endl;
        match("reservedword", "int");
        string id = tokens[index++].second;
        match("symbol", ";");
        tac.emit("DECL", id, "", "");
        cout << "Exiting Declaration" << endl;
    }

    void Statements() {
        cout << "Entering Statements" << endl;
        while (index < tokens.size() && tokens[index].second != "}") {
            Statement();
        }
        cout << "Exiting Statements" << endl;
    }

    void Statement() {
        cout << "Entering Statement" << endl;
        debugCurrentToken();
        if (tokens[index].second == "while") {
            While(); // Corrected from While() to Parser::While()
        } else if (tokens[index].second == "return") {
            Return();
        } else if (tokens[index].second == "cout") {
            Print();
        } else if (tokens[index].first == "identifier") {
            Assignment();
        } else {
            ++index;  // Skip unknown tokens
        }
        cout << "Exiting Statement" << endl;
    }

    void Assignment() {
        cout << "Entering Assignment" << endl;
        string id = tokens[index++].second;
        match("symbol", "=");
        string expr = Expression();
        match("symbol", ";");
        tac.emit("ASSIGN", expr, "", id);
        cout << "Exiting Assignment" << endl;
    }

    void While() {
        cout << "Entering While" << endl;
        match("reservedword", "while");
        match("symbol", "(");
        string cond = Condition();
        match("symbol", ")");
        match("symbol", "{");
        Statements();
        match("symbol", "}");
        tac.emit("WHILE", cond, "", "");
        cout << "Exiting While" << endl;
    }

    void Return() {
        cout << "Entering Return" << endl;
        match("reservedword", "return");
        string expr = Expression();
        match("symbol", ";");
        tac.emit("RETURN", expr, "", "");
        cout << "Exiting Return" << endl;
    }

    void Print() {
        cout << "Entering Print" << endl;
        match("reservedword", "cout");
        match("symbol", "<<");
        string str = tokens[index++].second;
        match("symbol", ";");
        tac.emit("PRINT", str, "", "");
        cout << "Exiting Print" << endl;
    }

    string Expression() {
        cout << "Entering Expression" << endl;
        string term = Term();
        while (index < tokens.size() && (tokens[index].second == "+" || tokens[index].second == "-")) {
            string op = tokens[index++].second;
            string nextTerm = Term();
            string temp = newTemp();
            tac.emit(op, term, nextTerm, temp);
            term = temp;
        }
        cout << "Exiting Expression" << endl;
        return term;
    }

    string Term() {
        cout << "Entering Term" << endl;
        string factor = Factor();
        while (index < tokens.size() && (tokens[index].second == "*" || tokens[index].second == "/")) {
            string op = tokens[index++].second;
            string nextFactor = Factor();
            string temp = newTemp();
            tac.emit(op, factor, nextFactor, temp);
            factor = temp;
        }
        cout << "Exiting Term" << endl;
        return factor;
    }

    string Factor() {
        cout << "Entering Factor" << endl;
        string value;
        if (tokens[index].first == "number" || tokens[index].first == "identifier" || tokens[index].first == "string") {
            value = tokens[index++].second;
        } else if (tokens[index].second == "(") {
            match("symbol", "(");
            value = Expression();
            match("symbol", ")");
        } else {
            // Handle error or recovery
        }
        cout << "Exiting Factor" << endl;
        return value;
    }

    string Condition() {
        cout << "Entering Condition" << endl;
        string leftExpr = Expression();
        string relationalOp = tokens[index++].second;
        string rightExpr = Expression();
        string temp = newTemp();
        tac.emit(relationalOp, leftExpr, rightExpr, temp);
        cout << "Exiting Condition" << endl;
        return temp;
    }

    bool match(const string& expectedType, const string& expectedValue) {
        if (index < tokens.size() && tokens[index].first == expectedType && tokens[index].second == expectedValue) {
            ++index;
            return true;
        }
        return false;
    }
};

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

    vector<pair<string, string>> tokens = tokenize(code);

    Parser parser(tokens);
    parser.parse();
    parser.saveCode("output.txt");

    return 0;
}
