import numpy as np
import scipy.signal as signal
from scipy.io import wavfile
import matplotlib.pyplot as plt
from matplotlib import ticker

fs, x = wavfile.read('Audio/Hydroflask/third_full_vibrato.wav')

x = x[:,0] / 2**15

f, t, Sxx = signal.spectrogram(x, fs, nperseg=2048, nfft=4096*8)
Sxx /= np.max(np.abs(Sxx))
plt.pcolormesh(t, f, 20*np.log10(np.abs(Sxx)), cmap='inferno', vmin=-100)
plt.colorbar()
plt.yscale('log')
plt.gca().yaxis.set_major_formatter(ticker.ScalarFormatter())
plt.ylim(60, 20000)
plt.xlim(0, 3.2)

plt.xlabel('Time [seconds]')
plt.ylabel('Frequency [Hz]')
# plt.title('Synthesized Waterbottle with Swinging Vibrato')
plt.savefig('Figures/Specgrams/Vibrato.png')

plt.show()
