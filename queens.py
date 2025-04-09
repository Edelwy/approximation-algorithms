from pysat.formula import CNF
from pysat.solvers import Glucose3

PRINT_CNF = 0
PRINT_SOLUTION = 1

# Variables are presented as xij where row is the row and column the column.
# Since this cannot be the case with CNF we use a function to represent it linearly. 
def lin(n: int, row: int, column: int) -> int:
    return n*row + column + 1

def valid(n: int, row: int, column: int) -> bool:
    if row < 0 or row >= n or column < 0 or column >= n:
        return False
    return True

def one_per_clause(n: int, cnf: CNF, clauses: list) -> None:
    for row, clause_1 in enumerate(clauses):
        for clause_2 in clauses[row + 1:]:
            cnf.append([-clause_1, -clause_2])

def generate_CNF(n: int) -> CNF:
    cnf = CNF()

    # There can be only one queen per row and column.
    for row in range(n):
        row_constraints = []
        column_constraints = []
        for column in range(n):
            row_constraints.append(lin(n, row, column))
            column_constraints.append(lin(n, column, row))
        cnf.append(row_constraints) 
        cnf.append(column_constraints) 
        one_per_clause(n, cnf, row_constraints) 
        one_per_clause(n, cnf, column_constraints)
            
    # There can only be one queen per diagonal.
    for offset in range(n):
        upper_diag, lower_diag, upper_antidiag, lower_antidiag = [[] for _ in range(4)]
        for diag, antidiag in zip(range(n), range(n - 1, -1, -1)):
            if valid(n, diag, diag + offset):
                upper_diag.append(lin(n, diag, diag + offset))
            if valid(n, diag, diag - offset):
                lower_diag.append(lin(n, diag, diag - offset))
            if valid(n, antidiag, antidiag + offset):
                upper_antidiag.append(lin(n, diag, antidiag + offset))
            if valid(n, antidiag, antidiag - offset):
                lower_antidiag.append(lin(n, diag, antidiag - offset))
        one_per_clause(n, cnf, upper_diag)
        one_per_clause(n, cnf, lower_diag)
        one_per_clause(n, cnf, upper_antidiag)
        one_per_clause(n, cnf, lower_antidiag)
        
    return cnf

if __name__ == "__main__":
    cnf = generate_CNF(4)
    if PRINT_CNF: print(cnf.to_dimacs())
    if PRINT_SOLUTION: 
        solver = Glucose3()
        solver.append_formula(cnf)
        print(f"Solvable: {solver.get_model()}") if solver.solve() else print("Unslovable")
        solver.delete()