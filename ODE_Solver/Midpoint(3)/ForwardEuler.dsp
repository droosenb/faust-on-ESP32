import("stdfaust.lib");


fEuler(expr) = si.bus(n) <: si.bus(n), expr <: par(i, n, (ba.selector(i, 2*m) + ba.selector(i+m,2*m)/ma.SR))
with { 
  n = inputs(expr); 
  m = outputs(expr);
};

sho(x, v) = v+v0,  -1 * w^2 * x 
with{
    w = hslider("freq", 200, 100, 1000, .01);
    v0 = start;
    c = 1/10000;
    start = button("start");
};

lorentz(x, y, z) = o * (y - x), x * (p -z) - y, x*y - b*z
with{
    o = 10;
    p = 14;
    b = 2.7;
};


process = (_,  _) ~ fEuler(sho);