from .token import Token
from .block import Block
def parseTokens(line):
	i = 0
	leng = len(line)
	
	code = []
	
	while i < leng:
		car = line[i]
		
		if car.isdigit():
			base = ""
			while i < leng and line[i].isdigit():
				base += line[i]
				i += 1
			i -= 1
			code.append(Token(int(base)))

		elif car == '"' or car == "'":
			base = car
			i += 1
			while i < leng and (line[i] != car):
				base += line[i]
				i += 1
			base += car
			
			code.append(Token(base))
			
		elif car.isalpha():
			base = ""
			while i < leng and (line[i] == "_" or line[i].isalpha() or line[i].isdigit()):
				base += line[i]
				i += 1
			i -= 1
			code.append(Token(base))
			
		elif car in Token.separators:
			pass
			
		elif car == "/" and i+1 < leng and line[i+1].isalpha():
			base = "/"
			i += 1
			while i < leng and (line[i] == "_" or line[i].isalpha() or line[i].isdigit()):
				base += line[i]
				i += 1
			i -= 1
			code.append(Token(base))
		else:
			code.append(Token(car))
		i += 1
	return code

def concatBlocks(tokens, blockref):
	i = 0
	leng = len(tokens)
	
	while i < leng:
		tok = tokens[i]
		if tok.value == "{":
			level = 1
			j = i
			while tokens[j].value != "}" or level != 0:
				j += 1
				if tokens[j].value == "{":
					level += 1
				if tokens[j].value == "}":
					level -= 1
			newToks, blockref = concatBlocks(tokens[i+1:j], blockref)
			argsize = -1
			retsize = -1
			if len(newToks) > 2 and newToks[0].type == "int" and newToks[1].type == "blo_siz":
				argsize = newToks[0].value
				newToks = newToks[2:]
			#print(newToks)
			if len(newToks) > 2 and newToks[-1].type == "int" and newToks[-2].type == "blo_siz":
				retsize = newToks[-1].value
				newToks = newToks[:-2]
			
			blockref.append(Block(newToks, argsize, retsize))
			
			tokens = tokens[:i]+[Token(len(blockref)-1, "blo_ref")]+tokens[j+1:]
			leng = len(tokens)
		i += 1	 
	return tokens, blockref