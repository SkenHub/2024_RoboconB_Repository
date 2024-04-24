import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import math 
import numpy as np
import tkinter as tk

class ParaSimu():
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("放物線シミュレーション")
        self.root.geometry("1024x512")
        self.first_velocity = 0
        self.first_degree = 0
        self.ene = 0
        self.mass = 1
        self.x_vel = 5
        self.y_vel = 5
        self.make_GUI()
        self.make_plot(self.mass,self.x_vel,self.y_vel)
        self.drawing_plot()
        self.root.mainloop()
    
    def simulation(self):
        self.first_velocity = float(self.vel_entry.get())
        self.first_degree = float(self.deg_entry.get())
        self.mass = float(self.mass_entry.get())
        self.x_vel = self.first_velocity*math.cos(self.first_degree*(2*np.pi/360.0))
        self.y_vel = self.first_velocity*math.sin(self.first_degree*(2*np.pi/360.0))
        self.ene = 0.5*self.mass*(self.first_velocity*self.first_velocity)
        
        self.Ene_label["text"] = "運動エネルギー：" + str(self.ene) + "[J]"
        self.fig.clear()
        self.canvas.get_tk_widget().destroy()
        self.make_plot(self.mass,self.x_vel,self.y_vel)
        self.drawing_plot()

    def make_GUI(self):
        #ラベル
        self.vel_label = tk.Label(text="初速度　　　　　　　  [m/s]",font=("MSゴシック",20))
        self.deg_label = tk.Label(text="射出角度　　　　　　 [deg]",font=("MSゴシック",20))
        self.mass_label = tk.Label(text="質量　　　　　　　　　 [kg]",font=("MSゴシック",20))
        self.Ene_label = tk.Label(text="運動エネルギー",font=("MSゴシック",20))
        self.vel_label.place(x=650,y=50)
        self.deg_label.place(x=650,y=100)
        self.mass_label.place(x=650,y=150)
        self.Ene_label.place(x=650,y=270)
        #エントリー
        self.vel_entry = tk.Entry(font=("MSゴシック",20))
        self.deg_entry = tk.Entry(font=("MSゴシック",20))
        self.mass_entry = tk.Entry(font=("MSゴシック",20))
        self.vel_entry.place(x=770,y=50,width=100)
        self.deg_entry.place(x=770,y=100,width=100)
        self.mass_entry.place(x=770,y=150,width=100)
        #ボタン
        self.set_button = tk.Button(text="シミュレーション",font=("MSゴシック","20"),command=self.simulation)
        self.set_button.place(x=650,y=200)

    def make_plot(self,mass,x_vel,y_vel):
        self.fig = plt.figure()
        plt.xlim(-1.0,11.0)
        plt.ylim(-0.3,3.0)

        time = np.linspace(0,11.0/x_vel,110)
        x_dis = [None]*110
        y_dis = [None]*110
        for i in range(110):
            x_dis[i] = x_vel*time[i]
            y_dis[i] = -0.5*9.80665*(time[i]*time[i]) + y_vel*time[i]
        plt.plot(x_dis,y_dis)
        plt.grid()
    
    def drawing_plot(self):
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.root)
        self.canvas.get_tk_widget().pack(side=tk.LEFT,fill=tk.BOTH)
        self.canvas.draw()


if __name__ == "__main__":
    Para = ParaSimu()