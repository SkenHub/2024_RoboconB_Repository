import tkinter as tk
import IM920sL 

class DebugGui():
    def __init__(self,window_name="DebugWindow",geometory="1024x600"):
        self.im920 = IM920sL.IM920("COM15")
        self.im920_data = [0]*4
        self.x_vel = 0
        self.y_vel = 0
        self.omega = 0
        
        #ウィンドウ設定
        self.root = tk.Tk()
        self.root.title(window_name)
        self.root.geometry(geometory)
        #リセットボタンを作成
        self.home_button = tk.Button(text = "ホームに戻る",font = ("MSゴシック","20"))
        self.home_button.bind('<Button-1>',self.GUI_reset)
        self.home_button.place(x=830,y=10)
        #各ボタンの作成と設置
        self.main_menu()
        self.flag = True
        #while self.flag:
        #    self.mecanum_publish()
        self.root.mainloop()

    def main_destroy(self):
        try: self.manual_botton.destroy() 
        except: pass
        try: self.FB_scale.destroy()
        except: pass
        try: self.RL_scale.destroy()
        except: pass
        try: self.Omega_scale.destroy()
        except: pass
        try: self.reset_button.destroy()
        except: pass
        for i in range(15):
            try: self.param_entry[i].destroy()
            except: pass
            try: self.param_label[i].destroy()
            except: pass
            try: self.param_button[i].destroy()
            except: pass
        try: self.set_param_button.destroy()
        except: pass

    def GUI_reset(self,event):
        self.main_destroy()
        self.main_menu()
    
    def update_velo(self,event):
        self.x_vel = self.RL_scale_var.get()
        self.y_vel = self.FB_scale_var.get()
        self.omega = self.Omega_scale_var.get()
        print(f"{self.x_vel}    {self.y_vel}    {self.omega}")
        self.mecanum_publish()

    def reset_velo(self):
        self.x_vel = 0
        self.y_vel = 0
        self.omega = 0
        self.FB_scale_var.set(0)
        self.RL_scale_var.set(0)
        self.Omega_scale_var.set(0)
        print(f"{self.x_vel}    {self.y_vel}    {self.omega}")
        self.mecanum_publish()

    def main_menu(self):
        self.main_destroy()
        #手動操作
        self.manual_botton = tk.Button(text="手動操作",font=("メイリオ","20"),command=self.manual_control)
        self.manual_botton.place(x=20,y=60,width=450,height=200)
    
    def manual_control(self):
        self.main_destroy()
        self.reset_button = tk.Button(text="リセット",font=("メイリオ","20"),command=self.reset_velo)
        self.FB_scale_var = tk.IntVar()
        self.RL_scale_var = tk.IntVar()
        self.Omega_scale_var = tk.IntVar()
        self.FB_scale    = tk.Scale(variable=self.FB_scale_var   ,orient=tk.VERTICAL  ,length=500,width=50,sliderlength=40,from_=  1500,to= -1500,tickinterval=500,command=self.update_velo)
        self.RL_scale    = tk.Scale(variable=self.RL_scale_var   ,orient=tk.HORIZONTAL,length=500,width=50,sliderlength=40,from_= -1500,to=  1500,tickinterval=500,command=self.update_velo)
        self.Omega_scale = tk.Scale(variable=self.Omega_scale_var,orient=tk.HORIZONTAL,length=300,width=50,sliderlength=40,from_= -180 ,to=  180, tickinterval=45 ,command=self.update_velo)
        self.reset_button.place(x=800,y=500)
        self.FB_scale.place(x=50,y=60)
        self.RL_scale.place(x=300,y=270)
        self.Omega_scale.place(x=400,y=400)

    def mecanum_publish(self):
        self.im920_data[0] = self.x_vel
        self.im920_data[1] = self.y_vel
        self.im920_data[2] = self.omega
        print(self.im920_data)
        self.im920.write(self.im920_data)

if __name__ == "__main__":
    gui = DebugGui()
    while True:
        gui.mecanum_publish()