from flask import Flask, request, render_template, send_from_directory, send_file
from werkzeug.utils import secure_filename
from werkzeug.datastructures import FileStorage
import os
import io
import base64

from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
from matplotlib.figure import Figure

import numpy as np
from scipy.io import wavfile
import audio_dspy as adsp

from measurements_page import render_measurements_page
from database_page import render_database

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = app.root_path + '/uploads/'

@app.route('/measurements')
def measurements():
    return render_measurements_page()

@app.route('/database')
def database():
    return render_database(app)

@app.route('/')
def my_form():
    return render_template("bespoke.html")

@app.route('/', methods=['POST'])
def do_post():
    if (len(request.files) > 0):
        meas_file = request.files['file']
        plot = make_plot_of_meas_file(meas_file)
        app.config['dry_file'] = meas_file.filename
        name_root = meas_file.filename.split('.')[0]
        app.config['synth_file'] = name_root + '_synth.wav'
        app.config['mode_file'] = name_root + '.waterbottle'
        meas_file.save(os.path.join(app.config['UPLOAD_FOLDER'], secure_filename(meas_file.filename)))
        return render_template('bespoke.html', measurement=plot, actual_audio='/uploads/'+app.config['dry_file'])
    else:
        meas_file = FileStorage(filename=os.path.join(app.config['UPLOAD_FOLDER'], secure_filename(app.config['dry_file'])))
        plot1 = make_plot_of_meas_file(meas_file.filename)
        plot2 = do_modal_analysis(meas_file.filename)
        return render_template('bespoke.html', measurement=plot1, actual_audio='/uploads/'+app.config['dry_file'],
            synthesized=plot2, synth_audio='/uploads/'+app.config['synth_file'])

    return render_template("bespoke.html")

@app.route('/download')
def do_get():
    return send_from_directory(app.config['UPLOAD_FOLDER'],
        secure_filename(app.config['mode_file']), as_attachment=True)

@app.route('/uploads/<filename>')
def uploaded_file(filename):
    return send_from_directory(app.config['UPLOAD_FOLDER'], secure_filename(filename))

def make_plot_of_meas_file(meas_file):
    # read audio file
    fs, x = wavfile.read(meas_file)
    x = adsp.normalize(x)

    # make plot
    fig = Figure()
    axis = fig.add_subplot(1, 1, 1)
    axis.set_title("Original Audio")
    axis.set_xlabel("Time [seconds]")
    axis.plot(np.arange(len(x)) / fs, x)
    
    # Convert plot to PNG image
    pngImage = io.BytesIO()
    FigureCanvas(fig).print_png(pngImage)
    
    # Encode PNG image to base64 string
    pngImageB64String = "data:image/png;base64,"
    pngImageB64String += base64.b64encode(pngImage.getvalue()).decode('utf8')
    return pngImageB64String

def do_modal_analysis(meas_file):
    # read audio file
    fs, x = wavfile.read(meas_file)
    x = adsp.normalize(x)

    # perform modal analysis
    freqs, peaks = adsp.find_freqs(x, fs, thresh=20, above=40, frac_off=0.025, plot=False)
    taus = adsp.find_decay_rates(freqs, x[:int(fs*1.5)], fs, 30, thresh=-20, eta=0.005, plot=False)
    amps = adsp.find_complex_amplitudes(freqs, taus, len(x), x, fs)
    y = adsp.generate_modal_signal(amps, freqs, taus, len(amps), len(x), fs)

    # Plot FFTs
    X = adsp.normalize(np.fft.rfft (x))
    Y = adsp.normalize(np.fft.rfft (y))
    f = np.linspace (0, 48000/2, num=len(Y))

    fig = Figure()
    axis = fig.add_subplot(1, 1, 1)
    axis.semilogx (f, 20 * np.log10 (np.abs (X)))
    axis.semilogx (f, 20 * np.log10 (np.abs (Y)))
    axis.set_xlim(20, 20000)
    axis.set_ylim(-100)
    axis.legend(['Measured Signal', 'Modal Model'])
    axis.set_xlabel('Frequency [Hz]')
    axis.set_ylabel('Magnitude [dB]')
    axis.set_title('Modal Analysis')

    # write audio to file
    wavfile.write(os.path.join(app.config['UPLOAD_FOLDER'], secure_filename(app.config['synth_file'])), fs, (2**15 * adsp.normalize(y)).astype(np.int16))

    # Convert plot to PNG image
    pngImage = io.BytesIO()
    FigureCanvas(fig).print_png(pngImage)
    
    # Encode PNG image to base64 string
    pngImageB64String = "data:image/png;base64,"
    pngImageB64String += base64.b64encode(pngImage.getvalue()).decode('utf8')

    # format mode data
    numModes = len(freqs)
    mode_data = np.zeros((1, numModes*3), dtype=np.complex128)
    mode_data[0][:numModes] = freqs
    mode_data[0][numModes:2*numModes] = taus
    mode_data[0][2*numModes:] = amps

    data_to_write = np.zeros((numModes+1, 4), dtype=float)
    data_to_write[0][0] = fs
    data_to_write[0][1] = numModes

    for i in range(numModes):
        data_to_write[i+1][0] = np.real(mode_data[0][i])
        data_to_write[i+1][1] = np.real(mode_data[0][i+numModes])
        data_to_write[i+1][2] = np.real(mode_data[0][i+2*numModes])
        data_to_write[i+1][3] = np.imag(mode_data[0][i+2*numModes])

    np.savetxt(os.path.join(app.config['UPLOAD_FOLDER'], secure_filename(app.config['mode_file'])),
        data_to_write, delimiter=',')

    return pngImageB64String

if __name__ == '__main__':
    app.run(debug=True)
