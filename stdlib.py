from glob import Global
import token
import random

def dup():
	Global.stack.append(Global.stack[-1])

def pop():
	Global.stack.pop()

def repeat():
	block = Global.stack.pop()
	nb = Global.stack.pop()
	
	if isinstance(block, token.Token) and block.type == "blo_ref":
		for x in range(nb):
			token.executeTokens(Global.blocks[block.value].tokens)
	else:
		raise RuntimeError("Cannot if on something else than a block")

def mywhile():
	block = Global.stack.pop()
	condition = Global.stack.pop()
	if isinstance(block, token.Token) and isinstance(condition, token.Token) and block.type == "blo_ref" and condition.type == "blo_ref":
		token.executeTokens(Global.blocks[condition.value].tokens)
		yes = Global.stack.pop() != 0
		while yes:
			token.executeTokens(Global.blocks[block.value].tokens)
			
			token.executeTokens(Global.blocks[condition.value].tokens)
			yes = Global.stack.pop() != 0
	else:
		raise RuntimeError("Cannot while on something else than blocks")
		
def inputAsked():
	Global.stack.append('"'+input()+'"')

def getpush():
	Global.stack.append(Global.stack[-Global.stack.pop()-1])
	
def stack():
	print(Global.stack)
	
def variables():
	print(Global.variables)
	
def pack():
	n = Global.stack.pop()
	pack = [Global.stack.pop() for x in range(n)]	
	pack = pack[::-1]
	Global.stack.append(token.Token(pack, "pack"))

def unpack():
	pack = Global.stack.pop()
	if isinstance(pack, token.Token) and pack.type == "pack":
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
	print(Global.stack.pop())
	
def exch():
	ind = Global.stack.pop()
	value = Global.stack.pop()
	Global.stack.append(Global.stack[-ind-1])
	Global.stack[-ind-2] = value

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
	Global.stack.append(1 if value <= value2 else 0)
	
def geq():
	value = Global.stack.pop()
	value2 = Global.stack.pop()
	Global.stack.append(1 if value >= value2 else 0)
	
def eq():
	value = Global.stack.pop()
	value2 = Global.stack.pop()
	Global.stack.append(1 if value == value2 else 0)

def stoi():
	value = Global.stack.pop()
	if value[0] == "'" or value[0] == '"':
		value = value[1:-1]
	Global.stack.append(int(value))

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
			 "stoi":				stoi,
			 "while":				mywhile,
			 "enablelocalstack": 	enablelocalstack,
			 "disablelocalstack": 	disablelocalstack}
