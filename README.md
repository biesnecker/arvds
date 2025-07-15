# arvds

A simple utility for converting from [arvelie dates](https://wiki.xxiivv.com/site/arvelie.html) to YYYY-MM-DD datestrings, and vice versa.

It is written in C89 and requires GCC or something like it to compile. A makefile is included, but it's a single file with no dependencies so feel free to compile it however you see fit.

```
Usage: arvds <direction> <start-year> <timestamp>

arvds is a tool for converting an arvelie date to a
YYYY-MM-DD date, and vice versa.

See: https://wiki.xxiivv.com/site/arvelie.html

Example usage:

  arvds from 2020 2020-01-01 -> 00A00
  arvds to 2020 03C07 -> 2024-02-04

Year must be greater than 0.
```

Made by hand, for fun, and without AI, because computers are supposed to be fun.
