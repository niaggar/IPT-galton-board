import random

class GaltonBoard:

    def __init__(self, num_slots: int, num_rows: int, num_balls: int):
        self.num_slots = num_slots
        self.num_rows = num_rows
        self.num_balls = num_balls
        self.startSlot = 0

        self.slots = [0] * self.num_slots

    def setStartSlot(self, startSlot: int):
        self.startSlot = startSlot

    def run(self):
        for _ in range(self.num_balls):
            self.dropBall()

    def dropBall(self):
        slot = self.startSlot
        for _ in range(self.num_rows):
            direction = self.getDirection()

            tempSlot = slot + direction
            if tempSlot >= 0 and tempSlot < self.num_slots:
                slot = tempSlot
        
        self.slots[slot] += 1

    def getDirection(self):
        return random.choice([-1, 0, 1])
    
    def reset(self):
        self.slots = [0] * self.num_slots
