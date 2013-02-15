#!/usr/bin/python

# plyer

import wx

class Player(wx.Frame):
    def __init__(self, parent, id, title):
        wx.Frame.__init__(self, parent, id, title, size=(350,300))
        panel = wx.Panel(self, -1)

        sub_panel1 = wx.Panel(self, -1)
        sub_panel1.SetBackgroundColour(wx.BLACK)
        sub_panel2 = wx.Panel(self, -1)

        menubar = wx.MenuBar()
        file = wx.Menu()
        play = wx.Menu()
        view = wx.Menu()
        tools = wx.Menu()
        favorites = wx.Menu()
        help = wx.Menu()

        file.Append(101, "&Quit", "Quit application")

        menubar.Append(file, "&File")
        menubar.Append(play, "&Play")
        menubar.Append(view, "&View")
        menubar.Append(tools, "&Tools")
        menubar.Append(favorites, "F&avorites")
        menubar.Append(help, "&Help")

        slider1 = wx.Slider(sub_panel2, -1, 0, 0, 1000)
        pause = wx.BitmapButton(sub_panel2, -1, wx.Bitmap("pause.png"))
        play = wx.BitmapButton(sub_panel2, -1, wx.Bitmap("play.png"))
        next = wx.BitmapButton(sub_panel2, -1, wx.Bitmap("next.png"))
        prev = wx.BitmapButton(sub_panel2, -1, wx.Bitmap("previous.png"))
        volume = wx.BitmapButton(sub_panel2, -1, wx.Bitmap("volume.png"))
        slider2 = wx.Slider(sub_panel2, -1, 0, 0, 100, size=(120, -1))

        vbox = wx.BoxSizer(wx.VERTICAL)
        hbox1 = wx.BoxSizer(wx.HORIZONTAL)
        hbox2 = wx.BoxSizer(wx.HORIZONTAL)

        hbox1.Add(slider1, 1)

        hbox2.Add(pause)
        hbox2.Add(play, 0, flag=wx.RIGHT, border=5)
        hbox2.Add(next, 0, flag=wx.LEFT, border=5)
        hbox2.Add(prev)
        hbox2.Add((-1, -1), 1) # locate empty space
        hbox2.Add(volume)
        hbox2.Add(slider2, flag=wx.TOP | wx.LEFT, border=5)


        vbox.Add(hbox1, flag=wx.EXPAND|wx.BOTTOM, border=10)
        vbox.Add(hbox2, 1, flag=wx.EXPAND)

        sub_panel2.SetSizer(vbox)

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(sub_panel1, 1, flag=wx.EXPAND)
        sizer.Add(sub_panel2, 0, flag=wx.EXPAND | wx.BOTTOM | wx.TOP, border=10)

        self.SetMinSize((350, 300))
        self.CreateStatusBar()
        self.SetSizer(sizer)

        self.SetMenuBar(menubar)

        self.Show()

app = wx.App()
Player(None, -1, "Player")
app.MainLoop()
