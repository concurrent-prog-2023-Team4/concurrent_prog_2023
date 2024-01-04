import subprocess

# Specify the command to run your program and command-line arguments
program_command = "./hw3_4"
input_file = "tests/input.txt"
parameter = "3"
iterations = 100

# Run the program 100 times
for i in range(1, iterations + 1):
    print(f"Running iteration {i}...")
    
    # Use subprocess to run the program with command-line arguments and input redirection
    try:
        subprocess.run([program_command, input_file, parameter], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error during iteration {i}: {e}")
        # You can choose to handle errors as needed

print("Program has been run 100 times.")
