
#include "UmlBaseRelation.h"
#include "UmlRelation.h"
#include "UmlClass.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"

#include "UmlCom.h"
//Added by qt3to4:
#include <QByteArray>
UmlRelation * UmlBaseRelation::create(aRelationKind kind, UmlClass * start, UmlClass * end)
{
    UmlCom::send_cmd(start->_identifier, createCmd, aRelation,
                     kind, end->_identifier);
    UmlRelation * result = (UmlRelation *) UmlBaseItem::read_();

    if (result != 0) {
        start->reread_children_if_needed_();

        switch (kind) {
        case anAssociation:
        case anAggregation:
        case anAggregationByValue:
            // bi-directional
            end->reread_children_if_needed_();
            break;

        case aGeneralisation:
        case aRealization:
            if (!end->formals().isEmpty())
                // to update actuals & base_type
                start->reread_if_needed_();

        default:
            break;
        }
    }

    return result;
}

anItemKind UmlBaseRelation::kind()
{
    return aRelation;
}

aRelationKind UmlBaseRelation::relationKind()
{
    read_if_needed_();

    return _rel_kind;
}

bool UmlBaseRelation::isReadOnly()
{
    read_if_needed_();

    return _read_only;
}

bool UmlBaseRelation::set_isReadOnly(bool y)
{
    return set_it_(_read_only, y, setIsReadOnlyCmd);
}

const QByteArray & UmlBaseRelation::defaultValue()
{
    read_if_needed_();

    return _default_value;
}

bool UmlBaseRelation::set_DefaultValue(const char * s)
{
    return set_it_(_default_value, s, setDefaultValueCmd);
}

bool UmlBaseRelation::set_Stereotype(const QByteArray & s)
{
    if (UmlBaseItem::set_Stereotype(s)) {
        UmlBaseRelation * other = (UmlBaseRelation *) UmlBaseItem::read_();

        if (other != 0)
            other->_stereotype = s;

        return TRUE;
    }
    else
        return FALSE;
}

UmlClass * UmlBaseRelation::roleType()
{
    read_if_needed_();

    return _role_type;
}

const QByteArray & UmlBaseRelation::roleName()
{
    read_if_needed_();

    return _role_name;
}

bool UmlBaseRelation::set_RoleName(const char * s)
{
    return set_it_(_role_name, s, setRoleNameCmd);
}

const QByteArray & UmlBaseRelation::multiplicity()
{
    read_if_needed_();

    return _multiplicity;
}

bool UmlBaseRelation::set_Multiplicity(const char * s)
{
    return set_it_(_multiplicity, s, setMultiplicityCmd);
}

UmlOperation * UmlBaseRelation::getOperation()
{
    read_if_needed_();

    return _get_oper;
}

bool UmlBaseRelation::addGetOperation()
{
    UmlCom::send_cmd(_identifier, addGetOperCmd);

    if (UmlCom::read_bool()) {
        reread_children_if_needed_();
        return TRUE;
    }
    else
        return FALSE;
}

UmlOperation * UmlBaseRelation::setOperation()
{
    read_if_needed_();

    return _set_oper;
}

bool UmlBaseRelation::addSetOperation()
{
    UmlCom::send_cmd(_identifier, addSetOperCmd);

    if (UmlCom::read_bool()) {
        reread_children_if_needed_();
        return TRUE;
    }
    else
        return FALSE;
}

#ifdef WITHCPP
bool UmlBaseRelation::cppVirtualInheritance()
{
    read_if_needed_();

    return _cpp_virtual_inheritance;
}

bool UmlBaseRelation::set_CppVirtualInheritance(bool y)
{
    bool b;

    if (set_it_(b, y, setCppVirtualInheritanceCmd)) {
        _cpp_virtual_inheritance = y;
        return TRUE;
    }
    else
        return FALSE;
}

bool UmlBaseRelation::isCppMutable()
{
    read_if_needed_();

    return _cpp_mutable;
}

bool UmlBaseRelation::set_isCppMutable(bool y)
{
    bool b;

    if (set_it_(b, y, setIsCppMutableCmd)) {
        _cpp_mutable = y;
        return TRUE;
    }
    else
        return FALSE;
}
#endif

#ifdef WITHJAVA
bool UmlBaseRelation::isJavaTransient()
{
    read_if_needed_();

    return _java_transient;
}

bool UmlBaseRelation::set_isJavaTransient(bool y)
{
    bool b;

    if (set_it_(b, y, setIsJavaTransientCmd)) {
        _java_transient = y;
        return TRUE;
    }
    else
        return FALSE;

}
#endif

#ifdef WITHIDL
bool UmlBaseRelation::isIdlTruncatableInheritance()
{
    read_if_needed_();

    return _idl_truncatable;
}

bool UmlBaseRelation::set_isIdlTruncatableInheritance(bool y)
{
    bool b;

    if (set_it_(b, y, setIsIdlTruncatableCmd)) {
        _idl_truncatable = y;
        return TRUE;
    }
    else
        return FALSE;
}

QByteArray UmlBaseRelation::idlCase()
{
    read_if_needed_();

    return (_idl_case != 0) ? _idl_case->name() : _idl_explicit_case;
}

bool UmlBaseRelation::set_IdlCase(UmlAttribute * a)
{
    UmlCom::send_cmd(_identifier, setIdlCaseCmd, a->_identifier, "");

    if (UmlCom::read_bool()) {
        _idl_case = a;
        _idl_explicit_case = 0;
        return TRUE;
    }
    else
        return FALSE;
}

bool UmlBaseRelation::set_IdlCase(const char * s)
{
    UmlCom::send_cmd(_identifier, setIdlCaseCmd, (void *) 0, s);

    if (UmlCom::read_bool()) {
        _idl_case = 0;
        _idl_explicit_case = s;
        return TRUE;
    }
    else
        return FALSE;
}
#endif

void UmlBaseRelation::unload(bool rec, bool del)
{
    _role_name = 0;
    _multiplicity = 0;
    _default_value = 0;
#ifdef WITHIDL
    _idl_explicit_case = 0;
#endif
    UmlBaseClassMember::unload(rec, del);
}

void UmlBaseRelation::read_uml_()
{
    UmlBaseClassMember::read_uml_();
    _rel_kind = (aRelationKind) UmlCom::read_char();
    _role_type = (UmlClass *) UmlBaseItem::read_();
    _role_name = UmlCom::read_string();
    _multiplicity = UmlCom::read_string();
    _default_value = UmlCom::read_string();
    _read_only = UmlCom::read_bool();
    _get_oper = (UmlOperation *) UmlBaseItem::read_();
    _set_oper = (UmlOperation *) UmlBaseItem::read_();
}

#ifdef WITHCPP
void UmlBaseRelation::read_cpp_()
{
    UmlBaseClassMember::read_cpp_();
    _cpp_mutable = UmlCom::read_bool();
    _cpp_virtual_inheritance = UmlCom::read_bool();
}
#endif

#ifdef WITHJAVA
void UmlBaseRelation::read_java_()
{
    UmlBaseClassItem::read_java_();
    _java_transient = UmlCom::read_bool();
}
#endif

#ifdef WITHIDL
void UmlBaseRelation::read_idl_()
{
    UmlBaseClassItem::read_idl_();
    _idl_truncatable = UmlCom::read_bool();
    _idl_case = (UmlAttribute *) UmlBaseItem::read_();

    if (_idl_case == 0)
        _idl_explicit_case = UmlCom::read_string();
}
#endif

