#!/usr/local/bin/python3
from .glob import Global
from . import token
from . import parser
from . import stdlib

def execute(line):
	#print("Parsing", line)
	tokens = parser.parseTokens(line)
	#print("Raw parsing is", tokens)
	tokens, Global.blocks = parser.concatBlocks(tokens, Global.blocks)
	#print("Executing tokens", tokens)
	token.executeTokens(tokens)
	#print("Stack is now", Global.stack)
	#print("Variables is now", Global.variables)
	

def parseFile(filename):		
	all = []
	with open(str(filename), "r+") as file:
		for line in file:
			commentaire = line.find("#")
			if commentaire == -1:
				all += line
			else:
				all += line[:commentaire]
	execute(all)

def parseLine(line):
	all = ""
	for line in line.split("\n"):
		line += " "
		commentaire = line.find("#")
		if commentaire == -1:
			all += line
		else:
			all += line[:commentaire]
	execute(all)
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
