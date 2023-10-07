from .galtonBoard import GaltonBoard


class Experiment:
    def __init__(self):
        self.boards_slots = []

    def run(self, galton_board: GaltonBoard, num_trials: int):
        for _ in range(num_trials):
            galton_board.run()
            self.boards_slots.append(galton_board.slots.copy())
            galton_board.reset()

    def getAverageSlots(self) -> list:
        average_slots = [0] * len(self.boards_slots[0])

        for board_slots in self.boards_slots:
            for i, slot in enumerate(board_slots):
                average_slots[i] += slot
            
        for i, slot in enumerate(average_slots):
            average_slots[i] = slot / len(self.boards_slots)
        
        return average_slots

