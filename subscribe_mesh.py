import pika
import json
import logging
from influxdb import InfluxDBClient

def insert_to_influx(data):
    msg = json.loads(data)
    client = InfluxDBClient(host='127.0.0.1', port=8086, username='iot', password='iot12345')
    namenode = 'node' + str(msg['node'])
    client.switch_database('iot_multinode_DB')

    for i in msg:
        if i != 'node':
            value = msg[i]
            measurement = i
            data = [{
                "measurement": measurement,
                "tags": {
                    "name": "IoT"
                },
                "fields": {
                    "value": value,
                    "namenode": namenode,
                }
            }]

            client.write_points(data)

credentials = pika.PlainCredentials('admin', '12345coba')

parameters = pika.ConnectionParameters('127.0.0.1', 5672, '/', credentials)

connection = pika.BlockingConnection(parameters)
channel = connection.channel()

channel.exchange_declare(exchange='test-exchange', exchange_type='topic', durable=True)

result=channel.queue_declare(queue='', exclusive=True)

queue_name=result.method.queue

channel.queue_bind(queue=queue_name, exchange='test-exchange', routing_key="mesh-Gateway")

def insert_to_log(data):
    msg = json.loads(data)
    namenode = 'node' + str(msg['node'])

    for i in msg:
        if i != 'node':
            measurement = i
            value = msg[i]
            logging.basicConfig(filename='test.log', filemode='a', format='%(asctime)s -- %(message)s')
            logging.error(measurement + ' -- ' + namenode + ' -- ' + str(value))

def callback(ch, method, properties, body):
    print(" [x] Received %r" % body)
    insert_to_influx(body)
    insert_to_log(body)


channel.basic_consume(queue=queue_name, on_message_callback=callback)

print(' [*] Waiting for messages. To exit press CTRL+C')

channel.start_consuming()

