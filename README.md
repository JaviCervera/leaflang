# ![leaf](logo.png) The Leaf programming language

A super simple interpreted language inspired by Lua and BASIC. It transpiles to Lua, and comes with a
custom interpreter that, in the future, will be able to load modules from dynamic libraries.

The language is imperative and supports classic procedural programming. It has no structs, but
hashes can be used to group data instead.

## Language syntax

As said, the language is largely inspired by Lua and BASIC, aiming to provide a syntax which is really easy
to learn and comfortable to read. It also draws inspiration from other languages like Python.

### Comments

A comment is ignored by the compiler, so you can write any message on it. It starts with the `//`
symbol, and continues to the end of the line. For example:

```
// This comment takes the entire line
a = 50 // This comment starts after the number 50
```

Multiline comments are also supported. They start with `/*`, and continue until `*/`, even if
it is on a different line.

```
/*
    This comment spans across
    multiple lines.
*/
```

### Data types

Leaf can represent these types of data:

* Integer numbers.
* Real numbers.
* Strings.
* Hashes.
* References.

Hashes are associative collections of string keys and values of any type. References point to data that is defined by an external library.

### Variables

Data can be stored in variables. Variables are not declared, but they need to be assigned before
they can be accessed (failing to do so will cause a compilation error). On this first assignment
(which is called initialization), the type of the variable will be inferred from the type of the
expression assigned to it. For example:

```
integer = 50
real = 21.4
text = "Hello"
```

When assigning a value to a variable, the type of the value must be compatible with the type of the
variable (for example, a string value cannot be assigned to an integer variable). To assign an
incompatible type, expression must be cast. A cast is performed by adding a suffix to the end of an
expression. The valid suffixes are:

* `|i`: Integer.
* `|r`: Real.
* `|s`: String.

For example:

```
integer = "128"|i
real = "36.12"|r
string = 5|s
```

Variables initialized within functions will be local the function (so two variables with the same
name on different functions are actually two different variables, so their type can of course be
different).

On the other hand, variables initialized outside functions will be globally accessible *from the
point they are initialized*. For example, in the following snippet:

```
function Foo()
    var = "Foo"
end

var = "A global var"

function Bar()
    var = "Bar"
end

Print(var) // Prints "A global var"
Foo()
Print(var) // Prints "A global var"
Bar()
Print(var) // Prints "Bar"
```

This means that when the `Foo` function is defined, `var` has not yet been declared as a global,
so the function modifies a local variable instead. On the other hand, when `Bar` is called, `var`
has already been defined as a global, so the function modifies that global.

To learn more about functions, read the corresponding section.

### Statements

...

### Functions

...

### Conditionals

...

There is no specific boolean type, all types can be evaluated as booleans instead:

* Integers: `0` is false, all others true.
* Real numbers: `0.0` is false, all others true.
* Strings: `""` (empty string) is false, all others true.
* Hashes: Always true.
* References: `null` is false, all others true.

There are two literals, `true` and `false`, with the values `1` and `0` respectively.

### Loops

...
