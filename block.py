class Block:
	def __init__(self, tokens, size):
		self.tokens = tokens
		self.size = size
	def appendToken(self, token):
		self.tokens.append(token)