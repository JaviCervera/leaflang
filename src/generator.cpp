#include "generator.h"

using namespace std;
using namespace swan;

string Generator::GenProgram(const vector<string>& functions, const vector<string>& program, const Definitions& definitions) const {
    const string headerStr =
        "function _onassign(old,new) pico._IncRef(new); pico._DecRef(old); return new end\n"
        "function _bool(a) if a ~= false and a ~= nil and a ~= 0 and a ~= \"\" then return true else return false end end\n"
        "function _and(a, b) if _bool(a) then return b else return a end end\n"
        "function _or(a, b) if _bool(a) then return a else return b end end\n"
        "function _not(a) if _bool(a) then return 0 else return 1 end end\n"
        "function _int2int(v) return v end\n"
        "function _int2real(v) return v + 0.0 end\n"
        "function _int2string(v) return tostring(v) end\n"
        "function _real2int(v) return math.floor(v) end\n"
        "function _real2real(v) return v end\n"
        "function _real2string(v) return tostring(v) end\n"
        "function _string2int(v) return _or(tonumber(v), 0) end\n"
        "function _string2real(v) return _or(tonumber(v), 0) + 0.0 end\n"
        "function _string2string(v) return v end\n"
        "_args = {}\n"
        "function pico.AddIntArg(v) _args[#_args+1] = v end\n"
        "function pico.AddRealArg(v) _args[#_args+1] = v end\n"
        "function pico.AddStringArg(v) _args[#_args+1] = v end\n"
        "function pico.Call(f) local ret = pico[f](table.unpack(_args)) _args = {} return ret end\n"
        "function pico.CallInt(f) return Int(pico.CallFloat(f)) end\n"
        "function pico.CallReal(f) return tonumber(pico.Call(f)) end\n"
        "function pico.CallString(f) return tostring(pico.Call(f)) end\n"
        "function pico.Callable(f) return type(pico[f]) == \"function\" end\n";
    string functionsStr;
    for (size_t i = 0; i < functions.size(); ++i) {
        functionsStr += functions[i] + "\n";
    }
    string programStr;
    for (size_t i = 0; i < program.size(); ++i) {
        programStr += program[i];
    }
    return headerStr
        + functionsStr
        + programStr
        + GenFunctionCleanup(NULL, definitions.GetGlobals())
        + "\n";
}

string Generator::GenFunctionDef(const Function& func, const string& block, const Definitions& definitions) const {
    return GenFunctionHeader(func)
        + block
        + GenIndent(1)
        + GenStatement(GenFunctionCleanup(&func, definitions.GetLocals()))
        + "end\n";
}

string Generator::GenStatement(const string& exp) const {
    return exp + "\n";
}

string Generator::GenIf(const string& exp, const string& block, const string& elseifs, const string& else_, const string& end) const {
    return "if _bool(" + exp + ") then\n" + block + elseifs + else_ + end;
}

string Generator::GenElseIf(const string& exp, const string& block) const {
    return "elseif _bool(" + exp + ") then\n" + block;
}

string Generator::GenElse(const string& block) const {
    return "else\n" + block;
}

string Generator::GenEnd() const {
    return "end\n";
}

string Generator::GenFor(const Var& _, const string& assignment, const string& to, const string& step, const string& block, const string& end) const {
    const string fixedAssignment = (assignment.find("local ", 0) == 0)
        ? assignment.substr(6)
        : assignment;
    return "for " + fixedAssignment + ", " + to + (block != "" ? (", " + step) : "") + " do\n" + block + end;
}

string Generator::GenWhile(const string& exp, const string& block, const string& end) const {
    return "while _bool(" + exp + ") do\n" + block + end;
}

string Generator::GenReturn(const Function* func, const string& exp, const Definitions& definitions) const {
    return GenFunctionCleanup(func, definitions.GetLocals(), exp)
        + "do return "
        + ((func->type == TYPE_TABLE) ? ("pico._AutoDec(" + exp + ")") : exp)
        + " end\n";
}

string Generator::GenVarDef(const Var& var, int expType, const string& exp, bool isGlobal) const {
    return (!isGlobal ? "local " : "") + GenAssignment(var, expType, exp);
}

string Generator::GenAssignment(const Var& var, int expType, const string& exp) const {
    const string varId = GenVarId(var.name);
    if (expType == TYPE_TABLE) {
        return varId + " = _onassign(" + varId + ", " + exp + ")";
    } else {
        return varId + " = " + exp;
    }
}

string Generator::GenBinaryExp(int expType, const Token& token, const string& left, const string& right) const {
    const string op =
        (token.type == TOK_OR) ? " or " :
        (token.type == TOK_AND) ? " and " :
        (token.type == TOK_EQUAL) ? " == " :
        (token.type == TOK_NOTEQUAL) ? " ~= " :
        (token.type == TOK_LESSER) ? " < " :
        (token.type == TOK_LEQUAL) ? " <= " :
        (token.type == TOK_GREATER) ? " > " :
        (token.type == TOK_GREATER) ? " >= " :
        (token.type == TOK_PLUS && expType != TYPE_STRING) ? "+" :
        (token.type == TOK_PLUS && expType == TYPE_STRING) ? ".." :
        (token.type == TOK_MINUS) ? "-" :
        (token.type == TOK_MUL) ? "*" :
        (token.type == TOK_DIV) ? "/" :
        "%";
    return
        (token.type == TOK_OR) ? ("_or(" + left + ", " + right + ")") : 
        (token.type == TOK_AND) ? ("_and(" + left + ", " + right + ")") :
        (left + op + right);
}

string Generator::GenCastExp(int castType, int expType, const std::string& exp) const {
    const string expTypeName = GenType(expType);
    const string castTypeName = GenType(castType);
    return "_" + expTypeName + "2" + castTypeName + "(" + exp + ")";
}

string Generator::GenUnaryExp(const Token& token, const string& exp) const {
    if (token.type == TOK_NOT) return ("_not(" + exp + ")") ;
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
        return "nil";
    case TOK_TRUELITERAL:
        return "1";
    case TOK_FALSELITERAL:
        return "0";
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
    return "function " + GenFuncId(func.name) + GenParams(func) + "\n";
}

string Generator::GenParams(const Function& func) {
    string params;
    for (size_t i = 0; i < func.params.size(); ++i) {
        params += GenVarId(func.params[i].name);
        if (i < func.params.size() - 1) params += ", ";
    }
    return "(" + params + ")";
}

string Generator::GenType(int type) {
    switch (type) {
        case TYPE_INT:
            return "int";
        case TYPE_REAL:
            return "real";
        case TYPE_STRING:
            return "string";
        default:
            return ""; // Should not get here
    }
}

string Generator::GenFuncId(const string& id) {
    return "pico." + id;
}

string Generator::GenVarId(const string& id) {
    return "__pico__" + id;
}

string Generator::GenFunctionCleanup(const Function* func, const vector<Var>& varsInScope, string exclude) {
    vector<Var> vars = GetTableVars(varsInScope);
    if (func) {
        const vector<Var> params = GetTableVars(func->params);
        for (size_t i = 0; i < params.size(); ++i) {
            if (GenVarId(params[i].name) != exclude) {
                vars.push_back(params[i]);
            }
        }
    }
    string str = "pico._DoAutoDec() ";
    for (size_t i = 0; i < vars.size(); ++i) {
        const Var& var = vars[i];
        if (GenVarId(var.name) != exclude) {
            str += "pico._DecRef(" + GenVarId(var.name) + ") ";
        }
    }
    return str;
}

std::vector<Var> Generator::GetTableVars(const std::vector<Var>& vars) {
    std::vector<Var> result;
    for (size_t i = 0; i < vars.size(); ++i) {
        const Var& var = vars[i];
        if (var.type == TYPE_TABLE) {
            result.push_back(var);
        }
    }
    return result;
}
