//
// Created by Lee Kerley on 4/22/24.
//

#include "StructTypeDesc.h"


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



TypeDesc createStructTypeDesc(std::string_view name)
{
    return {name, TypeDesc::BASETYPE_STRUCT};
}

void registerStructTypeDesc(std::string_view name)
{
    auto structTypeDesc = createStructTypeDesc(name);
    TypeDescRegistry register_struct(structTypeDesc, std::string(name));
}
