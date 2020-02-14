import("stdfaust.lib");

freq = nentry("freq",440,20,1000,0.01) : si.smoo;
gain = nentry("gain[acc: 0 0 -10 0 10]",0.5,0,1,0.01) : si.smoo;

process = os.sawtooth(freq)*gain;
