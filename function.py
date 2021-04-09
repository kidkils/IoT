from influxdb import InfluxDBClient
from datetime import datetime

def getData(query):
    client = InfluxDBClient(host='192.168.43.70', port=8086, username='iot', password='iot12345')
    client.switch_database('iot_multinode_DB')
    result = client.query(query).raw

    if result:
        return result
    
    return 0

def getDataMeasurements(measurement = None, node = None, timeformat = None):
    measurements = ['temperature', 'humidity', 'pressure', 'altitude', 'moisture', 'rain']
    data_measurement = {}
    number_of_node = 6
    nodes = range(1, number_of_node + 1)

    if measurement != None:
        if measurement in measurements:
            if node != None:
                if int(node) in nodes:
                    query = "SELECT time, value FROM "+ measurement.capitalize() +" where namenode='node"+ str(node) +"' ORDER BY desc LIMIT 1"
                    result = getData(query)

                    for data in result['series'][0]['values']:
                        data[0] = datetime.strptime(data[0], '%Y-%m-%dT%H:%M:%S.%fZ')
                        if timeformat == 'datetime':
                            data[0] = data[0].strftime('%Y-%m-%d %H:%M:%S')
                        else:
                            data[0] = data[0].strftime('%H:%M:%S')

                        data_measurement['time'] = data[0]
                        data_measurement['value'] = data[1]
            else:
                for i in nodes:
                    data_measurement['node' + str(i)] = {}
                    query = "SELECT time, value FROM "+ measurement.capitalize() +" where namenode='node"+ str(i) +"' ORDER BY desc LIMIT 1"
                    result = getData(query)
                    
                    if result['series']:
                        for data in result['series'][0]['values']:
                            data[0] = datetime.strptime(data[0], '%Y-%m-%dT%H:%M:%S.%fZ')
                            data[0] = data[0].strftime('%Y-%m-%d %H:%M:%S')

                            data_measurement['node' + str(i)]['time'] = data[0]
                            data_measurement['node' + str(i)]['value'] = data[1]

    else:
        for i in nodes:
            data_measurement['node' + str(i)] = {}
            
            queryGetLastTime = "SELECT last(value), time FROM Temperature, Humidity, Pressure, Altitude where namenode='node"+ str(i) +"'"
            resultLastTime = getData(queryGetLastTime)

            if resultLastTime['series']:
                times = []
                for data in resultLastTime['series']:
                    time = datetime.strptime(data['values'][0][0], '%Y-%m-%dT%H:%M:%S.%fZ')
                    times.append(time)
                
                times.sort(reverse = True)
                last_time = times[0].strftime('%Y-%m-%d %H:%M:%S')
                data_measurement['node' + str(i)]['last_connected'] = last_time
                    
            for m in measurements:
                data_measurement['node' + str(i)][m] = {}
                query = "SELECT time, value FROM "+ m.capitalize() +" where namenode='node"+ str(i) +"' ORDER BY desc LIMIT 1"
                result = getData(query)
                
                if result['series']:
                    for data in result['series'][0]['values']:
                        data[0] = datetime.strptime(data[0], '%Y-%m-%dT%H:%M:%S.%fZ')
                        data[0] = data[0].strftime('%Y-%m-%d %H:%M:%S')

                        data_measurement['node' + str(i)][m]['time'] = data[0]
                        data_measurement['node' + str(i)][m]['value'] = data[1]
    
    if data_measurement:
        return data_measurement
        
    return 0

# def getDataTemperature(node):
#     number_of_node = 6
#     Temperature = {}
#     nodes = range(1, number_of_node + 1)

#     if int(node) in nodes:
#         query = "SELECT time, value FROM Temperature where namenode='node"+ str(node) +"' ORDER BY desc LIMIT 1"
#         result = getData(query)

#         for data in result['series'][0]['values']:
#             data[0] = datetime.strptime(data[0], '%Y-%m-%dT%H:%M:%S.%fZ')
#             data[0] = data[0].strftime('%H:%M:%S')

#             Temperature['time'] = data[0]
#             Temperature['value'] = data[1]

#     if Temperature:
#         return Temperature
#     return 0

# def getDataKelembapan(node):
#     number_of_node = 5
#     kelembapan = {}
#     nodes = range(1, number_of_node + 1)

#     if int(node) in nodes:
#         query = "SELECT time, value FROM kelembapan where namenode='node"+ str(node) +"' ORDER BY desc LIMIT 1"
#         result = getData(query)

#         for data in result['series'][0]['values']:
#             data[0] = datetime.strptime(data[0], '%Y-%m-%dT%H:%M:%S.%fZ')
#             data[0] = data[0].strftime('%H:%M:%S')

#             kelembapan['time'] = data[0]
#             kelembapan['value'] = data[1]

#     if kelembapan:
#         return kelembapan
    
#     return 0

# def getDataHujan(node):
#     number_of_node = 5
#     hujan = {}
#     nodes = range(1, number_of_node + 1)

#     if int(node) in nodes:
#         query = "SELECT time, value FROM hujan where namenode='node"+ str(node) +"' ORDER BY desc LIMIT 1"
#         result = getData(query)

#         for data in result['series'][0]['values']:
#             data[0] = datetime.strptime(data[0], '%Y-%m-%dT%H:%M:%S.%fZ')
#             data[0] = data[0].strftime('%H:%M:%S')

#             hujan['time'] = data[0]
#             hujan['value'] = data[1]

#     if hujan:
#         return hujan
    
#     return 0

# def getDataMoisture(node):
#     number_of_node = 5
#     moisture = {}
#     nodes = range(1, number_of_node + 1)

#     if int(node) in nodes:
#         query = "SELECT time, value FROM moisture where namenode='node"+ str(node) +"' ORDER BY desc LIMIT 1"
#         result = getData(query)

#         for data in result['series'][0]['values']:
#             data[0] = datetime.strptime(data[0], '%Y-%m-%dT%H:%M:%S.%fZ')
#             data[0] = data[0].strftime('%H:%M:%S')

#             moisture['time'] = data[0]
#             moisture['value'] = data[1]

#     if moisture:
#         return moisture
    
#     return 0


# def getDataKetinggian(node):
#     number_of_node = 5
#     ketinggian = {}
#     nodes = range(1, number_of_node + 1)

#     if int(node) in nodes:
#         query = "SELECT time, value FROM ketinggian where namenode='node"+ str(node) +"' ORDER BY desc LIMIT 1"
#         result = getData(query)

#         for data in result['series'][0]['values']:
#             data[0] = datetime.strptime(data[0], '%Y-%m-%dT%H:%M:%S.%fZ')
#             data[0] = data[0].strftime('%H:%M:%S')

#             ketinggian['time'] = data[0]
#             ketinggian['value'] = data[1]

#     if ketinggian:
#         return ketinggian
    
#     return 0

# def getDataTekanan(node):
#     number_of_node = 5
#     tekanan = {}
#     nodes = range(1, number_of_node + 1)

#     if int(node) in nodes:
#         query = "SELECT time, value FROM tekanan where namenode='node"+ str(node) +"' ORDER BY desc LIMIT 1"
#         result = getData(query)

#         for data in result['series'][0]['values']:
#             data[0] = datetime.strptime(data[0], '%Y-%m-%dT%H:%M:%S.%fZ')
#             data[0] = data[0].strftime('%H:%M:%S')

#             tekanan['time'] = data[0]
#             tekanan['value'] = data[1]

#     if tekanan:
#         return tekanan
    
#     return 0