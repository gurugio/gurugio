#!/usr/bin/python
# -*-coding: utf-8-*-

import wx

class Example(wx.Frame):
    def __init__(self, *args, **kargs):
        super(Example, self).__init__(*args, **kargs)
        self.InitUI()

    def InitUI(self):
        panel = wx.Panel(self)

        hbox = wx.BoxSizer()
        sizer = wx.GridSizer(2, 2, 2, 2)

        btn1 = wx.Button(panel, label="info")
        btn2 = wx.Button(panel, label="Error")
        btn3 = wx.Button(panel, label="Question")
        btn4 = wx.Button(panel, label="Alert")

        sizer.AddMany([btn1, btn2, btn3, btn4])

        hbox.Add(sizer, 0, wx.ALL, 15)
        panel.SetSizer(sizer)

        btn1.Bind(wx.EVT_BUTTON, self.ShowMessage1)
        btn2.Bind(wx.EVT_BUTTON, self.ShowMessage2)
        btn3.Bind(wx.EVT_BUTTON, self.ShowMessage3)
        btn4.Bind(wx.EVT_BUTTON, self.ShowMessage4)

        self.SetSize((300,200))
        self.SetTitle("Messages")
        self.Centre()
        self.Show(True)

    def ShowMessage1(self, event):
        dial = wx.MessageDialog(None, "Download completed", "info", wx.OK)
        dial.ShowModal()

    def ShowMessage2(self, event):
        dial = wx.MessageDialog(None, "Error loading file", "error",
                                wx.OK | wx.ICON_ERROR)
        dial.ShowModal()

    def ShowMessage3(self, event):
        dial = wx.MessageDialog(None, "Are you sure to quit?", "Question",
                                wx.YES_NO | wx.NO_DEFAULT | wx.ICON_QUESTION)
        dial.ShowModal()

    def ShowMessage4(self, event):
        dial = wx.MessageDialog(None, "Unallowed operation", "Exclamation",
                                wx.OK | wx.ICON_EXCLAMATION)
        dial.ShowModal()

def main():
    ex = wx.App()
    Example(None)
    ex.MainLoop()

if __name__ == '__main__':
    main()
