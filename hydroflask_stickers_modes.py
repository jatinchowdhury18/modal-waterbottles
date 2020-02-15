# %%
from do_mode_fitting import *

# %%
# print(get_fields('HydroFlask.mat', 'HydroFlask', type='Sticker'))

# %%
modes = []
modes.append(create_mode_dict('AllSticker', 'meas2', 'sticker-all', thresh=6, above=90, frac_off=0.005))
modes.append(create_mode_dict('LessSticker1', 'meas1', 'sticker1', thresh=3.4, above=90, frac_off=0.005))
modes.append(create_mode_dict('LessSticker2', 'meas3', 'sticker2', thresh=2.2, above=90, frac_off=0.01))
modes.append(create_mode_dict('LessSticker3', 'meas3', 'sticker3', thresh=16.7, above=90, frac_off=0.01))

# get_measurements('HydroFlask.mat', 'HydroFlask', modes[-1], type='Sticker')

xs = []
for m in modes:
    xs.append(filter_sig(get_admittance_signal('HydroFlask.mat', 'HydroFlask', m['tag'], m['meas'], type='Sticker')))

# i = 3
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
    # plt.savefig('Figures/HydroFlask_Stickers/{}.png'.format(m['name']))

    # write_to_file(x, y, 'HydroFlask_Stickers/{}'.format(m['name']))

np.savetxt('hydroflask_sticker_modes.csv', mode_data)

# %%
