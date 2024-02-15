from serial import Serial
import time

class RadioConfig:
    def __init__(self, freq:float, bandwidth:int, spreading_factor:int, coding_rate:str, power:int):
        self.freq = freq
        self.power = power
        self.bandwidth = bandwidth
        self.spreading_factor = spreading_factor
        self.coding_rate = coding_rate
        
class RadioClient:
    PACKET_LENGHT = 128
    PREFIX = 0x00
    SUFFIX = 0xFF
    NUM_SENTINEL_BYTES = 3

    def __init__(self, device_path:str, radio_config:RadioConfig):
        self.device_path = device_path
        self.radio_config = radio_config

        radio_config_str = f"{radio_config.freq},{radio_config.bandwidth},"
        radio_config_str += f"{radio_config.spreading_factor},{radio_config.coding_rate},"
        radio_config_str += f"{radio_config.power}\n"

        self.radio = Serial(device_path, 115200)
        self.reset_device()
        time.sleep(2.0)

        self.radio.write(radio_config_str.encode("ascii"))
        self.radio.flush()

        self.framed_packet_length = self.PACKET_LENGHT + (self.NUM_SENTINEL_BYTES * 2)


    def reset_device(self):
        self.radio.setDTR(False)
        self.radio.setRTS(False)
        time.sleep(0.1)
        self.radio.setDTR(True)
        self.radio.setRTS(True)
        time.sleep(0.1)
        self.radio.setDTR(False)
        self.radio.setRTS(False)
        time.sleep(0.1)
        

    def send_string(self, string:str):
        encoded = string.encode("utf-8")
        if len(encoded) > self.PACKET_LENGHT:
            return False

        if len(encoded) < self.PACKET_LENGHT:
            encoded += bytes([0] * (self.PACKET_LENGHT - len(encoded)))
        
        print('sending: ', encoded)
        self._send_packet(encoded)
        return True

    def _send_packet(self, packet:bytes):
        framed_packet = bytes([self.PREFIX] * self.NUM_SENTINEL_BYTES)
        framed_packet += packet
        framed_packet += bytes([self.SUFFIX] * self.NUM_SENTINEL_BYTES)

        self.radio.write(framed_packet)
        self.radio.flush()
    