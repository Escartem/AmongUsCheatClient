import os
import time
import argparse
parser = argparse.ArgumentParser(add_help=False)
parser.add_argument("--dev", help=argparse.SUPPRESS, action="store_true")
args = parser.parse_args()
try:
	if args.dev:
		dll_path = os.path.abspath("AmongUsCheatClient-22s\\Release\\AUCC.dll")
	else:
		dll_path = os.path.abspath("AUCC-0.0.3-beta2.dll")
	print("Found the dll file")
except:
	print("Could not load the dll file")
	print("Are you sure the client is extracted correctly ?")
try:
	print("sStarting the injector")
	os.system('Injector.exe --process-name \"Among Us.exe\" --inject ' + dll_path)
except:
	print("Could not start the Injector")
	print("Are you sure the client is extracted correctly ?")
time.sleep(5)