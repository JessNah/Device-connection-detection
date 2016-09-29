# Device-connection-detection
Uses a class GUID to find all devices of that type connected to the PC, when the searched for device is found (USB mouse in this case), it is indicated as having been found and connected. The program then starts a timer to keep a connection status check on the device, checking every 5 seconds to ensure that it is still connected or if it has been disconnected



<img src="https://cloud.githubusercontent.com/assets/14356838/18960703/abdf70ee-8638-11e6-9102-a8381b2e0cf8.png" >

Above images provides an example of the code searching for a mouse
Displaying its connection status every 5 seconds.
In this test, I unplugged the mouse and replugged it in,
showing that the program is able to dynamically recognize disconnection/reconnection events
