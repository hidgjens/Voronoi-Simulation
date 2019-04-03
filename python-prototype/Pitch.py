import numpy as np

class Pitch:
    pitch_x : float = 105.0
    pitch_y : float = 68.0

    def __init__(self, pitch_x = 105.0, pitch_y = 68.0):
        self.pitch_x = pitch_x
        self.pitch_y = pitch_y

    def xLimit(self) -> float:
        return(self.pitch_x / 2.0)

    def yLimit(self) -> float:
        return(self.pitch_y / 2.0)

    def pitchScale(self) -> float:
        return(np.sqrt( self.pitch_x * self.pitch_y ))

    def xSamples(self) -> int:
        return(int(self.pitch_x))

    def ySamples(self) -> int:
        return(int(self.pitch_y))

    def weight(self, r : np.ndarray) -> float:
        return(1.0)