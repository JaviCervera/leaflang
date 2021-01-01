#include "definitions.h"

using namespace std;
using namespace swan;

void Definitions::AddFunction(const Function& func) {
    functions.push_back(func);
}

void Definitions::AddGlobal(const Var& global) {
    globals.push_back(global);
}

void Definitions::AddLocal(const Var& local) {
    locals.push_back(local);
}

void Definitions::ClearLocals() {
    locals.clear();
}

const Function* Definitions::FindFunction(const string& name) const {
    const string fixedName = strmanip::lower(name);
    for (size_t i = 0; i < functions.size(); ++i) {
        if (strmanip::lower(functions[i].name) == fixedName) {
            return &functions[i];
        }
    }
    return NULL;
}

size_t Definitions::NumFunctions() const {
    return functions.size();
}

const Function* Definitions::GetFunction(size_t index) const {
    if (index >= 0 && index < NumFunctions()) {
        return &functions[index];
    } else {
        return NULL;
    }
}

const Var* Definitions::FindVar(const string& name) const {
    const string fixedName = strmanip::lower(name);
    const Var* local = FindLocal(fixedName);
    return (local != NULL)
        ? local
        : FindGlobal(fixedName);
}

const bool Definitions::IsGlobal(const string& name) const {
    const string fixedName = strmanip::lower(name);
    return FindGlobal(fixedName) != NULL;
}

size_t Definitions::NumGlobals() const {
    return globals.size();
}

const Var* Definitions::GetGlobal(size_t index) const {
    if (index >= 0 && index < NumGlobals()) {
        return &globals[index];
    } else {
        return NULL;
    }
}

const Var* Definitions::FindLocal(const string& fixedName) const {
    for (size_t i = 0; i < locals.size(); ++i) {
        if (strmanip::lower(locals[i].name) == fixedName) {
            return &locals[i];
        }
    }
    return NULL;
}

const Var* Definitions::FindGlobal(const string& fixedName) const {
    for (size_t i = 0; i < globals.size(); ++i) {
        if (strmanip::lower(globals[i].name) == fixedName) {
            return &globals[i];
        }
    }
    return NULL;
}
