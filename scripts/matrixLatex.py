import sys
import numpy as np
import sympy as sp

M = np.loadtxt(sys.argv[1], delimiter=None)
#print cleanly with sympy
sp_matrix = sp.Matrix(M)
sp.pprint(sp_matrix)
# skip a line after printing
print()
