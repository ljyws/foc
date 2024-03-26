from PyQt5 import QtWidgets
from src.commonInterface.commonInterface import (GuiToolsKit,WorkAreaTabWidget)


class FocMonitortool(WorkAreaTabWidget):
  def __init__(self,  parent=None):
    super().__init__(parent)
    self.verticalLayout = QtWidgets.QVBoxLayout(self)
    self.verticalLayout.setObjectName('verticalLayout')

    def getTabIcon(self):
        return GuiToolsKit.get_icon_by_name('connect')
