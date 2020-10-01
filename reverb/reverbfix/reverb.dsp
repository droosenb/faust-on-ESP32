import("stdfaust.lib");

ap(gain, delay, delay_max) = ( + <: de.delay(delay_max, delay -1 ), *(-gain)) ~ *(gain): _' + _; 

ap_constant(gain, delay) = ( + <: de.delay(delay, delay -1), *(-gain)) ~ *(gain): _' + _; 

osc(freq) = rdtable(tablesize, os.sinwaveform(tablesize), int(os.phasor(tablesize,freq)))
with{
    tablesize = 1 << 15; // instead of 1 << 16
};

onepole(gain) =  _*gain :( +)~*(gain); 

bandwidth = hslider("/bandwidth", .65, 0 , 1, .01);
indiff1 = hslider("/diff/in1", .7, 0 , 1, .01);
indiff2 = hslider("/diff/in2", .7, 0 , 1, .01);
decaydiff1 = hslider("/diff/decay1", .75, 0 , 1, .01);
decaydiff2 = hslider("/diff/decay2", .625, 0 , 1, .01);
damping = hslider("/damping", .38, 0 , 1, .01);
decay = hslider("/decay", .87, 0 , 1, .01);
wet = hslider("/mix", .87, 0 , 1, .01);
ingain = hslider("/ingain", 1, 0 , 2, .01);

inputdiffuser(gain1, gain2) =  ap_constant(-gain1, 142) : ap_constant(-gain1, 107) : ap_constant(-gain2, 379) : ap_constant(-gain2, 277);

lefttank(gain1, gain2, gain3, gain4) = ap(gain1, 672 + 16*osc(2/3), 686) : _@(4453) : onepole(gain2) : ap(gain3, 1800 + 16*osc(19/17), 1816) :  @(3720)*(gain4); 

righttank(gain1, gain2, gain3, gain4) = ap(gain1, 908 + 16*osc(5/7), 686) : _@(4217) : onepole(gain2) : ap(gain3, 2656 + 16*osc(13/11), 1816) :  @(3162)*(gain4); 

tank(in) =  (+(in):  lefttank(decaydiff1, damping, decaydiff2, decay) <: +(in), _ : righttank(decaydiff1, damping, decaydiff2, decay), _) ~ _ : _' + _ + in; 


process = _*(ingain) <: onepole(bandwidth), _ : inputdiffuser(indiff1, indiff2), _ : tank, _: _* wet + _ * (1-wet) <: _, _;

