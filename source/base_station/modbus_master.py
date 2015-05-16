import os
#import xively
import subprocess
import time
import datetime
import requests
import logging

from time import sleep
from pymodbus.client.sync import ModbusSerialClient as ModbusClient

logging.basicConfig()
log = logging.getLogger()
log.setLevel(logging.DEBUG)

FEED_ID = 1468413317
API_KEY = 'UYXGfoLicnxaADBGZqxtQLxEEUjQK03a55T9q4sMFFhUeith'
DEBUG = True

REGISTERS = [
    'HUMIDITY',
    'TEMPERATURE_C',
    'TEMPERATURE_F',
    'BAY_1_DISTANCE_CM',
    'BAY_2_DISTANCE_CM',
    'BAY_1_LED_STATE',
    'BAY_2_LED_STATE',
    'BAY_1_DOOR_CONTROL',
    'BAY_2_DOOR_CONTROL',
]

if __name__ == '__main__':
#    api = xively.XivelyAPIClient(API_KEY)
#    feed = api.feeds.get(FEED_ID)
    try:
        while True:
            client = ModbusClient(method='rtu', port='/dev/ttyUSB0', baudrate=9600, timeout=2)
            print("Client Connected: %s" % client.connect())

            rr = client.read_holding_registers(0, len(REGISTERS), unit=100)
            if rr is None:
                continue
            print("Exception Response: %s" % (int(rr.function_code) > 0x80, ))
            for i in range(len(REGISTERS)):
                print("Register %s: %s" % (REGISTERS[i], rr.registers[i]))

            wr = client.write_register(7, 1, unit=100)
            if wr is None:
                continue
            print("Exception Response: %s" % (int(wr.function_code) > 0x80, ))

            print("Updating Feed")
#            feed.datastreams = [
#                xively.Datastream(id='Bay_1_Distance_CM',
#                    current_value = rr.registers[3],
#                    at = datetime.datetime.utcnow()),
#                xively.Datastream(id='Bay_2_Distance_CM',
#                    current_value = rr.registers[4],
#                    at = datetime.datetime.utcnow()),
#                xively.Datastream(id='Temperature_C',
#                    current_value = rr.registers[1] / 100.,
#                    at = datetime.datetime.utcnow()),
#                xively.Datastream(id='Temperature_F',
#                    current_value = rr.registers[2] / 100.,
#                    at = datetime.datetime.utcnow()),
#                xively.Datastream(id='Humidity',
#                    current_value = rr.registers[0] / 100.,
#                    at = datetime.datetime.utcnow()),
#            ]
#            feed.update()
            time.sleep(10.0)
    finally:
        client.close()
