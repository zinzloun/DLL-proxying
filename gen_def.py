import pefile
import sys
import os.path
import argparse
import subprocess
from pathlib import Path

def prGreen(skk): print("\033[92m {}\033[00m" .format(skk))
def prRed(skk): print("\033[91m {}\033[00m" .format(skk))



# Create the parser
parser = argparse.ArgumentParser(description = "Pass the path to dll rom which to export the functions; the path to C payload source file to compile")
# Add an argument
parser.add_argument('--dll', type=str, required=True)
parser.add_argument('--payload', type=str, required=True)

# Parse the argument
args = parser.parse_args()

if not os.path.exists(args.dll) or not os.path.exists(args.payload):
    prRed("Cannot find dll or source file, please check the provided parameters")
    exit(1)

dll = pefile.PE(args.dll)
dll_basename = os.path.splitext(args.dll)[0]
defF = dll_basename + ".def"

if os.path.exists(defF): os.remove(defF)

f = open(defF, "a")  # append mode

f.write("LIBRARY " + dll_basename)

f.write("\nEXPORTS\n")
for export in dll.DIRECTORY_ENTRY_EXPORT.symbols:
    if export.name:
        f.write('{}={}.{} @{}'.format(export.name.decode(), dll_basename, export.name.decode(), export.ordinal) + '\n')

dllOut = str(Path(args.payload).with_suffix(".dll"))
  
# Define the command to compile the DLL
command = ["gcc", "-shared", "-o", dllOut, args.payload, defF, "-s", "-lws2_32"]
prGreen("File def created")
print("Now execute the following command to compile the payload as DLL:\n")
prGreen(' '.join(command))

