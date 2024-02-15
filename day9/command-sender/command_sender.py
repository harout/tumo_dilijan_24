#pip install pyserial
#ls /dev/tty.usbs*

import sys
import json
import time

from radio_client import RadioClient
from radio_client import RadioConfig
from processing_client import ProcessingClient


# Get the serial port name from the command line
if len(sys.argv) < 2:
    print("Usage: python3 command-sender.py <serial port name>")
    sys.exit(1)

serial_port = sys.argv[1]
print(f"Connecting to {serial_port}...")

radio_params = json.load(open("radio_params.json", "r"))
radio_config = RadioConfig(radio_params['freq'],
                           radio_params['bandwidth'],
                           radio_params['spreading_factor'],
                           radio_params['coding_rate'],
                           radio_params['power'])
radio_client = RadioClient(serial_port, radio_config)

processing_client = ProcessingClient(radio_client)

processing_client.begin_transaction()
processing_client.set_background_color(0, 255, 0)
processing_client.set_text_size(30)
processing_client.set_fill_color(255, 0, 0)
processing_client.draw_string("Hello World!", 100, 100)
processing_client.end_transaction()

# radio_client.send_string("bt")
# time.sleep(0.5)
# radio_client.send_string("b,255,0,0")
# time.sleep(0.5)
# radio_client.send_string("rot,45.0")
# time.sleep(0.5)
# radio_client.send_string("r,100,100,50,200")
# time.sleep(0.5)
# radio_client.send_string("et")
# time.sleep(0.5)

# time.sleep(3.0)

# radio_client.send_string("bt")
# time.sleep(0.5)
# radio_client.send_string("b,0,0,255")
# time.sleep(0.5)
# radio_client.send_string("tsl,300,300")
# time.sleep(0.5)
# radio_client.send_string("rot,5.0")
# time.sleep(0.5)
# radio_client.send_string("r,0,0,50,200")
# time.sleep(0.5)
# radio_client.send_string("et")
# time.sleep(0.5)