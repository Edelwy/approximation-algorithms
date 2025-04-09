from pysat.formula import CNF
from pysat.solvers import Glucose3

PRINT_CNF = 0
PRINT_SOLUTION = 1

# Variables are presented as xij where i is the row and j the column.
# Since this cannot be the case with CNF we use a function to represent it linearly. 
def lin(n, i, j):
    return n*i + j + 1

def valid(n, i, j):
    if i < 0 or i >= n or j < 0 or j >= n:
        return False
    return True

def move(pair, i, j):
    return (pair[0] + i, pair[1] + j)

def one_per_clause(n, cnf, clauses):
    for i, clause_1 in enumerate(clauses):
        for clause_2 in clauses[i+1:]:
            cnf.append([-clause_1, -clause_2])

def generate_CNF(n: int) -> CNF:
    cnf = CNF()

    # There can be only one queen per row and column.
    for i in range(n):
        row_constraints = []
        column_constraints = []
        for j in range(n):
            row_constraints.append(lin(n, i, j))
            column_constraints.append(lin(n, j, i))
        cnf.append(row_constraints)
        cnf.append(column_constraints)
        one_per_clause(n, cnf, row_constraints)
        one_per_clause(n, cnf, column_constraints)
            
    # There can only be one queen per diagonal.
    for i in range(n):
        for j in range(n):
            start = lin(n, i, j)
            up_left_con, up_right_con, down_left_con, down_right_con = [[start] for _ in range(4)]
            up_left = up_right = down_left = down_right = (i, j)
            out_of_frame = False
            while not out_of_frame:
                out_of_frame = True
                up_left = move(up_left, -1, -1)
                up_right = move(up_right, -1, 1)
                down_left = move(down_left, 1, -1)
                down_right = move(down_right, 1, 1)

                if valid(n, *up_left):
                    up_left_con.append(lin(n, *up_left))
                    out_of_frame = False
                if valid(n, *up_right):
                    up_right_con.append(lin(n, *up_right))
                    out_of_frame = False
                if valid(n, *down_left):
                    down_left_con.append(lin(n, *down_left))
                    out_of_frame = False
                if valid(n, *down_right):
                    down_right_con.append(lin(n, *down_right))
                    out_of_frame = False
            one_per_clause(n, cnf, up_left_con)
            one_per_clause(n, cnf, up_right_con)
            one_per_clause(n, cnf, down_left_con)
            one_per_clause(n, cnf, down_right_con)

    return cnf

if __name__ == "__main__":
    cnf = generate_CNF(3)
    if PRINT_CNF: print(cnf.to_dimacs())
    if PRINT_SOLUTION: 
        solver = Glucose3()
        solver.append_formula(cnf)
        print(f"Solvable: {solver.get_model()}") if solver.solve() else print("Unslovable")
        solver.delete()