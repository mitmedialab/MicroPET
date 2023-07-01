import serial
import time

def start_experiment():
    ser = serial.Serial('/dev/tty.usbmodem91431001')  # open serial port
    ser.write('START'.encode())  # write a string

def log_experiment():
    ser = serial.Serial('/dev/tty.usbmodem91431001')  # open serial port
    print(ser.name)  # check which port was really used
    ser.write('START'.encode())  # write a string

    # while True:
    #     print(ser.readline().decode("utf-8"))
    #         # seq.append(i)  ## convert from ACSII?
    #         # joined_seq = ''.join(str(v) for v in seq)  ## Make a string from array
    #         #
    #         # if i == '\n':
    #         #     print("Line: " + str(count) + "" + str(
    #         #         datetime.datetime.now()) + joined_seq)  ## append a timestamp to each row of data
    #         #     seq = []
    #         #     count += 1
    #         #     break
    ser.close()

if __name__ == '__main__':
    # start_experiment()
    time.sleep(3);
    log_experiment()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
