class ExprNode:
	def __init__(self, value):
		self.value = value
		self.childs = []
		self.parent = None
	def isLeaf(self):
		return len(self.childs) == 0
	def insert(self, node):
		if type(node) == tuple:
			return self.insert(ExprNode(node))
		self.childs.append(node)
		node.parent = self
	def evaluate(self):
		if self.isLeaf():
			return self.value
		else:
			for node in self.childs[::-1]:
				node.evaluate()
			val = self.value[1] if self.value != None else None
			if val == None:
				self.value = [child.value for child in self.childs]
			elif val in ["+", "-", "*", "/", "^", "%"]:
				one = self.childs[1].value
				two = self.childs[0].value
				if one[0] == "var":
					one = variables[one[1]][1]
				else:
					one = one[1]
				if two[0] == "var":
					two = variables[two[1]][1]
				else:
					two = two[1]
			elif val == "@":
				one = self.childs[1].value
				two = self.childs[0].value
				print(two)
				if two[0] == "var":
					two = variables[two[1]]
				variables[self.childs[1].value[1]] = two
				self.value = variables[self.childs[1].value[1]]
			return self.value
	def __str__(self):
		if self.isLeaf():
			return str(self.value)
		return "("+ ', '.join([str(x) for x in self.childs]) + ", " + str(self.value) + ")"
	def show(self):
		if self.isLeaf():
			return str(self.value[1])
		return "("+ ', '.join([x.show() for x in self.childs]) + ", " + str(self.value) + ")"