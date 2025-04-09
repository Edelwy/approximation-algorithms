from pysat.formula import CNF
from pysat.solvers import Glucose3

PRINT_CNF = 0
PRINT_SOLUTION = 1

def generate_CNF(n: int) -> CNF:
    cnf = CNF()
    return cnf

if __name__ == "__main__":
    cnf = generate_CNF(4)
    if PRINT_CNF: print(cnf.to_dimacs())
    if PRINT_SOLUTION: 
        solver = Glucose3()
        solver.append_formula(cnf)
        print(f"Solvable: {solver.get_model()}") if solver.solve() else print("Unslovable")
        solver.delete()