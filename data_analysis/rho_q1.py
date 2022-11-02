import glob
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

raw_data_dir = "../raw_data/rho_q1/"
files = glob.glob(raw_data_dir + f"q_choice_1_a0.100000_b0.700000*.csv")

data = 0;
for f in files:
    c = np.loadtxt(f, delimiter=",")
    data += c


plt.plot(data[:, 0]/len(files), data[:, 1]/len(files));
plt.plot(data[:, 0]/len(files), data[:, 2]/len(files));
plt.grid()
plt.savefig("q_choice_1_a0.100000_b0.700000.pdf")
