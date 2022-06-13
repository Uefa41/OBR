import numpy as np
import cv2


def detectBlack(img, threshold, kernel):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    _, mask = cv2.threshold(gray, threshold, 255, 0)
    mask_opened = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)

    return mask_opened

def getError(mask, frame):
    height, width = mask.shape

    black_center = width // 2
    i = 0
    for line in mask:
        black_count = np.sum(line == 0)
        black_pos = np.where(line == 0)

        black_center = np.sum(black_pos) // black_count

        cv2.circle(frame, (black_center, i), 1, (0, 0, 255), 1)
        i+=1

    return frame


