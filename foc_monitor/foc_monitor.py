from PyQt5 import QtWidgets, QtCore
import sys
import logging

if __name__ == '__main__':
    try:
        QtWidgets.QApplication.setAttribute(QtCore.Qt.AA_EnableHighDpiScaling, True) 
        QtWidgets.QApplication.setAttribute(QtCore.Qt.AA_UseHighDpiPixmaps, True)
        app = QtWidgets.QApplication(sys.argv)
        mainWindow = QtWidgets.QMainWindow()
        mainWindow.show()
        sys.exit(app.exec_())
    except Exception as exception:\
    logging.error(exception, exc_info=True)
