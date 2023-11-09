# MMKLogixCompiler

**MMKLogixCompiler** is a _simple_ native Windows runtime compiler/interpreter, which works with simple 3-address coded logical language.

## Concurrency
This compiler can execute your custom code both sequentially and concurrently.

# How it works?
This application simply reads "program.txt" for its source code, then compiles each line of code and converts it to a semantic token, then starts interpretic those tokens. Eventually it will print out all the varilables' values that you have defined after performing the operations you've written in "program.txt".
