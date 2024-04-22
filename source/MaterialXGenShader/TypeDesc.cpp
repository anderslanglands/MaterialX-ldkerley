//
// Copyright Contributors to the MaterialX Project
// SPDX-License-Identifier: Apache-2.0
//

#include <MaterialXGenShader/TypeDesc.h>

#include <MaterialXCore/Exception.h>

MATERIALX_NAMESPACE_BEGIN

namespace
{
    using TypeDescMap = std::unordered_map<string, TypeDesc>;
    using TypeDescNameMap = std::unordered_map<uint32_t, string>;

    // Internal storage of registered type descriptors
    TypeDescMap& typeMap()
    {
        static TypeDescMap map;
        return map;
    }

    TypeDescNameMap& typeNameMap()
    {
        static TypeDescNameMap map;
        return map;
    }

    using StructTypeDescStorage = std::vector<StructTypeDesc>;
    StructTypeDescStorage& structTypeStorage()
    {
        static StructTypeDescStorage  storage;
        return storage;
    }

} // anonymous namespace

const string TypeDesc::NONE_TYPE_NAME = "none";

const string& TypeDesc::getName() const
{
    TypeDescNameMap& typenames = typeNameMap();
    auto it = typenames.find(_id);
    return it != typenames.end() ? it->second : NONE_TYPE_NAME;
}

TypeDesc TypeDesc::get(const string& name)
{
    TypeDescMap& types = typeMap();
    auto it = types.find(name);
    return it != types.end() ? it->second : Type::NONE;
}

void TypeDesc::remove(const string& name)
{
    TypeDescNameMap& typenames = typeNameMap();

    TypeDescMap& types = typeMap();

    auto it = types.find(name);
    if (it == types.end())
        return;

    typenames.erase(it->second.typeId());
    types.erase(name);

}

StructTypeDesc StructTypeDesc::get(unsigned int index)
{
    StructTypeDescStorage& structs = structTypeStorage();
    return structs[index];

    // TODO - come back and see if we want to return a NONE type object.
//    if (index < structs.size())
//        return structs[index]
//
//    return (index < structs.size()) ? structs[index] : S
}

unsigned int StructTypeDesc::emplace_back(StructTypeDesc structTypeDesc)
{
    StructTypeDescStorage& structs = structTypeStorage();
    structs.emplace_back(structTypeDesc);
    return structs.size()-1;
}

void StructTypeDesc::clear()
{
    StructTypeDescStorage& structs = structTypeStorage();
    for (const auto& structType: structs)
    {
        // need to add typeID to structTypeDesc - and use it here to reference back to typeDesc obj and remove it.

        TypeDesc::remove(structType.
    }
    structs.clear();
}

TypeDescRegistry::TypeDescRegistry(TypeDesc type, const std::string& name)
{
    TypeDescMap& types = typeMap();
    TypeDescNameMap& typenames = typeNameMap();
    types[name] = type;
    typenames[type.typeId()] = name;
}

TypeDesc createStructTypeDesc(std::string_view name)
{
    return {name, TypeDesc::BASETYPE_STRUCT};
}

void registerStructTypeDesc(std::string_view name)
{
    auto structTypeDesc = createStructTypeDesc(name);
    TypeDescRegistry register_struct(structTypeDesc, std::string(name));
}

namespace Type
{

/// 
/// Register type descriptors for standard types.
///
TYPEDESC_REGISTER_TYPE(NONE, "none")
TYPEDESC_REGISTER_TYPE(BOOLEAN, "boolean")
TYPEDESC_REGISTER_TYPE(INTEGER, "integer")
TYPEDESC_REGISTER_TYPE(INTEGERARRAY, "integerarray")
TYPEDESC_REGISTER_TYPE(FLOAT, "float")
TYPEDESC_REGISTER_TYPE(FLOATARRAY, "floatarray")
TYPEDESC_REGISTER_TYPE(VECTOR2, "vector2")
TYPEDESC_REGISTER_TYPE(VECTOR3, "vector3")
TYPEDESC_REGISTER_TYPE(VECTOR4, "vector4")
TYPEDESC_REGISTER_TYPE(COLOR3, "color3")
TYPEDESC_REGISTER_TYPE(COLOR4, "color4")
TYPEDESC_REGISTER_TYPE(MATRIX33, "matrix33")
TYPEDESC_REGISTER_TYPE(MATRIX44, "matrix44")
TYPEDESC_REGISTER_TYPE(STRING, "string")
TYPEDESC_REGISTER_TYPE(FILENAME, "filename")
TYPEDESC_REGISTER_TYPE(BSDF, "BSDF")
TYPEDESC_REGISTER_TYPE(EDF, "EDF")
TYPEDESC_REGISTER_TYPE(VDF, "VDF")
TYPEDESC_REGISTER_TYPE(SURFACESHADER, "surfaceshader")
TYPEDESC_REGISTER_TYPE(VOLUMESHADER, "volumeshader")
TYPEDESC_REGISTER_TYPE(DISPLACEMENTSHADER, "displacementshader")
TYPEDESC_REGISTER_TYPE(LIGHTSHADER, "lightshader")
TYPEDESC_REGISTER_TYPE(MATERIAL, "material")

TYPEDESC_REGISTER_TYPE(TEXCOORD, "texcoord")

}


MATERIALX_NAMESPACE_END
