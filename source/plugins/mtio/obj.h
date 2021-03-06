/*
    FRG Shader Editor, a Node-based Renderman Shading Language Editor
    Copyright (C) 2011  Sascha Fricke

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OBJ

#define OBJ

#include "QTextStream"
#include "data/nodes/data_node.h"
#include "source/plugins/datatypes/Object/object.h"

class MeshData;
class ObjImportNode;

class ObjImporter
{
public:
    ObjImporter(std::string filepath);
    virtual ~ObjImporter();

    std::shared_ptr<Group> getGroup();

private:
    void readData(QTextStream &stream);
    std::shared_ptr<GeoObject> addObject(QString line);
    void addVertex(QString line, std::shared_ptr<GeoObject> obj);
    void addNormal(QString line, std::shared_ptr<GeoObject> obj);
    void addFace(QString line, std::shared_ptr<GeoObject> obj);
    void addUV(QString line, std::shared_ptr<GeoObject> obj);

    std::shared_ptr<Group> grp;
    QString name;
};

class ObjImportNode : public MindTree::DNode
{
public:
    ObjImportNode(bool raw=false);
    ObjImportNode(const ObjImportNode &node);
    std::string getFilePath()const;
};

#endif /* end of include guard: OBJ*/
