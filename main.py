# Most simple file possible to inject dll to the game
import os
#import time
dll_path = os.path.abspath("AUCC-0.0.2.dll")
os.system('start Injector.exe --process-name \"Among Us.exe\" --inject ' + dll_path)
#time.sleep(5)