class Block:
	def __init__(self, tokens, argsize, retsize):
		self.tokens = tokens
		self.argsize = argsize
		self.retsize = retsize
	def appendToken(self, token):
		self.tokens.append(token)