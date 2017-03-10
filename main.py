import sys
sys.path.append('../')
import pystacklang
if len(sys.argv) >= 2:
	pystacklang.language.parseFile(sys.argv[1])
