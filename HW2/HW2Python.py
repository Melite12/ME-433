import board
import time
import pwmio

servo = pwmio.PWMOut(board.GP16, variable_frequency = True)
servo.frequency = 50
servo.duty_cycle = 60000

while True:
    for angle in range (0, 180, 1):
        servo.duty_cycle = int((0.02 + (angle/180.0)*0.11)*60000)
        time.sleep(0.01)
        print("Hello World")