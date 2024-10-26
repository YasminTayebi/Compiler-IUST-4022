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

// Node structure for parse tree
struct Node {
    string type;
    string lexeme;
    vector<Node*> children;
};

// Tokenizer function
vector<Token> tokenize(const string& code) {
    vector<Token> tokens;
    string reservedword = "\\b(int|float|void|cin|cout|continue|break|#include|using|namespace|std|main|if|else|while|for|return)\\b";
    string identifier = "\\b[a-zA-Z_][a-zA-Z0-9_]*\\b";
    string digit = "\\b[0-9]\\b";
    string number = "\\b[0-9]+(?:\\.[0-9]+)?\\b";
    string stringsPattern = R"(".*?")";
    string symbol = R"([{}()<>|&=!;,+\-*\/%])"; // Updated to include arithmetic operators and more symbols
    string whitespacePattern = "\\s+";

    string tokenRegexStr = "(" + reservedword + "|" + identifier + "|" + digit + "|" + number + "|" + stringsPattern + "|" + symbol + "|" + whitespacePattern + ")";
    regex tokenRegex(tokenRegexStr);

    smatch match;
    int lineNumber = 1;
    string::const_iterator start = code.begin();
    string::const_iterator end = code.end();
    while (regex_search(start, end, match, tokenRegex)) {
        if (!regex_match(match[0].str(), regex(whitespacePattern))) {
            Token token;
            token.lexeme = match[0];
            token.line = lineNumber;
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
        while (start != end && (*start == '\n' || *start == '\r')) {
            if (*start == '\n') lineNumber++;
            ++start;
        }
    }
    return tokens;
}

// Parser class
class Parser {
    vector<Token>::iterator currentToken;
    vector<Token>::iterator endToken;

public:
    Parser(vector<Token>& tokens) : currentToken(tokens.begin()), endToken(tokens.end()) {}

    Node* parse() {
        return program();
    }

private:
    Node* program() {
        Node* node = new Node{ "Program", "" };
        node->children.push_back(includes());
        node->children.push_back(usingNamespace());
        node->children.push_back(function());
        return node;
    }

    Node* includes() {
        Node* node = new Node{ "Includes", "" };
        if (match("Reservedword", "#include")) {
            node->children.push_back(new Node{ "Include", consume().lexeme });
            expect("Symbol", "<");
            node->children.push_back(new Node{ "Identifier", consume().lexeme });
            expect("Symbol", ">");
        }
        return node;
    }

    Node* usingNamespace() {
        Node* node = new Node{ "UsingNamespace", "" };
        if (match("Reservedword", "using")) {
            node->children.push_back(new Node{ "Using", consume().lexeme });
            expect("Reservedword", "namespace");
            node->children.push_back(new Node{ "Identifier", consume().lexeme });
            expect("Symbol", ";");
        }
        return node;
    }

    Node* function() {
        Node* node = new Node{ "Function", "" };
        expect("Reservedword", "int");
        node->children.push_back(new Node{ "Identifier", consume().lexeme });
        expect("Symbol", "(");
        expect("Symbol", ")");
        expect("Symbol", "{");
        node->children.push_back(statements());
        expect("Symbol", "}");
        return node;
    }

    Node* statements() {
        Node* node = new Node{ "Statements", "" };
        while (!match("Symbol", "}")) {
            node->children.push_back(statement());
        }
        return node;
    }

    Node* statement() {
        if (match("Reservedword", "int")) {
            return declaration();
        }
        else if (match("Identifier")) {
            return assignment();
        }
        else if (match("Reservedword", "while")) {
            return whileLoop();
        }
        else if (match("Reservedword", "cin")) {
            return cin();
        }
        else if (match("Reservedword", "cout")) {
            return cout();
        }
        else if (match("Reservedword", "return")) {
            return returnStatement();
        }
        return nullptr;
    }

    Node* declaration() {
        Node* node = new Node{ "Declaration", "" };
        expect("Reservedword", "int");
        node->children.push_back(new Node{ "Identifier", consume().lexeme });
        expect("Symbol", ";");
        return node;
    }

    Node* assignment() {
        Node* node = new Node{ "Assignment", "" };
        node->children.push_back(new Node{ "Identifier", consume().lexeme });
        expect("Symbol", "=");
        node->children.push_back(expression());
        expect("Symbol", ";");
        return node;
    }

    Node* whileLoop() {
        Node* node = new Node{ "WhileLoop", "" };
        expect("Reservedword", "while");
        expect("Symbol", "(");
        node->children.push_back(condition());
        expect("Symbol", ")");
        expect("Symbol", "{");
        node->children.push_back(statements());
        expect("Symbol", "}");
        return node;
    }

    Node* cin() {
        Node* node = new Node{ "Cin", "" };
        expect("Reservedword", "cin");
        expect("Symbol", ">>");
        node->children.push_back(new Node{ "Identifier", consume().lexeme });
        expect("Symbol", ";");
        return node;
    }

    Node* cout() {
        Node* node = new Node{ "Cout", "" };
        expect("Reservedword", "cout");
        expect("Symbol", "<<");
        node->children.push_back(new Node{ "String", consume().lexeme });
        expect("Symbol", "<<");
        node->children.push_back(new Node{ "Identifier", consume().lexeme });
        expect("Symbol", ";");
        return node;
    }

    Node* returnStatement() {
        Node* node = new Node{ "Return", "" };
        expect("Reservedword", "return");
        node->children.push_back(new Node{ "Number", consume().lexeme });
        expect("Symbol", ";");
        return node;
    }

    Node* condition() {

        Node* node = new Node{ "Condition", "" };
        node->children.push_back(new Node{ "Identifier", consume().lexeme });
        expect("Symbol", ">=");
        node->children.push_back(new Node{ "Number", consume().lexeme });
        return node;
    }

    Node* expression() {
        Node* node = new Node{ "Expression", "" };
        node->children.push_back(new Node{ "Identifier", consume().lexeme });
        if (match("Symbol", "+")) {
            consume();
            node->children.push_back(new Node{ "Identifier", consume().lexeme });
        }
        return node;
    }

    bool match(const string& type, const string& lexeme = "") {
        return currentToken != endToken && currentToken->type == type && (lexeme.empty() || currentToken->lexeme == lexeme);
    }

    Token consume() {
        return *currentToken++;
    }

    void expect(const string& type, const string& lexeme = "") {
        if (!match(type, lexeme)) {
            cerr << "Syntax error: expected " << type << " '" << lexeme << "', got " << currentToken->type << " '" << currentToken->lexeme << "'\n";
            exit(EXIT_FAILURE);
        }
        consume();
    }
};

// Function to print parse tree
void printParseTree(Node* node, int depth = 0) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << node->type;
    if (!node->lexeme.empty()) cout << " (" << node->lexeme << ")";
    cout << endl;
    for (Node* child : node->children) {
        printParseTree(child, depth + 1);
    }
}

// Main function
int main() {
    string code = R"(
       
        using namespace std;
        int main){
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
    Parser parser(tokens);
    Node* parseTree = parser.parse();
    printParseTree(parseTree);
    return 0;
}