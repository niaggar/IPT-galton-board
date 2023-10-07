import numpy as np
import matplotlib.pyplot as plt

from .galtonBoard import GaltonBoard


def shwoHistogram(slots) -> None:
    plt.bar(range(len(slots)), slots)
    plt.show()