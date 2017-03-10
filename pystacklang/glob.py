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
		Global.variables = ChainMap()
		Global.stack = []
		Global.refdefstack = []
		Global.blocks = []
		Global.localstackenabled = False