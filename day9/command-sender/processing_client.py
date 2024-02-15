import time

from radio_client import RadioClient

class ProcessingClient:
    def __init__(self, radio_client:RadioClient):
        self.radio_client = radio_client

    def __sleep(self):
        time.sleep(0.2)

    def begin_transaction(self):
        self.radio_client.send_string("bt")
        self.__sleep()

    def end_transaction(self):
        self.radio_client.send_string("et")
        self.__sleep()

    def draw_string(self, str, x, y):
        self.radio_client.send_string(f"s,{str},{x},{y}")
        self.__sleep()

    def set_text_size(self, size):
        self.radio_client.send_string(f"ts,{size}")
        self.__sleep()

    def set_stroke_color(self, r, g, b):
        self.radio_client.send_string(f"sc,{r},{g},{b}")
        self.__sleep()

    def set_fill_color(self, r, g, b):
        self.radio_client.send_string(f"f,{r},{g},{b}")
        self.__sleep()

    def set_background_color(self, r, g, b):
        self.radio_client.send_string(f"b,{r},{g},{b}")
        self.__sleep()