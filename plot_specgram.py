# %%
import numpy as np
from scipy.io import wavfile
import scipy.signal as signal
import matplotlib.pyplot as plt
import audio_dspy as adsp
from matplotlib import ticker
# plt.style.use('dark_background')

# %%
def plot_specgram(x, fs, title=''):
    plt.figure()
    f, t, Zxx = signal.spectrogram(x, fs=fs, nperseg=1024, noverlap=1024-32, nfft=1024*4)
    Zxx = np.abs(adsp.normalize(Zxx)) + np.finfo(float).eps
    plt.pcolormesh(t, f, 20*np.log10(Zxx), cmap='inferno', vmin=-90)
    plt.colorbar()
    plt.xlabel('Time [sec]')
    plt.ylabel('Frequency [Hz]')
    plt.gca().yaxis.set_major_formatter(ticker.ScalarFormatter())
    plt.ylim(100, 16000)
    plt.yscale('log')
    plt.title(title)
    # plt.plot(x)

# %%
files = []
# files.append({'file': 'DAFX/full_model',  'title': 'DAFx Full Model'})
# files.append({'file': 'DAFX/full_actual', 'title': 'DAFx Full Recorded'})
files.append({'file': 'HydroFlask/full_model',  'title': 'HydroFlask Full Model'})
files.append({'file': 'HydroFlask/full_actual', 'title': 'HydroFlask Full Recorded'})

N = 60000
for f in files:
    fs, x = wavfile.read('Audio/{}.wav'.format(f['file']))
    x = adsp.normalize(x[:N])
    plot_specgram(x, fs, title=f['title'])
    plt.savefig('Figures/Specgrams/{}.png'.format(f['title']))

# %%
plt.show()
