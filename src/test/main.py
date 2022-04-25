import subprocess
import os

BASE_PATH = "/".join(__file__.split("/")[:-1])
SNIPPETS_PATH = BASE_PATH + "/snippets/"
OUTPUTS_PATH = BASE_PATH + "/outputs/"
INTERPRETER_PATH = BASE_PATH + "/../../build/sb"
TEST_FILES = os.listdir(SNIPPETS_PATH)

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

for file in TEST_FILES:
    expected_output = ""
    with open(OUTPUTS_PATH + file, "r") as f:
        expected_output = f.read()
    cmd = INTERPRETER_PATH + " " + SNIPPETS_PATH + file
    result = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output = result.stderr.decode("utf-8")
    output += result.stdout.decode("utf-8")
    try:
        assert output == expected_output
        print(f" - Start Test for {file} - ")
        print(f" --- ACTUAL OUTPUT --- ")
        print(output)
        print(f" --- EXPECTED OUTPUT ---")
        print(expected_output)
        print(" --------------------- ")
        print(f"{bcolors.OKGREEN}Passed{bcolors.ENDC} assertion for file {file}")
    except Exception as e:
        print(
            f"{bcolors.FAIL}Failed{bcolors.ENDC} assertion for file "
            + file
            + "\nEXPECTED OUTPUT:\n"
            + expected_output
            + "\nACTUAL OUTPUT:\n"
            + output
        )
