import os
import xively
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

NUM_REGISTERS = 7

FEED_ID = 1468413317
API_KEY = 'UYXGfoLicnxaADBGZqxtQLxEEUjQK03a55T9q4sMFFhUeith'
DEBUG = True


if __name__ == '__main__':
    api = xively.XivelyAPIClient(API_KEY)
    feed = api.feeds.get(FEED_ID)
    try:
        while True:
            client = ModbusClient(method='rtu', port='/dev/ttyAMA0', baudrate=9600, timeout=2)
            print("Client Connected: %s" % client.connect())

            rr = client.read_holding_registers(0, NUM_REGISTERS, unit=100)
            print("Exception Response: %s" % rr.function_code < 0x80)
            for i in range(NUM_REGISTERS):
                print("Register %s: %s" % (i, rr.registers[i]))

            print("Updating Feed")
            feed.datastreams = [
                xively.Datastream(id='Bay_1_Distance_CM', current_value = 1, at = datetime.datetime.utcnow()),
                xively.Datastream(id='Bay_2_Distance_CM', current_value = 2, at = datetime.datetime.utcnow()),
                xively.Datastream(id='Temperature_C', current_value = 3, at = datetime.datetime.utcnow()),
                xively.Datastream(id='Temperature_F', current_value = 4, at = datetime.datetime.utcnow()),
            ]
            feed.update()
            time.sleep(10.0)
    finally:
        client.close()
