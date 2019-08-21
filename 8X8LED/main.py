from kivy.app import App
from kivy.uix.screenmanager import ScreenManager,Screen
from kivy.uix.widget import Widget
from kivy.uix.button import Button
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.gridlayout import GridLayout
import kivy
import numpy as np
from kivy.core.window import Window 
from kivy.config import Config

import serial

Config.window_icon = "./test1.jpg"


Window.size = (600, 800) 


kivy.require("1.11.1")

###########################################
buf=np.ones((8,8))
buf=buf.astype('uint8')
ser=serial.Serial('COM4',4800,timeout=0.8)#不一定是COM4，暂时没写串口检测
#全局变量
###########################################

class LedButton(Button):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        
    def status(self):
        #global buf
        pos_x=int(self.id.split('_')[1])
        pos_y=int(self.id.split('_')[2])
        if buf[pos_x][pos_y]==1:
            buf[pos_x][pos_y]=0
        else:
            buf[pos_x][pos_y]=1
        if buf[pos_x][pos_y]==0:
            self.background_color=[0.1,0.9,0.2,1]
        else:
            self.background_color=[1,1,1,0.8]
        
class Background(BoxLayout):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.btns=[]

        for i in range(8):
            for j in range(8):
                btn = LedButton(text='',id='led_%d_%d'%(i,j))
                self.ids.LEDs.add_widget(btn)
                self.btns.append(btn)


    def create_hex(self):
        base=np.array([128,64,32,16,8,4,2,1])
        base=base.astype('uint8')
        hex_num=''
        
        for line in buf:
            
            hex_num=hex_num+hex(sum(base*line))+','
        
        self.ids.hex.text=hex_num
    
    def clear(self):
        buf[0:8,0:8]=1
        for btn in self.btns:
            btn.background_color=[1,1,1,0.8]

        self.create_hex()
        
    def send_data(self):
        self.create_hex()

        if ser.isOpen():
            print("open success")
        else:
            print("open failed")
        hex_num=self.ids.hex.text.split(',')
        hex_str=''
        
        for hex in hex_num:
            if hex!='':
                if hex=='0x0':
                    hex_str+='00'
                else:
                    hex_str=hex_str+hex[2:]+' '
        
        hex_str=bytes.fromhex(hex_str)   
        print(hex_str)
        ser.write(hex_str)
        
class LED_dots(App):
    def build(self):
        self.icon='test1.jpg'
        return Background()


if __name__ == '__main__':
    LED_dots().run()
