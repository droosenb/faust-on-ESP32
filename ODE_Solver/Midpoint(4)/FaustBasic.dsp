import("stdfaust.lib");

//freq = nentry("freq", 440, 20, 2000, .01) : si.smoo;
sfreq = hslider("sFreq", 60, 0, 1000, .01);
sQ = hslider("sQ", 1, 0, 100, .001);

speaker = fi.resonbp(sfreq, sQ, 1);

lambda = hslider("lambda", 1, 0.001 , 1, .001);
phase = hslider("phase", 0, 0, 3.14, .01);
a0 =  hslider("michelsonAmpOut", 1, 0, 2, .01);

michelson(s) = s/lambda + phase : (cos)^2 * a0 : fi.dcblocker;

dfreq = hslider("dFreq", 600, 0, 1000, .01);
dQ = hslider("dQ", 1, 0, 100, .001);

detector = fi.resonlp(dfreq, dQ, 1);

process = os.osc(os.sawtooth(.1)*200 + 80) :  speaker: michelson(_) : detector * 4 <: _ , _;
 