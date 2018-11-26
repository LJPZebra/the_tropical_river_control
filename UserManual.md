# User Manual The Tropical River

## Writing a protocol

Two mains writing protocol style:

- Relative time: instructions separated by a waiting time in milliseconds.

- Absolute time: instructions executed at a given time in seconds until the start of the protocol.


### Serials

The Tropical River comes with build-in serial integrated in the Ui:

- heater: control the river temperature via a PID
- river: temperature sensor and peristaltic pump.
- valves: control the injection by the
- fl1: control the flow rate inside The Tropical River.

Any Arduino device that follows the sketch standard can be connected add-on. Get the list of all Arduino devices connected by port series by taping in the serial terminal panel:

```ls serial```

To communicate with the device types:

```device::command:value```

### Protocol standard

#### General commands

1. To print a message in the status bar:
    ```print::Your message```

2. To control the camera:
    ```camera::start```
    ```camera::stop```

3. To make a pause:
    ```wait::time in ms```

4. To control a serial device:
    ```serialId::command:value```

5. To create a directory to save image, protocol and parameter files:

  ```data::create directory```

#### Writes protocol with relative time

Writes instructions separated by wait time:

```camera:start
camera::start
fl1::setFlow:10
wait::1000
fl1::setFlow:20
wait::1000
fl1::stop
```

In this protocol the camera will start, then the flow rate is set to 10, 1 second later it is set to 20 and then 1 second later it will stop.

#### Writes protocol with absolute time

Writes instructions as time until start->command.

```camera::start
0->camera::start
0->fl1::setFlow:10
1->fl1::setFlow:20
2->fl1::stop
```

#### Notes

Do not mix the two writing style!

### Example

For example you have a flow controller named **fl** and you want to record during 6 seconds and set the flow rate to 1L / min:

```
# Example protocol
print:Starting protocol...

# Create directory and save parameter and protocol files
data:create directory

# Start images recording
camera:start

#Set flow rate of fl device to 1
fl::setFlow:1

# Wait
wait:6000

fl:stop

# Stop images recording
camera:stop
print:Protocol ended

```



## Add a device by serial port

### Arduino device

The connection of an Arduino device is automatic if the device follows the following guidelines:

1. The serial server has to have a **getId** method that gives an id to the device.
2. The serial server has to accept input as **command:value** or **command**.

### Non Arduino device

The connection of a non Arduino device has to be manual by creating a QSerialPort object.

## Serial terminal

A console to talk to devices (only Arduino automatically detected) is available in the terminal panel. You can list connected devices by taping **ls serial**. All commands allowed by the devices are available. For example if you want to set the flow rate of a device named **fl** that has a **setFlowRate** method just tapes:

```fl::setFlowRate:20``` 



## Software modularity

The organization of this software made it easy to reuse for other purpose. 

* If you follow the Arduino guidelines you can play protocols with any Arduino devices without modifying the code.
* You can add many non Arduino devices by adding a class in the source (see for example the Nestlab_Rte class).
* You can change the ui by modifying the mainwindow class without changing the internal mechanic (protocol playing, camera recording etc...). 