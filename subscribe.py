import pika
import json
import logging
from influxdb import InfluxDBClient

def insert_to_influx(data, routing_key):
    client = InfluxDBClient(host='127.0.0.1', port=8086, username='iot', password='iot12345')
    keys = routing_key.split('-')
    measurement = keys[0]
    namenode = keys[1]
    client.switch_database('iot_multinode_DB')
    data = [{
        "measurement": measurement,
        "tags": {
            "name": "IoT"
        },
        "fields": {
            "value": data,
            "namenode": namenode
        }
    }]

    client.write_points(data)


credentials = pika.PlainCredentials('admin', '12345coba')

parameters = pika.ConnectionParameters('127.0.0.1', 5672, '/', credentials)

connection = pika.BlockingConnection(parameters)
channel = connection.channel()

channel.exchange_declare(exchange='test-exchange', exchange_type='topic', durable=True)

result=channel.queue_declare(queue='', exclusive=True)

routing_key = ['Rain', 'Temperature', 'Humidity', 'Moisture','Altitude', 'Pressure']

for i in range(1, 6):
    for rk in routing_key:
        channel.queue_bind(queue=queue_name, exchange='test-exchange', routing_key=rk + "-node" + str(i))

def insert_to_log(routing_key, value):
    keys = routing_key.split('-')
    measurement = keys[0]
    namenode = keys[1]

    logging.basicConfig(filename='test.log', filemode='a', format='%(asctime)s -- %(message)s')
    logging.error(measurement + ' -- ' + namenode + ' -- ' + str(value))

def callback(ch, method, properties, body):
    print(" [x] Received %r" % body)
    insert_to_influx(body, method.routing_key)
    insert_to_log(method.routing_key, body)

channel.basic_consume(queue=queue_name, on_message_callback=callback)

print(' [*] Waiting for messages. To exit press CTRL+C')

channel.start_consuming()
