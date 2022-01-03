import subprocess
import os

BASE_PATH = "/".join(__file__.split("/")[:-1])
SNIPPETS_PATH = BASE_PATH + "/snippets/"
OUTPUTS_PATH = BASE_PATH + "/outputs/"
INTERPRETER_PATH = BASE_PATH + "/../../build/sb"
TEST_FILES = os.listdir(SNIPPETS_PATH)

for file in TEST_FILES:
    expected_output = ""
    with open(OUTPUTS_PATH + file, "r") as f:
        expected_output = f.read()
    cmd = INTERPRETER_PATH + " " + SNIPPETS_PATH + file
    result = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE)
    output = result.stdout.decode("utf-8")
    try:
        assert output == expected_output
        print("Passed assertion for file " + file)
    except Exception as e:
        print(
            "Failed assertion for file "
            + file
            + " EXPECTED OUTPUT:\n"
            + expected_output
            + "\nACTUAL OUTPUT:\n"
            + output
        )
