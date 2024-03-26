import os
from PyQt5 import (QtGui, QtWidgets,QtCore)
from serial.tools import list_ports

class GuiToolsKit(object):
    RED_COLOR = (255, 92, 92)
    GREEN_COLOR = (57, 217, 138)
    BLUE_COLOR = (91, 141, 236)
    ORANGE_COLOR = (253, 172, 66)
    YELLOW_COLOR = (255,255,51)
    PURPLE_COLOR = (75,0,130)
    MAROON_COLOR = (222,184,135)

    @staticmethod
    def get_icon_by_name(icon_name):
        file_index = {
            'connect': 'connect.png',
            'disconnect': 'disconnect.png'
        }
        current_dir = os.path.dirname(__file__)
        icon_path = os.path.join(current_dir, '../gui/resources', file_index[icon_name])
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(icon_path), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        return icon


class WorkAreaTabWidget(QtWidgets.QTabWidget):
    def __init__(self, parent=None):
        '''Constructor for ToolsWidget'''
        super().__init__(parent)

    def getTabIcon(self):
        raise NotImplemented

    def getTabName(self):
        raise NotImplemented