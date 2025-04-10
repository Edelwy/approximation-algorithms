from two_less import get_solution, translate
import signal
import os
import time

# 5 minute timeout.
TIMEOUT = 10
MAX_N = 8
MIN_N = 7

# Function to append results to a file
def append_to_file(filename, result, n):
    with open(filename, 'a') as file:
        file.write(f"SOLUTION FOR {n}:\n {result}" + "\n\n")

def format_result(unsat, result):
    k, sol = result
    formatted = " < ".join([f"{pos}: {three_tuple}" for pos, three_tuple in sol.items()])
    if unsat:
        return f"TIMEOUT - BEST {k}:\n{formatted}"
    return f"MAXIMUM - BEST {k}:\n{formatted}"

def timeout_handler(signum, frame):
    raise TimeoutError("Function call timed out.")

# Function to run with a timeout.
def run_with_timeout(timeout, func, *args, **kwargs):
    signal.signal(signal.SIGALRM, timeout_handler)
    signal.alarm(timeout) 
    result = func(*args, **kwargs)
    signal.alarm(0) 
    return result


if __name__ == "__main__":
    filename="./results.txt"
    for n in range(MIN_N, MAX_N):
        max_solution = None
        unsat = False
        try:
            for k in range(n, n**3):
                result = run_with_timeout(TIMEOUT, get_solution, n, k)
                if not result:
                    break
                max_solution = (k, translate(n, result))
        except TimeoutError:
            unsat = True
        if max_solution:
            formatted_result = format_result(unsat, max_solution)
            append_to_file(filename, formatted_result, n)
        if unsat:
            break