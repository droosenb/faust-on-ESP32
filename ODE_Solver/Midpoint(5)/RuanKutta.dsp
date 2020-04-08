import("stdfaust.lib");

F1(expr) = si.bus(n) <: par(i,n,ba.selector(i,n)) : expr <: par(i,n,ba.selector(i,m)''*2/ma.SR)
with { 
  n = inputs(expr); 
  m = outputs(expr);
};

F2(expr) = si.bus(n) <: si.bus(n) , F1(expr) <: par(i, n, ba.selector(i, 2*n) + ba.selector(i+n,2*n)*.5) : expr <: par(i,n,ba.selector(i,m)'*2/ma.SR)
with { 
  n = inputs(expr); 
  m = outputs(expr);
};

midpoint(expr) = si.bus(n) <: si.bus(n), F2(expr) <: par(i, n, ba.selector(i, 2*n) + ba.selector(i+n,2*n))
with { 
  n = inputs(expr); 
  m = outputs(expr);
};

sho(v, x) = -1*w^2 * x , v
with{
    w = 1/200;
};

lorentz(x, y, z) = o * (y - x), x * (p -z) - y, x*y - b*z
with{
    o = 10;
    p = 14;
    b = 2.7;
};


process = button("start") : (_, +) ~ midpoint(sho); 