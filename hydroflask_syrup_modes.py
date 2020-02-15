# %%
from do_mode_fitting import *

# %%
# print(get_fields('HydroFlask.mat', 'HydroFlask', type='Maple'))

# %%
modes = []
modes.append(create_mode_dict('MapleSyrup40', 'meas2', 'maple40', thresh=15, above=90, frac_off=0.018))
modes.append(create_mode_dict('MapleAll60', 'meas3', 'maple60', thresh=13, above=90, frac_off=0.0195))
modes.append(create_mode_dict('MapleAllAround', 'meas0', 'maple-all', thresh=2, above=90, frac_off=0.0207))

# get_measurements('HydroFlask.mat', 'HydroFlask', modes[-1], type='Maple')

xs = []
for m in modes:
    xs.append(filter_sig(get_admittance_signal('HydroFlask.mat', 'HydroFlask', m['tag'], m['meas'], type='Maple')))

# i = 2
# get_mode_freqs(xs[i], modes[i])

# %%
for i, m in enumerate(modes):
    x = xs[i]
    freqs, peaks = adsp.find_freqs(x, 48000, thresh=m['thresh'], above=m['above'], frac_off=m['frac_off'], plot=False)
    taus = adsp.find_decay_rates(freqs, x[:int(48000*2)], 48000, 30, thresh=-20, eta=0.005, plot=False)
    amps = adsp.find_complex_amplitudes(freqs, taus, len(x), x, 48000)
    y = adsp.generate_modal_signal(amps, freqs, taus, len(amps), len(x), 48000)

    plt.figure()
    plot_signals(x, y, title='HydroFlask {} Modal Model'.format(m['tag']))
    plt.savefig('Figures/HydroFlask_Maple/{}.png'.format(m['name']))

    write_to_file(x, y, 'HydroFlask_Maple/{}'.format(m['name']))


# %%
