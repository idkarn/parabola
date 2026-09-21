# CParabola

> Interpreter for the Parabola programming language

## Syntax

```ebnf
program         = { statement } ;

statement       = constant_decl
                | variable_decl
                | proc_decl
                | until_stmt
                | when_stmt
                | return_stmt
                | assignment_stmt
                | expression_stmt
                | block ;

constant_decl   = "sure", [ "only" ],
                  declarator, { ",", declarator },
                  "=", expression, ";" ;

variable_decl   = ( "var" | "let" ), [ "only" ],
                  declarator, { ",", declarator },
                  [ "=", expression ], ";" ;

declarator      = identifier, [ ":", type ] ;

proc_decl       = "proc", identifier,
                  [ parameter, { ",", parameter } ],
                  "=>", [ type ], block ;

parameter       = identifier, [ ":", type ] ;

until_stmt      = "until", expression, block ;

when_stmt       = "when", expression, block,
                  [ "otherwise", ( block | when_stmt ) ] ;

return_stmt     = "ret", [ expression ], ";" ;

assignment_stmt = lvalue, "=", expression, ";" ;
lvalue          = identifier, { "[", expression, "]" } ;

expression_stmt = expression, ";" ;

block           = "{", { statement }, "}" ;

type            = identifier ;

expression      = logical_or ;
logical_or      = logical_xor, { ("or" | "nor"), logical_xor } ;
logical_xor     = logical_and, { "xor", logical_and } ;
logical_and     = logical_not, { ("and" | "nand"), logical_not } ;
logical_not     = { "not" }, comparison ;
comparison      = additive, [ ("=" | "~=" | "!=" | ">" | "<" | ">>" | "<<"), additive ] ;
additive        = multiplicative, { ("+" | "-"), multiplicative } ;
multiplicative  = postfix, { ("*" | "/"), postfix } ;
postfix         = primary, { "[" expression "]" | "(" [ expression, { ",", expression } ] ")" } ;
primary         = identifier | literal | list | "(" expression ")" ;

list            = "[", [ expression, { ",", expression } ], "]" ;

literal         = string | number | boolean ;
boolean         = "true" | "false" | "maybe" ;
number          = [ "-" ], ( integer | float ) ;
integer         = digit, { digit } ;
float           = digit, { digit }, ".", digit, { digit } ;

string          = "'", { string_char }, "'" ;
string_char     = escape | ? any symbol except "'" and newline character ? ;
escape          = "\", ( "'" | "\" | "n" | "t" | "r" ) ;

identifier      = ( letter | "_" ), { letter | digit | "_" } ;
letter          = "A" | ... | "Z" | "a" | ... | "z" ;
digit           = "0" | ... | "9" ;

comment         = "^", { ? any symbol except newline character ? }, newline ;
```