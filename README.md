Docs is in the [wiki](https://github.com/thorongil05/IoT-Project/wiki)



# Flags Handler

## Idea

In car competitions, expecially in the Formula 1, there is a system of flags that is useful to signal important information to the drivers through a visual form.
The flags could be used in the case of off-track or crashes.

The flags could assume three different colors:

- green: the situation is ok
- yellow: there is an off-track, drivers must be careful in that part of the track
- red: there is a huge danger or there was a serious crash, drivers must come back on the boxes.

## Implementation

The system provides:

- sensors to check if a veicle crossed the track limits
- actuators to change the color of the flags, in order to signal the danger to the drivers.
- a command line tool

### The track-limit Node

The sensor behaviour is simulated through a function

```cpp
bool isCrossed() {
    int p = 1 + rand()%100;
    return p <= 10;
}
```

### The actuator node

The flag, for simplicity, is represented as a led.
We have two process-threads:

- the first one, in which there is the registration of the resource
- the second one, to handle the behaviour of the temporarly flag: here there is a timer that is triggered by a post request

### CLI

On start-up, on a separate thread, the application handles the registrations of the resources.

The user can interact with a menu, in which he can:

- show the flags registered.
- show the tracklimits registered.
- check the status of the flags, typing the name of a flag to see its actual color.
- change the status of the flags, typing the name of the flag and the color to set.
