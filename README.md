The Teensy MIDI Sequencer is a standalone sequencing system based on the Teensy 4.1.   
It features eight polyphonic tracks, each with eight clips. Every clip consists of  
96 MIDI ticks and allows up to twelve simultaneous notes. Clips can be arranged   
into a linear song structure using an arranger that supports up to 256 bars.   
Each bar can store parameters such as clip selection, transposition, volume,   
probability, and a set of 16 freely assignable MIDI CC commands.    
The song progression can be looped with customizable start and end points.

Each track can have individual settings such as clock division, clip length,   
MIDI output channel, musical scale, and active Playmode.   
  
Multiple play modes are available for sequence generation.    
   
Playmode	Description    
🎲 Random	Mono  -    mode with random notes, inspired by Marbles or Turing Machine   
🔁 Analog	Mono  -  step sequencer with a classic potentiometer-style aesthetic   
⭕ Euclid     -  	Polyphonic for all 12 voices, with adjustable steps and rotation (Euclidean rhythm)   
🎚 Rclid	      -    Monophonic with controlled or random variation, similar to "Random"   
🥁 BeatArray	-    128 drum patterns for all 12 voices – perfect for rhythmic experimentation   
🧮 bitRead	   -   Interprets a number from 0–256 as an 8-bit pattern and plays it across 12 voices   
🪂 DropSeq	  -    A "weighted" or subtractive sequencer with random reduction or density   

All play modes can be transferred into a clip of choice for further editing.   
They can also be used in arrangement mode and deactivated if needed.   
   
In addition to MIDI control, the system includes an audio engine with 16 internal plugins,   
a mixer, and three effects (reverb, bitcrusher, delay).   
Audio output is handled by the Teensy Audio Board.   
    
The system is designed to run with external clock sync at 24 PPQN,   
but it can also operate in internal sync mode.
