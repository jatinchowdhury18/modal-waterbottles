# %%
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# %%
mode_data = np.loadtxt('hydroflask_water_modes.csv', dtype=np.complex128)

# %%
x = np.linspace(0, 1, 7)
x_long = np.linspace(0, 1, 100)

def rev_sigmoid(x, a, b, c, d):
    return c * (1.0 - (1.0 / (1.0 + np.exp(-b*(x-a))))) + d

# %%
plt.figure()
mode_freqs = np.flip(np.abs(mode_data[:,0]))
plt.scatter(x, mode_freqs, marker='x', color='r', label='Measured')

# z_freq = np.polyfit(x, mode_freqs, 3)
# p_freq = np.poly1d(z_freq)
# plt.plot(x_long, p_freq(x_long), label='Modelled')
popt, pcov = curve_fit(rev_sigmoid, x, mode_freqs, bounds=([0, 1, -100, -200], [1, 100, 100, 200]))
plt.plot(x_long, rev_sigmoid(x_long, *popt), label='Modelled')
# print('Freq function:')
# print(p_freq)
print(popt)

# plt.title('First mode frequency vs water amount')
plt.ylabel('Frequency [Hz]')
plt.xlabel('Water as fraction of bottle volume')
plt.xticks(x, ['0', '1/32', '1/16', '1/8', '1/4', '1/2', '1'])
plt.legend()
plt.grid()
plt.savefig('Figures/Water_Freq.png')

# %%
# plt.figure()
# colors = ['r', 'k']
# for i in range(2):
#     mode_damps = np.abs(mode_data[:,40+i])
#     tau_gains = np.exp(-1.0 / mode_damps)
#     t60s = np.log(0.001) / (48000 * np.log(tau_gains))
#     plt.scatter(x, t60s, label='Mode #{}'.format(i+1), marker='x', color=colors[i])

# mode_damps1 = np.mean(np.abs(mode_data[:,40:41]), axis=1)
# mode_damps2 = np.mean(np.abs(mode_data[:,41:42]), axis=1)
# mode_damps_avg = np.mean(np.abs(mode_data[:,40:42]), axis=1)
# # # plt.plot(x, mode_damps, label='Avg')

# def model_mode_damp(mode_damps, label):
#     z_damp = np.polyfit(x, mode_damps, 4)
#     p_damp = np.poly1d(z_damp)
#     tau_gains = np.exp(-1.0 / p_damp(x_long))
#     t60s = np.log(0.001) / (48000 * np.log(tau_gains))
#     plt.plot(x_long, t60s, label=label)
#     # print('Damp function:')
#     # print(p_damp)

# model_mode_damp(mode_damps1, 'Mode 1 Model')
# model_mode_damp(mode_damps2, 'Mode 2 Model')
# model_mode_damp(mode_damps_avg, 'Avg Model')

# plt.legend()
# # plt.title('First 2 modes damping vs water amount')
# plt.ylabel(r'$T_{60}$ [seconds]')
# plt.xlabel('Water as fraction of bottle volume')
# plt.xticks(x, ['0', '1/32', '1/16', '1/8', '1/4', '1/2', '1'])
# plt.grid()
# plt.savefig('Figures/Water_Damping.png')

# %%
plt.show()