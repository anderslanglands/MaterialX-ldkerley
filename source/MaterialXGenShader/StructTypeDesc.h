//
// Created by Lee Kerley on 4/22/24.
//

#ifndef MATERIALX_STRUCTTYPEDESC_H
#define MATERIALX_STRUCTTYPEDESC_H




// TODO - complete documentation

/// @class StructTypeDesc
/// A type descriptor for MaterialX struct types.
///
/// All types need to have a type descriptor registered in order for shader generators
/// to know about the type. It can be used for type comparisons as well as getting more
/// information about the type. Type descriptors for all standard library data types are
/// registered by default and can be accessed from the Type namespace, e.g. Type::FLOAT.
///
/// To register custom types use the macro TYPEDESC_DEFINE_TYPE to define it in a header
/// and the macro TYPEDESC_REGISTER_TYPE to register it in the type registry. Registration
/// must be done in order to access the type's name later using getName() and to find the
/// type by name using TypeDesc::get().
///
/// The class is a POD type of 64-bits and can efficiently be stored and passed by value.
/// Type compare operations and hash operations are done using a precomputed hash value.
///
class MX_GENSHADER_API StructTypeDesc
{
  public:
    /// Empty constructor.
    StructTypeDesc(uint32_t typeID) noexcept : _typeID(typeID) {}

    void addMember(const std::string& name, TypeDesc type, ValuePtr defaultValue)
    {
        _memberNames.emplace_back(name);
        _memberTypes.emplace_back(type);
        _memberDefaultValues.emplace_back(defaultValue);
    }

    /// Return a type description by index.
    static StructTypeDesc get(unsigned int index);
    static unsigned int emplace_back(StructTypeDesc structTypeDesc);
    static void clear();

  private:

    uint32_t _typeID;
    std::vector<std::string> _memberNames;
    std::vector<TypeDesc> _memberTypes;
    std::vector<ValuePtr> _memberDefaultValues;
};


class MX_GENSHADER_API StructTypeDescRegistry
{
  public:
    StructTypeDescRegistry();
};



#endif // MATERIALX_STRUCTTYPEDESC_H
