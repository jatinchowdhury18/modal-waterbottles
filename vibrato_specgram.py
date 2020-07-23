# %%
import numpy as np
import scipy.signal as signal
from scipy.io import wavfile
import audio_dspy as adsp
import matplotlib.pyplot as plt
from matplotlib import ticker

# %%
mode_data = np.loadtxt('hydroflask_water_modes.csv', dtype=np.complex_)
sixteenth_full = mode_data[4]

freqs = sixteenth_full[:40].astype(float)
taus = sixteenth_full[40:80].astype(float) * 1.8
amps = sixteenth_full[80:]

FS = 48000
N = int(3 * FS)
# x = adsp.generate_modal_signal(amps, freqs, taus, len(amps), int(3 * FS), FS)

# %%
imp = adsp.impulse(N)
x = np.zeros(N)

for n in range(len(amps)):
    if n < 5:
        freq_osc = freqs[n] * 0.1 * np.sin(2 * np.pi * 1.5 * np.arange(N) / FS) * 0.99996**np.arange(N)
        filt = adsp.Filter(2, FS)

        for k in range(N):
            b, a = adsp.design_modal_filter(
                amps[n], freqs[n] + freq_osc[k], taus[n], FS, fs_measure=FS)
            filt.set_coefs(b, a)
            x[k] += filt.process_sample(imp[k])
    
    else:
        b, a = adsp.design_modal_filter(
            amps[n], freqs[n], taus[n], FS, fs_measure=FS)

        x += signal.lfilter(b, a, imp)

x = adsp.normalize(x)

# %%
eps = np.finfo(float).eps

f, t, Sxx = signal.spectrogram(x, FS, nperseg=1024, noverlap=1024-32, nfft=1024*4) # nperseg=2048, nfft=4096*4)
Sxx /= np.max(np.abs(Sxx))
plt.figure()
plt.pcolormesh(t, f, 20*np.log10(np.abs(Sxx) + eps), cmap='inferno', vmin=-100)
plt.colorbar()
plt.yscale('log')
plt.gca().yaxis.set_major_formatter(ticker.ScalarFormatter())
plt.ylim(60, 20000)
plt.xlim(0, 1.6)

plt.xlabel('Time [seconds]')
plt.ylabel('Frequency [Hz]')
# plt.title('Synthesized Waterbottle with Swinging Vibrato')
plt.savefig('Figures/Specgrams/Vibrato.png')

plt.show()

# %%
