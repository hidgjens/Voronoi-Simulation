import numpy as np
from copy import deepcopy
from typing import List

from Team import Team
from Player import Player
from Pitch import Pitch

PositionArray = List[np.ndarray]
ControlArray = List[float]

class Frame:
    home_player_positions : PositionArray = []
    away_player_positions : PositionArray = []

    home_player_control : ControlArray = []
    away_player_control : ControlArray = []

    home_control : float = 0.0
    away_control : float = 0.0

    total_control : float = 0.0

    possession : bool = True

    pitch : Pitch = None

    frame_number : int

    def __init__(self, num : int, pitch : Pitch = Pitch()):
        self.frame_number = num
        self.pitch = pitch

    def store_data(self, home_team : Team, away_team : Team, poss : bool) -> NotImplemented:
        self.possession = poss
        self.home_player_positions = []
        self.away_player_positions = []

        for p in home_team.players:
            player_position : np.ndarray = deepcopy(p.position)
            self.home_player_positions.append(player_position)
            
        for p in away_team.players:
            player_position : np.ndarray = deepcopy(p.position)
            self.away_player_positions.append(player_position)

        self.compute_control()

    def print_frame(self) -> None:
        print('==================================================================')
        print('\nFrame %i - Possession: %s\n' % (self.frame_number, self.poss_string()))
        print('==================================================================')

        
        for i, info in enumerate(zip(self.home_player_positions, self.home_player_control)):
            pos, ctrl = info
            print('\tHome Shirt %i: (%f,%f) - %.3f' % (i, pos[0], pos[1], ctrl))

        print('- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ')

        for i, info in enumerate(zip(self.away_player_positions, self.away_player_control)):
            pos, ctrl = info
            print('\tAway Shirt %i: (%f,%f) - %.3f' % (i, pos[0], pos[1], ctrl))

        print('==================================================================')
        print('Scores: Home %.3f - Away %.3f' % (self.home_control, self.away_control))
        print('==================================================================')


    def poss_string(self) -> str:
        if self.possession:
            return ("True")
        else:
            return ("False")

    def distance_between(self, x1 : np.ndarray, x2 : np.ndarray) -> float:
        return(np.sqrt( (x1[0] - x2[0]) ** 2 + (x1[1] - x2[1]) ** 2) )

    def convert_idx_to_coord(self, i : int, j : int) -> np.ndarray:
        samples_x : int = self.pitch.xSamples()
        samples_y : int = self.pitch.ySamples()

        x : float = ((i + 0.5) / samples_x - 0.5) * self.pitch.pitch_x
        y : float = ((j + 0.5) / samples_y - 0.5) * self.pitch.pitch_y

        return(np.asarray([x,y]))

    def compute_control(self) -> None:
        samples_x : int = self.pitch.xSamples()
        samples_y : int = self.pitch.ySamples()

        self.home_control = 0.0
        self.home_player_control = [0.0 for i in self.home_player_positions]
        self.away_control = 0.0
        self.away_player_control = [0.0 for i in self.away_player_positions]

        self.total_control = 0.0

        #print(samples_y, samples_x)

        for j in range(samples_y):
            for i in range(samples_x):
                test_pos : np.ndarray = self.convert_idx_to_coord(i, j)
                #print(test_pos)

                closest_shirtnum : int = 0 # -ve for away, +ve for home
                closest_distance : float = -10.0 # -ve initially

                #print('len', len(self.home_player_positions))

                for sn, pos in enumerate(self.home_player_positions):
                    
                    distance : float = self.distance_between(pos, test_pos)
                    
                    if (distance < closest_distance) or (closest_distance < 0):
                        closest_distance = distance
                        closest_shirtnum = sn + 1

                for sn, pos in enumerate(self.away_player_positions):
                    distance : float = self.distance_between(pos, test_pos)
                    
                    if (distance < closest_distance) or (closest_distance < 0.0):
                        closest_distance = distance
                        closest_shirtnum = -1 * (sn + 1)

                weight : float = self.pitch.weight(test_pos)
                dC : float = 1.0 * weight
                #print(closest_distance)
                #print(closest_shirtnum)

                if closest_shirtnum > 0:
                    # home
                    #print('home', closest_shirtnum)
                    self.home_control += dC
                    self.home_player_control[int(closest_shirtnum - 1)] += dC
                    self.total_control += dC
                elif closest_shirtnum < 0:
                    # away
                    #print('away', closest_shirtnum)
                    self.away_control += dC
                    self.away_player_control[int( -1 * closest_shirtnum - 1)] += dC
                    self.total_control += dC

        self.home_control /= self.total_control
        self.away_control /= self.total_control
        self.home_player_control = [h/self.total_control for h in self.home_player_control]
        self.away_player_control = [a/self.total_control for a in self.away_player_control]
                




