import pygame
from pygame.locals import *

SCREENWID = 800
SCREENHEI = 600

pygame.init()
screen = pygame.display.set_mode((SCREENWID, SCREENHEI))

DURATION = 1.0
TICKS = 42
TICKLEN = DURATION / TICKS

TICKSCREENWID = float(SCREENWID) / TICKS

YSCALE = 63

data = [0.5] * TICKS

def dump():
	print ", ".join([str(int(YSCALE * d)) for d in data])

def grid():
	for i in range(TICKS-1):
		pygame.draw.line(screen, (255, 0, 0), ((i+1)*TICKSCREENWID-1, 0), ((i+1)*TICKSCREENWID-1, SCREENHEI))
	pygame.draw.line(screen, (255, 0, 0), (0, SCREENHEI/2), (SCREENWID, SCREENHEI/2))

def updatetick(idx, val):
	"""val [-1,1]"""
	leftx = idx * TICKSCREENWID
	topy = SCREENHEI / 2
	height = 2 * (val - 0.5) * SCREENHEI / 2
	pygame.draw.rect(screen, (0, 0,0), (leftx, 0, TICKSCREENWID-1, SCREENHEI))
	pygame.draw.rect(screen, (0,255,0), (leftx, topy, TICKSCREENWID-1, height))
	pygame.display.flip()
	data[idx] = val

def mouseclick(pos):
	tick = int(pos[0] // TICKSCREENWID)
	val = float(pos[1]) / SCREENHEI
	val = float(int(val * YSCALE)) / YSCALE

	updatetick(tick, val)

def doevents():
	for event in pygame.event.get():
		if event.type == QUIT:
			return False
		elif event.type == KEYDOWN:
			if event.key == K_ESCAPE:
				return False
			elif event.key == K_SPACE:
				dump()
		elif event.type == KEYUP:
			pass
		elif event.type == MOUSEMOTION:
			if event.buttons[0]:
				mouseclick(event.pos)
		elif event.type == MOUSEBUTTONUP:
			if event.button == 1:
				mouseclick(event.pos)
		elif event.type == MOUSEBUTTONDOWN:
			pass
		elif event.type == ACTIVEEVENT:
			pass
		else:
			print "Unhandled event",event
			print event.type
	return True

def main():
	grid()
	pygame.display.flip()
	while True:
		if not doevents():
			return
	pass

if __name__ == "__main__":
	main()
