import subprocess
import math
import os

subprocess.call(["g++", "/home/natasha/CLionProjects/threads_project_2/example.cpp","-o", "example" ,"-std=c++11", "-pthread"])
print("Compiled")
os.chdir("./cmake-build-debug")
configType = str(input("Do you want to open config file(f) or enter from console(c)? f/c "))
if configType == "c":
	f = open("./read.txt", "w")
	abs=str(input("Enter absolute value, please... "))
	rel=str(input("Enter relative value, please... "))
	xS=str(input("Enter x starting point , please... "))
	xE=str(input("Enter x ending point, please... "))
	yS=str(input("Enter y  starting point , please... "))
	yE=str(input("Enter y ending point, please... "))
	m=str(input("Enter m , please... "))
	N=str(input("Enter number of threads, please... "))

	f.write('absolute='+abs+'\n')
	f.write('relative='+rel +'\n')
	f.write('xStart='+xS+'\n')
	f.write('threads='+N + '\n')
	f.write('yStart='+yS+'\n')
	f.write('xEnd='+xE +'\n')
	f.write('yEnd='+yE+'\n')
	f.write('m='+m + '\n')
	f.close()
else:
	f = open("./read.txt", "r")
	print("It's yours configuration...")
	for line in f:
		print(line)
number = int(input("How many times do you want to run file: "))
while number > 0: 
	subprocess.call("../example")
	number -= 1

f= open("./result.txt", "r")
words = []
for line in f:
        words.append([str(n) for n in line.strip().split(' ')])
all_min = math.inf

for n in range(len(words)):
	if float(words[n][2]) < all_min:
		all_min = float(words[n][2])

print("\n" + "----- Minimum time at all -----")
print("All time: " + str(all_min))

