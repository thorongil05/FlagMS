# Flags Handler

## Idea

In car competitions, expecially in the Formula 1, there is a system of flags that is useful to signal important information to the drivers through a visual form.
The flags could be used in the case of off-track or crashes.

The system provides:

- sensors to check if a veicle crossed the track limits
- actuators to change the color of the flags, in order to signal the danger to the drivers.
- a command line tool

The flags could assume three different colors:

- green: the situation is ok
- yellow: there is an off-track, drivers must be careful in that part of the track
- red: there is a huge danger or there was a serious crash, drivers must come back on the boxes.

## The track-limit sensor

The sensor behaviour is simulated through a function

```cpp
bool isCrossed() {
    int p = 1 + rand()%100;
    return p <= 10;
}
```

## The actuator

The flag, for simplicity, is represented as a led.

## CLI

The command line interface will provide a menu, in which the user can:

- read the status of every sector of the circuit
- change the flag in a specific sector