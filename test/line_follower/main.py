import numpy as np
import cv2
import color_detect as cd


def main():
    cap = cv2.VideoCapture(1)

    while True:
        ret, frame = cap.read()

        kernel = np.ones((5, 5), np.uint8)

        black = cd.detectBlack(frame, 50, kernel)

        frame = cd.getError(black, frame)

        cv2.imshow('Original', frame)
        cv2.imshow('Black', black)

        if cv2.waitKey(1) & 0xFF == ord('x'):
            break


if __name__ == "__main__":
    main()
