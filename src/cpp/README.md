# The C++ Interpreter for H

This includes 2 files. 

`Main.cpp` - the interpreter

`MathInterpreter.cpp` - the math solver

If you need just the interpreter, copy the H and Utils namespaces from Main.cpp.

If you already implemented the utilities, copy just the H namespace (from Main.cpp).

If you need just a math expression solver, copy the MathInterpreter.cpp file.

## String Special Characters

`\n` - a newline

`\H` - H vendor string

`\"` - " (quote)

`\{` - { (left parenthesis)

`\}` - } (right parenthesis)

## Special Functions (C++ only)

`hinf; - print information about H (treated as keyword)

`setcolor(num);` - sets color of id num (please check your interpreters' and OS's settings)

**Possible values:**

- `black`
- `red`
- `green`
- `brown`
- `blue`
- `purple`
- `cyan`
- `light_grey`
- `dark_grey`
- `light_red`
- `light_green`
- `yellow`
- `light_blue`
- `pink`
- `light_cyan`
- `white`

**WARNING: Do not forget to add quotes!**

`resetcolor();` - resets color

`declline(identifier);` - gets declaration line of function or variable

## Included

-utilities

-interpreter

-shell handler

-standalone math interpreter

## Warnings and Errors

### Warnings

Extra semicolon at line N

### Errors

//todo: add warnings and errors

