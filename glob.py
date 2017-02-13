from collections import ChainMap
class Global:
	variables = ChainMap()
	stack = []
	refdefstack = []
	blocks = []
	localstackenabled = False
	optionalOutput = ""
	outputToString = False
	
	def reset():
		variables = ChainMap()
		stack = []
		refdefstack = []
		blocks = []
		localstackenabled = False