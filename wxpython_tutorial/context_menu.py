#!/usr/bin/env python
import wx

MY_ID_VIEW = 1
APP_EXIT = 2

class MyPopupMenu(wx.Menu):
    def __init__(self, parent):
        wx.Menu.__init__(self)
        self.parent = parent

        mmi = wx.MenuItem(self, wx.NewId(), "Minimize")
        self.AppendItem(mmi)
        self.Bind(wx.EVT_MENU, self.OnMinimize, mmi)

        cmi = wx.MenuItem(self, wx.NewId(), "Close")
        self.AppendItem(cmi)
        self.Bind(wx.EVT_MENU, self.OnClose, cmi)

    def OnMinimize(self, e):
        self.parent.Iconize()

    def OnClose(self, e):
        self.parent.Close()
        

class MainWindow(wx.Frame):
    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, title=title, size=(400,300))
        # status bar is created later
        #self.CreateStatusBar() # status bar in the bottom of the window
        self.mypanel = wx.Panel(self, wx.ID_ANY)
        self.sizer = wx.BoxSizer(wx.VERTICAL)
                
        # file menu
        filemenu = wx.Menu()

        # # #aboutMenu = filemenu.Append(wx.ID_ABOUT, "&About", " Information about this program")
        aboutMenu = wx.MenuItem(filemenu, APP_EXIT, '&About\tCtrl+A', " Information about this program")
        filemenu.AppendItem(aboutMenu)
        
        filemenu.AppendSeparator()
        exitMenu = filemenu.Append(wx.ID_EXIT, "E&xit", " Terminate the program")

        # # view menu
        viewmenu = wx.Menu()
        runViewMenu = viewmenu.Append(MY_ID_VIEW, "Run &View", "Run viewer")

        # # check menu
        # # self.shst and shtl are local var
        self.shst = viewmenu.Append(wx.ID_ANY, "Show statusbar",
                                    "Show Statusbar", kind=wx.ITEM_CHECK)
        self.shtl = viewmenu.Append(wx.ID_ANY, "Show toolbar",
                                    "Show Toolbar", kind=wx.ITEM_CHECK)

        viewmenu.Check(self.shst.GetId(), True)
        viewmenu.Check(self.shtl.GetId(), True)

        # # menu-bar
        menuBar = wx.MenuBar()
        menuBar.Append(filemenu,"&File")
        menuBar.Append(viewmenu,"&View")
        
        self.SetMenuBar(menuBar)

        # # tool bar
        self.toolbar = self.CreateToolBar()
        self.toolbarImg = wx.Bitmap('text.png')
        self.toolbarImg.SetSize(
            (self.toolbarImg.GetWidth() / 2, self.toolbarImg.GetHeight() / 2))
        self.toolbar.AddLabelTool(1, "Toolbar label", self.toolbarImg)
        self.toolbar.Realize()

        #        self.sizer.Add(self.toolbar, 0, wx.TOP)

        # # status bar
        self.statusbar = self.CreateStatusBar()
        self.statusbar.SetStatusText("Ready")

        # text editor
        self.hbox_text = wx.BoxSizer(wx.HORIZONTAL)
        self.textctrl = wx.TextCtrl(self.mypanel, style=wx.TE_MULTILINE)

        self.hbox_text.Add(self.textctrl, proportion=1, flag=wx.EXPAND)
        
        self.sizer.Add(self.hbox_text,
                       proportion=1,
                       flag=wx.TOP|wx.LEFT|wx.RIGHT|wx.EXPAND, border=10)

        self.sizer.Add((-1, 20))
        
        hbox1 = wx.BoxSizer(wx.HORIZONTAL)
        st1 = wx.StaticText(self.mypanel, label='Class Name')
        hbox1.Add(st1, flag=wx.RIGHT, border=8)
        tc = wx.TextCtrl(self.mypanel)
        hbox1.Add(tc, proportion=1)
        self.sizer.Add(hbox1, 1,
                       flag=wx.EXPAND|wx.LEFT|wx.RIGHT|wx.TOP,
                       border=10)
                       
        self.mypanel.SetSizer(self.sizer)        



        # # event

        # self.Bind(wx.EVT_MENU, self.OnAbout, aboutMenu)
        # self.Bind(wx.EVT_MENU, self.OnExit, exitMenu)
        # self.Bind(wx.EVT_MENU, self.ToggleStatusBar, self.shst)
        # self.Bind(wx.EVT_MENU, self.ToggleToolBar, self.shtl)

        self.mypanel.Bind(wx.EVT_RIGHT_DOWN, self.OnRightDown)


        self.Show(True)

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

    def OnRightDown(self, e):
        print "Right down"
        self.PopupMenu(MyPopupMenu(self), e.GetPosition())

app = wx.App()
frame = MainWindow(None, 'Small editor')
app.MainLoop()

