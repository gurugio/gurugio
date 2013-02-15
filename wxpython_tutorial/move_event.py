#!/usr/bin/python

# move_event.ph

import wx

class MoveEvent(wx.Frame):
    def __init__(self, parent, id, title):
        wx.Frame.__init__(self, parent, id, title, size=(240, 180))

        wx.StaticText(self, -1, "x:", (10,10))
        wx.StaticText(self, -1, "y:", (10, 30))
        self.st1 = wx.StaticText(self, -1, "", (30, 10))
        self.st2 = wx.StaticText(self, -1, "", (30, 30))

        wx.StaticText(self, -1, "x:", (10,50))
        wx.StaticText(self, -1, "y:", (10, 70))
        self.st3 = wx.StaticText(self, -1, "", (30, 50))
        self.st4 = wx.StaticText(self, -1, "", (30, 70))

        self.Bind(wx.EVT_MOVE, self.OnMove)
        self.Bind(wx.EVT_MOTION, self.OnMouseMove)
        self.Centre()
        self.Show(True)

    def OnMove(self, event):
        x, y = event.GetPosition()
        self.st1.SetLabel(str(x))
        self.st2.SetLabel(str(y))
    def OnMouseMove(self, event):
        x, y = event.GetPosition()
        self.st3.SetLabel(str(x))
        self.st4.SetLabel(str(y))
        
        

app = wx.App()
MoveEvent(None, -1, "move event")
app.MainLoop()

