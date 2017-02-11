from collections import ChainMap
class Global:
	variables = ChainMap()
	stack = []
	globalstack = []
	blocks = []