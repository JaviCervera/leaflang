// ------------------------------------
// Console
// ------------------------------------

function Print(msg) {
    console.info(msg);
}

// ------------------------------------
// Math
// ------------------------------------

const DEG2RAD = 0.01745329;
const RAD2DEG = 57.29578;

function ASin(x) {
    return Math.asin(x) * RAD2DEG;
}

function ATan(x) {
    return Math.atan(x) * RAD2DEG;
}

function ATan2(x, y) {
    return Math.atan2(x, y) * RAD2DEG;
}

function Abs(x) {
    return Math.abs(x);
}

function Ceil(x) {
    return Math.ceil(x);
}

function Clamp(x, min, max) {
    return Min(Max(x, min), max);
}

function Cos(x) {
    return Math.cos(x * DEG2RAD);
}

function Exp(x) {
    return Math.exp(x);
}

function Floor(x) {
    return Math.floor(x);
}

function Log(x) {
    return Math.log(x);
}

function Max(x, y) {
    return Math.max(x, y);
}

function Min(x, y) {
    return Math.min(x, y);
}

function Pow(x, y) {
    return Math.pow(x, y);
}

function Sgn(x) {
    return (x > 0) ? 1 : (x < 0) ? -1 : 0;
}

function Sin(x) {
    return Math.sin(x * DEG2RAD);
}

function Sqrt(x) {
    return Math.sqrt(x);
}

function Tan(x) {
    return Math.tan(x * DEG2RAD);
}

function Int(num) {
    value = parseInt(num);
    return isNaN(value) ? 0 : value;
}

// ------------------------------------
// String
// ------------------------------------

function Len(str) {
    return str.length;
}

function Left(str, count) {
    return str.substr(0, count);
}

function Right(str, count) {
    return str.substr(str.length - count);
}

function Mid(str, offset, count) {
    return str.substr(offset, count);
}

function Lower(str) {
    return str.toLowerCase();
}

function Upper(str) {
    return str.toUpperCase();
}

function Find(str, find, offset) {
    return str.indexOf(find, offset);
}

function Replace(str, find, replace) {
    return str.replace(find, replace);
}

function Trim(str) {
    return str.trim();
}

function Join(table, separator) {
    var result = "";
    var size = Size(table);
    for (var i = 0; i < size; ++i) {
        if (i > 0) result += separator;
        result += IndexString(table, i);
    }
    return result;
}

function Split(str, separator) {
    var delim = (Len(separator) > 0) ? separator[0] : " ";
    var split = str.split(delim);
    var table = DimTable(table);
    for (var i = 0; i < split.length; ++i) {
        SetIndexString(table, i, split[i])
    }
    return table;
}

const char* StripExt(const char* filename) {
    static string result;
    result = strmanip::stripext(filename);
    return result.c_str();
}

const char* StripDir(const char* filename) {
    static string result;
    result = strmanip::stripdir(filename);
    return result.c_str();
}

const char* ExtractExt(const char* filename) {
    static string result;
    result = strmanip::extractext(filename);
    return result.c_str();
}

const char* ExtractDir(const char* filename) {
    static string result;
    result = strmanip::extractdir(filename);
    return result.c_str();
}

int Asc(const char* str, int index) {
    return int(str[index]);
}

const char* Chr(int c) {
    static string result;
    result = string(1, char(c));
    return result.c_str();
}

const char* Str(int val) {
    static string result;
    result = strmanip::fromint(val);
    return result.c_str();
}

const char* StrF(float val) {
    static string result;
    result = strmanip::fromdouble(val);
    return result.c_str();
}

int Val(const char* str) {
    return strmanip::toint(str);
}

float ValF(const char* str) {
    return strmanip::tofloat(str);
}

const char* LoadString(const char* filename) {
    static string result;
    result = strmanip::read(filename);
    return result.c_str();
}

void SaveString(const char* filename, const char* str, int append) {
    strmanip::write(str, filename, append);
}
