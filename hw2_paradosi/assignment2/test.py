import subprocess

# Specify the command to run your program
program_command = "./hw2_2"
input_file = "tests/input.txt"
iterations = 1000   

# Run the program 100 times
for i in range(1, iterations + 1):
    print(f"Running iteration {i}...")
    
    # Use subprocess to run the program with input redirection
    try:
        subprocess.run([program_command, "8"], stdin=open(input_file), check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error during iteration {i}: {e}")
        # You can choose to handle errors as needed

print("Program has been run 100 times.")
