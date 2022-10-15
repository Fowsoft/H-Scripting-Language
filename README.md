# H Scripting Language

H is a C-like scripting language with some innovative features:

-blacklists - An interpreter can decide if some libraries or features will not be accessible

-simple base - The base of the language is very minimal, allowing rapid intgration. Functionality is provided by including (allowing) parts of standard library.

-custom integration layer - The HIL helps you quickly integrate H functionality by just providing some basic functionality

## Contents

This repo includes:

-the base C++ interpreter and integration base

-the base Java interpreter and integration base

-the standard library

-the official specifications

## H File Format

`.hs` - H Script

`.hi` - H Interpreted Layer

## Syntax

-ends with semicolon `;`

## Standard Library

-standard library is in the hsl/main

## Hello World

```
import("stdlib")
void main(Arguments aArgs) {
    stdlib.out.print("Hello, world!", FLAG_BOLD);
};
```


