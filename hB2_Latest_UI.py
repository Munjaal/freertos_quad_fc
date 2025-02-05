import socket
import time
import threading
from queue import Queue, Empty
import numpy as np
import struct
# import flet as ft

print('{0:07b}'.format(12))

commands = {
    "BEAT": {"id": 0, "length": 0},
    "ZER_IMU": {"id":  1, "length": 0, "dtype":None},
    "FLY_TKO": {"id":  2, "length": 4, "dtype":"int32"},
    "FLY_ALT": {"id":  3, "length": 4, "dtype":"int32"},
    "FLY_LND": {"id":  4, "length": 0, "dtype":None},
    "SET_PHI": {"id":  5, "length": 4, "dtype":"fp32"},
    "SET_THT": {"id":  6, "length": 4, "dtype":"fp32"},
    "SET_PSI": {"id":  7, "length": 4, "dtype":"fp32"},
    "GET_ATT": {"id":  8, "length": -12, "dtype":"RECV"},
    "SET_OMG": {"id":  9, "length": 16, "dtype":"int32"},
    "SET_XYZ": {"id": 10, "length": 12, "dtype":"fp32"},
    "GET_XYZ": {"id": 11, "length": -12, "dtype":"RECV"},
    "SET_STP": {"id": 12, "length": 0, "dtype":None},
    "GET_BAT": {"id": 13, "length": -4, "dtype":"RECV fp32"},
    "GET_STT": {"id": 14, "length": -1, "dtype":"RECV"}, #NOT IMPLEMENTED
    "REB_OOT": {"id": 15, "length": 0, "dtype":None},
    "SET_KP":  {"id": 16, "length": 4, "dtype":"fp32"},
    "SET_KI":  {"id": 17, "length": 4, "dtype":"fp32"},
    "SET_KD":  {"id": 18, "length": 4, "dtype":"fp32"}
}

def communication_thread(arduino_ip, arduino_port, command_queue):
    sent_time = 0
    recv_time = 0
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((arduino_ip, arduino_port))
        while True:
            # Always send heartbeat first
            # print(bytes([commands["BEAT"]["id"]]))
            s.sendall(bytes([commands["BEAT"]["id"]]))
            sent_time = time.time()
            # print(f"{sent_time}: Heartbeat Sent")
            data = s.recv(1)
            if data == b'a':  # Check for ACK from Arduino
                recv_time = time.time()
                # print(f"{recv_time}: Heartbeat ACK received")
                print(f"Delta T: {recv_time - sent_time}")      #Print the timedelay between heartbeat send and ack recv
                # THE ABOVE VALUE MUST BE LOW FOR SAFE FLIGHT
                pass
            else:
                print("No ACK received")
                pass
                

            # Check if there are other commands to send
            try:
                # Non-blocking check for command in the queue
                cmd_total = command_queue.get_nowait() 
                cmd_name = cmd_total[0]
                if cmd_name in commands:
                    cmd_id = commands[cmd_name]["id"]
                    cmd_length = commands[cmd_name]["length"]
                    if cmd_length == 0:
                        s.sendall(bytes([cmd_id]))
                        print(f"Sent command {cmd_name}")
                    elif cmd_length > 0:
                        if len(cmd_total[1]) == cmd_length/4:
                            if cmd_total[2] == "int32":
                                payload = cmd_total[1]
                                msg = bytes([cmd_id])
                                for _data in payload:
                                    msg += _data.to_bytes(4,'little')
                                print(msg)
                                s.sendall(msg)
                            elif cmd_total[2] == "fp32":
                                payload = cmd_total[1]
                                # int_representation = struct.unpack('I', struct.pack('f', cmd_total[1]))[0]
                                # payload = int_representation
                                msg = bytes([cmd_id])
                                print(cmd_length)
                                for _data in payload:
                                    int_rep = struct.unpack('I', struct.pack('f', _data))[0]
                                    msg += int_rep.to_bytes(4,'little')
                                # for i in range(int(cmd_length/4)):
                                #     msg += payload.to_bytes(4,'little')
                                print(cmd_id)
                                print(msg)
                                s.sendall(msg)
                                pass
                        else:
                            print("CMD LENGTH MISMATCH")
            except Empty:
                pass
            
            time.sleep(0.625)



def send_cmd(cmd,payload=[]):
    if not cmd in commands:
        print('Invalid command.')
        return
    
    if commands[cmd]["length"] != 0:
        if len(payload) != commands[cmd]["length"]/4:
            print("Invalid payload length.")
            return
    
    print(cmd)
    print(payload)

def main(arduino_ip, arduino_port):
    command_queue = Queue()
    thread = threading.Thread(target=communication_thread, args=(arduino_ip, arduino_port, command_queue))
    thread.start()

    # Simulate user input or command triggers
    time.sleep(2.5)  # Wait some time before sending commands

    # command_data = [1100,1100,1100,1100]
    # cmd = "SET_OMG"
    # command_queue.put((cmd,command_data,commands[cmd]["dtype"]))
    # time.sleep(0.7)

    # command_data = [0]
    # cmd = "REB_OOT"
    # command_queue.put((cmd,command_data,commands[cmd]["dtype"]))

    command_data = [0]
    cmd = "SET_KP"
    command_queue.put((cmd,command_data,commands[cmd]["dtype"]))
    time.sleep(2)

    while(True):
        cd = float(input())
        command_data = [cd]
        cmd = "SET_KD"
        command_queue.put((cmd,command_data,commands[cmd]["dtype"]))
        time.sleep(0.7)

    # command_data = [0.15]
    # cmd = "SET_KI"
    # command_queue.put((cmd,command_data,commands[cmd]["dtype"]))


    # More commands can be queued here as needed


if __name__ == "__main__":
    ARDUINO_IP = '192.168.1.11'  # Replace with the correct IP address
    ARDUINO_PORT = 80        # Replace with the correct port
    main(ARDUINO_IP, ARDUINO_PORT)

