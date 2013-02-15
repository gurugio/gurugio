#!/usr/bin/python
# -*-coding: utf-8-*-


import wx

import pdb

class Example(wx.Frame):
    def __init__(self, *args, **kargs):
        super(Example, self).__init__(*args, **kargs)
        self.InitUI()

    def InitUI(self):
        menubar = wx.MenuBar()
        helpmenu = wx.Menu()
        h = helpmenu.Append(-1, "&About")
        self.Bind(wx.EVT_MENU, self.OnAboutBox, h)

        menubar.Append(helpmenu, "&Help")
        self.SetMenuBar(menubar)

        self.SetSize((300,200))
        self.SetTitle("About dialog box")
        self.Centre()
        self.Show(True)

    def OnAboutBox(self, e):
        description = """description
        ommitted"""

        license = """no
        license"""

        info = wx.AboutDialogInfo()

        info.SetIcon(wx.Icon("hunter.png", wx.BITMAP_TYPE_PNG))
        info.SetName("File Hunter")
        info.SetVersion("1.0")
        info.SetDescription(description)
        info.SetCopyright("(asdfasdf")
        info.SetWebSite("http://asdf")
        info.SetLicence(license)
        info.AddDeveloper("asdf")
        info.AddDocWriter("asdf")
        info.AddArtist("asdf")
        info.AddTranslator("asdfasdf")

        wx.AboutBox(info)

def main():
    ex = wx.App()
    Example(None)
    ex.MainLoop()

if __name__ == "__main__":
    main()
