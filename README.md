# Voronoi-Simulation

## Simulation
The purpose of the simulation is to test strategies on 2 teams of automita and measure their performance by the amount of control they hold throughout a game. Control is measured using approximated Voronoi cells (by sampling) with weightings applied to each sample via a heatmap. At each time-step the teams are told to update the positions of their players - the teams must decide where to move in order to gain the most control.

### Structure
Outlined here are some details of the main features of the simulation which may be useful before playing.

#### Time-Step
The simulation is run in discrete time, so the state of the pitch is updated in _time-steps_. The ```current_frame``` counter starts at 0 and increases by 1 via a ```tick()``` call at each time-step until the total number of frames has elapsed. The ```tick``` function contains the steps executed to update the pitch:
1. See if possession changes.
2. Increase ```current_frame``` by 1.
3. Update player positions
4. Check for collisions caused. Scatter colliding players.
5. Calculate control for current time-step.
6. Store current pitch state in a ```Frame```. Store ```Frame``` if full save is required.

#### Frames
Objects which store all the information needed to reconstruct the state of the match at a given time-step. A frame contains:
* The positions of the players
* The control of the players (percentage of pitch)
* The value of possession

#### Teams and Strategies
A ```Team``` stores 11 ```Player``` objects and the strategy that the team will use to determine where the players move at each time-step.

##### Players
A ```Player``` primarily stores its current ```position```, as well as information such as ```Team``` and shirt number. It also ensures that it has not left the pitch after moving.

##### Strategies
A strategy in a coding sense is a class derived from base-class ```TeamStrategy``` which has member-function ```updateTeam(Team&, Frame);``` defined to update the ```Team``` passed to it using the information provided in the ```Frame```. 

#### Saving
There are two methods of storing a match. A **summary** is saved for each match run, while a **full save** is optional.

##### Summary
Creates a ```.sum``` file containing the mean control per frame of the home and away team. This information is often enough for a simple analysis.

##### Full Save
When full save is used, the frames are recalled chronologically and written to a ```.csv``` file for more detailed analysis.


## Config Files
In order to launch the simulation the match configuration and the two teams' configurations must be provided by a ```run_name```. The ```run_name``` is given in the format ```Match:HomeTeam:AwayTeam``` where the placeholders must be filled with the name - excluding extension - of the ```.cfg``` file found in the directories ```configs/matches``` and ```configs/teams``` for match and team configurations respectively.
```
    # e.g. run_name
    Std:Exchange:Random # Standard match, Home team Exchange.cfg vs Away team Random.cfg
```
New ```.cfg``` files can be made to change a team's behaviour.

### Match Config Options: 
There is the option to change features of the match played by the teams by altering the standard config file (```configs/matches/std.cfg```) or creating a new config file in the ```configs/matches``` directory. 

#### Saving
Change whether a match saves a full ```.csv``` file. It could be worth noting that if the option to save a full match is disabled, the simulation will not store frames once they are no longer needed. This should reduce the amount of memory the simulation uses.
```
    SaveName=name # NOTE: deprecated - has no effect as SaveName is set via command line arguments
    FullSave=true/false
```
#### Match
These are features of the game rules that can be changed.
```
    NumberOfFrames=2000 # length of match
    PossessionChance=50 # (integer) reciprocal probability to toggle possession per frame
    PlayerRadius=0.4 # defined size of players
    CollisionScatter=2.5 # amount to scatter colliding players by
```

#### Pitch
Dimensions and _resolution_ (samples) of pitch.
```
    PitchX=105.0
    PitchY=68.0
    SamplesX=105 
    SamplesY=68
```

#### Pitch Model
The type of weighting model to be used and any additional options.
```
    PitchModel=Uniform/Xlinear
    xmin=0.5 # Xlinear
    xmax=1.0 # Xlinear
```

### Team Config Options:
Each team follows a strategy defined in their config file. When the simulation is launched, the names of the config files to be used by the home and away teams must be provided via the command line arguments. A list of the options for a config file is given below. The values provided are the default values if none is provided - the only required option is the Strategy.

#### Team
Parameters which apply to all team strategies.
```
    PlayerCount=11
    Strategy=Random/Exchange/Metric/Spreading/Tether
    MinDist=3 # minimum distance between teammates in units of player step-size
```

#### Exchange/Metric Parameters
Values only used by the Metric/Exchange strategies (and their derivatives).
```
    DecayCoeff=1.0
    AttrCoeff=1.0 # Metric
    RepelCoeff=1.0 # Metric
```

#### Spreading Parameters
Values only used by the Spreading strategies (and its derivatives).
```
    EdgeCoeff=1.0
    AllyCoeff=1.0
    PlayersToSum=5
    MaxiSamples=36
```

#### Tether
Values used by strategies with the Tether property.
```
    PostDistance=20.0 # maximum distance players can move from their post
```



