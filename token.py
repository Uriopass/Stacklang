from glob import Global
from stdlib import functions
import math
class Token:
	blocks = ["{", "}"]
	math_operators = ["+", "-", "*", "/", "^", "%", "!"]
	var_operators = ["=", "?"]
	separators = [" ", "\n", "\t"]
	blocksize = ["<", ">"]
	
	def __init__(self, value, type = None):
		#print(value)
		if type == None:
			self.type = Token.typeFromValue(value)
		else:
			self.type = type
		self.value = value
		if self.type == "str" and (value[0] == "'" or value[0] == '"'):
			self.value = self.value[1:-1]
		if self.type == "ref_def":
			self.value = self.value[1:]
		
	def typeFromValue(value):
		if value in Token.math_operators:
			return "mat_ope"
		if value in Token.var_operators:
			return "var_ope"
		if value in Token.separators:
			return "sep"
		if value in Token.blocks:
			return "blo"
		if value in Token.blocksize:
			return "blo_siz"
		if type(value) == int:
			return "int"
		if type(value) == str:
			if value[0] == "'" or value[0] == '"':
				return "str"
			elif value[0] == "/":
				return "ref_def"
			else:
				return "ref"
		return "unk"
		
	def __str__(self):
		return self.type +" "+ str(self.value)
	
	def __repr__(self):
		return self.__str__()
	
def execMathOperator(ope):
	val = ope
	if val == "!":
		Global.stack.append(math.factorial(Global.stack.pop()))
		return
	two = Global.stack.pop()
	one = Global.stack.pop()
	if val == "+":
		Global.stack.append(one + two)
	elif val == "-":
		Global.stack.append(one - two)
	elif val == "*":
		Global.stack.append(one * two)
	elif val == "/":
		Global.stack.append(one / two)
	elif val == "%":
		Global.stack.append(one % two)
	elif val == "^":
		Global.stack.append(one ** two)
		
def execVarOperator(operator):
	if operator == "=":
		assign = Global.stack.pop()
		ref = Global.stack.pop()
		if type(assign) == Token:
			if assign.type == "blo_ref":
				Global.variables[ref] = Token(assign.value, "func")
			elif assign.type == "pack":
				Global.variables[ref] = assign
			else:
				raise RuntimeError("Error assigning value")
		else:
			Global.variables[ref] = Token(assign)
	if operator == "?":
		no = Global.stack.pop()
		yes = Global.stack.pop()
		condition = Global.stack.pop()
		if condition == 0:
			if no.type == "blo_ref":
				executeTokens(Global.blocks[no.value].tokens)
			else:
				raise RuntimeError("Cannot if on something else than a block")
		else:
			if yes.type == "blo_ref":
				executeTokens(Global.blocks[yes.value].tokens)
			else:
				raise RuntimeError("Cannot if on something else than a block")

def executeTokens(tokens):
	for tok in tokens:
		#print(tok)
		if tok.type == "int" or tok.type == "str" or tok.type == "ref_def":
			Global.stack.append(tok.value)
		if tok.type == "blo_ref":
			Global.stack.append(tok)
		if tok.type == "pack":
			Global.stack.append(tok)
		if tok.type == "mat_ope":
			execMathOperator(tok.value)
		if tok.type == "var_ope":
			execVarOperator(tok.value)
		if tok.type == "func":
			Global.variables.maps.insert(0, {})
			
			block = Global.blocks[tok.value]
			
			if block.argsize > 0:
				Global.stack = Global.stack[-block.argsize:]
				beforeStack = Global.stack[:-block.argsize]
			else:
				beforeStack = []
			
			executeTokens(block.tokens)
			
			afterStack = list(Global.stack)
			
			Global.stack = beforeStack
			if block.retsize > 0:
				for x in afterStack[-block.retsize:]:
					Global.stack.append(x)
			elif block.retsize == -1:
				for x in afterStack:
					Global.stack.append(x)
					
			del Global.variables.maps[0]
		if tok.type == "ref":
			if tok.value in Global.variables:
				executeTokens([Global.variables[tok.value]])
			elif tok.value in functions:
				functions[tok.value]()
			else:
				raise RuntimeError("Undefined variable or function :", tok.value)
		#print(tok.type,"      \t",tok.value,"Stack:",Global.stack)