# %%
import numpy as np
import matplotlib.pyplot as plt

# %%
mode_data = np.loadtxt('hydroflask_water_modes.csv', dtype=np.complex128)

# %%
x = np.linspace(0, 1, 7)
x_long = np.linspace(0, 1, 100)

# %%
# plt.figure()
# mode_freqs = np.abs(mode_data[:,0])
# plt.plot(x, mode_freqs)

# z_freq = np.polyfit(x, mode_freqs, 3)
# p_freq = np.poly1d(z_freq)
# plt.plot(x_long, p_freq(x_long))
# print('Freq function:')
# print(p_freq)

# plt.title('First mode frequency vs water amount')
# plt.ylabel('Frequency [Hz]')
# plt.xlabel('Water as fraction of bottle volume')
# plt.xticks(x, ['0', '1/32', '1/16', '1/8', '1/4', '1/2', '1'])
# plt.legend(['Measured', 'Modelled'])

# %%
plt.figure()
for i in range(2):
    mode_damps = np.abs(mode_data[:,40+i])
    plt.plot(x, mode_damps, label='Mode #{}'.format(i+1))

mode_damps = np.mean(np.abs(mode_data[:,40:42]), axis=1)
# plt.plot(x, mode_damps, label='Avg')

z_damp = np.polyfit(x, mode_damps, 4)
p_damp = np.poly1d(z_damp)
plt.plot(x_long, p_damp(x_long), label='Model')
print('Damp function:')
print(p_damp)

plt.legend()
plt.title('First 2 modes damping vs water amount')
plt.ylabel('Tau [gain/sample]')
plt.xlabel('Water as fraction of bottle volume')
plt.xticks(x, ['0', '1/32', '1/16', '1/8', '1/4', '1/2', '1'])
# %%
plt.show()