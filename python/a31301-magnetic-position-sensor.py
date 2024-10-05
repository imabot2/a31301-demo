import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation
import serial

# Initialize serial communication
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

# Prepare the figure
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_xlim(-5000, 5000)
ax.set_ylim(-5000, 5000)
ax.set_zlim(0, 5000)
ax.set_xlabel('X-axis')
ax.set_ylabel('Y-axis')
ax.set_zlabel('Z-axis')



# Return the magnitude vector read from the serial device
def getMagnitude():
    parts = []
    while(len(parts)!=3):
        try:
            # Try to read a line from the serial device
            line = ser.readline().decode('ascii').strip()
            parts = line.split() if ' ' in line else line.split(',')
        except:
            None
    # If the line contains 3 integers, create the vector, otherwise returns a null vector
    if (len(parts) == 3):
        Mag = [ -int(parts[1]), -int(parts[0]), int(parts[2])]
    else:
        Mag = [ 0, 0, 0]
    
    # Flush the serial buffer to always get recent data
    ser.flushInput()

    # Returns the vector
    return Mag


# Get magnitude data and prepare vectors
arrow = [0,0,0]
arrow.extend(getMagnitude())
quiver = ax.quiver(*arrow)
quiverX = ax.quiver(*[0,0,0,arrow[3],0,0])
quiverY = ax.quiver(*[0,0,0,0,arrow[4],0])
quiverZ = ax.quiver(*[0,0,0,0,0,arrow[5]])



# Callback function to update display
def update(elapsed):
    global quiver
    global quiverX
    global quiverY
    global quiverZ

    quiver.remove()
    quiverX.remove()
    quiverY.remove()
    quiverZ.remove()

    arrow = [0,0,0]
    arrow.extend(getMagnitude())
    
    quiver = ax.quiver(*arrow, color='k', linewidths=2)
    quiverX = ax.quiver(*[0,0,0,arrow[3],0,0], arrow_length_ratio=0, color='r', alpha=0.4)
    quiverY = ax.quiver(*[0,0,0,0,arrow[4],0], arrow_length_ratio=0, color='g', alpha=0.4)
    quiverZ = ax.quiver(*[0,0,0,0,0,arrow[5]], arrow_length_ratio=0, color='b', alpha=0.4)


# Run the animation
ani = FuncAnimation(fig, update,  frames=np.linspace(0,2*np.pi,200), interval=0)
plt.show()