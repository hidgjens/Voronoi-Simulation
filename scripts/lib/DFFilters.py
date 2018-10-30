import pandas as pd

def Rm0Frame(dataframe):
    return(dataframe.loc[dataframe['FID'] != 0])

def RmBall(dataframe):
    return(dataframe.loc[dataframe['Team'] != 'Ball'])

def RmHomeTeam(dataframe):
    return(dataframe.loc[dataframe['Team'] != 'Home'])

def RmAwayTeam(dataframe):
    return(dataframe.loc[dataframe['Team'] != 'Away'])

def RmSmartPlayer(dataframe):
    return(dataframe.loc[dataframe['Smart'] != 'True'])

def RmNormPlayer(dataframe):
    return(dataframe.loc[dataframe['Smart'] != 'False'])

def Sl0Frame(dataframe):
    return(dataframe.loc[dataframe['FID'] == 0])

def SlBall(dataframe):
    return(dataframe.loc[dataframe['Team'] == 'Ball'])

def SlHomeTeam(dataframe):
    return(dataframe.loc[dataframe['Team'] == 'Home'])

def SlAwayTeam(dataframe):
    return(dataframe.loc[dataframe['Team'] == 'Away'])

def SlSmartPlayer(dataframe):
    return(dataframe.loc[dataframe['Smart'] == True])

def SlNormPlayer(dataframe):
    return(dataframe.loc[dataframe['Smart'] == False])

def RmStationary(dataframe):
    # iterate through each unique object in dataframe and remove if stationary
    # needs 0 frame!

    # iterate through teams
    for team in dataframe['Team'].unique():
        # iterate through shirt numbers
        for sh_num in dataframe.loc[dataframe['Team'] == team, 'Num'].unique():
            # iterate through frames
            for frame in dataframe.loc[(dataframe['Team'] == team) & (dataframe['Num'] == sh_num), 'FID'].unique():
                # if first frame, skip
                if frame == 0:
                    continue

                # check if positions are equal to last frames
                idx = dataframe.index[(dataframe['Team'] == team) & (dataframe['Num'] == sh_num) & (dataframe['FID'] == frame)].tolist()
                if len(idx) == 0:
                    continue
                idx = idx[0]

                idx_lst = dataframe.index[(dataframe['Team'] == team) & (dataframe['Num'] == sh_num) & (dataframe['FID'] == (frame - 1))].tolist()
                if len(idx_lst) == 0:
                    continue
                idx_lst = idx_lst[0]

                # compare two positions
                if dataframe.at[idx, 'X'] == dataframe.at[idx_lst, 'X'] and dataframe.at[idx, 'Y'] == dataframe.at[idx_lst, 'Y']:
                    # not change, drop the row
                    dataframe.drop(idx)
    return(dataframe)

def FilterData(dataframe, func):
    length = dataframe.shape[0] # number of rows
    dataframe = func(dataframe)
    cut = length - dataframe.shape[0] # calc change
    return(dataframe, cut)
