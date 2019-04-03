import numpy as np
from Pitch import Pitch

class Player:

    position : np.ndarray = np.zeros(2)
    home_team : bool = True
    pitch: Pitch = None

    def __init__ (self, team : bool, initial_position : np.ndarray = np.asarray([0, 0]), pitch = Pitch()):
        self.pitch = pitch
        self.home_team = team
        self.position = initial_position

    def createHomePlayer(self, initial_position : np.ndarray = np.asarray([0,0])):
        return Player(True, initial_position)
    
    def createAwayPlayer(self, initial_position : np.ndarray = np.asarray([0,0])):
        return Player(False, initial_position)

    def changePositionBy(self, dPos : np.ndarray) -> None:
        self.position += dPos

        if self.position[0] > self.pitch.xLimit():
            self.position[0] = self.pitch.xLimit()
        elif self.position[0] < -1.0 * self.pitch.xLimit():
            self.position[0] = -1.0 * self.pitch.xLimit()

        if self.position[1] > self.pitch.yLimit():
            self.position[1] = self.pitch.yLimit()
        elif self.position[1] < -1.0 * self.pitch.yLimit():
            self.position[1] = -1.0 * self.pitch.yLimit()