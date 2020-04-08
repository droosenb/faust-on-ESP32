import("stdfaust.lib");

F1(expr) = si.bus(n) <: par(i,n,ba.selector(i,n)') : expr <: par(i,n,ba.selector(i,m)*2/ma.SR)
with { 
  n = inputs(expr); 
  m = outputs(expr);
};

F2(expr) = si.bus(n) <: si.bus(n) , F1(expr) <: par(i, n, ba.selector(i, 2*n) + ba.selector(i+n,2*n)'*.5) : expr <: par(i,n,ba.selector(i,m)*2/ma.SR)
with { 
  n = inputs(expr); 
  m = outputs(expr);
};

midpoint(expr) = si.bus(n) <: si.bus(n), F2(expr) <: par(i, n, ba.selector(i, 2*n) + ba.selector(i+n,2*n))
with { 
  n = inputs(expr); 
  m = outputs(expr);
};

solve(expr) = si.bus(n) ~ midpoint(expr)
with{
    n = inputs(expr); 
    m = outputs(expr);
};

//define the ode's for the Simple Harmonic Oscillator
sho(x, v) = v+v0,  -1 * w^2 * x 
with{
    w = hslider("freq", 200, 100, 1000, .01)*ma.PI;
    v0 = start*100;
    c = 1/10000;
    start = button("start");
};
//define the ode's for the Lorentz atrractor
lorentz(x, y, z) = o * (y - x) + x0, x * (p -z) - y, x*y - b*z
with{
    o = hslider("sigma", 10, 0, 1000, .1);
    p = hslider("rho", 140, 0, 10000, .1);
    b = hslider("beta", 2.7, 0, 1000, .1);
    x0 = start;
    start = button("start");
};

//process = solve(sho);
process = solve(lorentz) : fi.dcblocker*.0005 + fi.dcblocker*.0005, fi.dcblocker*.0005 ; 