#include <iostream>
#include <cctype>
#include <string>
#include <vector>
using namespace std;

// Token types enum
enum TokenType {
    TOKEN_INT, TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_ASSIGN,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_IF, TOKEN_EQUAL, TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_SEMICOLON, TOKEN_EOF, TOKEN_UNKNOWN
};

// Token structure
struct Token {
    TokenType type;
    string text;
};

// Lexer class
class Lexer {
    string input;
    int pos;
public:
    Lexer(const string &source) : input(source), pos(0) {}
    
    char peek() { return (pos < input.size()) ? input[pos] : '\0'; }
    char get() { return (pos < input.size()) ? input[pos++] : '\0'; }
    
    vector<Token> tokenize() {
        vector<Token> tokens;
        while (pos < input.size()) {
            char c = peek();
            if (isspace(c)) { get(); continue; }
            
            if (isalpha(c)) {
                string id;
                while (isalnum(peek())) id += get();
                
                if (id == "int") tokens.push_back({TOKEN_INT, id});
                else if (id == "if") tokens.push_back({TOKEN_IF, id});
                else tokens.push_back({TOKEN_IDENTIFIER, id});
                continue;
            }
            
            if (isdigit(c)) {
                string num;
                while (isdigit(peek())) num += get();
                tokens.push_back({TOKEN_NUMBER, num});
                continue;
            }
            
            switch (c) {
                case '=': tokens.push_back({TOKEN_ASSIGN, string(1, get())}); break;
                case '+': tokens.push_back({TOKEN_PLUS, string(1, get())}); break;
                case '-': tokens.push_back({TOKEN_MINUS, string(1, get())}); break;
                case '{': tokens.push_back({TOKEN_LBRACE, string(1, get())}); break;
                case '}': tokens.push_back({TOKEN_RBRACE, string(1, get())}); break;
                case ';': tokens.push_back({TOKEN_SEMICOLON, string(1, get())}); break;
                default: tokens.push_back({TOKEN_UNKNOWN, string(1, get())}); break;
            }
        }
        tokens.push_back({TOKEN_EOF, "EOF"});
        return tokens;
    }
};

// Parser class
class Parser {
    vector<Token> tokens;
    int pos;
public:
    Parser(const vector<Token>& tokens) : tokens(tokens), pos(0) {}
    
    Token peek() { return (pos < tokens.size()) ? tokens[pos] : Token{TOKEN_EOF, "EOF"}; }
    Token get() { return (pos < tokens.size()) ? tokens[pos++] : Token{TOKEN_EOF, "EOF"}; }
    
    void parse() {
        while (peek().type != TOKEN_EOF) {
            Token token = get();
            cout << "Parsed: " << token.text << endl;
        }
    }
};

// Code Generator class
class CodeGenerator {
    vector<Token> tokens;
    int pos;
public:
    CodeGenerator(const vector<Token>& tokens) : tokens(tokens), pos(0) {}
    
    void generate() {
        cout << "\nGenerated Assembly Code:\n";
        while (pos < tokens.size()) {
            Token token = tokens[pos++];
            if (token.type == TOKEN_IDENTIFIER && pos < tokens.size() && tokens[pos].type == TOKEN_ASSIGN) {
                string var = token.text;
                pos++; // Skip '='
                Token value = tokens[pos++];
                cout << "MOV " << var << ", " << value.text << endl;
            } else if (token.type == TOKEN_IF) {
                pos++; // Skip '('
                Token conditionVar = tokens[pos++];
                pos++; // Skip '=='
                Token conditionValue = tokens[pos++];
                pos++; // Skip ')'
                cout << "CMP " << conditionVar.text << ", " << conditionValue.text << endl;
                cout << "JNE END_IF" << endl;
            } else if (token.type == TOKEN_RBRACE) {
                cout << "END_IF:" << endl;
            }
        }
    }
};

int main() {
    string source = "int a; a = 10; if (a == 10) { a = a + 1; }";
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();
    
    Parser parser(tokens);
    parser.parse();
    
    CodeGenerator codeGen(tokens);
    codeGen.generate();
    
    return 0;
}
