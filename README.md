# Stacklang
An home-made interpreter written in Python3 for a custom stack-based language heavily inspired from PostScript

### How to install :
1. Install python3 if you don't have it already  
2. Clone the repo  
### How to use :
python3 main.py examples/isprime.txt  

# Specifications

Stacklang is a Stack based language, which mean it executes almost all operations using a global stack for the values

A basic introduction would be the way Reverse Polish Notation (or postfix notation) works.  
An example of RPN is `3 2 +` which add 3 and 2 in this order.

Basically `3` and `2` are pushed on the stack, and `+` is an operator that pop theses 2 items and push the result.

There are multiples cool operators defined below for manipulating, and adding data to the stack.

## Comments

Comments work the same way as in Python, i.e using `#`  
There are no multi lines comment for the moment.

## Variables

As in any language there are variables in Stacklang, they use Dynamic Linking for the scoping system.  
Defining is done by using `/name`, and assigning using the `def` operator.

Here is an example for defining a hundred :
``` Python
/one_hundred 100 def # one_hundred is now 100
one_hundred 3 + # Pushed 103 to the stack
```

**Note** : There are no global variables for the moment.

## Blocks

Stacklang manages the functions and if and while etc. using blocks.  
Blocks are simply a list of operations.

```Python
{
	3 2 + # This block is simply the commands push 3, push 2, add.
}
```

**Warning** : Blocks are not executed when they are simply put in the middle of a code, actually they are just pushed onto the stack as a "block reference" to use for other operators like `def`.

## Functions

A function is just a variable that has been assigned a block, and when they are called they execute the block associated.

```Python
/add_3 {3 +} def
5 add_3 # The stack is now [8]
```

## Conditions

There are no booleans in Stacklang for the moment, so if there is a condition anywhere it is just testing an int with 0. i.e `test = value != 0`.

Conditions are done using the `if` operator.

The `if` operator works by taking 3 arguments from the stack :  
- A condition
- A `true` block
- A `false` block.

If the condition is true (i.e different from 0) then the `true` block is executed, if not the `false` block is.

```Python
/value 3 def
value 3 - # if value = 3 then value - 3 == 0
{
	"Value is not three" # This is the true block
}
{
	"Value is three" # This is the false block
}
if
# Stack is now ["Value is three"]
```

## Loops

There are 2 kinds of loops in Stacklang for the moment  

### Repeat

The `repeat` loop gets 2 values from the stack :
- a number of repetition
- a block to repeat

```Python
5 {
	3
} repeat
# Stack is now [3, 3, 3, 3, 3]
```

### While

The `while` loop gets 2 values from the stack :
- A condition block
- a block to execute if the condition is true

```Python
{ condition }
{ block }
while # Tests the condition and execute block if the condition is true, start over

#Example :
/i 3 def
{ i } # tests for i != 0
{
	/i 1 - def # i -= 1
	i # Puts i on the stack
}
while
# Stack is now [2, 1, 0]
```

## Recursion

Stacklang supports recursion

```Python
# Here is factorial using recursion :
/fact
{
	dup 1 - # "dup" duplicate the top of the stack (here, the input). Then tests for input != 1
	{
		dup 1 - fact * # Returns input * fact(input - 1)
	}
	{
		1 # Returns 1 since input = 1
	}
	if
} def

6 fact # Puts 120 on the stack
```

## All stack functions

Here is an exhaustive list of all the basic operators in the standard library :

### Math operators

Maths operators (except "!") all work by popping 2 inputs and pushing 1 output corresponding to the result.

`i` is the number of input that are popped and `o` is the number of output pushed.  
They respect the relation `len(oldstack) - i + o = len(newstack)`

| Operator | Name | i     |    o  | Description                                     | Example
| :------: | :---:| :---: | :---: | :---------------------------------------------: | :-----:
| +        | Add  | 2     | 1     | Add the two numbers on top of the stacks        | `3 2 + # Stack is now [5]`
| -        | Sub  | 2     | 1     | Substracts the two numbers on top of the stacks | `3 2 - # [1]`
| \*       | Mul  | 2     | 1     | Multiply the two numbers on top of the stacks   | `3 2 * # [6]`
| /        | Div  | 2     | 1     | Divide the two numbers on top of the stacks     | `4 2 / # [2]`
| ^        | Pow  | 2     | 1     | Add the two numbers on top of the stacks        | `2 3 ^ # [8]`
| %        | Mod  | 2     | 1     | Add the two numbers on top of the stacks        | `5 2 % # [1]`
| !        | Fac  | 1     | 1     | Add the two numbers on top of the stacks        | `3 ! # [6]`

### Stack manipulations

These operators only manipulate the stack and don't add any additional data

| Operator    | Name          | i   |  o  | Description                                                 | Example
| :---------: | :------------:| :-: | :-: | :---------------------------------------------------------: | :-----:
| exch        | Exchange      |  1  |  0  | Exchange the top and the (top - val) values                 | `[1, 2, 3] => 2 exch => [3, 2, 1]`
| swap        | Swap          |  2  |  2  | Equivalent to `1 exch`, swap the two top items              | `[1, 2, 3] => swap => [1, 3, 2]`
| getpush     | Get and Push  |  1  |  1  | Gets and push the nth element from the top                  | `[1, 2, 3] => 2 getpush => [1, 2, 3, 1]`
| dup         | Duplicate     |  1  |  2  | Equivalent to 0 getpush. Duplicate the top of the stack     | `[1, 2, 3] => dup => [1, 2, 3, 3]`
| pop         | Pop           |  1  |  0  | Remove the top of the stack                                 | `[1, 2, 3] => pop => [1, 2]`
| pack        | Package       |  1  |  ?  | Pack the n element from the top of the stack into 1 element | `[1, 2, 3] => 2 pack => [1, pack [2, 3]]`
| unpack      | Unpack        |  0  | ?  | Unpack the input if it's a pack or a string                   | `[1, pack [2, 3]] => unpack => [1, 2, 3]`

### Input/Output

| Operator | Name   | i   |  o  | Description                                                 | Example
| :------: | :-----:| :-: | :-: | :---------------------------------------------------------: | :-----:
| input    | Input  |  0  |  1  | Asks the user for input and push it                         | `[] => input => ["?"]`
| output   | Output |  1  |  0  | Outputs the current top of the stack                        | `[1, 2, 3] => output => [1, 2] # Prints "3"`
| rand     | Random |  1  |  1  | Add a random number to the top of the stack between 1 and n | `[1, 2] => 10 rand => [1, 2, 7 or 3 or ?]`

### Utils

| Operator | Name              | i   |  o  | Description                                                | Example
| :------: | :----------------:| :-: | :-: | :--------------------------------------------------------: | :-----:
| stoi     | String to Integer |  1  |  1  | Convert a string to the integer corresponding if possible  | `[1, 2, "3"] => stoi => [1, 2, 3]`
| len      | Length            |  1  |  1  | Push the length of top of the stack                        | `[1, 2, "test"] => len => [1, 2, 3]`

**Note** : `stoi` is often used by doing `/in input stoi def`

### Debug

| Operator  | Name      |  i  |  o  | Description                           | Example
| :------:  | :--------:| :-: | :-: | :-----------------------------------: | :-----:
| variables | Variables |  0  |  0  | Prints the current variables ChainMap | `/a 1 def variables => {"a": int 1}`
| stack     | Stack     |  0  |  0  | Prints the current stack              | `[1, 2] => stack => Prints [1, 2]`

### Conditions operator

| Operator | Name                  | i   |  o  | Description                      | Example
| :------: | :--------------------:| :-: | :-: | :------------------------------: | :-----:
| leq      | Less than or Equal    |  2  |  1  | Push 1 if a <= b and 0 if b > a  | `[1, 2] => leq => [1]`
| geq      | Greater than or Equal |  2  |  1  | Push 1 if a >= b and 0 if b < a  | `[1, 2] => geq => [0]`
| eq       | Equal                 |  2  |  1  | Push 1 if a == b and 0 if a != b | `[1, 2] => eq  => [0]`

## Advanced function definition

Stacklang includes an arguments system for the function by cutting the stack in a specific way and returning a specific number of elements to the parent stack.

For example `+` takes 2 inputs and 1 output. So we could define `add` as
```Python
/add {2> + >1} # Define add with 2 inputs and one output
```

Theses are of course optional and you need to do the `enablelocalstack` command to activate this feature  
You can also disable it by doing `disablelocalstack`.   
This is cool because it sandboxes the function letting it do its thing and returning only what is needed

```Python
enablelocalstack
/f {1>
	/n def #Set n to the input
	10 {
		n
	} repeat # Pushes 10 times n to the stack
	>2 # Only returns 2 of them (the 2 top ones)
} def

3 f 2 pack output # Prints pack [3, 3]
```
