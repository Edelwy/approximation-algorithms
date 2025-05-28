import csv

def calculate_row_averages(input_path):
    with open(input_path, newline="") as csvfile:
        reader = csv.reader(csvfile)

        print("Average")
        for row in reader:
            numbers = list(map(float, row))
            avg = round(sum(numbers) / len(numbers), 3)
            print(f"{avg}")

if __name__ == "__main__":
    calculate_row_averages("iterations.csv")