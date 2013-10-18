import pygame
from pygame.locals import *

screenSize = [800,600]

pygame.init()
screen = pygame.display.set_mode(screenSize)

DURATION = 1.0
TICKS = 50 # i.e. frequency
TICKLEN = DURATION / TICKS

TICKSCREENWID = screenSize[0] / TICKS

data = [0] * TICKS

YSCALE = 127

def dump():
	print [int(YSCALE * (1 + d)) for d in data]

def grid():
	for i in range(TICKS-1):
		pygame.draw.line(screen, (255, 0, 0), ((i+1)*TICKSCREENWID-1, 0), ((i+1)*TICKSCREENWID-1, screenSize[1]))

def updatetick(idx, val):
	"""val [-1,1]"""
	leftx = idx * TICKSCREENWID
	topy = screenSize[1] / 2
	boty = topy + screenSize[1] / 2 * val
	pygame.draw.rect(screen, (0, 0,0), (leftx, 0, TICKSCREENWID-1, screenSize[1]))
	pygame.draw.rect(screen, (0,255,0), (leftx, topy, TICKSCREENWID-1, boty-topy))
	pygame.display.flip()
	data[idx] = val

def mouseclick(pos):
	tick = pos[0] // TICKSCREENWID
	val = (float(pos[1]) - screenSize[1] / 2) / (screenSize[1] / 2)
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
