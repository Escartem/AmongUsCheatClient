# Most simple file possible to inject dll to the game
import os
import time
try:
	dll_path = os.path.abspath("AUCC-0.0.3-beta2.dll")
	print("Found the dll file")
except:
	print("Could not load the dll file")
	print("Are you sure the client is extracted correctly ?")
try:
	os.system('Injector.exe --process-name \"Among Us.exe\" --inject ' + dll_path)
	print("Successfully started the injector")
except:
	print("Could not start the Injector")
	print("Are you sure the client is extracted correctly ?")
time.sleep(5)