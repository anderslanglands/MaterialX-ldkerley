//
// Copyright Contributors to the MaterialX Project
// SPDX-License-Identifier: Apache-2.0
//

#include <MaterialXGenShader/TypeDesc.h>

#include <MaterialXGenShader/ShaderGenerator.h>
#include <MaterialXGenShader/GenContext.h>

MATERIALX_NAMESPACE_BEGIN

const string TypeDesc::NONE_TYPE_NAME = "none";

const string& TypeDesc::getName(const GenContext& context) const
{
    return context.getTypeDescName(*this);
}

ValuePtr TypeDesc::createValueFromStrings(const string& value, const GenContext& context) const
{
    const auto& typeName = getName(context);
    ValuePtr newValue = Value::createValueFromStrings(value, typeName);
    auto structMemberDescs = context.getStructMembers(*this);
    if (!isStruct() || !structMemberDescs)
        return newValue;

    // Value::createValueFromStrings() can only create a valid Value for a struct if it is passed
    // the optional TypeDef argument, otherwise it just returns a "string" typed Value.
    // So if this is a struct type we need to create a new AggregateValue.

    StringVec subValues = parseStructValueString(value);
    AggregateValuePtr result = AggregateValue::createAggregateValue(typeName);

    if (subValues.size() != structMemberDescs->size())
    {
        std::stringstream ss;
        ss << "Wrong number of initializers - expect " << structMemberDescs->size();
        throw ExceptionShaderGenError(ss.str());
    }

    for (size_t i = 0; i < structMemberDescs->size(); ++i)
    {
        result->appendValue( structMemberDescs->at(i).getTypeDesc().createValueFromStrings(subValues[i], context));
    }

    return result;
}

//uint16_t TypeDescStorage::registerStructMembers(ConstStructMemberDescVecPtr structTypeDesc)
//{
//    if (_structTypeStorage.size() >= std::numeric_limits<uint16_t>::max())
//    {
//        throw ExceptionShaderGenError("Maximum number of custom struct types has been exceeded.");
//    }
//    uint16_t index = static_cast<uint16_t>(_structTypeStorage.size());
//    _structTypeStorage.emplace_back(structTypeDesc);
//    return index;
//}
//
//void TypeDescStorage::registerTypeDesc(TypeDesc type, const string& name)
//{
//    _typeMap[name] = type;
//
//    // TODO - decide if we need to make this more threadsafe
//    // typeID is just a hash of the string value - so while
//    // we might set this entry multiple times - it's always going to be
//    // the same value...
//    // TODO - consider using OIIO::ustring?
//
//    // TODO - decide what it means to re-register the same type over again.
//    // We could...
//    // 1) Just blindly register this type on top of the existing type name.
//    // 2) Ignore any types where the names are already registered
//    // 3) When a type is re-registered we could go compare the existing registered type against the
//    // new candidate type, and raise an error if they differ.
//
//    _typeNameMap[type.typeId()] = name;
//}
//
//const string& TypeDescStorage::getTypeDescName(TypeDesc typeDesc) const
//{
//    auto it = _typeNameMap.find(typeDesc.typeId());
//    return it != _typeNameMap.end() ? it->second : TypeDesc::NONE_TYPE_NAME;
//}

MATERIALX_NAMESPACE_END
