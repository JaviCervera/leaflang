#pragma once

#include "common.h"

struct Var {
    const std::string name;
    const int type;

    Var(const std::string& name, int type) : name(name), type(type) {
    }
};

struct Function {
    const std::string name;
    const int type;
    const std::vector<Var> params;

    Function(const std::string& name, int type, const std::vector<Var>& params) :
            name(name), type(type), params(params) {
    }

    Function(const std::string& name, int type, const std::vector<int>& params) :
            name(name), type(type), params(ParseParams(params)) {
    }
private:
    std::vector<Var> ParseParams(const std::vector<int>& iparams) {
        std::vector<Var> out;
        for (size_t i = 0; i < iparams.size(); ++i)
            out.push_back(Var("", iparams[i]));
        return out;
    }
};

typedef std::vector<Function> Lib;

size_t FindLibFunction(const Lib& lib, const std::string& name);
