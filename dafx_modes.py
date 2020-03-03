# %%
from do_mode_fitting import *

# %%
# print(get_fields('DAFX.mat', 'DAFXFlask'))

# %%
modes = []
modes.append(create_mode_dict('FullWater', 'meas1', 'full'))
modes.append(create_mode_dict('HalfFullWater', 'meas3', 'half', thresh=16.5))
modes.append(create_mode_dict('QuarterFullWater', 'meas3', 'quarter', thresh=6, frac_off=0.005))
modes.append(create_mode_dict('EighthFullWater', 'meas3', 'eighth', thresh=16.4, frac_off=0.01))
modes.append(create_mode_dict('NoWater', 'meas3', 'empty', above=900, frac_off=0.005))

# get_measurements('DAFX.mat', 'DAFXFlask', modes[-1])

# %%
xs = []
for m in modes:
    xs.append(filter_sig(get_admittance_signal('DAFX.mat', 'DAFXFlask', m['tag'], m['meas'])))

# %%
# i = 3
# get_mode_freqs(xs[i], modes[i])

# %%
numModes = 25
mode_data = np.zeros((len(modes), numModes*3), dtype=np.complex128)
for i, m in enumerate(modes):
    x = xs[i]
    freqs, peaks = adsp.find_freqs(x, 48000, thresh=m['thresh'], above=m['above'], frac_off=m['frac_off'], plot=False)
    taus = adsp.find_decay_rates(freqs, x[:int(48000*1.5)], 48000, 30, thresh=-20, eta=0.005, plot=False)
    amps = adsp.find_complex_amplitudes(freqs, taus, len(x), x, 48000)
    y = adsp.generate_modal_signal(amps, freqs, taus, len(amps), len(x), 48000)

    mode_data[i][:numModes] = freqs
    mode_data[i][numModes:2*numModes] = taus
    mode_data[i][2*numModes:] = amps

    # plt.figure()
    # plot_signals(x, y, title='DAFX {} Modal Model'.format(m['tag']))
    # plt.savefig('Figures/DAFX/{}.png'.format(m['name']))

    # write_to_file(x, y, 'DAFX/{}'.format(m['name']))

# %%
fs = 48000

data_to_write = np.zeros((numModes+1, 4), dtype=float)
data_to_write[0][0] = fs
data_to_write[0][1] = numModes

for i in range(numModes):
    data_to_write[i+1][0] = np.real(mode_data[0][i])
    data_to_write[i+1][1] = np.real(mode_data[0][i+numModes])
    data_to_write[i+1][2] = np.real(mode_data[0][i+2*numModes])
    data_to_write[i+1][3] = np.imag(mode_data[0][i+2*numModes])

np.savetxt('dafx.waterbottle', data_to_write, delimiter=',')
