from flask import (
    Flask,
    g, 
    redirect, 
    render_template, 
    url_for, 
    jsonify, 
    make_response,
    Response, 
    request, 
    session
)
from flask_login.utils import login_user, login_required, current_user, logout_user
import function
import time
import json

class User:
    def __init__(self, id, username, password):
        self.id = id
        self.username = username
        self.password = password

    def __repr__(self):
        return f'<User: {self.username}>'

users = []
users.append(User(id=1, username='Agus', password='password'))
users.append(User(id=2, username='Becca', password='secret'))
users.append(User(id=3, username='Carlos', password='somethingsimple'))

app = Flask(__name__)
app.secret_key = 'somesecretkeythatonlyishouldknow'

@app.before_request
def before_request():
    g.user = None

    if 'user_id' in session:
        user = [x for x in users if x.id == session['user_id']][0]
        g.user = user
        
@app.route("/login", methods = ['GET', 'POST'])
def login():
    if request.method == 'POST':
        session.pop('user_id', None)

        username = request.form['username']
        password = request.form['password']
        next = request.args.get('next')
        
        user = [x for x in users if x.username == username][0]
        if user and user.password == password:
            session['user_id'] = user.id
            return redirect(next or url_for('index'))

        return redirect(url_for('login'))

    return render_template('login.html')

@app.route("/")
def index():
    return redirect(url_for('dashboard', measurement = 'temperature'))

@app.route("/dashboard/<measurement>")
def dashboard(measurement: str):
    if not g.user:
        return redirect(url_for('login', next = '/dashboard/' + measurement))
    
    measurements = ['temperature', 'humidity', 'pressure', 'altitude', 'moisture', 'rain']

    if measurement in measurements:
        return render_template('dashboard.html', measurement = measurement)
    
    return make_response('Not found'), 404

@app.route("/temperature")
def temperature():
    if not g.user:
        return redirect(url_for('login', next = '/temperature'))
    
    data = function.getDataMeasurements('temperature')
    return render_template('temperature.html', title='Temperature', data = data)

@app.route("/humidity")
def humidity():
    if not g.user:
        return redirect(url_for('login', next = '/humidity'))
    
    data = function.getDataMeasurements('humidity')
    return render_template('humidity.html', title='Humidity', data = data)

@app.route("/pressure")
def pressure():
    if not g.user:
        return redirect(url_for('login', next = '/pressure'))
    
    data = function.getDataMeasurements('pressure')
    return render_template('pressure.html', title='Pressure', data = data)

@app.route("/altitude")
def altitude():
    if not g.user:
        return redirect(url_for('login', next = '/altitude'))
    
    data = function.getDataMeasurements('altitude')
    return render_template('altitude.html', title='Approx Altitude', data = data)

@app.route("/soil")
def soil():
    if not g.user:
        return redirect(url_for('login', next = '/soil'))
    
    data = function.getDataMeasurements('moisture')
    return render_template('soil.html', title='Soil Moisture', data = data)

@app.route("/rain")
def rain():
    return render_template('rain.html', title='Rain')

@app.route("/tables")
def tables():
    data = function.getDataMeasurements()
    return render_template('tables.html', title='Tables', data = data)

@app.route('/data/<measurement>/<node>')
def dataTemp(measurement: str, node: int):
    timeformat = None
    if request.args.get('timeformat'):
        timeformat = request.args.get('timeformat')

    def generate_data_measurement():
        while True:
            data_measurement = function.getDataMeasurements(measurement, node, timeformat = timeformat)
            json_data = json.dumps(data_measurement)
            yield f"data:{json_data}\n\n"
            time.sleep(1)

    return Response(generate_data_measurement(), mimetype='text/event-stream')

@app.route('/datas')
def dataMes():
    def datas_measurement():
        while True:
            data_measurement = function.getDataMeasurements()
            json_data = json.dumps(data_measurement)
            yield f"data:{json_data}\n\n"
            time.sleep(1)

    return Response(datas_measurement(), mimetype='text/event-stream')


@app.route('/temperature/<node>')
def Temp(node: int):
    def generate_random_temperature():
        while True:
            temperature = function.getDataTemperature(node)
            json_data = json.dumps(temperature)
            yield f"data:{json_data}\n\n"
            time.sleep(1)

    return Response(generate_random_temperature(), mimetype='text/event-stream')

# @app.route('/hujan/<node>')
# def hujan(node: int):
#     def generate_data_kelembapan():
#         while True:
#             data = function.getDataHujan(node)
#             json_data = json.dumps(data)
#             yield f"data:{json_data}\n\n"
#             time.sleep(1)

#     return Response(generate_data_kelembapan(), mimetype='text/event-stream')

# @app.route('/moisture/<node>')
# def moisture(node: int):
#     def generate_data_moisture():
#         while True:
#             data = function.getDataMoisture(node)
#             json_data = json.dumps(data)
#             yield f"data:{json_data}\n\n"
#             time.sleep(1)

#     return Response(generate_data_moisture(), mimetype='text/event-stream')

# @app.route('/ketinggian/<node>')
# def ketinggian(node: int):
#     def generate_data_ketinggian():
#         while True:
#             data = function.getDataKetinggian(node)
#             json_data = json.dumps(data)
#             yield f"data:{json_data}\n\n"
#             time.sleep(1)

#     return Response(generate_data_ketinggian(), mimetype='text/event-stream')

# @app.route('/tekanan/<node>')
# def tekanan(node: int):
#     def generate_data_tekanan():
#         while True:
#             data = function.getDataTekanan(node)
#             json_data = json.dumps(data)
#             yield f"data:{json_data}\n\n"
#             time.sleep(1)

#     return Response(generate_data_tekanan(), mimetype='text/event-stream')
