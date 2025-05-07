# Author: Nina Mislej
# Date created: 11. 4. 2025
# Whole project available at: 
# https://github.com/Edelwy/approximation-algorithms/

from pysat.formula import CNF
from pysat.solvers import Cadical195
from pysat.card import CardEnc, EncType
from functools import cmp_to_key

PRINT_CNF = 1 # Print the CNF obtained.
PRINT_SOLUTION = 1 # Print the solution obtained.
PRINT_TRANSLATION = 1 # Print the translated solution to readable output.
LINEAR_VARIABLES = 1 # Use linear variables vs. concated variables.

# Variables are presented as (a,b,c) tuples.
# Since this cannot be the case with CNF we use a function to represent it linearly. 
# To ensure uniquenes we evalute the polynomial ax^2 + bc + c at n.

# We are not worried about the position of the tuple in the chain - like in the previous naive implementation.
# It is the same as finding a k-clique, take a graph where 2 tuples are connected if 
# they are comparable. In a k-clique all elements are comparable therefore we can
# create a k-chain of ordered tuples. Meaning our only constraint is not having two
# incomparable tuple variables set to 2.
def lin(n: int, a: int, b: int, c: int) -> int:
    if LINEAR_VARIABLES:
        return a * n**2 + b * n + c + 1  
    return int(f"{a}{b}{c}")

def decode(n: int, var: int) -> tuple:
    var = var - 1
    c = var % n
    var = var // n
    b = var % n
    var = var // n
    a = var % n
    return (a, b, c)

def translate(n: int, solution: list[int] ) -> dict[tuple]:
    translation = []
    for var in solution:
        if var > 0 and var <= n**3:
            a, b, c = decode(n, var)
            translation.append((a, b, c))
    return sorted(translation, key=cmp_to_key(comparator))

# Returns true if tuple 2 is greater than tuple 1.
def greater(tuple1: tuple, tuple2: tuple) -> bool:
    a, b, c = tuple1
    x, y, z = tuple2
    if x > a and y > b:
        return True
    if x > a and z > c:
        return True
    if y > b and z > c:
        return True
    return False

def transitive(tuple1: tuple, tuple2: tuple, tuple3: tuple) -> bool:
    if incomparable(tuple1, tuple3) or incomparable(tuple1, tuple2) or incomparable(tuple2, tuple1):
        return True
    if greater(tuple1, tuple2) and greater(tuple2, tuple3) and not greater(tuple1, tuple3):
        return False
    if greater(tuple1, tuple3) and greater(tuple3, tuple2) and not greater(tuple1, tuple2):
        return False
    if greater(tuple2, tuple3) and greater(tuple3, tuple1) and not greater(tuple2, tuple1):
        return False
    return True

def incomparable(tuple1: tuple, tuple2: tuple) -> bool:
    return not greater(tuple1, tuple2) and not greater(tuple2, tuple1)

def comparator(tuple1: tuple, tuple2: tuple) -> int:
    if incomparable(tuple1, tuple2):
        raise ValueError("Cannot have incomparable elements in chain.")
    if greater(tuple1, tuple2):
        return -1
    else:
        return 1

def generate_CNF(n: int, k: int) -> CNF:
    cnf = CNF()

    # Generate all tuples only once.
    tuples = []
    for a in range(n):
        for b in range(n):
            for c in range(n):
                three_tuple = (a, b, c)
                tuples.append(three_tuple)

    # We cannot have incomparable tuples in the k-clique.
    for ix, tuple1 in enumerate(tuples):
        for tuple2 in tuples[ix + 1:]:
            if incomparable(tuple1, tuple2):
                constraint = [-lin(n, *tuple1), -lin(n, *tuple2)]
                cnf.append(constraint)

    # The above constraint justifies that there is a sequence of comparable elements.
    # The problem is it is not necessarily linear it can have cycles.
    # For this we must ensure transitivity as well. This however drastically increases the number of clauses.
    for ix1, tuple1 in enumerate(tuples):
        for ix2, tuple2 in enumerate(tuples[ix1 + 1:]):
            for tuple3 in tuples[ix2 + 1:]:
                if not transitive(tuple1, tuple2, tuple3):
                    constraint = [-lin(n, *tuple1), -lin(n, *tuple2), -lin(n, *tuple3)]
                    cnf.append(constraint)

    # We must have exactly k tuples se to true.
    # We are using an inbuild mechanism for this as it is optimized better.
    # It is using additional k couter variables to ensure there is at most and at least k variables true.
    all_vars = [lin(n, *three_tuple) for three_tuple in tuples]
    constraint = CardEnc.equals(lits=all_vars, bound=k, encoding=EncType.seqcounter)
    cnf.extend(constraint)
    return cnf

def get_solution(n: int, k: int) -> None | list[int]:
    cnf = generate_CNF(n, k)
    if PRINT_CNF: 
        print(cnf.to_dimacs())
    solver = Cadical195()
    solver.append_formula(cnf)
    solver.solve()
    return solver.get_model()

if __name__ == "__main__":
    n = 7
    k = 18

    solution = get_solution(n, k)
    if PRINT_SOLUTION: 
        print(f"Solvable: {translate(n, solution)}") if solution else print("Unslovable")