from math import sin, cos, pi

sines = []
coses = []
for x in range(64):
	t = x / 64.0 * 2 * pi
	sines.append(255 * (1 + sin(t)) / 2);
	coses.append(255 * (1 + cos(t)) / 2);

sines = ", ".join(map(str, map(int, sines)))
coses = ", ".join(map(str, map(int, coses)))

print "static uint8_t mysin[64] = { %s };" % sines
print "static uint8_t mycos[64] = { %s };" % coses
