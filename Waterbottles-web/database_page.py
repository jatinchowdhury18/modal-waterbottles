from pathlib import Path

def render_database(app):
    html = '''
        <h1>Waterbottle database</h1>
        '''

    uploads = Path(app.root_path + "/uploads/")
    for file in uploads.glob('*.wav'):
        if 'synth' in file.name:
            continue
            
        name_root = file.name.split('.')[0]
        synth_file = name_root + '_synth.wav'
        mode_file = name_root + '.waterbottle'

        html += f'''
            <p>{file.name}:</p>
            <audio controls>
                <source src="/uploads/{file.name}">
            </audio>
            <p>Synthesized:</p>
            <audio controls>
                <source src="/uploads/{synth_file}">
            </audio><br>
            <form method="get" action="/uploads/{mode_file}">
                <button type="submit">Download</button>
            </form>
            </br><br></br>
            '''
    
    return html
