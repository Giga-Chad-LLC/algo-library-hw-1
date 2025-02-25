import argparse
import yaml
import subprocess
from typing import Dict, List, Union


def read_config(config_filepath: str) -> Dict[str, Union[Dict, List]]:
    """
    Reads the YAML configuration file and returns its content.

    :param config_filepath: Path to the YAML configuration file
    :return: Parsed configuration as a dictionary
    """
    with open(config_filepath, 'r') as file:
        config_data = yaml.safe_load(file)  # Parse the YAML file
    return config_data


def command_exists(command: str) -> bool:
    valgrind_check = subprocess.run(
        ["which", command],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    return valgrind_check.returncode == 0


def run_benchmarking_binary(binary_filepath: str, tree_name: str, benchmark_mode: str, memcheck_mode: bool = False) -> str:
    """
    Runs the benchmarking binary as a subprocess, passing the tree name and benchmark mode as arguments.

    :param binary_filepath: Path to the benchmarking binary
    :param tree_name: The name of the tree to pass as an argument
    :param benchmark_mode: The benchmark mode to pass as an argument
    :param memcheck_mode: if `False`, run the binary normally, else run it under valgrind and massif tool (requires valgrind installed platform!)
    :return: The output of the subprocess
    """
    try:
        if memcheck_mode is True:
            # Check if valgrind is installed
            valgrind = "valgrind"
            if not command_exists(valgrind):
                raise RuntimeError(f"Error: '{valgrind}' is not installed or not available in PATH. Memcheck mode is only available when valgrind is callable as `valgrind`")

            # Run the binary under valgrind with the massif tool
            result = subprocess.run(
                ["valgrind", "--tool=massif", f"--massif-out-file={tree_name}-{benchmark_mode}.txt", binary_filepath, tree_name, benchmark_mode],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
        else:
            # Run the binary with tree name and benchmark mode as arguments
            result = subprocess.run(
                [binary_filepath, tree_name, benchmark_mode],
                stdout=subprocess.PIPE,  # Capture stdout
                stderr=subprocess.PIPE,  # Capture stderr
                text=True  # Decode the output into a string
            )

        # Check if the process completed successfully
        if result.returncode == 0:
            # Return the standard output
            return result.stdout
        else:
            # Print the error if it failed and return stderr
            return f"Error: The binary exited with code {result.returncode}\n{result.stderr}"
    except FileNotFoundError:
        return f"Error: The binary at '{binary_filepath}' was not found."
    except Exception as e:
        return f"Error: Failed to run the binary. Exception: {str(e)}"


def main():
    # Parse command-line arguments
    parser = argparse.ArgumentParser(description="""
        Run a benchmarking binary for multiple trees specified in a YAML config. 
        The expected usage is: `executable [tree_name] [benchmark_mode]`.
        
        Example:
        ```
        python ./benchmarking.py -b /users/me/build/playground/playground_tree_algorithms  -c ../configs/benchmarking.yaml
        ```
    """)
    parser.add_argument("-b", "--binary_filepath", type=str, required=True, help="Path to the benchmarking binary to execute")
    parser.add_argument("-c", "--config_filepath", type=str, required=True, help="Path to the YAML configuration file")
    args = parser.parse_args()

    # Read the YAML configuration
    print(f"Reading configuration from: {args.config_filepath}")
    config = read_config(args.config_filepath)

    # Extract the values from the YAML configuration
    trees = config.get("tree_benchmark_config", {}).get("trees", [])
    benchmark_mode = config.get("tree_benchmark_config", {}).get("parameters", {}).get("benchmark_mode", "all")
    memcheck_mode =  config.get("tree_benchmark_config", {}).get("parameters", {}).get("memcheck_mode", False)

    print(f"Configured Trees: {trees}")
    print(f"Benchmark Mode: {benchmark_mode}")
    print(f"Memcheck Mode: {memcheck_mode}")

    # Check if benchmark execution is enabled
    if not config.get("tree_benchmark_config", {}).get("execution", {}).get("run_benchmarks", False):
        print("Benchmark execution is disabled in the configuration.")
        return

    # Iterate over the trees and run the binary for each
    for tree in trees:
        print(f"Running benchmark for tree: {tree} with mode: {benchmark_mode}")
        output = run_benchmarking_binary(args.binary_filepath, tree, benchmark_mode, memcheck_mode=memcheck_mode)
        print(f"==== Output for {tree} ====")
        print(output)


if __name__ == "__main__":
    main()
