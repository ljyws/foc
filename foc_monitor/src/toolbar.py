from PyQt5 import QtWidgets
from src.commonInterface.commonInterface import GuiToolsKit

class FocMonitorConfigToolBar(QtWidgets.QToolBar):
  def __init__(self,main_window, devicesTabedWidget,  parent=None):
      super().__init__(parent)
      self.add_connect_action = QtWidgets.QAction(main_window)
      self.add_connect_action.setIcon(GuiToolsKit.get_icon_by_name("connect"))
      self.add_connect_action.setToolTip("Connect the monitor")
      self.add_connect_action.setObjectName("add_connect_action")
      self.add_connect_action.triggered.connect(devicesTabedWidget.connect_monitor)
      self.addAction(self.add_connect_action)