import sys
import token
import parser

from glob import Global

import stdlib

if len(sys.argv) == 0:
	print("no argv lel")
	exit()

def execute(line):
	#print("Parsing", line)
	tokens = parser.parseTokens(line)
	#print("Raw parsing is", tokens)
	tokens, Global.blocks = parser.concatBlocks(tokens, Global.blocks)
	#print("Executing tokens", tokens)
	token.executeTokens(tokens)
	#print("Stack is now", Global.stack)
	#print("Variables is now", Global.variables)
	

		
all = []
with open(str(sys.argv[1]), "r+") as file:
	for line in file:
		commentaire = line.find("#")
		if commentaire == -1:
			all += line
		else:
			all += line[:commentaire]
execute(all)
'''
def buildRPNTree(root): #Rever polish notation tree
		while canConsume():
				cur = consumeToken()
				if cur[0] == "rel":
						root.insert(cur)
						#print(root.childs)
				elif cur[0] == "var":
						root.insert(cur)
				elif cur[0] == "ope":
						#print("I see", cur)
						last2 = [root.childs.pop(), root.childs.pop()]
						#print("Last 2 are", last2)
						expr = ExprNode(cur)
						expr.insert(last2[0])
						expr.insert(last2[1])
						root.insert(expr)
				elif cur[0] == "spe":
						last2 = [root.childs.pop(), root.childs.pop()]

						expr = ExprNode(cur)
						expr.insert(last2[0])
						expr.insert(last2[1])
						root.insert(expr)
		'''
'''
def djistraConv():
		operatorsPrec = {"+":1, "-":1, "*":2, "/":2, "%": 2, "^": 3, "@":4}
		outputS = []
		operS = []
		for tok in code:
				if tok[0] == "rel":
						outputS.append(tok)
				elif tok[0] == "var":
						outputS.append(tok)
				elif tok[1] in operatorsPrec:
						while(len(operS) > 0 and operS[-1][1] in operatorsPrec and operatorsPrec[operS[-1][1]] >= operatorsPrec[tok[1]]):
								outputS.append(operS.pop())
						operS.append(tok)
				elif tok[1] == "(":
						operS.append(tok)
				elif tok[1] == ")":
						top = operS.pop()
						while top[1] != "(":
								outputS.append(top)
								top = operS.pop()
		while len(operS) > 0:
				top = operS.pop()
				if top[1] == "(" or top[1] == ")":
						print("Mismatch")
				else:
						outputS.append(top)
		return outputS
'''
