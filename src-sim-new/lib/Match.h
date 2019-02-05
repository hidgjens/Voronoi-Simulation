#ifndef MATCH_H
#define MATCH_H

#include "PitchModel.h"
#include "Frame.h"
#include "Pitch.h"
#include "cfg/MatchConfigFile.h"
#include "cfg/TeamConfigFile.h"

class Team;

class Match {
    private:
        // simulation settings
        bool store_frames;
        int total_frames;

        int x_samples;
        int y_samples;

        Pitch pitch_data;

        PitchModel* weight_model;

        std::string filename;
        int matchID;

        MatchConfigFile mcf;

        double possession_flip_reciprocal_probability;

        // teams
        Team* home_team;
        Team* away_team;

        // match data
        int current_frame_number;
        Frame current_frame;
        std::unique_ptr<Frame[]> frame_history;

        double home_team_control_sum;
        double away_team_control_sum;

        bool home_possession;
        

    public:
        // constructors
        Match();
        Match(TeamConfigFile, TeamConfigFile, MatchConfigFile);

        Match(Match&);
        Match(Match&& match)  { *this = std::move(match); }

        Match& operator=(Match&);
        Match& operator=(Match&&);

        void buildMatch();

        Pitch& getPitchData();

        void startSimulation();
        void saveFullMatch();
        void saveMatchSummary();

        void togglePossession();
        void randomPossession();
        void tryPossessionFlip();

        Frame createFrame();

        void tick(); // "update"

        void calculate_control();

        double avgHomeControl();
        double avgAwayControl();

        void setMatchID(int);

        Cart convertIdx2Coords(int, int);



};


#endif