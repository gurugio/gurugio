#!/usr/bin/python
# propagate.py

import wx

class MyPanel(wx.Panel):
    def __init__(self, parent, id):
        wx.Panel.__init__(self, parent, id)
        self.Bind(wx.EVT_BUTTON, self.OnClicked)

    def OnClicked(self, event):
        print "event reached panel class"
        event.Skip()

class MyButton(wx.Button):
    def __init__(self, parent, id, label, pos):
        wx.Button.__init__(self, parent, id, label, pos)
        self.Bind(wx.EVT_BUTTON, self.OnClicked)

    def OnClicked(self, event):
        print "event reached button class"
        event.Skip()

class Propagate(wx.Frame):
    def __init__(self, parent, id, title):
        wx.Frame.__init__(self, parent, id, title, size=(250, 150))

        panel = MyPanel(self, -1)

        bt = MyButton(panel, -1, "OK", (15, 15))
        print "button id is {0}".format(bt.GetId())
        print wx.ID_EXIT
        
        self.Bind(wx.EVT_BUTTON, self.OnClicked)

        self.Centre()
        self.Show(True)

    def OnClicked(self, event):
        print "event reached frame class"
        event.Skip()


app = wx.App()
Propagate(None, -1, "Propagate")
app.MainLoop()
