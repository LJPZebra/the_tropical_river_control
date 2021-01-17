# User Manual The_Tropical_River control software

## Writing a protocol

Two mains protocol writing styles are available:

- Relative time: instructions separated by waiting times in milliseconds.

- Absolute time: instructions executed at a given time in seconds since the start of the protocol.


### Serials

The Tropical River comes with build-in serial devices integrated into the UI:
- heater: control the river temperature via a PID.
- river: temperature sensor and peristaltic pump control.
- valves: switches valves to inject products or water in the injector.
- fl1: control the flow rate inside The Tropical River.

Any Arduino device that follows the sketch standard (see below) can be connected add-on. Get the list of all Arduino devices connected by port series by taping in the serial terminal panel:

```ls serial```

To communicate directly with the device types:

```device::command:value```

### Protocol standard

#### General commands

1. To print a message in the status bar:
    ```print::Your message```

2. To control the camera:
    ```camera::start```
    ```camera::stop```

3. To control a serial device:
    ```serialId::command:value```

4. To create a directory to save images, protocol, and parameter files:

    ```data::create directory```

#### Write a protocol with relative time

 Instructions are separated by a wait time in milliseconds:

```camera:start
camera::start
fl1::setFlow:10
wait::1000
fl1::setFlow:20
wait::1000
fl1::stop
```

In this protocol, the camera will start, then the flow rate is set to 10, 1 second later it is set to 20, and then 1 second later it will stop.

#### Write protocol with absolute time

The time in seconds (since the beginning of the protocol) at which the instruction must be executed is indicated before the instruction as time->instruction.

```camera::start
0->camera::start
0->fl1::setFlow:10
1->fl1::setFlow:20
2->fl1::stop
```

#### Notes

Do not mix the two writing styles!

### Example

For example, if you want to record an experiment where the flow rate is set to 1 L/s for 6 seconds:

```
# Example protocol in relative time
print::Starting protocol...

# Create a directory and save parameter and protocol files
data::create directory

# Start images recording
camera::start

#Set flow rate of fl device to 1
fl::setFlow:1

# Wait
wait::6000

fl::stop

# Stop images recording
camera::stop
print::Protocol ended

```

``````
# Example protocol in absolute time
t0->print::Starting protocol...

# Create a directory and save parameter and protocol files
t0->data::create directory

# Start images recording
t0->camera::start

# Set flow rate of fl device to 1
t0->fl::setFlow:1

# Stop flow rate
t6->fl:stop

# Stop images recording
t6->camera:stop
t6->print:Protocol ended
``````

## Add a device by serial port

### Arduino device

The connection of an Arduino device is automatic if it follows this guideline:

1. The Arduino serial server has to have a **getId** method that gives an id to the device.
2. The Arduino serial server has to accept input as **command:value** or **command** if no value is needed.

### Non-Arduino device

A non-Arduino device's connection has to be manually added in the source code (see, for example, the Neslab_Rte class).

## Serial terminal

A console to talk to devices (only Arduino automatically detected) is available in the terminal panel. You can list connected devices by taping **ls serial**. All commands allowed by the devices are available. For example, if you want to set the flow rate of a device named **fl** that has a **setFlowRate** method, just type:

```fl::setFlowRate:20``` 

## Todo

- Temperature manual control without PID.
- Offset in the PID temperature.
- Camera display widget auto-resize but conserving the aspect ratio. 
