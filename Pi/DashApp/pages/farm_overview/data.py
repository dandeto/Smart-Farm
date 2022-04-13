import pandas as pd
from utils import constants as uc
import numpy as np

headerNames = ["time", "recieverId", "tempInC", "pressure", "altitude", "humidity", "capacitance"]
dataCols = ["tempInC", "pressure", "altitude", "humidity", "capacitance"]
# TODO: potentially cache this.
df = pd.read_csv(uc.data_path, names=headerNames)
df = df[np.abs(df.capacitance - df.capacitance.mean()) <= (3 * df.capacitance.std())] #keep only the ones that are within +3 to -3 standard deviations in the column 'Data'.
df = df[~(np.abs(df.capacitance - df.capacitance.mean()) > (3 * df.capacitance.std()))] #or the other way around

"""
uniqueIDs = sorted(recievedDf.recieverId.unique())
print(f"IDs Connected: {uniqueIDs}")
figList = []
for uniqueID in uniqueIDs:
    figsOfId = []
    tempDf = recievedDf[recievedDf['recieverId']==uniqueID]
    tempDf = tempDf[np.abs(tempDf.capacitance - tempDf.capacitance.mean()) <= (3 * tempDf.capacitance.std())] #keep only the ones that are within +3 to -3 standard deviations in the column 'Data'.
    tempDf = tempDf[~(np.abs(tempDf.capacitance - tempDf.capacitance.mean()) > (3 * tempDf.capacitance.std()))] #or the other way around
    for col in dataCols:
        tempDf[f"{col}"] = tempDf[f"{col}"].apply(pd.to_numeric, errors="coerce")
        tempTitle = (f"DEVICE-{uniqueID}-{col}").upper()
        figsOfId.append(px.scatter(tempDf, x="time", y=col, title=tempTitle, template="plotly_white"))    
    figList.append(figsOfId)

for reciever in figList:
    for plot in reciever:
        place = pl.Path(f"output/images/{plot.layout.title.text}.png")
        plot.write_image(place)
"""

