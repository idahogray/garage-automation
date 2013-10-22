import os
import xively
import subprocess
import time
import datetime
import requests

FEED_ID = 1468413317
API_KEY = 'UYXGfoLicnxaADBGZqxtQLxEEUjQK03a55T9q4sMFFhUeith'
DEBUG = True


if __name__ == '__main__':
	api = xively.XivelyAPIClient(API_KEY)
	feed = api.feeds.get(FEED_ID)

	while True:
		print("Updating Feed")
		feed.datastreams = [
			xively.Datastream(id='Bay_1_Distance_CM', current_value = 1, at = datetime.datetime.utcnow()),
			xively.Datastream(id='Bay_2_Distance_CM', current_value = 2, at = datetime.datetime.utcnow()),
			xively.Datastream(id='Temperature_C', current_value = 3, at = datetime.datetime.utcnow()),
			xively.Datastream(id='Temperature_F', current_value = 4, at = datetime.datetime.utcnow()),
		]
		feed.update()
		time.sleep(10.0)
		
