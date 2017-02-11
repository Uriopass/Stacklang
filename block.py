class Block:
	def __init__(self, tokens):
		self.tokens = tokens
	def appendToken(self, token):
		self.tokens.append(token)