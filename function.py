class Function:
    def __init__(self, node):
        if(type(node) != ExprNode):
            raise
        self.node = node
    