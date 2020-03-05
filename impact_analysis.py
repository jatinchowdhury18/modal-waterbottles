# %%
import numpy as np
import matplotlib.pyplot as plt
import audio_dspy as adsp
from scipy.io import loadmat
from scipy.io import wavfile
from scipy import signal

# %%
def look_at_data(base_file, list, off=0):
    for n in list:
        data = loadmat(base_file + str(n) + '.mat')
        # print(data.keys())
        impact = data['indata'][off:]
        impact = np.asarray(impact).flatten()[:1000]

        plt.plot(impact, label='{}'.format(n))
    plt.legend()

# look_at_data('Impacts/stick_meas', [1, 2], off=0)
# plt.show()
# exit()

# %%
def create_impact_dict(tag, name, off=0, skip=100, take=1000):
    mdict = {'tag': tag, 'name': name, 'off': off, 'skip': skip, 'take': take}
    return mdict

def get_next_power_of_two(n):
    count = 0

    if(n and not(n & (n-1))):
        return n

    while(n != 0):
        n >>= 1
        count += 1

    return 1 << count

# %%
impacts = []
impacts.append(create_impact_dict('head_meas6', 'Head'))
impacts.append(create_impact_dict('elbow_meas1', 'Elbow', off=80))
impacts.append(create_impact_dict('gongMalletVF_damp_meas1', 'Gong_Mallet', off=20))
impacts.append(create_impact_dict('heal_meas5', 'Heel', off=150))
impacts.append(create_impact_dict('knee_meas5', 'Knee', off=170))
impacts.append(create_impact_dict('knuckles_meas2', 'Knuckles', off=105))
impacts.append(create_impact_dict('knucklesOne_meas2', 'One Knuckle', off=125))
impacts.append(create_impact_dict('palm_meas5', 'Palm', off=125))
impacts.append(create_impact_dict('shoulder_meas7', 'Shoulder', off=65))
impacts.append(create_impact_dict('toe_meas1', 'Toe', off=90))
impacts.append(create_impact_dict('fingerNails_meas2', 'Nail', off=60, skip=35, take=40))
impacts.append(create_impact_dict('brush_damp_meas1', 'Brush', off=130, skip=35, take=45))
impacts.append(create_impact_dict('dafxbottle_meas4', 'DAFx', off=160, skip=25, take=40))

# %%
def get_impact_response(impact_dict):
    data = loadmat('Impacts/{}.mat'.format(impact_dict['tag']))
    impact = data['indata'][impact_dict['off']:]
    impact = adsp.normalize(np.asarray(impact).flatten()[:impact_dict['take']] - impact[0])

    zero_cross = np.where(np.diff(np.sign(impact[impact_dict['skip']:])))[0] + impact_dict['skip'] + 1
    impact = impact[:zero_cross[0]]

    length = get_next_power_of_two(zero_cross[0])

    impact = np.concatenate((impact, np.zeros(length - len(impact))))

    plt.plot(impact, label=impact_dict['name'])
    return impact

# %%
impact_wavs = []
plt.figure()
plt.title('Waterbottle Impacts')
for i in impacts:
    impact_wavs.append(get_impact_response(i))
plt.legend()
plt.xlabel('Time [samples]')
plt.ylabel('Magnitude')
# plt.savefig('Figures/Impacts_time.png')

# %%
worN = np.logspace(1, 3.35, base=20, num=1000)
legend = []
plt.figure()
plt.title('Waterbottle Impacts Frequency Responses')
for idx, wav in enumerate(impact_wavs):
    adsp.plot_magnitude_response(wav, [1], worN=worN, fs=48000, norm=True)
    legend.append(impacts[idx]['name'])
    # wavfile.write('Impacts/Wavs/{}.wav'.format(impacts[idx]['name']), 48000, wav)

plt.legend(legend)
plt.ylim(-60)
# plt.savefig('Figures/Impacts_freq.png')

plt.show()
