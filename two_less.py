from pysat.formula import CNF
from pysat.solvers import Glucose3

PRINT_CNF = 0 # Print the CNF obtained.
PRINT_SOLUTION = 0 # Print the solution obtained.
PRINT_TRANSLATION = 1 # Print the translated solution to readable output.
LINEAR_VARIABLES = 1 # Use linear variables vs. concated variables.

# Variables are presented as (i,a,b,c) where (a,b,c) is the touple and i is the position in the chain.
# Since this cannot be the case with CNF we use a function to represent it linearly. 
# To ensure uniquenes we evalute the polynomial ix^3 + ax^2 + bc + c at n.
def lin(n: int, a: int, b: int, c: int, i: int) -> int:
    if LINEAR_VARIABLES:
        return i * n**3 + a * n**2 + b * n + c + 1  
    return int(f"{a}{b}{c}{i}")

def decode(n, var):
    var = var - 1
    c = var % n
    var = var // n
    b = var % n
    var = var // n
    a = var % n
    var = var // n
    i = var
    return (a, b, c, i)

def translate(n: int, solution: list[int] ) -> dict[tuple]:
    translation = {}
    for var in solution:
        if var > 0:
            a, b, c, i = decode(n, var)
            translation[i] = (a, b, c)
    return translation

# Returns true if tuple 2 is greater than tuple 1.
def greater(tuple1: tuple, tuple2: tuple):
    a, b, c = tuple1
    x, y, z = tuple2
    if x > a and y > b:
        return True
    if x > a and z > c:
        return True
    if y > b and z > c:
        return True
    return False

# Constraints if tuple 2 is not greater than tuple 1.
def not_consecutive(cnf: CNF, n: int, k: int, tuple1: tuple, tuple2: tuple) -> list:
    for i in range(k):
        for j in range(i):
            cnf.append([-lin(n, *tuple1, j), -lin(n, *tuple2, i)])

def one_per_clause(cnf: CNF, clauses: list) -> None:
    for row, clause1 in enumerate(clauses):
        for clause2 in clauses[row + 1:]:
            cnf.append([-clause1, -clause2])

def generate_CNF(n: int, k: int) -> CNF:
    cnf = CNF()

    # Generate all tuples only once.
    tuples = []
    for a in range(n):
        for b in range(n):
            for c in range(n):
                three_tuple = (a, b, c)
                tuples.append(three_tuple)

    # There can be exactly one tuple per position in chain.
    for i in range(k):
        chain_constraint = []
        for three_tuple in tuples:
            chain_constraint.append(lin(n, *three_tuple, i))
        cnf.append(chain_constraint)
        one_per_clause(cnf, chain_constraint)

    # One tuple can be in at most one position.
    for three_tuple in tuples:
        tuple_constraint = []
        for i in range(k):
            tuple_constraint.append(lin(n, *three_tuple, i))
        one_per_clause(cnf, tuple_constraint)

    # If tuple B is not greater than tuple A then we cannot have both (A,i) and (B,i+1) be true.
    # First element of the pair is the i-th position and the second element the i+1-th position.
    for tuple1 in tuples:
        for tuple2 in tuples:
            if not greater(tuple1, tuple2):
                not_consecutive(cnf, n, k, tuple1, tuple2)
    return cnf

def find_max(n):
    max_solution = None
    for k in range(n, n**3):
        solution = get_solution(n, k)
        if not solution:
            break
        max_solution = solution
    print(translate(n, max_solution))

def get_solution(n, k):
    cnf = generate_CNF(n, k)
    if PRINT_CNF: 
        print(cnf.to_dimacs())
    solver = Glucose3()
    solver.append_formula(cnf)
    solver.solve()
    return solver.get_model()

if __name__ == "__main__":
    n = 8
    find_max(n)
    