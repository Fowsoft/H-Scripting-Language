[![CC BY ND 4.0][cc-by-nd-shield]][cc-by-nd]

![H Logo](https://github.com/Fowsoft/H-Scripting-Language/blob/7960f2ac740c6f48dc44d8fee60fed8a000eb53a/H4096.png)
# H Scripting Language

H is an open-source C-like scripting language with some innovative features:

-blacklists - An interpreter can decide if some libraries or features will not be accessible

-simple base - The base of the language is very minimal, allowing rapid intgration. Functionality is provided by including (allowing) parts of standard library.

-custom integration layer - The HIL helps you quickly integrate H functionality by just providing some basic functionality

## Contents

This repo includes:

-the base C++ interpreter (src/cpp/)

-the base Java interpreter (src/java/)

-the base JavaScript interpreter (src/js/)

-the official specifications (spc/)

## H File Format

`.hsc` - H Script

`.hil` - H Interpreted Layer

## Syntax

-ends with semicolon `;`

## Standard Library

-specification separate

## Hello World

```
using std.io;
print("Hello, world!", FLAG_BOLD);
```

[![CC BY ND 4.0][cc-by-nd-image]][cc-by-nd]

[cc-by-nd]: https://creativecommons.org/licenses/by-nd/4.0/
[cc-by-nd-image]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png
[cc-by-nd-shield]: https://img.shields.io/badge/License-CC%20BY%20ND%204.0-lightgrey.svg

