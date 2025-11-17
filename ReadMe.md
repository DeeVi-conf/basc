# BasC Scripting Language

BasC (pronounced the same as Bash) is a Scripting language that is compiled to Bash files and ran.

## Primary Feature

- Simplified syntax
- Inspired by C syntax 

I built it because I love scripting but hate bash syntax. The syntax is inspired by C family of languages and should be far more comfortable and intutive for any one who is used to lower level languages

## Examples

basc sysntax:
```
var x = 10;
var y = 20;
var sum = x + y;
echo(sum);
```
Gets compiled to bash :

```
#!/usr/bin/env bash

x=10
y=20
sum=$((x + y))
echo "$sum"
```

The langauge is being developed, a full syntax doccumentation will be available with the first release.