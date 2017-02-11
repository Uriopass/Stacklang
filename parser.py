from token import Token
from block import Block
def parseTokens(line):
	i = 0
	leng = len(line)
	
	code = []
	
	while i < leng:
		car = line[i]
		
		if "0" <= car <= "9":
			base = ""
			while i < leng and "0" <= line[i] <= "9":
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
			
		elif "a" <= car <= "z" or "A" <= car <= "Z":
			base = ""
			while i < leng and ("_" == line[i] or "0" <= line[i] <= "9" or "a" <= line[i] <= "z" or "A" <= line[i] <= "Z"):
				base += line[i]
				i += 1
			i -= 1
			code.append(Token(base))
		elif car in Token.separators:
			pass
		elif car == "/":
			if i+1 < leng and "a" <= line[i+1] <= "z" or "A" <= line[i+1] <= "Z":
				base = "/"
				i += 1
				while i < leng and ("_" == line[i] or "0" <= line[i] <= "9" or "a" <= line[i] <= "z" or "A" <= line[i] <= "Z"):
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
			retsize = 0
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