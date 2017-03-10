from .glob import Global

import random
import time
from .token import *

def dup():
	Global.stack.append(Global.stack[-1])

def pop():
	Global.stack.pop()

def repeat():
	block = Global.stack.pop()
	nb = Global.stack.pop()
	
	if isinstance(block, Token) and block.type == "blo_ref":
		for x in range(nb):
			executeTokens(Global.blocks[block.value].tokens)
	else:
		raise RuntimeError("Cannot if on something else than a block")

def mywhile():
	block = Global.stack.pop()
	condition = Global.stack.pop()
	if isinstance(block, Token) and isinstance(condition, Token) and block.type == "blo_ref" and condition.type == "blo_ref":
		executeTokens(Global.blocks[condition.value].tokens)
		yes = Global.stack.pop() != 0
		while yes:
			executeTokens(Global.blocks[block.value].tokens)
			
			executeTokens(Global.blocks[condition.value].tokens)
			yes = Global.stack.pop() != 0
	else:
		raise RuntimeError("Cannot while on something else than blocks")
		
def inputAsked():
	Global.stack.append(input())

def getpush():
	Global.stack.append(Global.stack[-Global.stack.pop()-1])
	
def stack():
	print(Global.stack)
	
def variables():
	print(Global.variables)

def mytime():
	Global.stack.append(int(time.time()*1000))
	
def pack():
	n = Global.stack.pop()
	pack = [Global.stack.pop() for x in range(n)]	
	pack = pack[::-1]
	Global.stack.append(Token(pack, "pack"))

def unpack():
	pack = Global.stack.pop()
	if isinstance(pack, Token) and pack.type == "pack":
		for x in pack.value:
			Global.stack.append(x)
	elif isinstance(pack, str):
		if len(pack) == 1:
			Global.stack.append(ord(pack[0]))
		else:
			for x in pack:
				Global.stack.append(x)
	else:
		raise RuntimeError("Cannot unpack", pack)

def output():
	if Global.outputToString:
		Global.optionalOutput += str(Global.stack.pop())
	else:
		print(Global.stack.pop())
	
def exch():
	ind = Global.stack.pop()
	value = Global.stack.pop()
	Global.stack.append(Global.stack[-ind])
	Global.stack[-ind-1] = value

def swap():
	value = Global.stack.pop()
	value2 = Global.stack.pop()
	Global.stack.append(value)
	Global.stack.append(value2)

def rand():
	value = Global.stack.pop()
	Global.stack.append(random.randint(0, value))
	
def mylen():
	value = Global.stack.pop()
	Global.stack.append(len(value))

def leq():
	value = Global.stack.pop()
	value2 = Global.stack.pop()
	Global.stack.append(1 if value2 <= value else 0)
	
def geq():
	value = Global.stack.pop()
	value2 = Global.stack.pop()
	Global.stack.append(1 if value2 >= value else 0)

def neq():
	value = Global.stack.pop()
	value2 = Global.stack.pop()
	Global.stack.append(1 if value != value2 else 0)

def eq():
	value = Global.stack.pop()
	value2 = Global.stack.pop()
	Global.stack.append(1 if value == value2 else 0)

def stoi():
	value = Global.stack.pop()
	Global.stack.append(int(value))

def mystr():
	value = Global.stack.pop()
	Global.stack.append(str(value))

def mydef():
	execVarOperator("=")
	
def myif():
	execVarOperator("?")

def enablelocalstack():
	Global.localstackenabled = True

def disablelocalstack():
	Global.localstackenabled = False
	

functions = {"dup":					dup, 
			 "repeat":				repeat, 
			 "swap": 				swap, 
			 "getpush":				getpush, 
			 "pop":					pop, 
			 "variables":			variables, 
			 "stack":				stack, 
			 "input":				inputAsked, 
			 "output": 				output,
			 "pack":				pack, 
			 "unpack":				unpack,
			 "exch":				exch,
			 "len":					mylen,
			 "rand":				rand,
			 "leq":					leq,
			 "geq":					geq,
			 "eq":					eq,
			 "stoi":				stoi,
			 "while":				mywhile,
			 "enablelocalstack": 	enablelocalstack,
			 "disablelocalstack": 	disablelocalstack,
			 "def":					mydef,
			 "if":					myif,
			 "time":				mytime,
			 "str":					mystr,
			 "print":				output,
			 "neq":					neq}
