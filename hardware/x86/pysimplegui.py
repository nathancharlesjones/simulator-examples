#!/usr/bin/env python3

import PySimpleGUI as sg

max_accel = 10

layout = [  [[sg.Text("Max accel"), sg.Input(default_text='10', key='max_accel', enable_events=True)]],
            [sg.Slider((-max_accel, max_accel), orientation='horizontal', key='X')],
            [sg.Slider((-max_accel, max_accel), orientation='horizontal', key='Y')],
            [sg.Slider((-max_accel, max_accel), orientation='horizontal', key='Z')]]

window = sg.Window('Simulator Example', layout)

while True:
    event, values = window.read()   # Read the event that happened and the values dictionary
    if event == sg.WIN_CLOSED:     # If user closed window with X or if user clicked "Exit" button then exit
        break
    elif event == sg.InputText:
        window['X'].Update(range=(-max_accel, max_accel))
        window['Y'].Update(range=(-max_accel, max_accel))
        window['Z'].Update(range=(-max_accel, max_accel))
window.close()