# Electromagnetic 6-DOF Tracking for VR Applications

**Presenter:** Kirb (VRChat Meetup Lecture)  
**Source:** https://www.youtube.com/watch?v=iuzmvGJe8Bs  
**Date:** 2024 (estimated)

---

## Abstract

This document presents a comprehensive lecture on developing a low-cost electromagnetic 6-DOF tracking system for VR applications. The presenter, Kirb, discusses the limitations of existing VR tracking systems and demonstrates a custom-built magnetic tracking solution using 3D coils and sophisticated signal processing.

---

## Table of Contents

1. [Introduction](#introduction)
2. [Problems with Existing VR Tracking Systems](#problems-with-existing-vr-tracking-systems)
3. [Basics of Magnetic Field Tracking](#basics-of-magnetic-field-tracking)
4. [Existing Magnetic Tracking Systems](#existing-magnetic-tracking-systems)
5. [Project Implementation](#project-implementation)
   - [Magnetic Components](#magnetic-components)
   - [Coil Driver Design](#coil-driver-design)
   - [Tracker Hardware Design](#tracker-hardware-design)
   - [Signal Processing Pipeline](#signal-processing-pipeline)
6. [Results and Areas for Improvement](#results-and-areas-for-improvement)
7. [Future Development](#future-development)
8. [References](#references)

---

## Problems with Existing VR Tracking Systems

So all current full body tracking systems can be classified into two major categories which would be IMU systems or

optical systems. So, the IMU systems, they're this would include SlimeVR, HeraTorx or uh MoPop. I'm not sure if I

actually use that for VR, but uh these are very compact, immune to occlusion, and they get great battery life because

it's a very simple design. These shoes come from yaw drift, as well as they require body calibration because the

fact it's kinematics, the exact length of each one of your limbs to calculate the position. Uh it's not a true six

degrees of freedom system. It's three off that uses that before kinemax algorithm to get six degrees of freedom.

On the other hand, we have optical systems which would be Vive, Tundra or Slam in the case of the Vive ultimate

trackers which these get you stable true six degrees of freedom positioning. However, um and you don't ever have to

calibrate them as long as they stay still in your body. However, the issue is that you need a good optical environment for this. So if you have any

like mirrors or excessive IR light sources or anything that would complicate that, you'll have issues with

tracking. Uh their policies that require lots of photos and FPGA or cameras in a

CPU for slime systems uh as well as being very prone to occlusion uh as mentioned earlier.

So, what if there could exist a tracking system that would combine the advantages of both of these where you would have a tracker that's a full stable six degrees

of freedom positioning uh that can be fully oluded without losing tracking.

Well, that is what I'll be uh talking to you guys about today. So I will be going over the bas of how magnetic position

tracking works uh existing systems that use this technology and then going into

## Basics of Magnetic Field Tracking

visualization of a ideal magnetic dipole and for any position within here there is a known vector. So if you have a

device measuring the field intensity in the x and y axis in here, you could take the inverse tangent of that, that will

get you an angle. The angle corresponds the ratio of x to y position here. And

then doing the hypotenuse of that gets you an absolute intensity. This intensity is equal to the inverse square

of the distance from the center multiplied by a calibrated constant. And then there's the reference for this down

there. Uh, one issue that you'd see if there's only one field source is that if you rotate the device, because you're only

measuring X and Y from a single source, if you rotate it, the perceived angle will be different. So, it'll think it's

in a position where it's not. One uh way of fixing this is if you have uh two dipoles, one on each axis, uh from this,

if you're a if the sensor is able to differentiate which field is affecting it, then you can have both position and

rotation. So, this here would be a four degrees of freedom system where the sensor would know anywhere it is both um

translationally and rotationally within here. And then uh I'm not making a graphic for this cuz it would be a

complete mess. But uh for six degrees of freedom position, if you imagine another field that was going out of the screen

and back into it, that would get you three axes of position and rotation for a full six degrees of freedom uh

position. There is uh a bit of an issue with this where if the device is only measuring the magnetic field in X and Y,

the measurement in one corner will appear identical to the measurement in another corner. So there's kind of a

mirror position everywhere where plus X plus Y + Z is also valid for - X - Y - Z. This is known as the hemispheric

ambiguity. So to solve for this one property must be known and common between the

transmitter and the receiver. Some ways of doing this would be phase detection since as you cross over the center your

phase would be you would shift the phase by 180 degrees. So this sign you know be inverted at one end. Uh for this you

would need a reference signal on the device to compare phase against which adds a bit of complexity. You can also

use multiple transmit and receive coils. So if you'd imagine you'd have one transmitter here, one transmitter here and the sensor is somewhere in the

middle. uh because it's receiving the signal from both of them, it would be able to validate that it's not in this mirror position here or this one because

it wouldn't be receiving the same signal from the other source with that. The way that I went about solving this was uh

having an IMU inside the device which always knows which way is down and we assume that the transmitting coil is

always in one orientation cuz normally you don't want to move your base stations. Bad things can happen if you do that.

So we always assume one way is down from the base station and then we measure using the IMU which way is facing down

which would invalidate one of the possible positions. Uh some existing systems that have used

this technology uh one of the earliest ones is the Razer Hydra. This was introduced around the beginning of VR.

So think like Oculus DK1 days like very early before the Vive. These were a set

of desktop VR controllers that were not wireless. They were wired. However, they use the magnetic tracking positioning

system which proved to be uh very effective for this. However, they were very limited in their range because it

only has a 20 to 30 cm tether. So, you can have like the base station on your desk and then you can only move your

controllers like that far. It's not really designed for room scale or full body tracking. These were quickly

discontinued because the Vive Wands and the Steam VR tracking system came out and basically was better than this in

every way aside from the occlusion issue. A slightly more recent implementation of this is the Pico Neo2,

which what this headset did was it used typical slam tracking like a Quest headset where there's cameras on the

headset that position the headset within the room. But if you notice, these controllers don't have any kind of tracking rings or cameras on them. Uh

the trackers position themselves in relation to the headset using electromagnetic positioning system. So

the field source, it's kind of done in a backwards way to how these systems typically work where the field source is in the controllers and then the sensor

is in the headset. So kind of like switch between which controller is transmitting and the headset is able to

work out the position from the received field from that. There is a really neat video by Kai VR on this headset where he

bought one and just used it for a while and it was the only headset in existence that you can put your hands in your

pockets and maintain controller tracking. Uh this was very great. However, the rest of the headset kind of

sucked. It was very short-lived and discontinued quickly after the Quest 2 released.

Uh finally, there's one currently in production uh system that uses electromagnetic position tracking. This

is uh Amphitra. They have actually released a demo on their YouTube channel where they've put one of their sensors

on a headset and then put four field base stations in each corner of the room and they're able to get uh HMD tracking

which is on the same level of precision as slime or laser tracking. Uh this is also still in active development.

However, the issue with this system is that it is very expensive for consumer VR. Their development kit, which just

contains three trackers and one field source, runs €3,500, which uh for that price, you could get a

headset, a set of Vive trackers, and a computer. So, I don't know of anyone that actually uses this for VR due to

the price. So this all the issues with the existing systems and lack of availability is what led me to creating

my own um full body tracking system using electromagnetics. Starting with the actual magnetic

components themselves. So as I mentioned earlier, there are um implementations of

this already for those VR uh VR applications I mentioned as well as some medical applications. These devices do

exist. However, they are very niche. You can't just go on like mouse or DJ key and buy these. It's direct from

manufacturer and if you're not buying like 500 of them, they will not even tell you the price.

So, I found these um coils on AliExpress, which are very similar to

the Primo coils used in the Amputra system. They're relatively cheap and I believe these are replacement parts for

Audi car keys where they had kind of a system where if you hold the key near like one door, it'll only unlock that door. Like near the trunk, it'll only

open the trunk because it can co-osition itself with the car itself. Um, but yeah, I was able to get a data

sheet for these, the footprint and everything, and they proved to be suitable for this application. As far as

the base station magnets go, I wasn't able to find any uh available parts for

this online other than like contact manufacturers who won't give you a quote unless you're buying like 500 of them.

So, I wound my own around a 3D printed base. This was uh just printed in PLA

and then used 20 gauge uh copper wire or enamelcoated copper wire to wind around it using a stepper motor to kind of

speed up the process a bit. Uh it was still quite a timeconuming process because you have to make sure that every winding is very tight on here and

they're all wound in the correct order. One issue that came up with this was

because it was printed in PLA which has a relatively low glass transition temperature. as the coil heated up

during use, it would slightly deform the plastic over time. So, if you see in the image up there, that is when it was

being wound. The windings are very nice and tight there. No gaps between them. However, after it's been extensively

used here, you can see there's very there's a lot of loose spots in the coils because it melted and kind of warped the coil base inwards a bit.

To drive the coil, it it is designed to be resonant at 20 kHz, 25 kHz, and 30

kHz. each axis is running at a different frequency. So, uh you're able to

differentiate on the sensor from XY and Z-axis by uh measuring pulling a bin out

of the FFT for that frequency. So, in as I mentioned earlier where you need to

differentiate between which dipole is affecting the measurement,

you can do that through frequency separation. The inductance of the colay wound ranges

from 274 to 336 microhenre's which is a bit of variance. So the tuning

capacitance had to be a bit variable to allow it to resonate at the correct frequencies. The range of this was about

80 to 100 nanofarads. And the ESR of the entire system was a bit higher than ideal at 1.2 ohms which led to the coil

heating issues that I mentioned earlier. Uh the coil is also designed to be uh

have an impedance close to 50 ohms at these frequencies which made development a lot easier. So before this coil driver

was developed uh we were able to just hook the coil up to a 50 ohm output

function generator and have minimal issues from impedance mismatch to drive it. This is what the actual circuit that

is used for driving the coil looks like. It's um a bit cursed but worked pretty well. So the way that this works is you

have an Hbridgeidge and then inside of that H bridge you have the RLC circuit.

So it will start with the Hbridgeidge in one polarity. The current will begin flowing through the RLC and as it oscillates the current will eventually

go negative through that. So when the current goes negative there's a shunt resistor at the bottom that will measure

the current off that and then when there is a zero cross detected across it, it will flip the orientation of that Hbridgeidge. So as the H bridge switches

the current through the actual RLC itself is a very nice smooth sinosoid and the current as seen from the supply

is kind of a sign bouncing on that zero crossing. So we never have to generate a negative voltage or have a direct power

amplifier connection to this. It is uh the current is controlled by the supply

voltage. So as you vary what voltage is being supplied with you can change the current which is what we did for some uh

closed loop current control. And the frequency is fixed as I mentioned set by a tuning capacitor. This is the actual

PCB used for that coil driver. So there's three of those RLC circuits. Up the top there you can see the H bridge

fans supporting components. As I mentioned the current through it is controlled by the supply voltage. So

there's a variable buck converter on each axis. So in addition to the

frequency counter and zero cross detection, there's also a peak detector which will measure the peak current

through that and then we can do software to convert that to RMS and then run a closed loop algorithm. So if the current

through the coil is higher than it needs to be the supply voltage is decreased and vice versa. This is uh powered by

USB PD 15VT 3 amp. It's just a readily available way to get 15 volts which is a

suitable drive voltage for this. Uh the kind of quirk you get with powering something over USB is that if you have a

high capacitance system with lots of switching components and H bridges here uh you have a lot of capacitance there

and the USB spec does not allow for more than 10 microfarads I believe of capacitance on BB bus. So there's a FET

that once uh power is negotiated, it will slowly turn the gate of that FET on to soft start all those capacitors and

not induce excessive inrush current. There's also a software override on here

for starting and stopping. The oscillator is not actually able to

self start itself. So what has to be done is you manually switch the H bridge at approximately the right frequency a

few times until the oscillation builds up. Then it can switch to the free running mode. As far as stopping it goes, this turned out to be a way bigger

issue than uh was actually expected. So when you have the drive current that uh

I was running on here was about 500 milliamps RMS. If you have that current flowing and then you suddenly open all

the FETSS, that current will want to keep flowing somewhere and usually does not find a very nice path to keep

flowing. There was a lot of IC's that got damaged by them just being the path that the current took. A way to solve

this is to just lock the H bridges in one position. There's always a path for the current to keep flowing, which would

be from ground to the supply rail. And because you're not actively switching and adding more current to it, the

oscillations will eventually die out and then it can be opened without damaging anything.

The tuning of the coil driver was done using an array of capacitors on the back of it. So, we're able to step response

the coil and see the actual frequency of it there. if it's too high or too low, more or less capacitance can be added to

these bus bars to tune it in to operate at the desired frequency.

Uh, as I mentioned earlier, it uses USB PD as the power source, but that USB

port is actually the only port on the device. So, if you want to do like development or debugging on it, it has

to be done through that port. And if you want to debug while it's powered, you'll need a way to get both USB PD power as

well as data through the same port. So this is what led me to create this um kind of abomination here where you take

data from one port, power from the other port, and combine into one type-C port. Uh device looks like this. This is what

happens when you make a silk screen in a Discord call.

## Coil Driver Design

my project the magnetics design the coil driver design and the hardware of the tracker as well as the full signal path

## Tracker Hardware Design

Moving on to the hardware design of the tracker. So what this must do is sample

the signals from the minute sensor, perform both frequency domain and time domain analysis on the signal, then transmit that process um data to the

computer. I'll go over the hardware design of this in three sections being power, digital circuitry, and the analog

front end. Starting with the power circuitry, there are three power rails

in the device that uh must run from a lithium cell. That'd be the low voltage analog front end supply, the

microcontroller digital supply, and the low uh low power always on supply.

Uh because it's a lithium battery, it also must be charged. There's a 5 watt switch mode battery charger in here and

a voltage based fuel gauge. The I found an IC that was very suitable

for this, the LTC 3586. It's a monolithic PUMIC that integrates a USB

compliant power path regulator and charger. So, if you plug this into a USB outlet that's only able to provide 500

milliamps, it'll only charge that. Or if it's able to provide an amp or one and a half amps, it'll charge at the max it's able to there. The chip integrates four

programmable switching regulators, only two of which I actually use. So, that' be one of them is for generating the

5volt analog frontend supply voltage. One is for the 3.3 volt logic supply. So, that' be a boost and a buck boost.

There are two other converters in it that are just unused as of now. Uh it also integrates an LDO for the low power

circuitry. The battery indication was done through a series of comparators

here. Uh never do this. This wastes an enormous amount of PCB space as well as

you need to route like a million traces across to where all your LEDs are when you could just like have an AC on there

and do it all in software. Uh the device has a singular button on that's used to

control both uh powering on powering off and the digital state of the device to

there's a hard power off circuit implement in here where if you hold the button for more than 15 seconds without any software involvement it can fully

cut power to it. The way that was achieved was whenever the button is pressed, you'll discharge this 10

microfarad capacitor through a 1 megga ohm resistor which will eventually fall below the threshold of the Schmidt

trigger and force every enable line to go low as a foolproof way of killing all

the power. Uh this ran from a 1.3 hour lipo or 1.3 amp lipo battery which was

able to grant about 4 to 6 hours of run time. Some issues did pop up with the

power supply in here where when I was doing some testing of the noise floor of the device just in an RF chamber isolate

from all outside signals where there's only one induced signal around 30 kHz which uh is what the coil would be there

was always no matter what the test setup was a peak at roughly 4 kHz here. Uh,

Tempo, a member of the DJ Techch community, suggested this would be a beat frequency between multiple free

running switching inverters, which after some EMC probing turned out to be true. This uh PUMIC here, all of these

switching rails run at roughly 2.25 MHz. However, each of them is running from its own oscillator. So, there's a slight

frequency shift between them just due to tolerance of the component, which is what created that beat frequency that

popped up in the noise floor. Moving on to the analog front end. What

this must do is amplify the very weak signals coming from the coil the sensor

to a level that's readable by the ADC while providing sufficient dynamic corction on clip when it's close to

transmitter. So this the intensity of the signal on this coil follows the inverse square law. So if you're very

close to transmitter you might see a signal of a few volts. However, if you're really far it'll very quickly

fall off to the micro microvolt range. uh inverse square law. So moving it away by a factor of two will decrease your

signal by a factor of four. Uh unwanted frequencies must also be

heavily filtered out here as the low frequency magnetic band there's a lot of interference here. So anything like a

switching power supply any motor driven by PLM or even speakers can produce noise in this frequency band which can

couple into here. So those filters those frequencies must be filtered out. Uh, another consideration with the analog

front end is that it can't be overloaded when the presence of a very strong field. So, an example this would be putting on top of like a transformer

under a heavy load that would induce a very heavy 60 Hz signal or putting it on top of a wireless charger that would

also induce a huge voltage in the coil that shouldn't damage anything.

For the design of the analog front end, each channel has its own dual opamp for mitigating the cross talk. I could have

went with a single opamp for all the channels to simplify the bomb cost. However, the issue with that is that

let's say this is a very onaxis uh the measure from this coil is very onaxis and the other coil is very offaxis. So

you have a very weak signal here, very strong signal here. You don't want those to couple into each other and introduce

position error. So each amplifier is fully separate. The pass band of the filter is 15 kHz to 35 kHz which gives a

5 kHz guard band on either side of the signal. Uh this also make sure that 60 Hz which

is absolutely everywhere gets very well filtered out. Um since like if anywhere

near like a power line in your wall or transformer or anything you don't want that to mess with your dynamic range. Uh

on the topic of strong coming in there's a just very basic diode clipping network which clamps the signal to the supply

voltage of the amplifier to make sure that it doesn't damage anything. Since it is a single supply design, there's no

negative voltage being generated here. Every virtual ground point and filter has to be biased to a reference voltage.

This uh there's a single reference voltage dist distributed across every amplifier of 2 and 1/2 volts. This is a

two quadrant reference generated by an op amp. So if you have multiple filters sourcing current at the same time, it's

the reference voltage generator is able to source all that. and that they're all syncing. It's also able to sync that

without raising or lowering the voltage. Uh, as I mentioned, there's a lot of dynamic range needed for this. So, like

I said, like you'll have signals in the level of a few volts when you're very close or microvolts when you're far away. There's a set of FETSS here that

will change the bias or change which resistor is effectively used in the feedback network of the amplifier to

switch the gain digitally between 30 dB and 100 dB.

Finally, the last stage of the pipeline is the digital circuitry, which this samples the signals that are coming out

of the analog front end to perform the signal processing on this and convert time domain into frequency domain and

then send this data over to the computer. Um, this also something that had to be

done out of spite was under voltage protection because that really fancy PUMIC that's designed to be used with

lithium batteries I use, there's no undervoltage feature on that. So, usually with a lithium battery, you'd

want to discharge it down to only about 3.2ish volts, but this PMIC, if you give

it 2 volts, the boost converters will just keep trying to go up to what it's supposed to output. So, the undervoltage

had to be implemented in software to not overdischarge the batteries.

The main mic controller used on the device was the ESP32S3. It's a very powerful 240 MHz dualcore

mic controller with an integrated 2.4 4 GHz radio. It's also quite the power hog. This thing is the sole reason why

this isn't able to get like 10 plus hours of battery life. This alone while running all the digital signal

processing and uh transmitting the data wirelessly uses 200 to 300 milliamps

which is quite significant. The ADCs used were uh there's two 24-bit

96 kil sample per second ADCs. Each one's dual channel. There's a total of four channels. So three of those

channels are used for the X, Y, and Z axes respectively. And then the last channel is used for generating the or

for measuring the reference voltage. The reason why the reference voltage is measured is there's still a small amount

of noise on there. So you'll get like a couple microvolts of noise if you have like multiple filters syncing into at

the same time, which will slightly raise that voltage up. So if we measure the

voltage on that reference or the voltage of that reference and invert that signal and add it to all the other ones, we're

able to cancel out any combo noise that would exist between all three channels. There's also a the IMU used for the uh

mentioned earlier hemispherical ambiguity. Uh it's a BMI 270 just a kind of basic Bosch IMU. All that it's really

doing here is measuring which way is down. However, through further software development, there is a capability of

sensor fusion and interpolation for this. So, you could reduce any jitter by

kind of doing sensor fusion with the IMU and also fall back to purely integration

of acceleration when the signal drops. Uh, it would also be possible to run like a slime VR like algorithm when the

signal drops out. With these the PCB used for the tracker was a

fairly standard four layer board stackup signal power ground signal. The it's

actually impossible to make one of these without using four layers at least using a coil like the one I used here because

um if you look at the pin mapping here the x-axis coil starts here and terminates here. Y-axis starts here and

terminates here. Z-axis starts here and terminates here. So you have three

crossovers here that all have to be on different layers because you're running the signals differentially. So like the

you'd have the differential pairs, one here, one here on different layers of the board so that they don't ever

interact and they're as close to each other as possible. Once that gets down to the input of the analog front end, it

converts it from a differential signal to a single-ended signal as the entire analog front end is a single-ended

design. uh the coil you'll also mention there is

no well very minimal copper solder mask or anything around that that's because

uh once again to the inverse square law if you have any metal that's nearby the sensor you'll have field distortion

around it which causes positioning error that can be calibrated out however attenuation will push your signal closer

down to the noise floor and reduce performance overall

uh another board that was implemented in here which wasn't really necessary was this flexible printed circuit that

integrated 32 addressable LEDs. This was used for like power state indication as

well as um kind of a demo that I was planning on doing where it would always have a compass on it that points towards

the base station. It's kind of like a very easy to demonstrate way that it's functioning.

This uh WS202s get very bright at full duty cycle. So, I actually for all

indication purposes, I ran them at like three or 4% duty cycle. When it's at 100, showing white, the thing turns into

a quite bright flashlight that lights an entire room while drawing an amp from the battery. Uh, this housing is not

actually supposed to be transparent. It just looks that way because of the amount of light shining through the thing.

The housing of it is a four-piece 3D print. Um, fairly simple. They just have

main housing, button, the cover for the FPC, and a snap fit lid. Um, I went with

SlimeVR style straps that kind of mount out the side like this. The reason why I'm going with a slime style mount

instead of the standard Vive mount is the Vive mount, you'll have like a tripod screw that goes into the device

and then that goes onto a strap. However, the strap, you don't know how much metal is used in the construction of that. So it could be just like a

small screw underneath the tracker. That's all the metal there is. Or it could be like a full metal base plate

which would go under the coil and shunt the field from there. So to eliminate any error you might get from a strap

that contains a lot of metal is just using these loop points for Velcro straps to attach to.

## Signal Processing Pipeline

Uh so in total the full signal path of a device like this would start with one

closed loop for driving the coils. So you have your variable power supply running to the zero current switching

driver resonant tank and the coil generates a field. The current through these coils is measured goes through the

first closed loop back to the power supply. So this loop here the goal of that is to maintain the transit power at

a constant current from the generated magnetic field. There's the free space and off-axis attenuation. And then we

get picked up by the pickup coil sensor on the tracker. This goes into the second closed loop which is the variable

gain analog front end which gets sampled by the ADC and then we run digital signal processing. Time domain digital

signal processing runs back to the gain switching logic. So if you have a certain amount of signals that are above

a threshold or if you're clipping at the top of it, you'll switch from the high

gain range to low gain range. And then if you're at the low gain range and then no signals fall above threshold, you

switch to the high gain range to maintain the maximum dynamic range of the system. The other output from the

digital signal processing is the frequency domain data. This is done through a fast 4A transform algorithm

and then we isolate the frequencies from it. So the 20 25 and 30 kHz from the

field generator those get isolated to just single FFT bins which are sent over

a wireless link to the computer which runs the positioning algorithm and the visualizer. Uh over on this side here I

have some of the actual output data from the device. So this is kind of a graph

over time of what the signal looks like. So each row here is the axis from the

transmitting coil. So transmitting coil X axis, transmitting coil Y axis, transmitting coil Z-axis and then each

row is the receiving coil. So you have a 3x3 matricy that you pass through to the

positioning algorithm. This is a full visualization with a bit more data where we have the raw fast for transform

output of the device. This is time domain data and then the 3D visualization of the position of the

device. So in the end I was able to produce five

trackers and one base station. Uh the functioning prototype I made has a detection range of roughly 2 m before

the signal falls below the noise floor. Uh the tracking jitter was not great once again to do noise floor issues. So

at a distance of 30 cm from the transmitter you get about 10 cm of position jitter and like I mentioned the

battery life is 4 to 6 hours. So with this prototype here, I was able to demonstrate that the viability of a

lowcost main tracking system for VR is possible. However, the current state of this prototype uh due to the noise floor

issues is not usable for VR. So there's some room for improvement with the

design uh such as filtering in more advanced sensor fusion algorithms

actually incorporating the IMU into the uh positioning solving stack further tuning of the analog front end and power

line filtering the 5V analog supply range while it is uh low noise there's

currently no filtering on that so you get a bit of noise from your switching

power supply coupling into the output signal uh Power efficiency could also use a lot of improvement. Right now, the

transmitting coil while active draws about 40 watts and then the receiving device draws about 300 milliamps from

the battery. So, you're able to get 4 to 6 hours. However, if this switched from the ESP32 platform to NRF, that could

drastically improve the battery life as well as directly driving the uh coil

from a power amplifier instead of an RLC circuit. The thing with the RLC circuit is it's very simple to kind of build and

design this it to operate this way. However, as the coil heats up, the inductions will slightly change. Same with the capacitors. You've got a

temperature coefficient on everything. So, as it warms up, your frequency might shift from like 20.5 kHz to like 20.4

20.3. It'll just slowly shift around over time, which means you'll have to actively correct for that on the tracker

to know which FFT bins to pull out. And this also would make a heterodiner lock in amplification system impossible as

you would have to constantly be synchronizing the phase and frequency where if you were driving it directly

from a power amplifier that's able to just do a fix fixed frequency fixed amplitude you can have a uh almost exact

reference signal on the device to compare against. What do I want the future of this

project to look like? I'm currently in the progress of a full system redesign uh within the DJ tech community. A lot

of ideas have been suggested such as using a lock in amplifier or heterodine

architecture where this would instead of just using FFT to pull out the frequency domain data, you have a reference

oscillator either in analog or in digital on the device which gets

multiply gets mixed with the incoming signal pull out the signal uh a single frequency content which could in the

case of a locking amplifier be below the noise floor which would improve the tracking performance. Uh there's also

been suggestions for more signal processing and filtering utilizing multiple sensors or further uh

developing the algorithms. Uh so I would like to end this off on a note saying that if anyone has any interest in the

project or ideas for how they can improve it. I just want to see it develop. Uh don't hesitate to head over

to the Nentech Discord. invite is there and I'd love to see some feedback about

the design or anything that could be improved. Thank you all for coming to my lecture

and now it's time for questions.

## Results and Areas for Improvement

and any conclusions that uh I got from the project. So let's start with the basis here. Uh this graph here is a

## References

- **Source Video:** https://www.youtube.com/watch?v=iuzmvGJe8Bs
- **VRChat Meetup:** Electromagnetic 6-DOF tracking lecture
- **Presenter:** Kirb (Electronics Technologist, Calgary, Alberta)

### Related Systems Mentioned

- **Razer Hydra:** Early magnetic VR controllers
- **Pico Neo 2:** Headset with electromagnetic controller tracking
- **Amphitrack:** Professional electromagnetic tracking system
- **Polhemus/Ascension:** Medical electromagnetic tracking

### Technical Components

- **Coils:** 3D11-722J (AliExpress replacement for Audi key fobs)
- **Microcontroller:** ESP32-S3 (240MHz dual-core)
- **ADCs:** 24-bit, 96ksps dual-channel ADCs
- **IMU:** BMI270 (Bosch)
- **Power Management:** LTC3586 (USB PD charger + regulators)

---

*This document is a reformatted transcript of Kirb's VRChat meetup lecture. Original content preserved with improved structure for technical reference.*
