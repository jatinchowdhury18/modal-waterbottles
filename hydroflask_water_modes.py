# %%
from do_mode_fitting import *

# %%
# print(get_fields('HydroFlask.mat', 'HydroFlask'))

# %%
modes = []
modes.append(create_mode_dict('FullWater', 'meas1', 'full', thresh=10.5, above=90))
modes.append(create_mode_dict('HalfFullWater', 'meas3', 'half', thresh=17, above=90, frac_off=0.01))
modes.append(create_mode_dict('QuarterFullWater', 'meas1', 'quarter', thresh=16.5, above=90, frac_off=0.014))
modes.append(create_mode_dict('EighthFullWater', 'meas1', 'eighth', thresh=10.2, above=90, frac_off=0.007))
modes.append(create_mode_dict('SixteenthFullWater', 'meas3', 'sixteenth', thresh=26.7, above=90, frac_off=0.013))
modes.append(create_mode_dict('ThirtySeconthFullWater', 'meas2', 'thirtysecond', thresh=12, above=90, frac_off=0.007))
modes.append(create_mode_dict('EmptyWater', 'meas2', 'empty', thresh=13, above=90, frac_off=0.005))

# get_measurements('HydroFlask.mat', 'HydroFlask', modes[-1])

xs = []
for m in modes:
    xs.append(filter_sig(get_admittance_signal('HydroFlask.mat', 'HydroFlask', m['tag'], m['meas'])))

# i = 0
# get_mode_freqs(xs[i], modes[i])

# %%
mode_data = np.zeros((len(modes), 120), dtype=np.complex128)
for i, m in enumerate(modes):
    x = xs[i]
    freqs, peaks = adsp.find_freqs(x, 48000, thresh=m['thresh'], above=m['above'], frac_off=m['frac_off'], plot=False)
    taus = adsp.find_decay_rates(freqs, x[:int(48000*2)], 48000, 30, thresh=-20, eta=0.005, plot=False)
    amps = adsp.find_complex_amplitudes(freqs, taus, len(x), x, 48000)
    y = adsp.generate_modal_signal(amps, freqs, taus, len(amps), len(x), 48000)

    mode_data[i][:40] = freqs
    mode_data[i][40:80] = taus
    mode_data[i][80:] = amps

    # plt.figure()
    # plot_signals(x, y, title='HydroFlask {} Modal Model'.format(m['tag']))
    # plt.savefig('Figures/HydroFlask/{}.png'.format(m['name']))

    # write_to_file(x, y, 'HydroFlask/{}'.format(m['name']))

# print(freqs)
# print(mode_data[0][:40])
# print(taus)
# print(mode_data[0][40:80])
# print(amps)
# print(mode_data[0][80:])
np.savetxt('hydroflask_water_modes.csv', mode_data)

# %%
# m = modes[0]
# x = xs[0]
# freqs, peaks = adsp.find_freqs(x, 48000, thresh=m['thresh'], above=m['above'], frac_off=m['frac_off'], plot=True)
# plt.ylim(-50)
# plt.xlim(20, 24000)
# plt.title('Finding Modes for Full Hydroflask')

# %%
# taus = adsp.find_decay_rates(freqs, x[:int(48000*2)], 48000, 30, thresh=-20, eta=0.005, plot=True)

# %%
# amps = adsp.find_complex_amplitudes(freqs, taus, len(x), x, 48000)
# y = adsp.generate_modal_signal(amps, freqs, taus, len(amps), len(x), 48000)
# plot_signals(x, y, title='HydroFlask {} Modal Model'.format(m['tag']))

# %%
