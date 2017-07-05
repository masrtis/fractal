import matplotlib.pyplot as plt
import numpy as np
import time

baseDirectory = "A:/Code/Mandelbrot/"

file = baseDirectory + "rawArrays/1.txt"
#Read frame variables
with open(file, 'r') as f:
    textValues = f.readline().split(',')
(xMin,xMax,yMin,yMax,iterationCap) = [float(x) for x in textValues]

#Read csv data
print("Reading in numpy array\n")
start = time.time()
nums = np.genfromtxt(file, delimiter=",", skip_header=1)
end = time.time()
print((end - start)*1000,"ms")


print("Generating plot")
fig, ax = plt.subplots()
ax.imshow(nums,extent=[xMin,xMax,yMin,yMax],cmap='gist_rainbow')

plt.show()