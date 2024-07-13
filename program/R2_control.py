import IM920sL
import keyboard

if __name__ == "__main__":
    im920 = IM920sL.IM920("COM18")
    key_name = ['w','s','d','a','e','q']
    while True:
        data = [0,0,0,0,0,0,0,0]
        for i in range(8):
            if keyboard.is_pressed(f'{i+1}'):
                data[0] |= 1<<i
        for i in range(6):
            if keyboard.is_pressed(key_name[i]):
                data[1] |= 1<<i


        print(data)
        im920.byte_write(data)