from two_less import get_solution, translate
import signal
import re

# 1 minute timeout.
TIMEOUT = 30 * 60
MAX_TIMEOUTS = 6
MAX_N = 15
MIN_N = 7

# Benchmark results.
FILENAME = "./results.txt"

# Function to append results to a file
def append_to_file(filename, result, n):
    with open(filename, 'a') as file:
        file.write(f"SOLUTION FOR {n}:\n {result}" + "\n\n")

def parse_lower_bound(n, filename):
    with open(filename, 'r') as file:
        text = file.read()
        match = re.findall(f"BEST (\d+)", text)
        if match: 
            return int(match[-1]) + 1
        return MIN_N
         
def remove_solutions(n, filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    for i, line in enumerate(lines):
        if re.match(f"SOLUTION FOR {n}", line):
            # Remove everything from this line onward.
            lines = lines[:i]
            break 

    with open(filename, 'w') as file:
        file.writelines(lines)

def format_result(prefix, result):
    k, sol = result
    formatted = " < ".join([f"{pos}: {three_tuple}" for pos, three_tuple in sol.items()])
    return f"{prefix} - BEST {k}:\n{formatted}"

def timeout_handler(signum, frame):
    raise TimeoutError("Function call timed out.")

# Function to run with a timeout.
def run_with_timeout(timeout, func, *args, **kwargs):
    signal.signal(signal.SIGALRM, timeout_handler)
    signal.alarm(timeout) 
    result = func(*args, **kwargs)
    signal.alarm(0) 
    return result

def write_result(filename, unsat, n, solution):
    if not solution:
        return n
    formatted_result = format_result(unsat, max_solution)
    append_to_file(filename, formatted_result, n)
    print(f"Written result for {n}.")
    return solution[0]

if __name__ == "__main__":
    remove_solutions(MIN_N, FILENAME)
    lower_bound = parse_lower_bound(MIN_N, FILENAME)
    timeouts = 0

    for n in range(MIN_N, MAX_N):
        max_solution = None
        prefix = "MAXIMUM"
        try:
            for k in range(lower_bound, n**3):
                result = run_with_timeout(TIMEOUT, get_solution, n, k)
                if not result:
                    break
                max_solution = (k, translate(n, result))
        except TimeoutError:
            prefix = "TIMEOUT"
            print(f"Timeout on loop {n}.")
            timeouts = timeouts + 1
            if timeouts > MAX_TIMEOUTS:
                print("Maximum number of timeouts reached.")
                break
        except KeyboardInterrupt:
            prefix = "INTERRUPT"
            break
        finally:
            lower_bound = write_result(FILENAME, prefix, n, max_solution)
            print(f"Loop {n} done.")