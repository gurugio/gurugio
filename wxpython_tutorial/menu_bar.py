#!/usr/bin/env python
import wx
import os

MY_ID_VIEW = 1

class MainWindow(wx.Frame):
    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, title=title, size=(400,100))
        self.control = wx.TextCtrl(self, style=wx.TE_MULTILINE)
        self.CreateStatusBar() # status bar in the bottom of the window

        # make menus
        filemenu = wx.Menu()

        menuOpen = filemenu.Append(wx.ID_OPEN,
                                   "&Open",
                                   " Open text file")
        filemenu.AppendSeparator()
        menuAbout = filemenu.Append(wx.ID_ABOUT,
                                    "&About",
                                    " Information about this program")
        menuExit = filemenu.Append(wx.ID_EXIT,
                                   "E&xit",
                                   " Terminate the program")

        viewmenu = wx.Menu()
        menuRunView = viewmenu.Append(MY_ID_VIEW,
                                      "Run Image &View",
                                      "Run image viewer")

        menuBar = wx.MenuBar()
        menuBar.Append(filemenu,"&File")
        menuBar.Append(viewmenu,"&View")
        
        self.SetMenuBar(menuBar)

        # set event
        self.Bind(wx.EVT_MENU, self.OnOpen, menuOpen)
        self.Bind(wx.EVT_MENU, self.OnAbout, menuAbout)
        self.Bind(wx.EVT_MENU, self.OnExit, menuExit)
        self.Bind(wx.EVT_MENU, self.OnRunView, menuRunView)
        
        self.Show(True)

    def OnAbout(self, event):
        print "about menu selected - event {0}".format(event)
        dlg = wx.MessageDialog(self,
                               "A small text editor",
                               "About sample editor",
                               wx.OK|wx.CENTRE|wx.CANCEL)
        dlg.ShowModal()
        dlg.Destroy()

    def OnExit(self, event):
        print "exit"
        self.Close(True)

    def OnRunView(self, event):
        print "runview selected"

    def OnOpen(self, event):
        """Open a file"""
        self.dirname = ''
        dlg = wx.FileDialog(self, # parent
                            "Choose a file", # string message
                            self.dirname, #default dir
                            "", #default file
                            "*.txt", #wild card
                            wx.FD_DEFAULT_STYLE) #style
        dlg.ShowModal()
        dlg.Destroy()

app = wx.App(False)
frame = MainWindow(None, 'Sample editor')
app.MainLoop()

