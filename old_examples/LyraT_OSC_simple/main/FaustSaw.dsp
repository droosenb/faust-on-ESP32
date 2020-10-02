import("stdfaust.lib");

freq = hslider("freq", 440, 20, 2000, .01) : si.smoo;

process = os.sawtooth(freq);
