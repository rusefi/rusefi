*PID CIC*
A PID implementation with a modified cascaded integrator-comb (CIC) filtering.
Used for incremental auto-IAC control. See autoIdle() in idle_thread.cpp
22 Dec 2017.

(Also see explanation in Russian here: https://rusefi.com/forum/viewtopic.php?f=9&t=1315)

*** A bit of background ***

@darxfame and @andreika have been experimenting with some of the PID auto-tuning methods for IAC (Ziegler etc.). 
They tested the obtained coefficients on the existing ("classic") PID implementation in rusEFI and looked at what was going on using the debug logs. 
It's observed that the I-term tends to exceed the min/max limits very often, especially at the initial stages of PID tuning.
This greatly distorts the results and negates the perception of the reaction of the system when selecting parameters. 
In the classic PID implementation, the I-term is limited to min/max values of the PID itself. And that's by no means always acceptable, 
especially for high-inertia engines and slow IAC actuators, where the integrator is highly accumulated.

The simplest solution, proposed in the comments of the PID implementation, was to simply add separate (and extended) limits for the I-term. 
But this approach has one drawback: it cannot "forget" past errors, relying on the fact that the PID is already well tuned, 
and the error will inevitably decrease. However, in the case of an unconfigured PID or other reasons, 
when self-oscillations of the controller appear (aka "oscillation build-up"), this does not happen. The integrator still bumps into the limits, even if extended. 
But, most importantly, the errors accumulated "inside" the I-value are too old and no longer relevant to the current position of the controller.

*** The proposal ***

Therefore, the idea came up: to make a time window for the integrator (measured in N seconds, depending on the setting of the PID period), 
and then there will be no accumulation of past non-relevant errors, and the I-term itself will be naturally limited. 

Unfortunately, we cannot afford the "straightforward" implementation of the running sum, because you will have to store all error values in a large buffer 
during the entire integration window (and it can be tens of seconds). 

Therefore, a cascade integrator has been implemented - an algorithm like a CIC filter (cascaded integrator-comb), 
which requires much less memory for its buffer, and at the same time allows to maintain a running sum in a rather large window.

*** The implementation details ***

Actually, only two cascades are currently used (to save more memory) and, accordingly, one circular buffer - 
this is enough for the integration window with a width of more than 10 seconds, even for the smallest period of 10 ms. 
Moreover, it occupies 32 * 4 bytes of RAM. We also use buffer averaging to smooth out the transition between buffer positions 
(otherwise a "sharp" CIC can trigger PID excitation).

The algorithm is implemented in a separate PidCic class, and thus there is a possibility to choose any of the available PID implementations - old and new. 
This also allows to declare a large memory buffer (iTermBuf) only if needed - making the "classic" PID still affordable for memory-limited board configs.
Backward compatibility is also fully preserved and there will be no inconvenience for existing users.

This algorithm has been already tested on a real car, yet the testing is still ongoing.
