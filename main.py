from models import GaltonBoard, Experiment, shwoHistogram

if __name__ == "__main__":
    num_slots = 30
    num_rows = 40
    num_balls = 1000
    
    num_trials = num_balls * 2
    num_experiments = 1

    experiments: list[Experiment] = []

    for _ in range(num_experiments):
        galton_board = GaltonBoard(num_slots, num_rows, num_balls)
        galton_board.setStartSlot(10)

        experiment = Experiment()
        experiment.run(galton_board, num_trials)

        experiments.append(experiment)

    print(len(experiments[0].boards_slots))

    shwoHistogram(experiments[0].getAverageSlots())
