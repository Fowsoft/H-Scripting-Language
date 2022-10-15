![H Logo](https://github.com/Fowsoft/H-Scripting-Language/blob/7960f2ac740c6f48dc44d8fee60fed8a000eb53a/H4096.png)
# H Scripting Language

H is an open-source C-like scripting language with some innovative features:

-blacklists - An interpreter can decide if some libraries or features will not be accessible

-simple base - The base of the language is very minimal, allowing rapid intgration. Functionality is provided by including (allowing) parts of standard library.

-custom integration layer - The HIL helps you quickly integrate H functionality by just providing some basic functionality

## Contents

This repo includes:

-the base C++ interpreter and integration base (src/cpp/)

-the base Java interpreter and integration base (src/java/)

-the standard library (src/hsl/)

-the official specifications (spc/)

## H File Format

`.hs` - H Script

`.hi` - H Interpreted Layer

## Syntax

-ends with semicolon `;`

## Standard Library

## Hello World

```
import("stdlib")
void main(Arguments aArgs) {
    stdlib.out.print("Hello, world!", FLAG_BOLD);
};
```


