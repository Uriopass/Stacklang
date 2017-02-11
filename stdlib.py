from glob import Global
import token

def dup():
	Global.stack.append(Global.stack[-1])

def pop():
	Global.stack.pop()

def repeat():
	block = Global.stack.pop()
	nb = Global.stack.pop()
	
	if block.type == "blo_ref":
		for x in range(nb):
			token.executeTokens(Global.blocks[block.value].tokens)
	else:
		print("Cannot if on something else than a block")

def exch():
	a = Global.stack.pop()
	b = Global.stack.pop()
	Global.stack.append(a)
	Global.stack.append(b)

def inputAsked():
	Global.stack.append(input())

def getpush():
	Global.stack.append(Global.stack[-Global.stack.pop()])
	
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
	if type(pack) == token.Token and pack.type == "pack":
		for x in pack.value:
			Global.stack.append(x)
	elif type(pack) == str:
		if(len(pack) == 1):
			Global.stack.append(ord(pack[0]))
		else:
			for x in pack:
				Global.stack.append(x)
	else:
		print("Cannot unpack", pack)

def output():
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
	
	
def mylen():
	value = Global.stack.pop()
	Global.stack.append(len(value))
	
functions = {"dup":			dup, 
			 "repeat":		repeat, 
			 "swap": 		swap, 
			 "getpush":		getpush, 
			 "pop":			pop, 
			 "variables":	variables, 
			 "stack":		stack, 
			 "input":		inputAsked, 
			 "output": 		output,
			 "pack":		pack, 
			 "unpack":		unpack,
			 "exch":		exch,
			 "len":			mylen}
