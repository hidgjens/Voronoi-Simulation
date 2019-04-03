from Player import Player
from Pitch import Pitch
import numpy as np
from typing import List

PlayerArray = List[Player]

class Team:
    home_team : bool = True
    player_count : int = 11

    players : PlayerArray = []

    step_size : float = 2.5

    pitch: Pitch = None

    def __init__(self, team : bool, pitch : Pitch = Pitch()):
        self.home_team = team
        self.pitch = pitch
        # generates starting positions
        random_positions_x : np.ndarray = np.multiply(np.subtract(np.random.rand(self.player_count), 0.5), pitch.pitch_x)
        random_positions_y : np.ndarray = np.multiply(np.subtract(np.random.rand(self.player_count), 0.5), pitch.pitch_y) 
        random_positions = []

        for x, y in zip(random_positions_x, random_positions_y):
            random_positions.append(np.asarray([x,y]))

        # create players
        self.players = [Player(self.home_team, i_pos) for i_pos in random_positions]

    def updateTeam(self, frame) -> None:
        for p in self.players:
            random_angle : float = np.random.rand() * 2.0 * np.pi
            unit_change : np.ndarray = np.asarray([np.cos(random_angle), np.sin(random_angle)])
            change : np.ndarray = np.multiply(unit_change, self.step_size)
            
            p.changePositionBy(change)

        
