# %%
import numpy as np
import matplotlib.pyplot as plt
import audio_dspy as adsp
from scipy.io import loadmat
from scipy.io import wavfile
from scipy import signal

__fs__ = 48000

# %%
def get_admittance_signal(mat_file, flask, tag, meas_num, type='Water'):
    data = loadmat(mat_file)

    flask = data[flask]
    water = flask[type][0][0][tag]

    if 'meas1' in water[0][0].dtype.names:
        full_signal = water[0][0][meas_num][0][0]['admittance'][0][0]
    else:
        full_signal = water[0][0]['admittance'][0][0]
    return adsp.normalize(full_signal).astype(np.float32).reshape(np.shape(full_signal)[0])

# %%
def get_fields(mat_file, flask, type='Water'):
    data = loadmat(mat_file)

    flask = data[flask]
    return flask[type][0][0].dtype

# %%
def filter_sig(x, order=4, fc=10000, fs=__fs__):
    b, a = signal.butter(order, fc, fs=fs)
    return signal.lfilter(b, a, x)

# %%
def create_mode_dict(tag, meas, name, thresh=11.5, above=500, frac_off=0.0075):
    mode_dict = {'tag': tag, 'meas': meas, 'name': name, 'thresh': thresh, 'above': above, 'frac_off': frac_off}
    return mode_dict

# %%
def get_measurements(mat_file, flask, mode_dict, type='Water'):
    for m in ['meas1', 'meas2', 'meas3']:
        x = get_admittance_signal(mat_file, flask, mode_dict['tag'], m, type=type)
        plt.figure()
        plt.plot(x)

def get_mode_freqs(x, mode_dict, fs=__fs__):
    freqs, peaks = adsp.find_freqs(x, 48000, thresh=mode_dict['thresh'],
        above=mode_dict['above'], frac_off=mode_dict['frac_off'], plot=True)
    plt.ylim(-50)
    plt.xlim(20, 24000)
    print(len(freqs))
    print(freqs)


# %%
def plot_signals(x, y, title=''):
    X = adsp.normalize(np.fft.rfft (x))
    Y = adsp.normalize(np.fft.rfft (y))
    f = np.linspace (0, 48000/2, num=len(Y))
    plt.semilogx (f, 20 * np.log10 (np.abs (X)))
    plt.semilogx (f, 20 * np.log10 (np.abs (Y)))
    plt.xlim(20, 20000)
    plt.ylim(-100)
    plt.legend(['Measured Signal', 'Modal Model'])
    plt.xlabel('Frequency [Hz]')
    plt.ylabel('Magnitude [dB]')
    plt.title(title)

# %%
def write_to_file(x, y, name):
    wavfile.write('Audio/{}_model.wav'.format(name), __fs__, adsp.normalize(y))
    wavfile.write('Audio/{}_actual.wav'.format(name), __fs__, adsp.normalize(x))
