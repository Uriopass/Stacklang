#!/bin/bash
stack build &&
for f in ./examples/* ; do
	echo $f
	stack exec haskell-stacklang-exe < $f
done
