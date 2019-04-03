import numpy as np

from Team import Team
from Frame import Frame
from Pitch import Pitch
from typing import List

FrameArray = List[Frame]

class Match:

    home_team : Team = None
    away_team : Team = None

    pitch : Pitch = None

    current_frame : Frame = None
    stored_frames : FrameArray = []

    current_possession : bool = True

    current_frame_number : int = 0
    total_number_of_frames : int = 50

    def __init__(self):
        self.pitch = Pitch()
        self.home_team = Team(True, pitch=self.pitch)
        self.away_team = Team(False, pitch=self.pitch)
        self.current_frame_number = 0
        self.flip5050()
        self.store_frame()
        self.current_frame.print_frame()

    def flip5050(self):
        random_draw = np.random.rand()
        if random_draw <= 1.0 / 2.0:
            # toggle possession
            self.current_possession = not self.current_possession

    def random_flip(self):
        random_draw = np.random.rand()
        if random_draw < 1.0 / 50.0:
            # toggle possession
            self.current_possession = not self.current_possession

    def store_frame(self):
        frame = Frame(self.current_frame_number, pitch=self.pitch)
        frame.store_data(self.home_team, self.away_team, self.current_possession)
        self.stored_frames.append(frame)
        self.current_frame = frame

    def update_team(self, frame: Frame):
        self.home_team.updateTeam(frame)
        self.away_team.updateTeam(frame)

    def tick(self):
        self.random_flip()
        self.update_team(self.current_frame)
        self.current_frame_number += 1
        self.store_frame()
        self.current_frame.print_frame()

    def start_sim(self):
        while (self.current_frame_number < self.total_number_of_frames - 1):
            self.tick()
            
