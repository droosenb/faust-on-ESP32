import("stdfaust.lib");

F1(x, y, d) = d(x, y)'' * 2; 
F2(x, y, d) = F1 : _ + x, _ + y : d(_, _); 
//midpoint = 

sho(x, v) = v , -w^2 * x
with{
    w = 200;
};

process = F1(_, _, sho);