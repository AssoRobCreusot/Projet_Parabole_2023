#!/usr/bin/python3
import pathlib
import pygubu
import serial
import time
import logging

PROJECT_PATH = pathlib.Path(__file__).parent
PROJECT_UI = PROJECT_PATH / "hem0gApp.ui"
PORT = "COM3"
BAUD = 9600
PORT_SERIAL = serial.Serial(PORT, BAUD, timeout=1)
RECEIVED_MSG = "start"
LOG_FILENAME = "file.log"   


class Hem0gApp:
    def __init__(self, master=None):
        self.builder = builder = pygubu.Builder()
        builder.add_resource_path(PROJECT_PATH)
        builder.add_from_file(PROJECT_UI)
        # Main widget
        self.mainwindow = builder.get_object("toplevel1", master)

        self.speed_var = None
        self.s = None
        self.message_var = None

        builder.import_variables(self, ['speed_var', 's', 'message_var'])

        self.speed = builder.get_object("speed")
        self.message = builder.get_object("message")
        self.time = builder.get_object("time")
        builder.connect_callbacks(self)

    def run(self):
        self.mainwindow.mainloop()

    def send_command(self):
        time = self.time.get()
        self.send_motor_control(self.speed.get(), time)



    def send_motor_control(self, speed, turning_time):
        sending_string = bytes("\r"+str(speed)+";"+str(turning_time)+"s;", "utf-8")
        PORT_SERIAL.write(sending_string)
        self.message_var.set("Commande envoyée, en attente de la réponse")
        self.message.update_idletasks()
        time.sleep(1)   
        message = self.receive_serial()
        if RECEIVED_MSG in message :
            self.message_var.set("Commande en cours")
            self.message.update_idletasks()
            tt = int(turning_time.replace("s",''))
            self.log.info("Starting recording for : " + str(tt) + "s")
            init_time = time.time()
            cont = True
            while cont :
                message = self.receive_serial()
                if len(message) > 0 :
                    self.log.info(message)
                self.message_var.set("Commande en cours - temps restant : "+ str(round(time.time()-init_time, 0)))
                self.message.update_idletasks()
                if "finish" in message : 
                    cont = False
                    break
                
            if not(cont) : 
                self.log.info("End of recording")
                self.message_var.set("Fin d'enregistrement")
                self.message.update_idletasks()
            else : 
                message = self.receive_serial()
                if "finish" in message :
                    self.log.info("End of recording")
                    self.message_var.set("Fin d'enregistrement")
                    self.message.update_idletasks()
                else : 
                    self.log.info("Error finish not received")
                    self.message_var.set("Erreur en fin d'enregistrement")
                    self.message.update_idletasks()


            
        else : 
            self.message_var.set("Il y a eu une erreur, essayez de relancer la commande")
            self.message.update_idletasks()

        time.sleep(3)
        self.message_var.set("Prêt pour nouvel envoi de commande")
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
    app = Hem0gApp()
    app.setupLogger(LOG_FILENAME)
    app.log.info("Program Started")
    app.log.info("data : pump;pressure1;pressure2;flow")
    app.run()


