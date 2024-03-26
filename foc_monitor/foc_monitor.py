from PyQt5 import QtWidgets, QtCore
from src.mainWindow import UserInteractionMainWindow
import sys
import logging

if __name__ == '__main__':
    try:
        QtWidgets.QApplication.setAttribute(QtCore.Qt.AA_EnableHighDpiScaling, True) 
        QtWidgets.QApplication.setAttribute(QtCore.Qt.AA_UseHighDpiPixmaps, True)
        app = QtWidgets.QApplication(sys.argv)
        mainWindow = QtWidgets.QMainWindow()
        mainWindow.show()
        userInteractionMainWindow = UserInteractionMainWindow()
        userInteractionMainWindow.setupUi(mainWindow)
        sys.exit(app.exec_())
    except Exception as exception:\
    logging.error(exception, exc_info=True)
