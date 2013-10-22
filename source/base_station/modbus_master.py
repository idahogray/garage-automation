from time import sleep
from pymodbus.client.sync import ModbusSerialClient as ModbusClient

import logging
logging.basicConfig()
log = logging.getLogger()
log.setLevel(logging.DEBUG)

NUM_REGISTERS = 7

try:
    while 1:
        client = ModbusClient(method='rtu', port='/dev/ttyAMA0', baudrate=9600, timeout=2)
        print("Client Connected: %s" % client.connect())

        rr = client.read_holding_registers(0, NUM_REGISTERS, unit=100)
        print("Exception Response: %s" % rr.function_code < 0x80)
        for i in range(NUM_REGISTERS):
            print("Register %s: %s" % (i, rr.registers[i]))
	sleep(1.0)
finally:
    client.close()
