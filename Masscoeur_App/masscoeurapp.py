#!/usr/bin/python3
import pygubu
import serial
import time
import logging 

PROJECT_PATH = "./"
PROJECT_UI = PROJECT_PATH + "Masscoeur.ui"
PORT = "/dev/ttyACM0"
BAUD = 9600
PORT_SERIAL = serial.Serial(PORT, BAUD, timeout=1)
RECEIVED_MSG = "start"

class MasscoeurApp:
    def __init__(self, master=None):
        self.builder = builder = pygubu.Builder()
        builder.add_resource_path(PROJECT_PATH)
        builder.add_from_file(PROJECT_UI)
        # Main widget
        self.mainwindow = builder.get_object("toplevel1", master)

        self.motor = None
        self.start_motor = None
        self.command_sended = None
        builder.import_variables(
            self, ['motor', 'start_motor', 'command_sended'])
        self.motor_speed = builder.get_object("motor_speed")
        self.motor_start_speed = builder.get_object("motor_start_speed")
        self.message = builder.get_object("message")
        self.time = builder.get_object("time")
        # self.command_sended = builder.get_object("command_sended")
        builder.connect_callbacks(self)

    def run(self):
        self.mainwindow.mainloop()

    def send_command(self):

        time = self.time.get()
        self.send_motor_control(self.motor_start_speed.get(), self.motor_speed.get(), time)



    def send_motor_control(self, start_speed, speed, turning_time):
        sending_string = bytes("\r"+str(start_speed)+";"+str(speed)+";"+str(turning_time)+";", "utf-8")
        PORT_SERIAL.write(sending_string)
        self.command_sended.set("Commande en envoye")
        self.message.update_idletasks()
        time.sleep(1)   
        message = self.receive_serial()
        print(message)
        if RECEIVED_MSG in message :
            self.command_sended.set("Commande en cours")
            self.message.update_idletasks()
            tt = int(turning_time.replace("s",''))
            init_time = time.time()
            while (time.time() - init_time) < tt :
                time.sleep(1)
                self.command_sended.set("Commande en cours - temps restant : "+ str(round(time.time()-init_time, 0)))
                self.message.update_idletasks()
            message = self.receive_serial()
            if "finish" in message :
                self.command_sended.set("Reponse de l'arduino : Fin de tache")
                self.message.update_idletasks()

            
            
        else : 
            self.command_sended.set("Il y a eu une erreur")
            self.message.update_idletasks()
        time.sleep(5)
        self.command_sended.set("Pret pour nouvel envoi de commande")
        self.message.update_idletasks()

    def receive_serial(self):
        received_str = PORT_SERIAL.readline().decode('utf-8')
        return received_str

    def setupLogger(self, filename):    
        logger = logging.getLogger('Serial Logger')
        logger.setLevel(logging.DEBUG)
        # create file handler which logs even debug messages
        fh = logging.FileHandler(filename)
        fh.setLevel(logging.DEBUG)
        # create console handler with a higher log level
        ch = logging.StreamHandler()
        ch.setLevel(logging.DEBUG)
        # create formatter and add it to the handlers
        formatter = logging.Formatter('%(asctime)s ~ %(name)s ~ %(levelname)s ~ %(message)s')
        fh.setFormatter(formatter)
        ch.setFormatter(formatter)
        # add the handlers to the logger
        logger.addHandler(fh)
        # logger.addHandler(ch)
        # Return the Created logger
        self.log = logger

if __name__ == "__main__":
    app = MasscoeurApp()
    app.run()
