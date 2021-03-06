import MT

_compatibilityMaps = {}

def addCompatibility(type1, type2):
    if not type1 in _compatibilityMaps.keys():
        _compatibilityMaps[type1] = set()

    _compatibilityMaps[type1].add(type2)

    if not type2 in _compatibilityMaps.keys():
        _compatibilityMaps[type2] = set()

    _compatibilityMaps[type2].add(type1)

def getCompatibleSockets(outsocket, node, namepath=[]):
    compIns = {}
    for in_ in node.insockets:
        pathlist = namepath[:]
        pathlist.append(in_.name)
        path = ".".join(pathlist)
        if isCompatible(in_, outsocket):
            compIns[path] = in_
        for child in in_.childNodes:
            compIns.update(getCompatibleSockets(outsocket, child, pathlist))
    return compIns

def isList(t):
    if t.startswith("LIST:"):
        return True, t[t.find("LIST:"):]

def isCompatible(socket1, socket2):
    type1 = socket1.type
    type2 = socket2.type

    if type1 == type2:
        return True

    if type1 in ["VARIABLE", ""]:
        return True

    if type2 in ["VARIABLE", ""]:
        return True

    type1compset = None
    if type1 in _compatibilityMaps.keys():
        type1compset = _compatibilityMaps[type1]

    if type1compset is not None and type2 in type1compset:
        return True

MT.__dict__["addCompatibility"] = addCompatibility
MT.__dict__["isCompatible"] = isCompatible
MT.__dict__["getCompatibleSockets"] = getCompatibleSockets
