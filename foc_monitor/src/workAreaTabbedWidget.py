import json
from PyQt5 import QtWidgets
from src.gui.foc_monitor_tool import FocMonitortool
class WorkAreaTabbedWidget(QtWidgets.QTabWidget):
    def __init__(self, parent=None):
        super().__init__(parent)        
        self.setTabsClosable(False)
        self.setMovable(False)
        self.setObjectName('devicesTabWidget')

        self.connect_tab = None

    def connect_monitor(self):
        if self.connect_tab is None:
            self.connect_tab = FocMonitortool()
            self.addTab(self.connect_tab, self.connect_tab.getTabIcon(),'connrct')

