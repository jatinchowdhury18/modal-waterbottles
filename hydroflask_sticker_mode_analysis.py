# %%
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# %%
mode_data = np.loadtxt('hydroflask_sticker_modes.csv', dtype=np.complex128)
mode_data_water = np.loadtxt('hydroflask_water_modes.csv', dtype=np.complex128)

# %%
x = np.linspace(0, 1, 5)
x_long = np.linspace(0, 1, 100)

# %%
def get_data(lamb, indL, indH):
    data = lamb(mode_data[:,indL:indH])
    data = np.append(data, np.reshape(lamb(mode_data_water[-1,indL:indH]), (1,40)), axis=0)
    data = np.flip(data, axis=0)
    return data

# mode_freqs = get_data(np.abs, None, 40)
mode_taus = get_data(np.abs, 40, 80)
tau_gains = np.exp(-1.0 / mode_taus)
t60s = np.log(0.001) / (48000 * np.log(tau_gains))

# args = np.where(mode_taus_normalized[4,:] <= -0.0006)
# print(args)

# mode_taus_normalized = mode_taus_normalized[:,args] # np.delete(mode_taus_normalized, args, axis=1)
# print(np.shape(mode_taus_normalized))

# %%
# it seems like only the damping changes
plt.figure()
for i in range(len(t60s[0,:10])):
    plt.plot(x, t60s[:,i], label='Mode #{}'.format(i+1), marker='x')

# plt.title('Mode Dampings')
plt.xlabel('Amount of Stickers')
plt.ylabel(r'$T_{60}$ [seconds]')
plt.legend()
plt.grid()
plt.savefig('Figures/StickerDamping.png')

# %%
# slopes = []
# for i in range(len(mode_taus_normalized[0,:])):
#     z = np.polyfit(x, mode_taus_normalized[:,i], deg=1)
#     slopes.append(z[0])

# print(slopes)

# %%
# super-rough curve fit
# def decay_exp(x, a, b, c):
#     return a * np.exp(-b * x) + c

# popt, pcov = curve_fit(decay_exp, x, mode_taus[:,2],
#     bounds=([0, 0, 3000], [30000,  100, 6000]))

# print(popt)
# plt.plot(x, decay_exp(x, *popt))

# tau(x) = 1.864e4 * exp(-4.333*x) + 4.3896e3

# %%
plt.show()
