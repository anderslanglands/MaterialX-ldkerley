//
// Copyright Contributors to the MaterialX Project
// SPDX-License-Identifier: Apache-2.0
//

#include <MaterialXGenShader/TypeDesc.h>

#include <MaterialXGenShader/ShaderGenerator.h>
#include <MaterialXGenShader/GenContext.h>

MATERIALX_NAMESPACE_BEGIN

namespace
{

using TypeDescNameMap = std::unordered_map<uint32_t, string>;

TypeDescNameMap& typeNameMap()
{
    static TypeDescNameMap map;
    return map;
}

} // anonymous namespace

const string TypeDesc::NONE_TYPE_NAME = "none";

const string& TypeDesc::getName() const
{
    TypeDescNameMap& typenames = typeNameMap();
    auto it = typenames.find(_id);
    return it != typenames.end() ? it->second : NONE_TYPE_NAME;
}

ValuePtr TypeDesc::createValueFromStrings(const string& value, const GenContext& context) const
{
    ValuePtr newValue = Value::createValueFromStrings(value, getName());
    auto structMemberDescs = context.getStructMembers(*this);
    if (!isStruct() || !structMemberDescs)
        return newValue;

    // Value::createValueFromStrings() can only create a valid Value for a struct if it is passed
    // the optional TypeDef argument, otherwise it just returns a "string" typed Value.
    // So if this is a struct type we need to create a new AggregateValue.

    StringVec subValues = parseStructValueString(value);
    AggregateValuePtr result = AggregateValue::createAggregateValue(getName());

    if (subValues.size() != structMemberDescs->size())
    {
        std::stringstream ss;
        ss << "Wrong number of initializers - expect " << structMemberDescs->size();
        throw ExceptionShaderGenError(ss.str());
    }

    for (size_t i = 0; i < structMemberDescs->size(); ++i)
    {
        result->appendValue( structMemberDescs->at(i)._typeDesc.createValueFromStrings(subValues[i], context));
    }

    return result;
}

//
// TypeDescStorage methods
//

uint16_t TypeDescStorage::registerStructMembers(ConstStructMemberDescVecPtr structTypeDesc)
{
    if (_structTypeStorage.size() >= std::numeric_limits<uint16_t>::max())
    {
        throw ExceptionShaderGenError("Maximum number of custom struct types has been exceeded.");
    }
    uint16_t index = static_cast<uint16_t>(_structTypeStorage.size());
    _structTypeStorage.emplace_back(structTypeDesc);
    return index;
}

void TypeDescStorage::registerTypeDesc(TypeDesc type, const string& name)
{
    _typeMap[name] = type;

    // TODO - decide if we need to make this more threadsafe
    // typeID is just a hash of the string value - so while
    // we might set this entry multiple times - it's always going to be
    // the same value...
    // TODO - consider using OIIO::ustring?
    TypeDescNameMap& typenames = typeNameMap();
    typenames[type.typeId()] = name;
}

MATERIALX_NAMESPACE_END
