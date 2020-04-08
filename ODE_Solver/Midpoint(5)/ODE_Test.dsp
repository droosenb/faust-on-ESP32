import("stdfaust.lib");
Xi(expr) = si.bus(n) <: par(i,n,ba.selector(i,n)+1) : expr 
with { 
  n = inputs(expr); 
};
F1(expr) = si.bus(n) <: par(i,n,ba.selector(i,n)'') : expr <: par(i,n,ba.selector(i,m)*2)
with { 
  n = inputs(expr); 
  m = outputs(expr);
};
toto = os.osc(440),os.sawtooth(440), os.triangle(440);
funct = _ - _ <: _, _ ;
process = F1(funct);