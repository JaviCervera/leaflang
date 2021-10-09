# ![pico](logo.png) The Pico programming language

A super simple interpreted language inspired by BASIC. It transpiles to JavaScript, so programs
can be run natively on the browser, and on a computer through Node or QuickJS.

The language is imperative and supports classic procedural programming. It has no structs, but
tables can be used to group data instead.

## Language syntax

As said, the language is largely inspired by BASIC, aiming to provide a syntax which is really easy
to learn and comfortable to read. It also draws inspiration from other languages like Python.

### Comments

A comment is ignored by the compiler, so you can write any message on it. It starts with the `'`
symbol, and continues to the end of the line. For example:

```
'This comment takes the entire line
a = 50 'This comment starts after the number 50
```

### Data types

Pico can represent four types of data:

* Integer numbers.
* Real numbers.
* Strings.
* References.

A reference points to data that is defined by an external library, or to compund data
(for example, a table).

### Variables

Data can be stored in variables. Pico is a statically typed language, meaning that a type must be
specified for each variable that you use. For specifying the types, Pico uses suffixes, an approach
taken from BASIC. The suffixes are:

* `%`: Integer.
* `#`: Real.
* `$`: String.
* `@`: Reference.

Variables are not declared, but they need to be assigned before they can be accessed (failing to do
so will cause a compilation error). On this first assignment (which is called initialization), the
type must be specified suffixed to the name of the variable. If the typer is omitted, it will be
assumed that it is an integer. On further appearances, type suffix can be omitted, or it must match
that of initialization.

The type specified must be compatible with the value assigned (for example, a string value cannot
be assigned to an integer variable).

Variables initialized within functions will be local the function (so two variables with the same
name on different functions are actually two different variables, so their type can of course be
different).

On the other hand, variables initialized outside functions will be globally accessible *from the
point they are initialized*. For example, in the following snippet:

```
function Foo()
    var$ = "Foo"
end

var$ = "A global var"

function Bar()
    var$ = "Bar"
end

Print(var) 'Prints "A global var"
Foo()
Print(var) 'Prints "A global var"
Bar()
Print(var) 'Prints "Bar"
```

This means that when the `Foo` function is defined, `var$` has not yet been declared as a global,
so the function modifies a local variable instead. On the other hand, when `Bar` is called, `var`
has already been defined as a global, so the function modifies that global.

To learn more about functions, read the corresponding section.

### Statements

...

### Functions

...

### Conditionals

...

### Loops

...
