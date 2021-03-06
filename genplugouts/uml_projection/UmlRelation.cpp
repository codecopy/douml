
#include "UmlRelation.h"

#include "UmlClass.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "IdlSettings.h"
#include "PhpSettings.h"
#include "PythonSettings.h"
//Added by qt3to4:
#include <QByteArray>
void UmlRelation::uml2cpp(bool)
{
    bool composition = FALSE;

    switch (relationKind()) {
    case aGeneralisation:
    case aRealization:
        set_CppDecl("${type}");
        break;

    case aDependency:
        if (stereotype() == "friend")
            set_CppDecl("friend " +
                        CppSettings::classStereotype(roleType()->stereotype()) +
                        " ${type};");
        else
            set_CppDecl("#include in source");

        break;

    case anAggregationByValue:
    case aDirectionalAggregationByValue:
        composition = TRUE;

        // no break
    default: {
        QByteArray st = CppSettings::classStereotype(parent()->stereotype());

        set_CppDecl(((st == "enum") || (st == "typedef") || (st == "ignored"))
                    ? QByteArray("")
                    : CppSettings::relationDecl(composition, multiplicity()));
    }
    }
}

void UmlRelation::uml2java(bool)
{
    switch (relationKind()) {
    case aGeneralisation:
    case aRealization:
        set_JavaDecl("${type}");
        break;

    case aDependency:
        break;

    default: {
        QByteArray st = JavaSettings::classStereotype(parent()->stereotype());

        set_JavaDecl(((st == "enum") || (st == "ignored"))
                     ? QByteArray("")
                     : JavaSettings::relationDecl(multiplicity()));
    }
    }
}

void UmlRelation::uml2idl(bool)
{
    switch (relationKind()) {
    case aGeneralisation:
    case aRealization:
    case aDependency:
        set_IdlDecl("${type}");
        break;

    default: {
        QByteArray st = IdlSettings::classStereotype(parent()->stereotype());

        if ((st == "enum") || (st == "typedef") || (st == "ignored"))
            set_IdlDecl("");
        else if (st == "union")
            set_IdlDecl(IdlSettings::unionRelationDecl(multiplicity()));
        else if (st == "valuetype")
            set_IdlDecl(IdlSettings::valuetypeRelationDecl(multiplicity()));
        else
            set_IdlDecl(IdlSettings::relationDecl(multiplicity()));
    }
    }
}

void UmlRelation::uml2php(bool)
{
    switch (relationKind()) {
    case aGeneralisation:
    case aRealization:
    case aDependency:
        set_PhpDecl("${type}");
        break;

    default: {
        QByteArray st = PhpSettings::classStereotype(parent()->stereotype());

        if ((st == "enum") || (st == "ignored"))
            set_PhpDecl("");
        else
            set_PhpDecl(PhpSettings::relationDecl());
    }
    }
}

void UmlRelation::uml2Python(bool)
{
    bool composition = FALSE;

    switch (relationKind()) {
    case aGeneralisation:
    case aRealization:
    case aDependency:
        set_PythonDecl("${type}");
        break;

    case anAggregationByValue:
    case aDirectionalAggregationByValue:
        composition = TRUE;

        // no break
    default: {
        QByteArray st = PythonSettings::classStereotype(parent()->stereotype());

        if ((st == "enum") || (st == "ignored"))
            set_PythonDecl("");
        else
            set_PythonDecl(PythonSettings::relationDecl(composition, multiplicity()));
    }
    }
}

