from PyQt5 import (QtCore, QtWidgets)
from src.toolbar import FocMonitorConfigToolBar
from src.workAreaTabbedWidget import WorkAreaTabbedWidget
class UserInteractionMainWindow(object):
  def setupUi(self, main_window):

    main_window.setObjectName("MainWindow")
    main_window.resize(800, 600)
    main_window.setWindowTitle("foc_monitor")

    self.centralwidget = QtWidgets.QWidget(main_window)
    self.centralwidget.setObjectName('centralwidget')

    self.horizontalLayout = QtWidgets.QVBoxLayout(self.centralwidget)
    self.horizontalLayout.setObjectName('horizontalLayout')

    self.tabbedToolsWidget = WorkAreaTabbedWidget(self.centralwidget)
    self.horizontalLayout.addWidget(self.tabbedToolsWidget)

    self.toolBar = FocMonitorConfigToolBar(main_window,self.tabbedToolsWidget, main_window)
    main_window.addToolBar(QtCore.Qt.TopToolBarArea, self.toolBar)

    self.statusbar = QtWidgets.QStatusBar(main_window)
    self.statusbar.setObjectName('statusbar')
    main_window.setStatusBar(self.statusbar)

    # main_window.setCentralWidget(self.centralwidget)
