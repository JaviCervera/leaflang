#include "lib.h"

using namespace std;
using namespace swan;

size_t FindLibFunction(const Lib& lib, const string& name) {
    const string fixedName = strmanip::lower(name);
    for (size_t i = 0; i < lib.size(); ++i) {
        if (strmanip::lower(lib[i].name) == fixedName) {
            return i;
        }
    }
    return -1;
}
