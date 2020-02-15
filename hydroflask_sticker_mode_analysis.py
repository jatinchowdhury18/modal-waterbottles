# %%
import numpy as np
import matplotlib.pyplot as plt

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

mode_freqs = get_data(np.abs, None, 40)
mode_taus = get_data(np.abs, 40, 80)
mode_amps = get_data(np.abs, 80, None)
mode_phases = get_data(np.angle, 80, None)

# %%
# it seems like only the damping changes
plt.figure()
for i in range(40):
    plt.plot(x, mode_taus[:,i], label='Mode #{}'.format(i+1))

plt.title('Mode Dampings')
plt.xlabel('Amount of Stickers')
plt.ylabel('Tau')

# %%
plt.show()