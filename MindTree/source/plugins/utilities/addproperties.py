import MT, PyQt4
from PyQt4.QtGui import *
from PyQt4.QtCore import *

class CustomWidget(MT.pytypes.CustomNodeWidget):
    def __init__(self, node, parent=None):
        MT.pytypes.CustomNodeWidget.__init__(self, node, parent)

        self.setLayout(QHBoxLayout())
        button = QPushButton("Add Property")
        self.layout().addWidget(button)
        button.clicked.connect(self.addProperty)

    def addProperty(self):
        self.node.addInSocket("Property Name", "STRING")
        self.node.addInSocket("Property VALUE", "VARIABLE")
        self.editor.updateEditor(self.node)

class AddPropertiesNodeFactory(MT.pytypes.NodeFactory):
    type="ADDPROPERTIES"
    label="Objects.Add Properties"
    insockets = [("Object(s)", "GROUPDATA"),
            ("Property Name", "STRING"),
            ("Property Value", "VARIABLE")]
    outsockets = [("Object(s)", "GROUPDATA")]
    customwidget = CustomWidget

class FilterObjectsNodeFactory(MT.pytypes.NodeFactory):
    type="FILTEROBJECTS"
    label="Objects.Filter"
    insockets = [("Objects", "GROUPDATA"),
            ("Name Regex", "STRING")]
    outsockets = [("Objects", "GROUPDATA")]


def registerNodes():
    MT.registerNode(AddPropertiesNodeFactory)
    MT.registerNode(FilterObjectsNodeFactory)

registerNodes()