# %%
from do_mode_fitting import *
import scipy.stats as stats

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
PLOT=False
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

    if PLOT:
        # Pick modes
        X = np.fft.rfft(x)
        f = np.linspace(0, 48000/2, num=len(X))
        maxX = np.max(np.abs(X))
        plt.semilogx(f, 20 * np.log10(np.abs(X / maxX)), zorder=0)
        plt.scatter(freqs, 20*np.log10(peaks / maxX), marker='x', color='r', zorder=1)
        plt.ylabel('Magnitude [dB]')
        plt.xlabel('Frequency [Hz]')
        plt.xlim(100)
        plt.ylim(-60)
        plt.savefig('Figures/ModePick_ex.png')

        # Decay rate
        x_filt = adsp.filt_mode(x, freqs[0], 48000, 30)
        env = adsp.normalize(adsp.energy_envelope(x_filt, 48000, 0.005))

        start = int(np.argwhere(20 * np.log10(env) > -1)[0])
        end = int(np.argwhere(20 * np.log10(env[start:]) < -20)[0])
        slope, _, _, _, _ = stats.linregress(
            np.arange(len(env[start:end])), 20 * np.log10(env[start:end]))

        gamma = 10**(slope/20)
        tau = -1 / np.log(gamma)

        plt.figure()
        # plt.title('Decay model for mode = {0:.2f} Hz'.format(freq))
        n = np.arange(len(env))
        plt.plot(n / 48000, 20*np.log10(x_filt))
        plt.plot(n / 48000, 20*np.log10(env))
        plt.plot(n / 48000, 20*np.log10(np.exp(-1.0 * n / tau)), color='r')
        plt.xlabel('Time [s]')
        plt.ylabel('Magnitude [dB]')
        plt.ylim(-20 * 2, 5)
        plt.savefig('Figures/DecayFit_ex.png')

        plt.figure()
        plot_signals(x, y, title='')
        plt.savefig('Figures/Model_ex.png')
        
        plt.show()
        exit()



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
