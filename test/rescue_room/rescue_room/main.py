import numpy as np
import cv2

def main():
    cap = cv2.VideoCapture(3)

    while True:
        ret, frame = cap.read();

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        _, black = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)

        cv2.imshow('Original', frame)
        cv2.imshow('Black', black)

        if cv2.waitKey(1) & 0xFF == ord('x'):
            break


if __name__ == "__main__":
    main()
