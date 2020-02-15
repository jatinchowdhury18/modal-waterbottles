# %%
import numpy as np
from scipy.io import wavfile
import scipy.signal as signal
import matplotlib.pyplot as plt
import audio_dspy as adsp
plt.style.use('dark_background')

# %%
fs, x = wavfile.read('Audio/DAFX/full_model.wav')

# %%
def plot_specgram(x, fs, title='', log=True):
    plt.figure()
    f, t, Zxx = signal.stft(x, fs=fs)
    plt.imshow(20*np.log10(np.abs(adsp.normalize(Zxx))), cmap='inferno', origin='lower', aspect='auto', extent=[0, len(x)/48000, 0, 24000])
    plt.colorbar()
    plt.xlabel('Time [sec]')
    plt.ylabel('Frequency [Hz]')
    plt.ylim(20, 24000)
    if log:
        plt.yscale('symlog')
    plt.title(title)

# %%
files = []
files.append({'file': 'DAFX/full_model',  'title': 'DAFx Full Model'})
files.append({'file': 'DAFX/full_actual', 'title': 'DAFx Full Recorded'})
files.append({'file': 'HydroFlask/full_model',  'title': 'HydroFlask Full Model'})
files.append({'file': 'HydroFlask/full_actual', 'title': 'HydroFlask Full Recorded'})

for f in files:
    fs, x = wavfile.read('Audio/{}.wav'.format(f['file']))
    plot_specgram(x, fs, title=f['title'], log=False)
    plt.savefig('Figures/Specgrams/{}.png'.format(f['title']))

# %%
