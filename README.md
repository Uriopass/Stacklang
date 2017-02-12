# Stacklang
A Python3 interpreter for an home-made stack based language heavily inspired from PostScript

## How to install :
1 - Install python3 if you don't have it already
2 - Clone the repo
3 - python3 language.py examples/isprime.txt

# Specifications

Stacklang is a Stack based language, which mean it does almost all of it's operation using a stack for the values

It uses the Reverse Polish Notation (or postfix notation).

An example of RPN would be `3 2 +` which add 3 and 2 in this order.

Basically 3 and 2 are pushed on the stack, and + is an operator that pop theses 2 items and push the result.

There are multiples cool operators defined below for manipulating, and adding data to the stack.

# Comments

Comments work the same way as in Python, i.e using `#`
They are no multi lines comment for the moment.

# Variables

As in any language there are variables in Stacklang, they use Dynamic Linking as the scoping system.
Their name are defined using /name and they are assigned using the `=` operator

Here is an example for defining a hundred :
```
/one_hundred 100 = # one_hundred is now 100
one_hundred 3 + # Pushed 103 to the stack
```

Note : There are no global variables for the moments

# Blocks

Stacklang manages the functions and if and while etc. using blocks. Blocks are simply a list of operations and main is just a big block.

```
{
	3 2 + # This blocks is simply the commands push 3, push 2, add.
}
``` 

*Warning* : Blocks are not executed when they are simply put in the middle of the code, actually they are just pushed onto the stack as a "block reference" to use for other operators.

# Functions

A function is just a variable that has been assigned a block. And when they are called they execute the block associated

```
/add_3 {3 +} =
5 add_3 # Pushed 8 on the stack
```

# Conditions

They are no booleans in Stacklang for the moment, so if there is a condition anywhere it is just testing an int with 0. i.e test = value != 0

The `if` operator is actually `?`, but you can just set `if` as `?` easily using functions.
```
/if {?} = # Change if to do an "?" operation
```

The `?` operator works by taking 3 arguments from the stack : A condition, a "true" block and a "false" block.
If the condition is true (i.e different from 0) then the "true" block is executed, if not the "false" block is.

```
/value 3 =
value 3 - # if value = 3 then value - 3 == 0
{
	# value is not three
}
{
	# value is three
}
?   # if operator
```

# Loops

There are 2 kinds of loops in Stacklang for the moment

## Repeat

The `repeat` loop which gets 2 values from the stack : a number of repetition and a block to repeat
```
10 {
	3
} repeat
# Stack is now [3, 3, 3 ... 10 times ..., 3, 3]
```

## While 

The `while` loop which gets 2 values from the stack : A condition block and a block to execute if the condition is true
```
{ condition }
{ block }
while # Tests the condition and execute block if the condition is true, start over

#Example :
/i 3 =
{ i } # tests for i == 0
{
	/i 1 - = # i -= 1
	i # Puts i on the stack
}
while
# Stack is now [2, 1, 0]
```

# Recursion

Recursion works in Stacklang by referencing what you are currently defining.
```
# Here is factorial using recursion : 
/fact 
{
	dup 1 - # "dup" duplicates the top of the stack, here the input. Then test for input = 1
	{
		dup 1 - fact * # Returns input * fact(input - 1)
	}
	{
		1 # Returns 1 since input = 1
	}
	?
} =

6 fact # Puts 120 on the stack
```

# All stack functions

Here is an exhaustive list of all the basic operators in the standard library :

## Math operators

Maths operators (except "!") all work by popping 2 inputs and pushing 1 output corresponding to the result

- "+" Add operator
- "-" Sub operator
- "*" Mul operator
- "/" Div operator
- "^" Pow operator
- "%" Mod operator
- "!" Factorial operator (This one does 1 pop and 1 push)

## Stack manipulations

Theses operator only manipulate the stack and don't add additional data

- "n exch"		- Takes 1 input from the stack and exchange the top and the (top - n) values of the stack. 
- "swap"		- Equivalent to `1 exch`, swap the two top items
- "n getpush"	- Gets and push the n element from the top of the stack. With stack [1, 2, 3], 2 getpush makes the stack go [1, 2, 3, 1]
- "dup"			- Duplicate the top of the stack, equivalent to 0 getpush
- "pop"			- Remove the top of the stack
- "n pack"		- Pack the n element from the top of the stack into 1 element. With stack [1, 2, 3] 2 pack makes the stack go [1, [2, 3]]
- "unpack"		- Unpack the input if it's a pack or a string. With stack [1, [2, 3]] unpack makes the stack go [1, 2, 3]

## Input/Output

- "input"		- Takes the input as a string and pushed it to the stack
- "output"		- Pop the top of the stack and print it
- "n rand"		- Push a random number between 0 and n to the stack. 3 rand takes a random between 0 and 3. [], 3 rand -> [2] or [1] etc

## Utils

- "x stoi"	- Converts a string input to a int, often used by doing `/in input stoi =`
- "x len"		- Pop a string or a pack and pushed the length to the stack

## Debug

- "variables"	- Prints all the variables
- "stack"		- Prints the current stack

## Conditions operator

- "a b leq"		- Output 1 if a <= b and 0 if b > a
- "a b geq"		- Output 1 if a >= b and 0 if b < a
- "a b eq"		- Output 1 if a == b and 0 if a != b

# Advanced function definition

Stacklang includes a arguments system for the function by cutting the stack in a specific way and returning a specific number of element to the parent stack.

For example + takes 2 inputs and 1 output. So we could define `add` as
```
/add {2> + >1} # Define add with 2 inputs and one output
```

Theses are of course optional and you need to do `enablelocalstack` command to activate this feature
You can also disable it by doing `disablelocalstack`.
This is cool because it sandboxes the function letting it do its thing and returning only what is needed

```
enablelocalstack
/f {1> 
	/n = #Set n to the input
	10 {
		n
	} repeat # Pushed 10x n to the stack
	>2 # Only returns 2 of thems (the 2 top ones)
} =

3 f 2 pack output # Prints pack [3, 3]
```

