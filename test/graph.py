import numpy as np
import matplotlib.pyplot as plt

data_arr = []
plot_arr = []

with open("test/tmp.txt", "r") as data:
    for line in data:
        data_arr.append(int(line))


fig, ax = plt.subplots(nrows=1, ncols=int(len(data_arr)/2), figsize=(16,8))

for i in range(int(len(data_arr)/2)):
    val = data_arr[2*i]
    data_arr[2*i] = 1
    data_arr[2*i+1] = data_arr[2*i+1] / val

for i in range(int(len(data_arr)/2)):
    plot_arr.append([data_arr[2*i], data_arr[2*i+1]])

for i in range(int(len(data_arr)/2)):
    ax[i].bar(["set","avl"], plot_arr[i])
    ax[i].set(xlabel=i+1)
    ax[i].set_yticks([1])


plt.show()
