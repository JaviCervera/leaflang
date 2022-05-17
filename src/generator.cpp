#include "generator.h"

using namespace std;
using namespace swan;

string Generator::GenProgram(const vector<string>& functions, const vector<string>& program, const Definitions& definitions) const {
    const string headerStr =
        "#include <string.h>\n"
        "#include <core/core.h>\n"
        "#include <core/litemem.h>\n\n"
        "#define _bool(a, is_str) (a && (is_str ? strcmp((const char*)a, \"\") : 1))\n"
        "#define _and(a, b, is_str) (_bool(a, is_str) ? b : a)\n"
        "#define _or(a, b, is_str) (_bool(a, is_str) ? a : b)\n"
        "#define _not(a, is_str) (_bool(a, is_str) ? 0 : 1)\n"
        "#define _TInt2TInt(v) (v)\n"
        "#define _TInt2TFloat(v) ((float)v)\n"
        "#define _TInt2TString(v) (Str(v))\n"
        "#define _TFloat2TInt(v) ((int)v)\n"
        "#define _TFloat2TFloat(v) (v)\n"
        "#define _TFloat2TString(v) StrF(v)\n"
        "#define _TString2TInt(v) Val(v)\n"
        "#define _TString2TFloat(v) ValF(v)\n"
        "#define _TString2TString(v) (v)\n"
        "#define _TList2TString(v) _ListToString(v)\n"
        "#define _TDict2TString(v) _DictToString(v)\n"
        "const TChar* _strcat(const TChar* a, const TChar* b) { TChar* str = (TChar*)lmem_autorelease(lstr_allocempty(strlen(a) + strlen(b))); strcpy(str, a); return strcat(str, b); }\n\n";
    const string globals = GenVarDefs(definitions.GetGlobals(), GenIndent(0)) + "\n";
    string functionDeclsStr;
    for (size_t i = 0; i < definitions.NumFunctions(); ++i) {
        functionDeclsStr += GenStatement(GenFunctionHeader(*definitions.GetFunction(i)));
    }
    functionDeclsStr += "\n";
    string functionsStr;
    for (size_t i = 0; i < functions.size(); ++i) {
        functionsStr += functions[i] + "\n";
    }
    string programStr = "int main(int argc, char* argv[]) {\n";
    programStr += GenIndent(1) + "_SetArgs(argc, argv);\n";
    for (size_t i = 0; i < program.size(); ++i) {
        programStr += GenIndent(1) + program[i];
    }
    programStr += GenIndent(1) + GenStatement(GenFunctionCleanup(NULL, definitions.GetGlobals()));
    programStr += GenIndent(1) + "return 0;\n";
    programStr += "}";
    return headerStr
        + globals
        + functionDeclsStr
        + functionsStr
        + programStr
        + "\n";
}

string Generator::GenFunctionDef(const Function& func, const string& block, const Definitions& definitions) const {
    vector<Var> locals;
    locals.insert(locals.begin(), definitions.GetLocals().begin() + func.params.size(), definitions.GetLocals().end());
    return GenFunctionHeader(func) + " {\n"
        + GenVarDefs(locals, GenIndent(1))
        + block
        + GenIndent(1) + GenStatement(GenFunctionCleanup(&func, definitions.GetLocals()))
        + "}\n";
}

string Generator::GenStatement(const string& exp) const {
    return exp + ";\n";
}

string Generator::GenIf(const Expression& exp, const string& block, const string& elseifs, const string& else_, const string& end) const {
    return "if (" + GenBoolExp(exp.type, exp.code) + ") {\n" + block + elseifs + else_ + end;
}

string Generator::GenElseIf(const Expression& exp, const string& block) const {
    return "} else if (" + GenBoolExp(exp.type, exp.code) + ")) {\n" + block;
}

string Generator::GenElse(const string& block) const {
    return "} else {\n" + block;
}

string Generator::GenEnd() const {
    return "}\n";
}

string Generator::GenFor(const Var& _, const string& assignment, const string& to, const string& step, const string& block, const string& end) const {
    const string varName = assignment.substr(0, assignment.find(" ", 0));
    return "for ("
        + assignment + "; "
        + varName + " <= " + to + "; "
        + varName + " += " + (step != "" ? step : "1")
        + ") {\n"
        + block
        + end;
}

string Generator::GenWhile(const Expression& exp, const string& block, const string& end) const {
    return "while (" + GenBoolExp(exp.type, exp.code) + ") {\n" + block + end;
}

string Generator::GenReturn(const Function* func, const string& exp, const Definitions& definitions) const {
    return GenFunctionCleanup(func, definitions.GetLocals(), exp)
        + " return "
        + ((func->type == TYPE_STRING || func->type == TYPE_LIST || func->type == TYPE_DICT)
            ? ("_AutoDec(" + exp + ")")
            : exp)
        + ";\n";
}

string Generator::GenVarDef(const Var& var, int expType, const string& exp, bool isGlobal) const {
    return GenAssignment(var, expType, exp);
}

string Generator::GenAssignment(const Var& var, int expType, const string& exp) const {
    const string varId = GenVarId(var.name);
    if (expType == TYPE_STRING || expType == TYPE_LIST || expType == TYPE_DICT) {
        return "lmem_assign(" + varId + ", " + exp + ")";
    } else {
        return varId + " = " + exp;
    }
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
        (token.type == TOK_GEQUAL) ? " >= " :
        (token.type == TOK_PLUS) ? "+" :
        (token.type == TOK_MINUS) ? "-" :
        (token.type == TOK_MUL) ? "*" :
        (token.type == TOK_DIV) ? "/" :
        "%";
    return
        (token.type == TOK_OR) ? ("_or(" + left + ", " + right + ", " + GenIsStr(expType) + ")") : 
        (token.type == TOK_AND) ? ("_and(" + left + ", " + right + ", " + GenIsStr(expType) + ")") :
        (token.type == TOK_PLUS && expType == TYPE_STRING) ? ("_strcat(" + left + ", " + right + ")") :
        (token.type == TOK_EQUAL && expType == TYPE_STRING) ? ("(strcmp(" + left + ", " + right + ") == 0)") :
        (token.type == TOK_NOTEQUAL && expType == TYPE_STRING) ? ("(strcmp(" + left + ", " + right + ") != 0)") :
        (token.type == TOK_LESSER && expType == TYPE_STRING) ? ("(strcmp(" + left + ", " + right + ") < 0)") :
        (token.type == TOK_LEQUAL && expType == TYPE_STRING) ? ("(strcmp(" + left + ", " + right + ") <= 0)") :
        (token.type == TOK_GREATER && expType == TYPE_STRING) ? ("(strcmp(" + left + ", " + right + ") > 0)") :
        (token.type == TOK_GEQUAL && expType == TYPE_STRING) ? ("(strcmp(" + left + ", " + right + ") >= 0)") :
        (token.type >= TOK_EQUAL && token.type <= TOK_GEQUAL) ? GenBoolExp(expType, left + op + right) :
        (left + op + right);
}

string Generator::GenList(const vector<Expression>& values) const {
    string str = "_CreateList()";
    for (size_t i = 0; i < values.size(); ++i) {
        string funcName = "";
        switch (values[i].type) {
        case TYPE_INT:
            funcName = "_SetListInt";
            break;
        case TYPE_FLOAT:
            funcName = "_SetListFloat";
            break;
        case TYPE_STRING:
            funcName = "_SetListString";
            break;
        case TYPE_LIST:
            funcName = "_SetListList";
            break;
        case TYPE_DICT:
            funcName = "_SetListDict";
            break;
        case TYPE_RAW:
            funcName = "_SetListRaw";
            break;
        }
        str = funcName + "("
            + str
            + ", " + strmanip::fromint(i)
            + ", " + values[i].code + ")";
    }
    return str;
}

string Generator::GenDict(const vector<Expression>& keys, const vector<Expression>& values) const {
    string str = "_CreateDict()";
    for (size_t i = 0; i < values.size(); ++i) {
        string funcName = "";
        switch (values[i].type) {
        case TYPE_INT:
            funcName = "_SetDictInt";
            break;
        case TYPE_FLOAT:
            funcName = "_SetDictFloat";
            break;
        case TYPE_STRING:
            funcName = "_SetDictString";
            break;
        case TYPE_LIST:
            funcName = "_SetDictList";
            break;
        case TYPE_DICT:
            funcName = "_SetDictDict";
            break;
        case TYPE_RAW:
            funcName = "_SetDictRaw";
            break;
        }
        str = funcName + "("
            + str
            + ", " + keys[i].code
            + ", " + values[i].code + ")";
    }
    return str;
}

string Generator::GenNotExp(const Expression& exp) const {
    return "_not(" + exp.code + ", " + GenIsStr(exp.type) + ")";
}

string Generator::GenCastExp(int castType, int expType, const std::string& exp) const {
    const string expTypeName = GenType(expType);
    const string castTypeName = GenType(castType);
    return string("_")
        + Replace(Replace(Replace(expTypeName.c_str(), "TChar", "TString"), "struct ", ""), "*", "")
        + "2"
        + Replace(Replace(Replace(castTypeName.c_str(), "TChar", "TString"), "struct ", ""), "*", "")
        + "(" + exp + ")";
}

string Generator::GenNegExp(const string& exp) const {
    return "-" + exp;
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
    case TOK_FLOATLITERAL:
        return token.data;
    case TOK_STRINGLITERAL:
        return "lstr_get(\"" + token.data + "\")";
    case TOK_NULLLITERAL:
        return "0";
    case TOK_TRUELITERAL:
        return "1";
    case TOK_FALSELITERAL:
        return "0";
    default:
        return "";
    }
}

string Generator::GenListGetter(int type, const string& listCode, const std::string& indexCode) const {
    string funcName = "";
    switch (type) {
    case TYPE_INT:
        funcName = "_ListInt";
        break;
    case TYPE_FLOAT:
        funcName = "_ListFloat";
        break;
    case TYPE_STRING:
        funcName = "_ListString";
        break;
    case TYPE_LIST:
        funcName = "_ListList";
        break;
    case TYPE_DICT:
        funcName = "_ListDict";
        break;
    case TYPE_RAW:
        funcName = "_ListRaw";
        break;
    }
    return funcName + "("
        + listCode
        + ", " + indexCode + ")";
}

string Generator::GenListSetter(const string& listCode, const std::string& indexCode, const Expression& valueExp) const {
    string funcName = "";
    switch (valueExp.type) {
    case TYPE_INT:
        funcName = "_SetListInt";
        break;
    case TYPE_FLOAT:
        funcName = "_SetListFloat";
        break;
    case TYPE_STRING:
        funcName = "_SetListString";
        break;
    case TYPE_LIST:
        funcName = "_SetListList";
        break;
    case TYPE_DICT:
        funcName = "_SetListDict";
        break;
    case TYPE_RAW:
        funcName = "_SetListRaw";
        break;
    }
    return funcName + "("
        + listCode
        + ", " + indexCode
        + ", " + valueExp.code + ")";
}

string Generator::GenDictGetter(int type, const string& dictCode, const std::string& indexCode) const {
    string funcName = "";
    switch (type) {
    case TYPE_INT:
        funcName = "_DictInt";
        break;
    case TYPE_FLOAT:
        funcName = "_DictFloat";
        break;
    case TYPE_STRING:
        funcName = "_DictString";
        break;
    case TYPE_LIST:
        funcName = "_DictList";
        break;
    case TYPE_DICT:
        funcName = "_DictDict";
        break;
    case TYPE_RAW:
        funcName = "_DictRaw";
        break;
    }
    return funcName + "("
        + dictCode
        + ", " + indexCode + ")";
}

string Generator::GenDictSetter(const string& dictCode, const std::string& indexCode, const Expression& valueExp) const {
    string funcName = "";
    switch (valueExp.type) {
    case TYPE_INT:
        funcName = "_SetDictInt";
        break;
    case TYPE_FLOAT:
        funcName = "_SetDictFloat";
        break;
    case TYPE_STRING:
        funcName = "_SetDictString";
        break;
    case TYPE_LIST:
        funcName = "_SetDictList";
        break;
    case TYPE_DICT:
        funcName = "_SetDictDict";
        break;
    case TYPE_RAW:
        funcName = "_SetDictRaw";
        break;
    }
    return funcName + "("
        + dictCode
        + ", " + indexCode
        + ", " + valueExp.code + ")";
}

string Generator::GenIndent(int level) const {
    const string space = "    ";
    string indent;
    for (int i = 0; i < level; ++i) {
        indent += space;
    }
    return indent;
}

string Generator::GenFunctionHeader(const Function& func) const {
    return GenType(func.type) + " " + GenFuncId(func.name) + GenParams(func);
}

string Generator::GenParams(const Function& func) const {
    string params;
    for (size_t i = 0; i < func.params.size(); ++i) {
        params += GenType(func.params[i].type) + " " +  GenVarId(func.params[i].name);
        if (i < func.params.size() - 1) params += ", ";
    }
    return "(" + params + ")";
}

string Generator::GenType(int type) {
    switch (type) {
        case TYPE_INT:
            return "TInt";
        case TYPE_FLOAT:
            return "TFloat";
        case TYPE_STRING:
            return "TChar*";
        case TYPE_LIST:
            return "struct TList*";
        case TYPE_DICT:
            return "struct TDict*";
        case TYPE_RAW:
            return "void*";
        case TYPE_VOID:
            return "void";
        default:
            return ""; // Should not get here
    }
}

string Generator::GenVarDefs(const std::vector<Var>& locals, const string& indent) const {
    string str;
    for (size_t i = 0; i < locals.size(); ++i) {
        str += indent + GenStatement(GenType(locals[i].type) + " " + GenVarId(locals[i].name) + " = " + GenVarInit(locals[i].type));
    }
    return str;
}

string Generator::GenVarInit(int type) {
    switch (type) {
        case TYPE_INT:
            return "0";
        case TYPE_FLOAT:
            return "0.0f";
        case TYPE_STRING:
            return "0";
        case TYPE_LIST:
            return "0";
        case TYPE_DICT:
            return "0";
        case TYPE_RAW:
            return "0";
        case TYPE_VOID:
            return "";
        default:
            return ""; // Should not get here
    }
}

string Generator::GenFuncId(const string& id) {
    return id;
}

string Generator::GenVarId(const string& id) {
    return "lf_" + id;
}

string Generator::GenFunctionCleanup(const Function* func, const vector<Var>& varsInScope, string exclude) {
    vector<Var> vars = GetManagedVars(varsInScope);
    if (func) {
        const vector<Var> params = GetManagedVars(func->params);
        for (size_t i = 0; i < params.size(); ++i) {
            if (GenVarId(params[i].name) != exclude) {
                vars.push_back(params[i]);
            }
        }
    }
    string str = "_DoAutoDec(); ";
    for (size_t i = 0; i < vars.size(); ++i) {
        const Var& var = vars[i];
        if (GenVarId(var.name) != exclude) {
            str += "_DecRef(" + GenVarId(var.name) + "); ";
        }
    }
    return str;
}

vector<Var> Generator::GetManagedVars(const vector<Var>& vars) {
    vector<Var> result;
    for (size_t i = 0; i < vars.size(); ++i) {
        const Var& var = vars[i];
        if (var.type == TYPE_LIST || var.type == TYPE_DICT) {
            result.push_back(var);
        }
    }
    return result;
}

string Generator::GenBoolExp(int expType, const std::string& expCode) {
    return "_bool(" + expCode + ", " + GenIsStr(expType) + ")";
}

string Generator::GenIsStr(int expType) {
    return (expType == TYPE_STRING) ? "1" : "0";
}
