#include "generator.h"

using namespace std;
using namespace swan;

string Generator::GenProgram(const vector<string>& functions, const vector<string>& program, const Definitions& _) const {
    string functionsStr;
    for (size_t i = 0; i < functions.size(); ++i) {
        functionsStr += functions[i] + "\n";
    }
    string programStr;
    for (size_t i = 0; i < program.size(); ++i) {
        programStr += program[i];
    }
    return functionsStr + programStr + "\n";
}

string Generator::GenFunctionDef(const Function& func, const string& block) const {
    return GenFunctionHeader(func) + " {\n" + block + "}\n";
}

string Generator::GenStatement(const string& exp) const {
    return exp + ";\n";
}

string Generator::GenIf(const string& exp, const string& block, const string& elseifs, const string& else_, const string& end) const {
    return "if (" + exp + ") {\n" + block + elseifs + else_ + end;
}

string Generator::GenElseIf(const string& exp, const string& block) const {
    return "} else if (" + exp + ") {\n" + block;
}

string Generator::GenElse(const string& block) const {
    return "} else {\n" + block;
}

string Generator::GenEnd() const {
    return "}\n";
}

string Generator::GenFor(const Var& _, const string& assignment, const string& to, const string& step, const string& block, const string& end) const {
    return "for " + assignment + ", " + to + (block != "" ? (", " + step) : "") + " do\n" + block + end;
}

string Generator::GenWhile(const string& exp, const string& block, const string& end) const {
    return "while (" + exp + ") {\n" + block + end;
}

string Generator::GenReturn(int funcType, const string& exp) const {
    return "return " + exp + ";\n";
}

string Generator::GenVarDef(const Var& var, int expType, const string& exp, bool isGlobal) const {
    return (!isGlobal ? "var " : "") + GenAssignment(var, expType, exp);
}

string Generator::GenAssignment(const Var& var, int expType, const string& exp) const {
    return GenVarId(var.name) + " = " + exp;
}

string Generator::GenBinaryExp(int expType, const Token& token, const string& left, const string& right) const {
    const string op =
        (token.type == TOK_OR) ? " || " :
        (token.type == TOK_AND) ? " && " :
        (token.type == TOK_EQUAL) ? " == " :
        (token.type == TOK_NOTEQUAL) ? " != " :
        (token.type == TOK_LESSER) ? " < " :
        (token.type == TOK_LEQUAL) ? " <= " :
        (token.type == TOK_GREATER) ? " > " :
        (token.type == TOK_GREATER) ? " >= " :
        (token.type == TOK_PLUS) ? "+" :
        (token.type == TOK_MINUS) ? "-" :
        (token.type == TOK_MUL) ? "*" :
        (token.type == TOK_DIV) ? "/" :
        "%";
    return left + op + right;
}

string Generator::GenUnaryExp(const Token& token, const string& exp) const {
    if (token.type == TOK_NOT) return ("!" + exp) ;
    else return "-" + exp;
}

string Generator::GenGroupExp(const string& exp) const {
    return "(" + exp + ")";
}

string Generator::GenFunctionCall(const Function& func, const string& args) const {
    return GenFuncId(func.name) + args;
}

string Generator::GenArgs(const Function& func, const vector<Expression>& args) const {
    string result;
    for (size_t i = 0; i < args.size(); ++i) {
        result += args[i].code;
        if (i < args.size() - 1) result += ", ";
    }
    return "(" + result + ")";
}

string Generator::GenVar(const Var& var) const {
    return GenVarId(var.name);
}

string Generator::GenLiteral(const Token& token) const {
    switch (token.type) {
    case TOK_INTLITERAL:
    case TOK_REALLITERAL:
        return token.data;
    case TOK_STRINGLITERAL:
        return "\"" + token.data + "\"";
    case TOK_NULLLITERAL:
        return "null";
    default:
        return "<unknown>";
    }
}

string Generator::GenIndent(int level) const {
    const string space = "    ";
    string indent;
    for (int i = 0; i < level; ++i) {
        indent += space;
    }
    return indent;
}

string Generator::GenFunctionHeader(const Function& func) {
    return "function " + GenFuncId(func.name) + GenParams(func);
}

string Generator::GenParams(const Function& func) {
    string params;
    for (size_t i = 0; i < func.params.size(); ++i) {
        params += GenVarId(func.params[i].name);
        if (i < func.params.size() - 1) params += ", ";
    }
    return "(" + params + ")";
}

string Generator::GenFuncId(const string& id) {
    return "pico." + id;
}

string Generator::GenVarId(const string& id) {
    return "__pico__" + id;
}
