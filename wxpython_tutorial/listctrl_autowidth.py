#!/usr/bin/python

# autowidth.py

import wx
import sys
from wx.lib.mixins.listctrl import ListCtrlAutoWidthMixin

actresses = [('jessica alba', 'pomona', '1981'),
              ('sigourney weaver', 'new york', '1949'),
              ('angelina jolie', 'los angeles', '1975'),
              ('natalie portman', 'jerusalem', '1981'),
              ('rachel weiss', 'london', '1971'),
              ('scarlett johansson', 'new york', '1984' )]

class AutoWidthListCtrl(wx.ListCtrl, ListCtrlAutoWidthMixin):
    def __init__(self, parent):
        wx.ListCtrl.__init__(self, parent, -1, style=wx.LC_REPORT)
        ListCtrlAutoWidthMixin.__init__(self)

class Actresses(wx.Frame):
    def __init__(self, parent, id, title):
        wx.Frame.__init__(self, parent, id, title, size=(380, 230))

        hbox = wx.BoxSizer(wx.HORIZONTAL)

        panel = wx.Panel(self, -1)

        self.list = AutoWidthListCtrl(panel)
        self.list.InsertColumn(0, "name", width=140)
        self.list.InsertColumn(1, "place", width=130)
        self.list.InsertColumn(2, "year", wx.LIST_FORMAT_RIGHT, 90)

        for i in actresses:
            index = self.list.InsertStringItem(sys.maxint, i[0])
            self.list.SetStringItem(index, 1, i[1])
            self.list.SetStringItem(index, 2, i[2])

        hbox.Add(self.list, 1, wx.EXPAND)
        panel.SetSizer(hbox)

        self.Show(True)

app = wx.App()
Actresses(None, -1, "actresses")
app.MainLoop()

