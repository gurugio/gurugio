#!/usr/bin/env python
import wx

MY_ID_VIEW = 1
APP_EXIT = 2
MY_ID_TOOLBAR_TEXT = 3

class MainWindow(wx.Frame):
    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, title=title, size=(400,500))
        self.control = wx.TextCtrl(self, style=wx.TE_MULTILINE)
        # status bar is created later
        #self.CreateStatusBar() # status bar in the bottom of the window

        # file menu
        filemenu = wx.Menu()

        #aboutMenu = filemenu.Append(wx.ID_ABOUT, "&About", " Information about this program")
        aboutMenu = wx.MenuItem(filemenu, APP_EXIT, '&About\tCtrl+A', " Information about this program")
        filemenu.AppendItem(aboutMenu)
        
        filemenu.AppendSeparator()
        exitMenu = filemenu.Append(wx.ID_EXIT, "E&xit", " Terminate the program")

        # view menu
        viewmenu = wx.Menu()
        runViewMenu = viewmenu.Append(MY_ID_VIEW, "Run &View", "Run viewer")

        # check menu
        # self.shst and shtl are local var
        self.shst = viewmenu.Append(wx.ID_ANY, "Show statusbar",
                               "Show Statusbar", kind=wx.ITEM_CHECK)
        self.shtl = viewmenu.Append(wx.ID_ANY, "Show toolbar",
                               "Show Toolbar", kind=wx.ITEM_CHECK)

        viewmenu.Check(self.shst.GetId(), True)
        viewmenu.Check(self.shtl.GetId(), True)

        # menu-bar
        menuBar = wx.MenuBar()
        menuBar.Append(filemenu,"&File")
        menuBar.Append(viewmenu,"&View")
        
        self.SetMenuBar(menuBar)

        # tool bar
        self.toolbar = self.CreateToolBar()

        self.toolbarText = self.toolbar.AddLabelTool(MY_ID_TOOLBAR_TEXT,
                                                     "Toolbar label text",
                                                     wx.Bitmap("text.png"))
        self.toolbar.AddSeparator()
        
        self.toolbarImg = wx.Bitmap('exit.png')
        #self.toolbar.AddLabelTool(1, "Toolbar label", wx.Bitmap('Exit.png'))
        self.toolbarExit = self.toolbar.AddLabelTool(wx.ID_ANY,
                                                     "Toolbar label exit",
                                                     self.toolbarImg)
        
        self.toolbar.Realize()

        # status bar
        self.statusbar = self.CreateStatusBar()
        self.statusbar.SetStatusText("Ready")
        
        # event
        self.Bind(wx.EVT_MENU, self.OnAbout, aboutMenu)
        self.Bind(wx.EVT_MENU, self.OnExit, exitMenu)
        self.Bind(wx.EVT_MENU, self.ToggleStatusBar, self.shst)
        self.Bind(wx.EVT_MENU, self.ToggleToolBar, self.shtl)

        self.Bind(wx.EVT_TOOL, self.OnExit, self.toolbarExit)
        self.Bind(wx.EVT_TOOL, self.OnText, self.toolbarText)
        
        self.Show(True)

    def OnText(self, e):
        self.control.WriteText("Text toolbar clicked")
        self.toolbar.EnableTool(MY_ID_TOOLBAR_TEXT, False)

    def ToggleStatusBar(self, e):
        if self.shst.IsChecked():
            self.statusbar.Show()
        else:
            self.statusbar.Hide()

    def ToggleToolBar(self, e):
        if self.shtl.IsChecked():
            self.toolbar.Show()
        else:
            self.toolbar.Hide()
        
    def OnAbout(self, event):
        print "about menu selected - event {0}".format(event)

    def OnExit(self, event):
        print "exit"
        exit()

    def OnRunView(self, event):
        print "runview selected"

app = wx.App(False)
frame = MainWindow(None, 'Small editor')
app.MainLoop()

