# -*- coding: utf-8 -*-
"""
Created on Thu Aug  5 00:00:03 2021

@author: Fernando Sandoval
"""
import serial
from tkinter import *
from serial import Serial
from tkinter.font import Font
import time
import sys
#ONLY UNCOMMENT WHEN YOU ARE GOING TO TRANSFER ITEMS
#ser = serial.Serial('COM5',baudrate=9600,timeout=1)
class Window(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)               
        self.master = master
        self.init_window()
    def init_window(self):
        self.master.title("COM5")
        self.pack(fill=BOTH, expand=1)
        START= Button(self, text="START")
        START.place(x=130, y=20)
        SEND = Button(self, text="SEND")
        SEND.place(x=220, y=75)
        TXT = Label(self, text="Counter Value: ").place(x=105, y=60)
        self.name = Entry(TXT)
        self.name.focus()
        self.name.place(x=85, y=80)
        POT_1 = Label(self, text = 'POTENTIOMETER 1').place(x=40, y = 120)
        POT_2 = Label(self, text = 'POTENTIOMETER 2').place(x=160, y = 120)        
root = Tk()
root.geometry("300x200")    
app = Window(root)
root.mainloop()
ser.close()
print('COM IS DONE')
sys.exit(0)
