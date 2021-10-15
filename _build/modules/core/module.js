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
