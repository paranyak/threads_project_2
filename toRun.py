import subprocess
import math
subprocess.call(["g++", "/home/natasha/CLionProjects/threads_project_2/example.cpp","-o", "example" ,"-std=c++11", "-pthread"])
print("Compiled")
number = int(input("How many times do you want to run file: "))
while number > 0: 
	subprocess.call("./example")
	number -= 1

f= open("./result.txt", "r")
words = []
for line in f:
        words.append([str(n) for n in line.strip().split(' ')])
all_min = math.inf
for n in range(len(words)):
	if float(words[n][2]) < all_min:
		all_min = float(words[n][2])

print("----- Minimum time at all -----")
print("All time: " + str(all_min))

