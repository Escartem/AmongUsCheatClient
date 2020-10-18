import os
import time
import ctypes
import argparse

# VERSIONS #
program_ver = "1.0.0"
dll_ver = "0.0.3-beta3"
injector_ver = "0.0.4"
############

# SETUP ARGS #
parser = argparse.ArgumentParser(add_help=False)
parser.add_argument("--dev", help=argparse.SUPPRESS, action="store_true")
args = parser.parse_args()
##############

# ERRORS CODES #
errors = [
	"INJECTOR_NOT_STARTED",
	"INJECTOR_NOT_FOUND",
	"DLL_NOT_FOUND",
	"UNKNOW_ERROR"
]
################

# ERROR FIXES #
fixes = [
	"- make sure the program is extracted correctly",
	"- make sure any antivirus is blocking the client"
]
###############

os.system('cls')
ctypes.windll.kernel32.SetConsoleTitleW("AmongUsCheatClient Installer")
print("Installer version " + program_ver)
print("Client version " + dll_ver)
print("Injector version " + injector_ver)
print("Loading...\n")
# slow down a bit the program so the user can have the time to read
time.sleep(4)

try:
	# try to load the files
	if args.dev:
		dll_path = os.path.abspath("AmongUsCheatClient-22s\\Release\\AUCC.dll")
		injector_path = os.path.abspath("Injector-" + injector_ver + ".exe")
	else:
		try:
			dll_path = os.path.abspath("AUCC-" + dll_ver + ".dll")
		except:
			print("An error occured:")
			print(errors[2])
			print("Possible fixes:")
			print(fixes[0])
			print(fixes[1])
		try:
			injector_path = os.path.abspath("Injector-" + injector_ver + ".exe")
		except:
			print("An error occured:")
			print(errors[1])
			print("Possible fixes:")
			print(fixes[0])
			print(fixes[1])
	# start the injector
	try:
		# todo : use the returned values of the injector to handle them in here instead of having two separate process for error handling
		os.system('Injector-' + injector_ver + '.exe --process-name \"Among Us.exe\" --inject ' + dll_path)
	except:
		print("An error occured:")
		print(errors[0])
		print("Possible fixes :")
		print(fixes[0])
		print(fixes[1])
except:
	print("An error occured:")
	print(errors[3])

time.sleep(5)