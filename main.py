import sys
sys.path.append('../')
import Stacklang
if len(sys.argv) >= 2:
	Stacklang.language.parseFile(sys.argv[1])