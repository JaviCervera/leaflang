#pragma once

#include "definitions.h"
#include "expression.h"
#include "generator.h"
#include "lib.h"
#include "token.h"
#include "token_stream.h"
#include "var_def.h"

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    void Parse();
    void ParseLibrary(const std::vector<Token>& tokens);
    const Lib& GetLib() const;
    const std::string& GetCode() const;
private:
    Lib lib;
    Generator generator;
    Definitions definitions;
    TokenStream stream;
    std::string code;
    const Function* currentFunc;
    
    std::string ParseFunctionDef();
    Function ParseFunctionHeader();
    const std::string& ParseNewId();
    std::vector<Var> ParseParams();
    void ParseOpenParen();
    int ParseParamType();
    void ParseCloseParen();
    int ParseReturnType();
    std::string ParseBlock(int indent);
    std::string ParseStatement(int indent);
    bool IsAssignment() const;
    std::string ParseAssignment();
    const std::string& CheckId(const Token& token) const;
    void CheckTypes(int expected, int got, const Token& token);
    void CheckTypesForArg(int expected, int got, const Token& token);
    void ParseStatementEnd();
    std::string ParseControlStatement(int indent);
    std::string ParseIf(int indent);
    std::string ParseElseIf(int indent);
    std::string ParseElse(int indent);
    std::string ParseEnd(int indent);
    std::string ParseFor(int indent);
    Expression ParseTo();
    Expression ParseStep();
    std::string ParseWhile(int indent);
    std::string ParseReturn(int indent);
    std::string ParseVarDef();
    Expression ParseExp();
    Expression ParseOrExp();
    Expression ParseAndExp();
    Expression ParseEqualExp();
    Expression ParseRelExp();
    Expression ParseAddExp();
    Expression ParseMulExp();
    Expression ParseUnaryExp();
    Expression ParseGroupExp();
    Expression ParseAtomicExp();
    Expression ParseCast(const Token& typeToken);
    Expression ParseFunctionCall(const Token& nameToken);
    Expression ParseArgs(const Function* func);
    Expression ParseArg(int paramType, const Token& token);
    Expression ParseVarAccess(const Token& nameToken);
};
