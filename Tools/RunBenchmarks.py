import subprocess

#options based on executables menus
modes = [1]
algorithms = [1, 2, 3, 4, 5]
sizes = [100, 1000, 10000, 100000, 1000000]
array_types = [5]

executable_path = "./sort.exe" # should be in the same dir
for mode in modes:
    for algo in algorithms:
        for size in sizes:
            # Condition: Only test Bubble Sort (1) and Insertion Sort (2) up to size 100000
            if algo in [1, 2] and size > 100000:
                algo_name = "Bubble Sort" if algo == 1 else "Insertion Sort"
                print(f"Skipping {algo_name} for size {size}")
                continue
                
            for arr_type in array_types:
                print(f"Running -> Mode: {mode} | Algo: {algo} | Size: {size} | Type: {arr_type}")
                
                input_data = f"{mode}\n{algo}\n{size}\n{arr_type}\n"
                
                try:
                    # execute the program and feed it the simulated keyboard inputs
                    result = subprocess.run(
                        [executable_path],
                        input=input_data,
                        text=True,
                        capture_output=True, #captures the console output
                    )
                    
                except FileNotFoundError:
                    print(f"Error: Could not find '{executable_path}'. Make sure it's in the same folder.")
                    break
                except subprocess.TimeoutExpired:
                    print("Process timed out.")
                except Exception as e:
                    print(f"An error occurred: {e}")