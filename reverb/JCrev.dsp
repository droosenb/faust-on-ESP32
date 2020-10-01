import("stdfaust.lib");

process = re.jcrev :> _, *(-1) :> _ <: _, _;
