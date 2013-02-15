#!/usr/bin/python
# combobox.py


import wx

class ComboBox(wx.Frame):
    def __init__(self, parent, id, title):
        wx.Frame.__init__(self, parent, id, title, size=(250,270))

        panel = wx.Panel(self, -1, (75, 20), (100, 127), style=wx.SUNKEN_BORDER)
        self.picture = wx.StaticBitmap(panel)
        panel.SetBackgroundColour(wx.WHITE)

        self.images = ["exit.png",
                       "hunter.png",
                       "next.png",
                       "pause.png",
                       "play.png",
                       "previous.png",
                       "text.png",
                       "volume.png"]
        self.authors = ["exit",
                       "hunter",
                       "next",
                       "pause",
                       "play",
                       "previous",
                       "text",
                       "volume"]


        wx.ComboBox(self, -1, pos=(50, 170), size=(150, -1), choices=self.authors,
                    style=wx.CB_READONLY)
        wx.Button(self, 1, "Close", (80, 220))

        self.sb = self.CreateStatusBar()

        self.Bind(wx.EVT_BUTTON, self.OnClose, id=1)
        self.Bind(wx.EVT_COMBOBOX, self.OnSelect)
        
        self.Show()


    def OnClose(self, event):
        self.Close()

    def OnSelect(self, event):
        item = event.GetSelection()
        self.picture.SetFocus()
        self.picture.SetBitmap(wx.Bitmap(self.images[item]))
        self.sb.SetStatusText(self.images[item])

app = wx.App()
ComboBox(None, -1, "combobox.py")
app.MainLoop()
        
