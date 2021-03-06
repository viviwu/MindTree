#include "mindtree_core.h"
#include "../datatypes/Object/object.h"
#include "../datatypes/Object/dcel.h"
#include "data/cache_main.h"
#include "data/raytracing/ray.h"
#include "data/io.h"

namespace BPy = boost::python;
using namespace MindTree;

bool testSocketProperties()
{
    NodePtr node = NodeDataBase::createNode("Values.Float Value");
    node->getInSockets()[0]->setProperty(2.5);

    DataCache *cache = new DataCache(node->getOutSockets()[0]);

    std::cout << "cached value is: " << cache->getData(0).getData<double>() << std::endl;
    
    return cache->getData(0).getData<double>() == 2.5;
}

bool testProperties()
{
    bool success=true;
    Property floatprop{2.5};
    std::cout << "created " << floatprop.getType().toStr() << " Property: "
        << floatprop.getData<double>() << std::endl;
    success = success && floatprop.getData<double>() == 2.5;

    Property intprop{2};
    std::cout << "created " << intprop.getType().toStr() << " Property: "
        << intprop.getData<int>() << std::endl;
    success = success && intprop.getData<int>() == 2;

    Property stringprop{std::string("hallo test")};
    std::cout << "created " << stringprop.getType().toStr() << " Property: "
        << stringprop.getData<std::string>() << std::endl;
    success = success && stringprop.getData<std::string>() == "hallo test";

    Property colorprop{glm::vec4(1, 0, 0, 1)};
    glm::vec4 color = colorprop.getData<glm::vec4>();
    std::cout << "created " << colorprop.getType().toStr() << " Property: ("
        << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")" << std::endl;
    success = success && colorprop.getData<glm::vec4>() == glm::vec4(1, 0, 0, 1);

    glm::vec3 oldvec = glm::vec3(24454.456, 10, 25);
    Property vec3prop{oldvec};
    glm::vec3 vec3 = vec3prop.getData<glm::vec3>();
    std::cout << "created " << vec3prop.getType().toStr() << " Property: ("
        << vec3.x << ", " << vec3.y << ", " << vec3.z << ")" << std::endl;
    success = success && vec3 == oldvec;

    return success;
}

bool testPropertiesTypeInfo()
{
    Property prop{2.5};
    std::string type1 = prop.getType().toStr();
    std::cout << "original Prop Type: " << type1 << std::endl;
    if(type1 != "FLOAT") return false;

    Property copyProp(prop);
    std::cout << "copy Prop Type: " << copyProp.getType().toStr() << std::endl;
    if(type1 != copyProp.getType().toStr())
        return false;

    Property assignmentProp;
    assignmentProp = prop;
    std::cout << "assignment Prop Type: " << assignmentProp.getType().toStr() << std::endl;
    if(type1 != assignmentProp.getType().toStr())
        return false;

    return true;
}

bool testObjectInProperty()
{
    GeoObjectPtr obj = std::make_shared<GeoObject>();
    Property objProp{obj};
    std::cout << "original Prop Type: " << objProp.getType().toStr() << std::endl;
    if(objProp.getType() != "TRANSFORMABLE")
        return false;

    return true;
}

bool testPropertyConversion()
{
    int a = 25;
    std::cout << "creating int value: " << a << std::endl;
    Property intprop{a};
    std::cout << "Property has type: " << intprop.getType().toStr() << std::endl;
    double converted = intprop.getData<double>();

    std::cout << "converted to double the resulting value is: " << converted << std::endl;

    int b = converted;
    return a == b;
}

bool testRaycasting()
{
    glm::vec3 start(0, 0, -1);
    glm::vec3 dir(0, 0, 1);

    Ray r(start, dir);

    glm::vec3 hitpoint, uvdist;

    glm::vec3 p1(-1, -1, 0);
    glm::vec3 p2(2, -1, 0);
    glm::vec3 p3(-1, 2, 0);

    bool hit = r.intersect(p1, p2, p3, &uvdist, &hitpoint);

    std::cout << "uvcoords are: " << uvdist.x << ", " << uvdist.y << std::endl;
    std::cout << "distance is: " << uvdist.z << std::endl;
    std::cout << "the hitpoint is: ("
        << hitpoint.x
        << ", "
        << hitpoint.y
        << ", "
        << hitpoint.z << ")" << std::endl;
    return hit && (start + dir * uvdist.z) == hitpoint;
}

bool testSaveLoadProperties()
{
    Property prop{5};
    {
        IO::OutStream str("testSaveLoadProperties.mt");
        str.beginBlock("Property");
        str << prop;
        str.endBlock("Property");
    }

    Property newProp;
    {
        IO::InStream str("testSaveLoadProperties.mt");
        str.beginBlock("Property");
        str >> newProp;
        str.endBlock("Property");
    }

    return newProp.getData<int>() == prop.getData<int>();
}

bool testCreateList()
{
    NodePtr createListNode = NodeDataBase::createNode("General.Create List");
    NodePtr floatValueNode = NodeDataBase::createNode("Values.Float Value");
    NodePtr intValueNode = NodeDataBase::createNode("Values.Int Value");

    Project::instance()->getRootSpace()->addNode(createListNode);
    Project::instance()->getRootSpace()->addNode(floatValueNode);
    Project::instance()->getRootSpace()->addNode(intValueNode);

    floatValueNode->getInSockets()[0]->setProperty(5.0);
    intValueNode->getInSockets()[0]->setProperty(10);
    createListNode->getInSockets()[0]->setCntdSocket(floatValueNode->getOutSockets()[0]);
    createListNode->getInSockets()[1]->setCntdSocket(intValueNode->getOutSockets()[0]);

    DataCache cache(createListNode->getOutSockets()[0]);

    auto output = cache.getOutput();

    if(output.getType() != "LIST:FLOAT") {
        std::cout << output.getType().toStr() << " is supposed to be LIST:FLOAT" << std::endl;
        return false;
    }

    auto data = output.getData<std::vector<double>>();
    if(data.size() != 10) {
        std::cout << data.size() << " is supposed to be 10" << std::endl;
        return false;
    }

    if(data[0] != 5.0) {
        std::cout << "wrong value" << std::endl;
        return false;
    }

    return true;
}

bool testDCEL()
{
    double pi = std::acos(-1);
    auto mesh = std::make_shared<MeshData>();
    auto points = std::make_shared<VertexList>();
    auto polys = std::make_shared<PolygonList>();
    mesh->setProperty("P", points);
    mesh->setProperty("polygon", polys);

    static const int SIZE = 4;

    dcel::Adapter dcel_data(mesh);

    //center vertex
    auto *center = dcel_data.newVertex();

    dcel::Vertex *lastVert = nullptr, *firstVert = nullptr;
    //create simple disk shape
    for(int i = 0; i < SIZE; ++i) {
        auto *vert = dcel_data.newVertex();
        dcel_data.connect(vert, center);

        if(i == 0) {
            lastVert = vert;
            firstVert = vert;
            continue;
        }
        glm::vec3 p(std::sin(2 * pi * i/float(SIZE)),
                            0,
                    std::cos(2 * pi * i/float(SIZE)));
        vert->set("P", p);
        dcel_data.connect(vert, lastVert);
        dcel_data.fill({vert, lastVert, center});
        lastVert = vert;
    }

    dcel_data.connect(lastVert, firstVert);
    dcel_data.fill({lastVert, firstVert, center});
    dcel_data.updateMesh();

    if(points->size() != SIZE + 1) {
        std::cerr << "wrong number of points: " << points->size() << " vs " << SIZE+1 << std::endl;
        return false;
    }

    return true;
}

BOOST_PYTHON_MODULE(cpp_tests)
{
    BPy::def("testSocketPropertiesCPP", testSocketProperties);    
    BPy::def("testPropertiesCPP", testProperties);
    BPy::def("testPropertiesTypeInfoCPP", testPropertiesTypeInfo);
    BPy::def("testObjectInPropertyCPP", testObjectInProperty);
    BPy::def("testPropertyConversionCPP", testPropertyConversion);
    BPy::def("testRaycastingCPP", testRaycasting);
    BPy::def("testSaveLoadPropertiesCPP", testSaveLoadProperties);
    BPy::def("testCreateListCPP", testCreateList);
    BPy::def("testDCELCPP", testDCEL);
}
