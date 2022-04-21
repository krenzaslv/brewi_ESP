import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import requests
import schedule
import pandas as pd
import seaborn as sns

REMOTE_IP = 'http://10.0.22.80'
UPDATE_INTERVAL = 6
SLEEP_INTERVAL = 6
FILENAME = results.csv
LOAD_DATA = False

fig = plt.figure()
plt.ion()
plt.show()
with sns.axes_style("darkgrid"):
    ax1 = fig.add_subplot(131)
    ax2 = fig.add_subplot(132)
    ax3 = fig.add_subplot(133)
data = pd.DataFrame()

def saveData():
    global data
    data.to_csv("log") 

def request():
    response = requests.get(REMOTE_IP).json()
    d = pd.DataFrame([response['variables']])
    return d

def requestSchedule():
    global data
    d = request() 
    data = pd.concat([data, d], ignore_index=True)
    print(data)

def plotSchedule():
    global data
    ax1.plot(data['time'],data['temperature'], label="temperature", c="tab:blue")
    ax1.plot(data['time'],data['targetTemperature'], label="targetTemperature", c="tab:red")
    ax1.set_xlabel('Seconds')
    ax1.set_ylabel('Temperature')
    
    ax2.plot(data['time'],data['pidGain'], label="PID Gain", c="tab:blue")
    ax2.plot(data['time'],data['pidD'], label="PID D", c="tab:red")
    ax2.plot(data['time'],data['pidI'], label="PID I", c="tab:orange")
    ax2.plot(data['time'],data['pidP'], label="PID P", c="tab:cyan")
    ax2.set_xlabel('Seconds')
    ax2.set_ylabel('PID')


    ax3.plot(data['time'],data['dutyCycles'], label="Duty Cycle", c="tab:olive")
    ax3.plot(data['time'],data['isHeating'], label="IsHeating", marker="x", c="tab:red")
    ax3.set_xlabel('Seconds')

    plt.draw()
    plt.pause(0.001)

schedule.every(UPDATE_INTERVAL).seconds.do(requestSchedule)
schedule.every(1).minute.do(saveData)
schedule.every(UPDATE_INTERVAL).seconds.do(plotSchedule)

#Init data
if LOAD_DATA:
    data = pd.from_csv(FILENAME)
else:
    data = request()

plotSchedule()
ax1.legend()
ax2.legend()
ax3.legend()

while True:
    schedule.run_pending()
    time.sleep(SLEEP_INTERVAL)
