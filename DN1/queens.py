# Author: Nina Mislej
# Date created: 9. 4. 2025
# Whole project available at: 
# https://github.com/Edelwy/approximation-algorithms/

from pysat.formula import CNF
from pysat.solvers import Glucose3

PRINT_CNF = 1 # Print the CNF obtained.
PRINT_SOLUTION = 1 # Print the solution obtained.
LINEAR_VARIABLES = 1 # Use linear variables vs. concated variables.

# Variables are presented as (i,j) where i is the row and j the column.
# Since this cannot be the case with CNF we use a function to represent it linearly. 
def lin(n: int, row: int, column: int) -> int:
    if LINEAR_VARIABLES:
        return n*row + column + 1
    return int(f"{row}{column}")

def valid(n: int, row: int, column: int) -> bool:
    if row < 0 or row >= n or column < 0 or column >= n:
        return False
    return True

def one_per_clause(cnf: CNF, clauses: list) -> None:
    for row, clause1 in enumerate(clauses):
        for clause2 in clauses[row + 1:]:
            cnf.append([-clause1, -clause2])

def generate_CNF(n: int) -> CNF:
    cnf = CNF()

    # There can be exactly one queen per row and column.
    for row in range(n):
        row_constraints = []
        column_constraints = []
        for column in range(n):
            row_constraints.append(lin(n, row, column))
            column_constraints.append(lin(n, column, row))
        cnf.append(row_constraints) 
        cnf.append(column_constraints) 
        one_per_clause(cnf, row_constraints) 
        one_per_clause(cnf, column_constraints)
            
    # There can be at most one queen per diagonal.
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
        one_per_clause(cnf, upper_diag)
        one_per_clause(cnf, lower_diag)
        one_per_clause(cnf, upper_antidiag)
        one_per_clause(cnf, lower_antidiag)
        
    return cnf

def get_solution(n: int) -> None | list[int]:
    cnf = generate_CNF(n)
    if PRINT_CNF: 
        print(cnf.to_dimacs())
    solver = Glucose3()
    solver.append_formula(cnf)
    solver.solve()
    return solver.get_model()

if __name__ == "__main__":
    chessboard_size = 4
    solution = get_solution(chessboard_size)
    if PRINT_SOLUTION: 
        print(f"Solvable: {solution}") if solution else print("Unslovable")