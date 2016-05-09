// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
// Copyright 2012-2013 Nikolai Marchenko.
// Copyright 2012-2013 Leonardo Guilherme.
//
// This file is part of the DOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License Version 3.0 as published by
// the Free Software Foundation and appearing in the file LICENSE.GPL included in the
//  packaging of this file.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License Version 3.0 for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : doumleditor@gmail.com
// home   : http://sourceforge.net/projects/douml
//
// *************************************************************************





#include <QTextStream>
#include <qdir.h>
#include <qfile.h>
#include <qstring.h>
#include <qlayout.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qfont.h>
#include <qstringlist.h>
#include <qapplication.h>
#include <qbuffer.h>
//Added by qt3to4:

#include <QTextCodec>
#include <QFileInfo>

#include "myio.h"
#include "UmlWindow.h"
#include "BrowserView.h"
#include "strutil.h"
#include "codec.h"
#include "Labeled.h"
#include "DialogUtil.h"
#include "mu.h"
#include "UmlGlobal.h"
#include "err.h"
#include "translate.h"
#include "Logging/QsLog.h"
#include <QTextEdit>
// QFile::baseName() return string before the first '.' rather than the last

QString my_baseName(QFileInfo & fi)
{
    QString fn = fi.fileName();
    int index = fn.lastIndexOf('.');

    return (index == -1)
            ? fn
            : fn.left(index);
}

static ReadContext Context;
static bool ReadOnlyForced;

static int Indent;

ReadContext current_context()
{
    return Context;
}

void restore_context(ReadContext & c)
{
    Context = c;
}

bool read_only_file()
{
    return Context.read_only;
}

void set_read_only_file()
{
    Context.read_only = TRUE;
}

void force_read_only(bool y)
{
    ReadOnlyForced = y;
}

static bool OnLoadDiagram;

bool on_load_diagram()
{
    return OnLoadDiagram;
}

void set_on_load_diagram(bool y)
{
    OnLoadDiagram = y;
}


const char * stringify(UmlVisibility v)
{
    //QLOG_INFO() << "Call to stringigy detected";
    switch (v) {
    case UmlPublic:
        return "public";

    case UmlProtected:
        return "protected";

    case UmlPrivate:
        return "private";

    case UmlPackageVisibility:
        return "package";

    case UmlDefaultVisibility:
        return "default";

    default:
        return "WRONG VISIBILITY";
    }
}

const char * stringify(UmlParamDirection d)
{
    switch (d) {
    case UmlInOut:
        return "inout";

    case UmlIn:
        return "in";

    case UmlOut:
        return "out";

    default:
        return "return";
    }
}

const char * stringify(UmlParamEffect e)
{
    switch (e) {
    case UmlNoEffect:
        return "none";

    case UmlCreate:
        return "create";

    case UmlRead:
        return "read";

    case UmlUpdate:
        return "update";

    case UmlDelete:
        return "delete";

    default:
        return "WRONG CODE";
    }
}

const char * stringify(UmlActionKind a)
{
    switch (a) {
    case UmlOpaqueAction:
        return "opaque_action";

    case UmlAcceptEventAction:
        return "accept_event_action";

    case UmlReadVariableValueAction:
        return "read_variable_value_action";

    case UmlClearVariableValueAction:
        return "clear_variable_value_action";

    case UmlWriteVariableValueAction:
        return "write_variable_value_action";

    case UmlAddVariableValueAction:
        return "add_variable_value_action";

    case UmlRemoveVariableValueAction:
        return "remove_variable_value_action";

    case UmlCallBehaviorAction:
        return "call_behavior_action";

    case UmlCallOperationAction:
        return "call_operation_action";

    case UmlSendObjectAction:
        return "send_object_action";

    case UmlSendSignalAction:
        return "send_signal_action";

    case UmlBroadcastSignalAction:
        return "broadcast_signal_action";

    case UmlUnmarshallAction:
        return "unmarshall_action";

    case UmlValueSpecificationAction:
        return "value_specification_action";

    case UmlAcceptCallAction:
        return "accept_call_action";

    case UmlReplyAction:
        return "reply_action";

    case UmlCreateObjectAction:
        return "create_object_action";

    case UmlDestroyObjectAction:
        return "destroy_object_action";

    case UmlTestIdentityAction:
        return "test_identity_action";

    case UmlRaiseExceptionAction:
        return "raise_exception_action";

    case UmlReduceAction:
        return "reduce_action";

    default:
        return "WRONG CODE";
    }
}

const char * stringify(UmlExpansionKind k)
{
    switch (k) {
    case UmlParallel:
        return "parallel";

    case UmlIterative:
        return "iterative";

    case UmlStream:
        return "stream";

    default:
        return "WRONG CODE";
    }
}

const char * stringify(UmlOrderingKind k)
{
    switch (k) {
    case UmlUnordered:
        return "unordered";

    case UmlOrdered:
        return "ordered";

    case UmlLifo:
        return "lifo";

    case UmlFifo:
        return "fifo";

    default:
        return "WRONG CODE";
    }
}


const char * stringify(UmlCode c)
{
    switch (c)
    {
    case UmlAssociation:
        return "----";

    case UmlDirectionalAssociation:
        return "--->";

    case UmlGeneralisation:
    case UmlInherit:
        return "---|>";

    case UmlAggregation:
        return "o---";

    case UmlAggregationByValue:
        return "*---";

    case UmlDirectionalAggregation:
        return "o-->";

    case UmlDirectionalAggregationByValue:
        return "*-->";

    case UmlDependency:
    case UmlDependOn:
        return "-_->";

    case UmlRealize:
        return "-_-|>";

    case UmlAnchor:
        return "-_-_";

    case UmlProvided:
        return "---O";

    case UmlRequired:
        return "---(";

    case UmlInner:
        return "---+";

    case InitialPS:
        return "initial";

    case EntryPointPS:
        return "entry_point";

    case FinalPS:
        return "final";

    case TerminatePS:
        return "terminate";

    case ExitPointPS:
        return "exit_point";

    case DeepHistoryPS:
        return "deep_history";

    case ShallowHistoryPS:
        return "shallow_history";

    case JunctionPS:
        return "junction";

    case ChoicePS:
        return "choice";

    case ForkPS:
        return "fork";

    case JoinPS:
        return "join";

    case InitialAN:
        return "initial_node";

    case FlowFinalAN:
        return "flow_final";

    case ActivityFinalAN:
        return "activity_final";

    case DecisionAN:
        return "decision";

    case MergeAN:
        return "merge";

    case ForkAN:
        return "fork";

    case JoinAN:
        return "join";

    case UmlActivityAction:     // for InfoCanvas
        return "activity action";

    case UmlFlow:		      // for InfoCanvas
        return "flow";

    default:
        return "WRONG CODE";
    }
}

const char * stringify(Uml3States s)
{
    switch (s) {
    case UmlYes:
        return "yes";

    case UmlNo:
        return "no";

    case UmlDefaultState:
        return "default";

    default:
        return "WRONG 3STATES";
    }
}

const char * stringify(UmlColor c)
{
    switch (c) {
    case UmlTransparent:
        return "transparent";

    case UmlWhite:
        return "white";

    case UmlLightYellow:
        return "lightyellow";

    case UmlYellow:
        return "yellow";

    case UmlMediumYellow:
        return "mediumyellow";

    case UmlDarkYellow:
        return "gold";

    case UmlLightBlue:
        return "lightblue";

    case UmlBlue:
        return "blue";

    case UmlLightMediumBlue:
        return "lightmediumblue";

    case UmlMediumBlue:
        return "mediumblue";

    case UmlDarkBlue:
        return "darkblue";

    case UmlLightGreen:
        return "lightgreen";

    case UmlGreen:
        return "green";

    case UmlLightMediumGreen:
        return "lightmediumgreen";

    case UmlMediumGreen:
        return "mediumgreen";

    case UmlDarkGreen:
        return "darkgreen";

    case UmlRed:
        return "red";

    case UmlVeryLightGray:
        return "verylightgray";

    case UmlLightGray:
        return "lightgray";

    case UmlGray:
        return "gray";

    case UmlDarkGray:
        return "darkgray";

    case UmlBlack:
        return "black";

    case UmlLightRed:
        return "lightred";

    case UmlMidRed:
        return "midred";

    case UmlVeryLightOrange:
        return "verylightorange";

    case UmlLightOrange:
        return "lightorange";

    case UmlOrange:
        return "orange";

    case UmlDarkOrange:
        return "darkorange";

    case UmlLightMagenta:
        return "lightmagenta";

    case UmlMagenta:
        return "magenta";

    case UmlMidMagenta:
        return "midmagenta";

    case UmlDarkMagenta:
        return "darkmagenta";

    case UmlDefaultColor:
        return "default";

    default:
        return "WRONG COLOR";
    }
}

const char * stringify(UmlFont f)
{
    switch (f) {
    case UmlNormalFont:
        return "normal";

    case UmlNormalBoldFont:
        return "bold";

    case UmlNormalItalicFont:
        return "italic";

    case UmlNormalBoldItalicFont:
        return "bold_italic";

    case UmlSmallFont:
        return "small";

    case UmlSmallBoldFont:
        return "small_bold";

    case UmlSmallItalicFont:
        return "small_italic";

    case UmlSmallBoldItalicFont:
        return "small_bold_italic";

    case UmlLargeFont:
        return "large";

    case UmlLargeBoldFont:
        return "large_bold";

    case UmlLargeItalicFont:
        return "large_italic";

    case UmlLargeBoldItalicFont:
        return "large_bold_italic";

    case UmlNormalUnderlinedFont:
        return "underlined";

    case UmlNormalStrikeOutFont:
        return "strikeout";

    default:
        return "WRONG FONT";
    }
}

const char * stringify(ClassDrawingMode v)
{
    switch (v) {
    case asClass:
        return "class";

    case asInterface:
        return "interface";

    case asControl:
        return "control";

    case asBoundary:
        return "boundary";

    case asEntity:
        return "entity";

    case asActor:
        return "actor";

    case Natural:
        return "natural";

    case DefaultClassDrawingMode:
        return "default";

    default:
        return "WRONG CLASSDRAWINGMODE";
    }
}

const char * stringify(DrawingLanguage l)
{
    switch (l) {
    case UmlView:
        return "uml";

    case CppView:
        return "c++";

    case JavaView:
        return "java";

    case PhpView:
        return "php";

    case PythonView:
        return "python";

    case IdlView:
        return "idl";

    case DefaultDrawingLanguage:
        return "default";

    default:
        return "WRONG DRAWINGLANGUAGE";
    }
}

const char * stringify(ShowContextMode v)
{
    switch (v) {
    case noContext:
        return "no";

    case umlContext:
        return "uml";

    case namespaceContext:
        return "namespace";

    case javaPackageContext:
        return "java_package";

    case pythonPackageContext:
        return "python_package";

    case moduleContext:
        return "module";

    case DefaultShowContextMode:
        return "default";

    default:
        return "WRONG CLASSVIEWMODE";
    }
}

const char * stringify(CanvasFormat v)
{
    switch (v) {
    case IsoA0:
        return "A0";

    case IsoA1:
        return "A1";

    case IsoA2:
        return "A2";

    case IsoA3:
        return "A3";

    case IsoA4:
        return "A4";

    case IsoA5:
        return "A5";

    case UsA:
        return "A";

    case UsB:
        return "B";

    case UsC:
        return "C";

    case UsD:
        return "D";

    case UsE:
        return "E";

    case UsLetter:
        return "Letter";

    case UsLegal:
        return "Legal";

    case UsTabloid:
        return "Tabloid";

    case IsoA0Landscape:
        return "A0-Landscape";

    case IsoA1Landscape:
        return "A1-Landscape";

    case IsoA2Landscape:
        return "A2-Landscape";

    case IsoA3Landscape:
        return "A3-Landscape";

    case IsoA4Landscape:
        return "A4-Landscape";

    case IsoA5Landscape:
        return "A5-Landscape";

    case UsALandscape:
        return "A-Landscape";

    case UsBLandscape:
        return "B-Landscape";

    case UsCLandscape:
        return "C-Landscape";

    case UsDLandscape:
        return "D-Landscape";

    case UsELandscape:
        return "E-Landscape";

    case UsLetterLandscape:
        return "Letter-Landscape";

    case UsLegalLandscape:
        return "Legal-Landscape";

    case UsLedger:
        return "Ledger";

    default:
        return "WRONG CanvasFormat";
    }
}

const char * stringify(LineGeometry v)
{
    switch (v) {
    case HVGeometry:
        return "HV";

    case HVrGeometry:
        return "HVr";

    case VHGeometry:
        return "VH";

    case VHrGeometry:
        return "VHr";

    case HVHGeometry:
        return "HVH";

    case VHVGeometry:
        return "VHV";

    default:
        return "WRONG LineGeometry";
    }
}

UmlCode arrow_type(QString s)
{
    if (s =="---|>")
        return UmlGeneralisation;

    if (s =="----")
        return UmlAssociation;

    if (s =="--->")
        return UmlDirectionalAssociation;

    if (s =="-_->")
        return UmlDependency;

    if (s =="-_-_")
        return UmlAnchor;

    if (s =="---(")
        return UmlRequired;

    if (s =="---O")
        return UmlProvided;

    if (s =="---+")
        return UmlInner;

    msg_critical("Error",
                 Context.filename + " : " + s + " is not a line kind");
    THROW_ERROR 0;
}

UmlCode relation_type(QString s, bool simple_relation)
{
    if (s =="----")
        return UmlAssociation;

    if (s =="--->")
        return UmlDirectionalAssociation;

    if (s =="---|>")
        return (simple_relation)
                ? UmlInherit
                : UmlGeneralisation;

    if (s =="o---")
        return UmlAggregation;

    if (s =="*---")
        return UmlAggregationByValue;

    if (s =="<>-->" || s =="o-->")
        return UmlDirectionalAggregation;

    if (s =="<#>-->" || s =="*-->")
        return UmlDirectionalAggregationByValue;

    if (s =="-_->")
        return (simple_relation)
                ? UmlDependOn
                : UmlDependency;

    if (s =="-_-|>")
        return UmlRealize;

    msg_critical("Error",
                 Context.filename + " : " + s + " is not a relation kind");
    THROW_ERROR 0;
}

bool IsaSimpleRelation(UmlCode c)
{
    switch (c) {
    case UmlDependOn:
    case UmlInherit:
        return TRUE;

    default:
        return FALSE;
    }
}

UmlVisibility visibility(QString s)
{
    if (s == "public")
        return UmlPublic;
    else if (s == "protected")
        return UmlProtected;
    else if (s == "private")
        return UmlPrivate;
    else if (s == "package")
        return UmlPackageVisibility;
    else if (s == "default")
        return UmlDefaultVisibility;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not a visibility"));
    THROW_ERROR 0;
}

UmlParamDirection direction(QString s)
{
    if (s =="inout")
        return UmlInOut;

    if (s =="in")
        return UmlIn;

    if (s =="out")
        return UmlOut;

    if (s =="return")
        return UmlReturn;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not a direction"));
    THROW_ERROR 0;
}

UmlParamEffect effect(QString s)
{
    if (s =="none")
        return UmlNoEffect;

    if (s =="create")
        return UmlCreate;

    if (s =="read")
        return UmlRead;

    if (s =="update")
        return UmlUpdate;

    if (s =="delete")
        return UmlDelete;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not a parameter effect"));
    THROW_ERROR 0;
}

UmlActionKind activity_action_kind(QString s)
{
    if (s =="opaque_action")
        return UmlOpaqueAction;

    if (s =="accept_event_action")
        return UmlAcceptEventAction;

    if (s =="read_variable_value_action")
        return UmlReadVariableValueAction;

    if (s =="clear_variable_value_action")
        return UmlClearVariableValueAction;

    if (s =="write_variable_value_action")
        return UmlWriteVariableValueAction;

    if (s =="add_variable_value_action")
        return UmlAddVariableValueAction;

    if (s =="remove_variable_value_action")
        return UmlRemoveVariableValueAction;

    if (s =="call_behavior_action")
        return UmlCallBehaviorAction;

    if (s =="call_operation_action")
        return UmlCallOperationAction;

    if (s =="send_object_action")
        return UmlSendObjectAction;

    if (s =="send_signal_action")
        return UmlSendSignalAction;

    if (s =="broadcast_signal_action")
        return UmlBroadcastSignalAction;

    if (s =="unmarshall_action")
        return UmlUnmarshallAction;

    if (s =="value_specification_action")
        return UmlValueSpecificationAction;

    if (s =="accept_call_action")
        return UmlAcceptCallAction;

    if (s =="reply_action")
        return UmlReplyAction;

    if (s =="create_object_action")
        return UmlCreateObjectAction;

    if (s =="destroy_object_action")
        return UmlDestroyObjectAction;

    if (s =="test_identity_action")
        return UmlTestIdentityAction;

    if (s =="raise_exception_action")
        return UmlRaiseExceptionAction;

    if (s =="reduce_action")
        return UmlReduceAction;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not an activity action kind"));
    THROW_ERROR 0;
}

UmlExpansionKind expansion_mode_kind(QString s)
{
    if (s =="parallel")
        return UmlParallel;

    if (s =="iterative")
        return UmlIterative;

    if (s =="stream")
        return UmlStream;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not an expansion node kind"));
    THROW_ERROR 0;
}

UmlOrderingKind ordering(QString s)
{
    if (s == "unordered")
        return UmlUnordered;

    if (s =="ordered")
        return UmlOrdered;

    if (s =="lifo")
        return UmlLifo;

    if (s =="fifo")
        return UmlFifo;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not an ordering"));
    THROW_ERROR 0;
}

Uml3States state(QString s)
{
    if (s =="yes")
        return UmlYes;

    if (s =="no")
        return UmlNo;

    if (s =="default")
        return UmlDefaultState;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not a state"));
    THROW_ERROR 0;
}

ClassDrawingMode drawing_mode(QString s)
{
    if (s =="class")
        return asClass;

    if (s =="interface")
        return asInterface;

    if (s =="control")
        return asControl;

    if (s =="boundary")
        return asBoundary;

    if (s =="entity")
        return asEntity;

    if (s =="actor")
        return asActor;

    if (s =="natural")
        return Natural;

    if (s =="default")
        return DefaultClassDrawingMode;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not a class drawing mode"));
    THROW_ERROR 0;
}

DrawingLanguage drawing_language(QString s)
{
    if (s =="uml")
        return UmlView;

    if (s =="c++")
        return CppView;

    if (s =="java")
        return JavaView;

    if (s =="php")
        return PhpView;

    if (s =="python")
        return PythonView;

    if (s =="idl")
        return IdlView;

    if (s =="default")
        return DefaultDrawingLanguage;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not a drawing language"));
    THROW_ERROR 0;
}

ShowContextMode context_mode(QString s)
{
    if (s =="no")
        return noContext;

    if (s =="uml")
        return umlContext;

    if (s =="namespace")
        return namespaceContext;

    if (s =="package" || s =="java_package")
        return javaPackageContext;

    if (s =="python_package")
        return pythonPackageContext;

    if (s =="module")
        return moduleContext;

    if (s =="default")
        return DefaultShowContextMode;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not a context mode"));
    THROW_ERROR 0;
}

UmlColor color(QString s)
{
    if (s =="transparent")
        return UmlTransparent;

    if (s =="white")
        return UmlWhite;

    if (s =="lightyellow")
        return UmlLightYellow;

    if (s =="yellow")
        return UmlYellow;

    if (s =="mediumyellow")
        return UmlMediumYellow;

    if (s =="gold")
        return UmlDarkYellow;

    if (s =="lightblue")
        return UmlLightBlue;

    if (s =="lightmediumblue")
        return UmlLightMediumBlue;

    if (s =="mediumblue")
        return UmlMediumBlue;

    if (s =="blue")
        return UmlBlue;

    if (s =="darkblue")
        return UmlDarkBlue;

    if (s =="lightgreen")
        return UmlLightGreen;

    if (s =="green")
        return UmlGreen;

    if (s =="lightmediumgreen")
        return UmlLightMediumGreen;

    if (s =="mediumgreen")
        return UmlMediumGreen;

    if (s =="darkgreen")
        return UmlDarkGreen;

    if (s =="lightred")
        return UmlLightRed;

    if (s =="midred")
        return UmlMidRed;

    if (s =="red")
        return UmlRed;

    if (s =="verylightgray")
        return UmlVeryLightGray;

    if (s =="lightgray")
        return UmlLightGray;

    if (s =="gray")
        return UmlGray;

    if (s =="darkgray")
        return UmlDarkGray;

    if (s =="black")
        return UmlBlack;

    if (s =="verylightorange")
        return UmlVeryLightOrange;

    if (s =="lightorange")
        return UmlLightOrange;

    if (s =="orange")
        return UmlOrange;

    if (s =="darkorange")
        return UmlDarkOrange;

    if (s =="lightmagenta")
        return UmlLightMagenta;

    if (s =="magenta")
        return UmlMagenta;

    if (s =="midmagenta")
        return UmlMidMagenta;

    if (s =="darkmagenta")
        return UmlDarkMagenta;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not a color"));
    THROW_ERROR 0;
}

UmlFont font(QString s)
{
    if (s =="normal")
        return UmlNormalFont;

    if (s =="bold")
        return UmlNormalBoldFont;

    if (s =="italic")
        return UmlNormalItalicFont;

    if (s =="bold_italic")
        return UmlNormalBoldItalicFont;

    if (s =="small")
        return UmlSmallFont;

    if (s =="small_bold")
        return UmlSmallBoldFont;

    if (s =="small_italic")
        return UmlSmallItalicFont;

    if (s =="small_bold_italic")
        return UmlSmallBoldItalicFont;

    if (s =="large")
        return UmlLargeFont;

    if (s =="large_bold")
        return UmlLargeBoldFont;

    if (s =="large_italic")
        return UmlLargeItalicFont;

    if (s =="large_bold_italic")
        return UmlLargeBoldItalicFont;

    if (s =="underlined")
        return UmlNormalUnderlinedFont;

    if (s =="strikeout")
        return UmlNormalStrikeOutFont;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not a font"));
    THROW_ERROR 0;
}

CanvasFormat canvas_format(QString s)
{
    if (s =="A0")
        return IsoA0;

    if (s =="A1")
        return IsoA1;

    if (s =="A2")
        return IsoA2;

    if (s =="A3")
        return IsoA3;

    if (s =="A4")
        return IsoA4;

    if (s =="A5")
        return IsoA5;

    if (s =="A")
        return UsA;

    if (s =="B")
        return UsB;

    if (s =="C")
        return UsC;

    if (s =="D")
        return UsD;

    if (s =="E")
        return UsE;

    if (s =="Letter")
        return UsLetter;

    if (s =="Legal")
        return UsLegal;

    if (s =="Tabloid")
        return UsTabloid;

    if (s =="A0-Landscape")
        return IsoA0Landscape;

    if (s =="A1-Landscape")
        return IsoA1Landscape;

    if (s =="A2-Landscape")
        return IsoA2Landscape;

    if (s =="A3-Landscape")
        return IsoA3Landscape;

    if (s =="A4-Landscape")
        return IsoA4Landscape;

    if (s =="A5-Landscape")
        return IsoA5Landscape;

    if (s =="A-Landscape")
        return UsALandscape;

    if (s =="B-Landscape")
        return UsBLandscape;

    if (s =="C-Landscape")
        return UsCLandscape;

    if (s =="D-Landscape")
        return UsDLandscape;

    if (s =="E-Landscape")
        return UsELandscape;

    if (s =="Letter-Landscape")
        return UsLetterLandscape;

    if (s =="Legal-Landscape")
        return UsLegalLandscape;

    if (s =="Ledger")
        return UsLedger;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not a canvas format"));
    THROW_ERROR 0;
}

UmlCode pseudo_state_kind(QString s)
{
    if (s =="initial")
        return InitialPS;

    if (s =="entry_point")
        return EntryPointPS;

    if (s =="final")
        return FinalPS;

    if (s =="terminate")
        return TerminatePS;

    if (s =="exit_point")
        return ExitPointPS;

    if (s =="deep_history")
        return DeepHistoryPS;

    if (s =="shallow_history")
        return ShallowHistoryPS;

    if (s =="junction")
        return JunctionPS;

    if (s =="choice")
        return ChoicePS;

    if (s =="fork")
        return ForkPS;

    if (s =="join")
        return JoinPS;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not a pseudo state kind"));
    THROW_ERROR 0;
}

UmlCode activity_node_kind(QString s)
{
    if (s =="initial")
        // old releases
        return InitialAN;

    if (s =="initial_node")
        return InitialAN;

    if (s =="flow_final")
        return FlowFinalAN;

    if (s =="activity_final")
        return ActivityFinalAN;

    if (s =="decision")
        return DecisionAN;

    if (s =="merge")
        return MergeAN;

    if (s =="fork")
        return ForkAN;

    if (s =="join")
        return JoinAN;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not an activity node kind"));
    THROW_ERROR 0;
}

LineGeometry line_geometry(QString s)
{
    if (s =="HV")
        return HVGeometry;

    if (s =="HVr")
        return HVrGeometry;

    if (s =="VH")
        return VHGeometry;

    if (s =="VHr")
        return VHrGeometry;

    if (s =="HVH")
        return HVHGeometry;

    if (s =="VHV")
        return VHVGeometry;

    msg_critical(QObject::tr("Error"),
                 Context.filename + " : " + s + QObject::tr(" is not a line geometry"));
    THROW_ERROR 0;
}


//

void set_read_file_format(unsigned u)
{
    if(api_format(true) == 75)
        u = 75;
    Context.read_file_format = u;
}

unsigned read_file_format()
{
    return Context.read_file_format;
}

//

static char EmptyString[] = { 0 };
static char UnvalidString[] = { '?', '?', '?', 0 };

static void set_previous_word(char * s)
{
    if (Context.previous_word_allocated) {
        Context.previous_word_allocated = FALSE;
        delete [] Context.previous_word;
    }

    Context.previous_word = s;
}

void backup(QDir & d, const QString & fn)
{
    QString s;

    s.sprintf("_%u.bak", user_id());

    while (d.exists(fn) &&
           !d.rename(fn, fn + s) &&
           (msg_warning("Uml",
                        d.absoluteFilePath(fn) +
                        QObject::tr("\ncannot be renamed ") + fn + s +
                        QObject::tr(", retry ?\n"),
                        QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::Yes))
        ;
}

void delete_backup(const QDir & d)
{
    QStringList s;

    s.append(QString("*_%1.bak").arg(user_id()));
    //[lgfreitas] entryInfoList des not return a pointer anymore
    QFileInfoList l = d.entryInfoList(s);

    if (!l.empty()) {
        for (const auto& fileInfo : l) {
            QFile::remove(fileInfo.absoluteFilePath());
        }
    }
}

qint64 open_file(QFile & fp, int mode, bool silent)
{
    Context.filename = fp.fileName();

    while (! fp.open(QFlag(mode))) { //[lgfreitas] it does not accept a int anymore, but a QFlag. Hopefully, this does the trick
        if (silent ||
                (msg_critical("Uml",
                              Context.filename +
                              ((mode != QIODevice::ReadOnly)
                               ? QObject::tr("\ncannot be opened for write, retry ?\n")
                               : QObject::tr("\ncannot be opened for read, retry ?\n")),
                              QMessageBox::Yes, QMessageBox::No)
                 != QMessageBox::Yes))
            return -1;
    }

    if (mode == QIODevice::ReadOnly) {
        set_previous_word(EmptyString);
        Context.linenum = 1;
        Context.removed_char = ' ';
    }

    QFileInfo fi(fp);

    Context.read_only = !fi.isWritable() || ReadOnlyForced;
    return fi.size();
}

bool copy_file(QFileInfo * src, const QDir & dest)
{
    QString src_path = src->absoluteFilePath();
    FILE * fp_src;

    if ((fp_src = fopen((const char *) src_path.toLatin1().constData(), "rb")) == 0) {
        msg_critical("Error", QObject::tr("cannot open %1 to read it").arg(src_path),0,0,0);
        return FALSE;
    }

    QString dest_path = dest.absoluteFilePath(src->fileName());
    FILE * fp_dest;

    if ((fp_dest = fopen((const char *) dest_path.toLatin1().constData(), "wb")) == 0) {
        msg_critical("Error", QObject::tr("cannot create %1").arg(dest_path),0,0,0);
        fclose(fp_src);
        return FALSE;
    }

    char b[4096];
    size_t n;

    while ((n = fread(b, 1, sizeof(b) - 1, fp_src)) > 0) {
        if (fwrite(b, 1, n, fp_dest) != n) {
            msg_critical("Error", QObject::tr("error when writing %1").arg(dest_path));
            fclose(fp_src);
            fclose(fp_dest);
            return FALSE;
        }
    }

    if (! feof(fp_src)) {
        msg_critical("Error", QObject::tr("error when reading %1").arg(src_path));
        fclose(fp_src);
        fclose(fp_dest);
        return FALSE;
    }

    fclose(fp_src);
    fclose(fp_dest);
    return TRUE;
}

void save_if_needed(const char * filename, QString& newdef)
{
    QDir d = BrowserView::get_dir();
    QString path = d.absoluteFilePath(filename);
    FILE * fp = fopen(path.toLatin1().constData(), "rb");
    bool needed = false;

    if (fp == 0)
        needed = TRUE;
    else {
        QString oldDef;
        QTextStream st(fp, QIODevice::ReadOnly);
        st.setCodec("UTF-8");
        st>>oldDef;
        st.flush();
        if(newdef != oldDef)
            needed = true;
        fclose(fp);
    }

    if (needed) {
        backup(d, filename);

        for (;;) {
            while ((fp = fopen(path.toLatin1().constData(), "wb")) == 0)
                (void) msg_critical("Error", QObject::tr("Cannot create file\n") + path,
                                    QMessageBox::Retry);

            QLOG_INFO() << newdef;

            QTextStream st(fp);
            st.setCodec("UTF-8");
            st<<newdef;
            st.flush();
            break;

        }

        fclose(fp);
    }
}

char * read_file(QString filename)
{
    filename = (in_import())
            ? BrowserView::get_import_dir().absoluteFilePath(filename)
            : BrowserView::get_dir().absoluteFilePath(filename);

    QFile fp(filename);
    qint64 size;

    if ((size = open_file(fp, QIODevice::ReadOnly, TRUE)) != -1) {
        char * s = new char[size + 1];

        if (fp.read(s, size) == -1) {
            msg_critical(QObject::tr("Error"), QObject::tr("%1 cannot be read").arg(filename));
            delete [] s;
            return 0;
        }
        else {
            s[size] = 0;

            return s;
        }
    }
    else
        return 0;
}

QString abs_file_path(int id, const char * ext)
{
    QString s;
    if (ext[1] == 0)
        s.sprintf("%d_%d.%c", id,
                  root_permission() ? root_permission() : user_id(),
                  *ext);
    else
        s.sprintf("%d.%s", id, ext);

    return BrowserView::get_dir().absoluteFilePath(s);
}

char * read_definition(int id, const char * ext)
{
    QString s;
    char * r;

    // try to read modified definition

    s.sprintf("%d_%d.%c", id,
              root_permission() ? root_permission() : user_id(),
              *ext);

    if ((r = read_file(s)) != 0)
        return r;

    if (ext[1] == 0)
        // want the modified definition
        return 0;

    // try to read saved definition
    s.sprintf("%d.%s", id, ext);

    return read_file(s);
}

char * read_file(QString filename, qint64 offset, qint64 len)
{
    filename = BrowserView::get_dir().absoluteFilePath(filename);

    QFile fp(filename);

    if (open_file(fp, QIODevice::ReadOnly, TRUE) != -1) {
        char * s = new char[len + 1];

        fp.seek(offset);

        if (fp.read(s, len) == -1) {
            msg_critical(QObject::tr("Error"), QObject::tr("%1 cannot be read").arg(filename));
            delete [] s;
            return 0;
        }
        else {
            s[len] = 0;

            return s;
        }
    }
    else
        return 0;
}

char * read_definition(int id, const char * ext, qint64 offset, qint64 len)
{
    if (len == 0)
        return 0;

    QString s;
    char * r;

    // try to read modified definition

    s.sprintf("%d_%d.%c", id,
              root_permission() ? root_permission() : user_id(),
              *ext);

    if ((r = read_file(s, offset, len)) != 0)
        return r;

    if (ext[1] == 0)
        // want the modified definition
        return 0;

    // try to read saved definition
    s.sprintf("%d.%s", id, ext);

    return read_file(s, offset, len);
}

void save_definition(int id, const char * ext, QString def,
                     BooL & is_new)
{
    QString s;
    FILE * fp;
    QDir d = BrowserView::get_dir();

    if (ext[1] != 0) {
        // saved definition
        s.sprintf("%d.%s", id, ext);

        if (is_new)
            is_new = FALSE;
        else
            backup(d, s);
    }
    else
        // modified definition
        s.sprintf("%d_%d.%s", id,
                  root_permission() ? root_permission() : user_id(),
                  ext);

    s = d.absoluteFilePath(s);

    for (;;) {
        while ((fp = fopen(s.toLatin1().constData(), "wb")) == 0)
            (void) msg_critical(QObject::tr("Error"),
                                QObject::tr("Cannot create file\n") + s,
                                QMessageBox::Retry);

        QTextStream st(fp);
        st.setCodec("UTF-8");
        st<<def;
        st.flush();
        break;
    }

    fclose(fp);
}

void delete_definition(int id, const char * ext)
{
    QDir d = BrowserView::get_dir();
    QString s;

    if (ext[1] != 0) {
        // saved definition
        s.sprintf("%d.%s", id, ext);
        backup(d, s);
    }
    else {
        // modified definition
        s.sprintf("%d_%d.%s", id, user_id(), ext);
        QFile::remove(d.absoluteFilePath(s));
    }
}

void read_in(const QString & s)
{
    set_previous_word(EmptyString);
    Context.linenum = 1;
    Context.removed_char = ' ';
    Context.filename = s;
}

static QString where()
{
    return Context.filename + QObject::tr(" line ") + QString::number(Context.linenum) + "\n";
}

//

void save_string(QString p, QTextStream & st)
{
    QString toWrite = p;
    toWrite.replace('\\',"\\\\");
    toWrite.replace('"',"\\\"");
        st << '"';
        st << toWrite;
        st << '"';
}

void save_string_list(const QStringList & list, QTextStream & st)
{
    st << " " << list.count();

    for (const auto& s : list) {
        st << " ";
        save_string(s, st);
    }
}

void save_unicode_string_list(const QStringList & list, QTextStream & st)
{
    st << " " << list.count();
    for (const auto& s : list) {
        st << " ";
        save_string(s, st);
    }
}

void nl_indent(QTextStream & st)
{
    int i = Indent;

    st << '\n';

    while (i >= 4) {
        st << '\t';
        i -= 4;
    }

    static const char * const ss[] = { "", "  ", "    ", "      " };

    st << ss[i];
}

void indent(int d)
{
    Indent += d;
}

int indent()
{
    return Indent;
}

void indent0()
{
    Indent = 0;
}

//

static void skip_whitespace_comment(char *& s)
{
    if (Context.removed_char != ' ')
        *--s = Context.removed_char;

    for (;;) {
        switch (*s) {
        case '\n':
            Context.linenum += 1;

        case ' ' :
        case '\r':
        case '\t':
            s += 1;
            break;

        case '/':
            for (s += 2; *s != '\n'; s += 1)
                if (*s == 0)
                    return;

            Context.linenum += 1;
            s += 1;
            break;

        default:
            return;
        }
    }
}

bool at_end(char *& st)
{
    skip_whitespace_comment(st);

    return (*st == 0);
}

QString read_string(char *& st)
{
    skip_whitespace_comment(st);

    if (*st == 0) {
        msg_critical(QObject::tr("Error"),
                     where() + QObject::tr("premature end of file, string expected"));
        THROW_ERROR 0;
    }

    if (*st != '"') {
        msg_critical(QObject::tr("Error"),
                     where() + QObject::tr("string expected after '%1'").arg(Context.previous_word));
        THROW_ERROR 0;
    }

    char * r = st + 1;
    char * p = r;

    for (;;) {
        switch (*++st) {
        case 0:
            msg_critical(QObject::tr("Error"),
                         where() + QObject::tr("premature end of file reading a string"));
            THROW_ERROR 0;

        case '"':
            Context.removed_char = ' ';
            *st++ = 0;
            *p = 0;

            if (Context.previous_word_allocated)
                delete [] Context.previous_word;
            else
                Context.previous_word_allocated = TRUE;

            Context.previous_word = new char[p - r + 1];
            memcpy(Context.previous_word, r, p - r + 1);
            return QString::fromUtf8(r);

        case '\\':
            if (*++st  == 0) {
                msg_critical(QObject::tr("Error"),
                             where() + QObject::tr("premature end of file reading a string"));
                THROW_ERROR 0;
            }

            *p++ = *st;
            break;

        case '\r':
            break;

        case '\n':
            Context.linenum += 1;

            // no break
        default:
            *p++ = *st;
        }
    }
}

char * read_keyword(char *& st)
{
    skip_whitespace_comment(st);

    if (*st == 0) {
        msg_critical(QObject::tr("Error"),
                     where() + QObject::tr("premature end of file, keyword expected"));
        THROW_ERROR 0;
    }

    if (*st == '"') {
        msg_critical(QObject::tr("Error"),
                     where() +
                     QObject::tr("keyword expected after '%1'").arg(Context.previous_word));
        THROW_ERROR 0;
    }

    char * r = st;
    char c;

    do
        c = *++st;

    while ((c > ' ') && (c != '"'));

    Context.removed_char = c;
    *st++ = 0;
    set_previous_word(r);
    return r;
}

char * read_keyword(char *& st, const char * expected)
{
    skip_whitespace_comment(st);

    if (*st == 0) {
        msg_critical(QObject::tr("Error"),
                     where() +
                     QObject::tr("premature end of file, '%1' expected",
                                 expected));
        THROW_ERROR 0;
    }

    if (*st == '"') {
        msg_critical("Error",
                     where() +
                     QObject::tr("'%1' expected rather than a string after '%2'").arg(expected).arg(Context.previous_word),0,0,0);
        THROW_ERROR 0;
    }

    char * r = st;
    char c;

    do
        c = *++st;

    while ((c > ' ') && (c != '"'));

    *st++ = 0;

    if (strcmp(r, expected)) {
        msg_critical("Error",
                     where() +
                     QObject::tr("'%1' expected rather than '%2' after '%3'").arg(expected).arg(r).arg(Context.previous_word));
        THROW_ERROR 0;
    }

    set_previous_word(r);
    Context.removed_char = c;

    return r;
}

// for old versions
char * read_keyword(char *& st, const char * expected1,
                    const char * expected2)
{
    skip_whitespace_comment(st);

    if (*st == 0) {
        msg_critical("Error",
                     where() +
                     QObject::tr("premature end of file, '%1' expected").arg(expected1));
        THROW_ERROR 0;
    }

    if (*st == '"') {
        msg_critical("Error",
                     where() +
                     QObject::tr("'%1' expected rather than a string after '%2'").arg(expected1).arg(Context.previous_word));
        THROW_ERROR 0;
    }

    char * r = st;
    char c;

    do
        c = *++st;

    while ((c > ' ') && (c != '"'));

    *st++ = 0;

    if (strcmp(r, expected1) && strcmp(r, expected2)) {
        msg_critical("Error",
                     where() +
                     QObject::tr("'%1' expected rather than '%2' after '%3'").arg(expected1).arg(r).arg(Context.previous_word));
        THROW_ERROR 0;
    }

    set_previous_word(r);
    Context.removed_char = c;

    return r;
}

void unread_keyword(char * keyword, char *& st)
{
    // not at the beginning !
    st[-1] = Context.removed_char;
    Context.removed_char = ' ';

    st = keyword;
    set_previous_word(UnvalidString);	// to free Context.previous_word if necessary
}


int read_id(char *& st)
{
    skip_whitespace_comment(st);

    if (*st == 0) {
        msg_critical("Error",
                     where() + QObject::tr("premature end of file, id expected"));
        THROW_ERROR 0;
    }

    if (*st == '"') {
        msg_critical("Error",
                     where() + QObject::tr("id expected after '%1'").arg(Context.previous_word));
        THROW_ERROR 0;
    }

    char * r = st;
    int result = (int) strtol(r, &st, 10);

    if (st == r) {
        msg_critical(QObject::tr("Error"),
                     where() + QObject::tr("id expected after '%1'").arg(Context.previous_word));
        THROW_ERROR 0;
    }

    Context.removed_char = *st;
    *st++ = 0;
    set_previous_word(r);

    return result;
}

unsigned read_unsigned(char *& st)
{
    skip_whitespace_comment(st);

    if (*st == 0) {
        msg_critical(QObject::tr("Error"),
                     where() + QObject::tr("premature end of file, unsigned expected"));
        THROW_ERROR 0;
    }

    if (*st == '"') {
        msg_critical(QObject::tr("Error"),
                     where() + QObject::tr("unsigned expected after '%1'").arg(Context.previous_word));
        THROW_ERROR 0;
    }

    int result = 0;
    char * r = st;
    char c;

    while (((c = *st++) >= '0') && (c <= '9'))
        result = result * 10 + c - '0';

    if (st == r + 1) {
        msg_critical(QObject::tr("Error"),
                     where() + QObject::tr("unsigned expected after '%1'").arg(Context.previous_word));
        THROW_ERROR 0;
    }

    st[-1] = 0;
    set_previous_word(r);
    Context.removed_char = c;

    return result;
}

double read_double(char *& st)
{
    skip_whitespace_comment(st);

    if (*st == 0) {
        msg_critical(QObject::tr("Error"),
                     where() + QObject::tr("premature end of file, float expected"));
        THROW_ERROR 0;
    }

    if (*st == '"') {
        msg_critical(QObject::tr("Error"),
                     where() + QObject::tr("float expected after '%1'").arg(Context.previous_word));
        THROW_ERROR 0;
    }

    char * r = st;
    QString s;
    char c;

    while (((c = *st++) > ' ') && (c != '"'))
        s += c;

    bool ok;
    double result = s.toDouble(&ok);

    if (!ok) {
        // '.' or ',' problem ?
        int index = s.indexOf(',');

        if (index != -1) {
            s.replace(index, 1, ".");
            result = s.toDouble(&ok);
        }
        else if ((index = s.indexOf('.')) != -1) {
            s.replace(index, 1, ",");
            result = s.toDouble(&ok);
        }

        if (!ok) {
            msg_critical(QObject::tr("Error"),
                         where() +
                         QObject::tr("float expected rather than '%1' after '%2'").arg(s).arg(Context.previous_word));
            THROW_ERROR 0;
        }
    }

    Context.removed_char = c;
    st[-1] = 0;
    set_previous_word(r);

    return result;
}

void read_string_list(QStringList & list, char *& st)
{
    int n = read_unsigned(st);

    list.clear();

    while (n--)
        list.append(read_string(st));
}

void read_unicode_string_list(QStringList & list, char *& st)
{
    int n = read_unsigned(st);

    list.clear();

    while (n--)
        list.append(read_string(st));
}

char * skip_until(char *& st, const char * expected)
{
    char * k;

    for (;;) {
        skip_whitespace_comment(st);

        if (*st == 0) {
            msg_critical(QObject::tr("Error"),
                         where() + QObject::tr("premature end of file"));
            THROW_ERROR 0;
        }

        if (*st == '"')
            read_string(st);
        else if (strcmp((k = read_keyword(st)), expected) == 0)
            return k;
    }
}

void wrong_keyword(const char * k, const char * expected)
{
    msg_critical(QObject::tr("Error"),
                 where() + QObject::tr("'%1' expected rather than '%2'").arg(expected).arg(k));
    THROW_ERROR 0;
}

void unknown_keyword(const char * k)
{
    msg_critical(QObject::tr("Error"),
                 where() + QObject::tr("unknown or unexpected keyword '%1'").arg(k));
    THROW_ERROR 0;
}

void unknown_ref(const char * kind, int id)
{
    msg_critical(QObject::tr("Error"),
                 where() +
                 QObject::tr("%1 unknown %2 identifier").arg(QString::number(id).arg(QString(kind))));
    THROW_ERROR 0;
}

//

void save_xy(QTextStream & st, const QGraphicsItem * c, const char * s)
{
#ifdef FORCE_INT_COORD
    st << s << " " << (int) c->x() << " " << (int) c->y();
#else
    QString xs, ys;

    st << s << " " << xs.setNum(c->x()) << " " << ys.setNum(c->y());
#endif
}

void save_xyz(QTextStream & st, const QGraphicsItem * c, const char * s)
{
#ifdef FORCE_INT_COORD
    st << s << " " << (int) c->x() << " " << (int) c->y() << " " << (int) c->z();
#else
    QString xs, ys, zs;

    st << s << " " << xs.setNum(c->x()) << " " << ys.setNum(c->y()) << " " << zs.setNum(0);
#endif
}

void save_xyzwh(QTextStream & st, const QGraphicsRectItem *c, const char * s)
{
#ifdef FORCE_INT_COORD
    st << s << " " << (int) c->x() << " " << (int) c->y() << " " << (int) c->z()
       << " " << (int) c->width() << " " << (int) c->height();
#else
    QString xs, ys, zs;
/*
    st << s << " " << xs.setNum(c->boundingRect().x()) << " " << ys.setNum(c->boundingRect().y()) << " " << zs.setNum(0)
       << " " << (int) c->boundingRect().width() << " " << (int) c->boundingRect().height();
       */
    st << s << " " << xs.setNum(c->x()) << " " << ys.setNum(c->y()) << " " << zs.setNum(c->zValue())
       << " " << (int) c->rect().width() << " " << (int) c->rect().height();

#endif
}

void read_xy(char *& st, QGraphicsItem * c)
{
    double px = read_double(st);
    double py = read_double(st);

    c->moveBy(px - c->x(), py - c->y());
}

void bypass_xy(char *& st)
{
    read_double(st);
    read_double(st);
}

void read_xyz(char *& st, QGraphicsItem * c)
{
    double px = read_double(st);
    double py = read_double(st);

    c->moveBy(px - c->x(), py - c->y());
    c->setZValue(read_double(st));
}

void read_xyzwh(char *& st, QGraphicsRectItem * c)
{
    double px = read_double(st);
    double py = read_double(st);

    c->moveBy(px - c->x(), py - c->y());
    c->setZValue(read_double(st));

    double w = read_double(st);

    c->setRect(0,0,(int) w, (int) read_double(st));
}

void read_zwh(char *& st, QGraphicsRectItem * c)
{
    c->setZValue(read_double(st));

    double w = read_double(st);
    c->setRect(0,0,(int) w, (int) read_double(st));
}

void save_color(QTextStream & st, const char * s, UmlColor c, BooL & nl)
{
    if (c != UmlDefaultColor) {
        if (!nl) {
            nl_indent(st);
            nl = TRUE;
        }

        st << s << " " << stringify(c) << " ";
    }
}

void read_color(char *& st, const char * key, UmlColor & c, char *& k)
{
    if (!strcmp(k, key)) {
        c = color(read_keyword(st));

        if ((Context.read_file_format < 23) && (c == UmlWhite))
            c = UmlTransparent;

        k = read_keyword(st);
    }
}

void read_font(char *& st, const char * key, UmlFont & f, char *& k)
{
    if (!strcmp(k, key)) {
        f = font(read_keyword(st));
        k = read_keyword(st);
    }
    else
        f = UmlNormalFont;
}

//

void warn(const QString & s)
{
    // not showed dialog to compute needed size

    QDialog d_aux;
    QVBoxLayout * vbox_aux = new QVBoxLayout(&d_aux);

    vbox_aux->setMargin(5);
    QTextEdit * e = new QTextEdit(&d_aux);
    e->setText(s);

    // showed dialog

    QDialog * d = new QDialog;

    d->setWindowTitle("Uml");

    QVBoxLayout * vbox = new QVBoxLayout(d);

    vbox->setMargin(5);
    QTextEdit * t = new QTextEdit(d);
    QFontMetrics fm(QApplication::font());
    //int w = e->maxLineWidth() + e->minimumSizeHint().width();
    int maxw = (UmlWindow::get_workspace()->width() * 4) / 5;
    int maxh = (UmlWindow::get_workspace()->height() * 4) / 5;
    int he = (e->toPlainText().count('\n') + 5) * fm.height();

    t->setText(s);
    t->setMinimumSize(maxw, //(w > maxw) ? maxw : w,
                      (he > maxh) ? maxh : he);

    vbox->addWidget(t);
    d->show();
}

// history save/load

void save(const DiagramItem * di, QBuffer & b)
{
    b.write((char *) &di, sizeof(di));
}

DiagramItem * load_item(QBuffer & b)
{
    DiagramItem * di;

    b.read((char *) &di, sizeof(di));
    return di;
}

void save_ptr(const void * ptr, QBuffer & b)
{
    b.write((char *) &ptr, sizeof(ptr));
}

void * load_ptr(QBuffer & b)
{
    void * ptr;

    b.read((char *) &ptr, sizeof(ptr));
    return ptr;
}

void save(const QPoint & p, QBuffer & b)
{
    int i;

    i = p.x();
    b.write((char *) &i, sizeof(i));
    i = p.y();
    b.write((char *) &i, sizeof(i));
}

void load(QPoint & p, QBuffer & b)
{
    int i;

    b.read((char *) &i, sizeof(i));
    p.setX(i);

    b.read((char *) &i, sizeof(i));
    p.setY(i);
}

void save(const QPolygon & a, QBuffer & b)
{
    const QPoint * p = a.data();

    for (int i = 0; i != a.size(); i += 1)
        save(*p++, b);
}

void load(QPolygon & a, QBuffer & b)
{
    QPoint * p = a.data();

    for (int i = 0; i != a.size(); i += 1)
        load(*p++, b);
}

void save(double d, QBuffer & b)
{
    b.write((char *) &d, sizeof(d));
}

void load(double & d, QBuffer & b)
{
    b.read((char *) &d, sizeof(d));
}

double load_double(QBuffer & b)
{
    double d;

    b.read((char *) &d, sizeof(d));
    return d;
}

void save(float f, QBuffer & b)
{
    b.write((char *) &f, sizeof(f));
}

void load(float & f, QBuffer & b)
{
    b.read((char *) &f, sizeof(f));
}

double load_float(QBuffer & b)
{
    float f;

    b.read((char *) &f, sizeof(f));
    return f;
}

void save(int i, QBuffer & b)
{
    b.write((char *) &i, sizeof(i));
}

void load(int & i, QBuffer & b)
{
    b.read((char *) &i, sizeof(i));
}

// svg

static FILE * svg_fp;
static int pict_height;

bool start_svg(QString f, int w, int h)
{
    svg_fp = fopen(f.toLatin1().constData(), "w");

    if (svg_fp == 0)
        return FALSE;

    (void) fprintf(svg_fp,
                   "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
                   "<!-- Created with Douml (http://sourceforge.net/projects/douml/) -->\n"
                   "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
                   "<svg width=\"%d\" height=\"%d\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n",
                   w, h);

    pict_height = h;

    return TRUE;
}

void end_svg()
{
    (void) fputs("</svg>\n", svg_fp);

    fclose(svg_fp);
    svg_fp = 0;
}

FILE * svg()
{
    return svg_fp;
}

int svg_height()
{
    return pict_height;
}

const char * svg_color(UmlColor c)
{
    static const char * svg[UmlDefaultColor];
    const char * r = svg[c];

    if (r == 0) {
        if (c == UmlTransparent)
            r = svg[c] = "none";
        else {
            QColor col = color(c);

            r = svg[c] = new char[16];
            sprintf((char *) r, "#%06x", col.rgb() & 0xffffff);
        }
    }

    return r;
}

void draw_poly(FILE * fp, QPolygon & poly, UmlColor color, bool stroke)
{
    (void) fprintf(fp, (stroke) ? "\t<polygon fill=\"%s\" stroke=\"black\" stroke-opacity=\"1\""
                                : "\t<polygon fill=\"%s\" stroke=\"none\"",
                   svg_color(color));

    const char * sep = " points=\"";
    int n = poly.size();
    int i;

    for (i = 0; i != n; i += 1) {
        (void) fprintf(fp, "%s%d,%d", sep, poly.point(i).x(), poly.point(i).y());
        sep = " ";
    }

    (void) fputs("\" />\n", fp);
}

void draw_shadow(FILE * fp, QPolygon & poly)
{
    (void) fprintf(fp, "\t<polygon fill=\"#%06x\" stroke=\"none\"",
                   QColor(::Qt::darkGray).rgb() & 0xffffff);

    const char * sep = " points=\"";
    int n = poly.size();
    int i;

    for (i = 0; i != n; i += 1) {
        (void) fprintf(fp, "%s%d,%d", sep, poly.point(i).x(), poly.point(i).y());
        sep = " ";
    }

    (void) fputs("\" />\n", fp);
}

void draw_text(const QRect & r, int align, const QString & s, const QFont & fn, FILE * fp)
{
    draw_text(r.left(), r.top(), r.width(), r.height(),
              align, s, fn, fp);
}

static void xml_text(FILE * fp, const QString & s)
{
    WrapperStr cs = s.toUtf8();
    const char * p = cs;

    for (;;) {
        char c = *p;

        switch (c) {
        case 0:
            return;

        case '<':
            (void) fputs("&lt;", fp);
            break;

        case '>':
            (void) fputs("&gt;", fp);
            break;

        case '"':
            (void) fputs("&quot;", fp);
            break;

        case '&':
            (void) fputs("&amp;", fp);
            break;

        default:
            (void) fputc(c, fp);
        }

        p += 1;
    }
}

void draw_text(int x, int y, int w, int h, int align,
               const QString & sInit, const QFont & fn, FILE * fp,
               UmlColor fg, UmlColor bg)
{
    QString s = sInit;
    
    if (s.isEmpty())
        return;

    if (bg != UmlTransparent)
        (void) fprintf(fp, "\t<rect fill=\"%s\" stroke=\"none\" stroke-opacity=\"1\""
                           " x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
                       svg_color(bg), x, y, w, h);

    if (fg == UmlTransparent)
        fg = UmlBlack;

    int ps = fn.pixelSize();
    int pts = fn.pointSize();
    char header[256];

    sprintf(header, "\t<text font-family=\"%s\" font-size=\"%d\" fill=\"%s\" xml:space=\"preserve\"",
            (const char *) fn.family().toLatin1().constData(),
            // decrease size to help to have enough area
            (ps == -1) ? /* Qt3 */ pts : ps - 1,
            svg_color(fg));

    if (ps == -1)
        /* Qt3 */
        ps = pts;

    if (fn.bold())
        strcat(header, " font-weight=\"bold\"");

    if (fn.italic())
        strcat(header, " font-style=\"italic\"");

    if (fn.underline())
        strcat(header, " text-decoration=\"underline\"");

    bool wb = align & ::Qt::TextWordWrap;

    if ((s.indexOf('\n') == -1) && !wb) {
        // one line
        (void) fputs(header, fp);

        if ((align & ::Qt::AlignHCenter) != 0) {
            (void) fputs(" text-anchor=\"middle\"", fp);
            x += w / 2;
        }

        if ((align & ::Qt::AlignVCenter) != 0)
            y += (h + ps) / 2;
        else
            y += ps;

        (void) fprintf(fp, " x=\"%d\" y=\"%d\">", x, y);
        xml_text(fp, s);
        (void) fputs("</text>\n", fp);
    }
    else {
        // several lines
        // remove all \r, remove last \n and extract lines
        int index = s.length();
        QChar lf('\n');

        while ((index = s.indexOf('\r')) != -1)
            s.remove(index, 1);

        index = s.length();

        while ((index != 0) && s.at(index - 1) == lf)
            index -= 1;

        if (index == 0)
            return;

        s.truncate(index);

        QStringList l = s.split(lf);

        // cut lines too large
        QFontMetrics fm(fn);
        QStringList::Iterator iter = l.begin();
        QChar space(' ');
        QChar tab('\t');

        while (iter != l.end()) {
            s = *iter;

            if (fm.width(s) <= w)
                ++iter;
            else {
                QString left;
                int lastsep = 0;

                index = 0;

                do {
                    QChar c = s.at(index);

                    if ((c == space) || (c == tab))
                        lastsep = index;

                    left += c;
                    index += 1;
                }
                while (fm.width(left) <= w);

                if ((lastsep == 0) || !wb)
                    // must cut word
                    lastsep = index - 1;
                else
                    s.remove(lastsep, 1);

                left.truncate(lastsep);
                *iter = left;
                ++iter;
                s = s.mid(lastsep);

                if (! s.isEmpty())
                    // last char is != a space/tab
                    iter = l.insert(iter, s);
            }
        }

        // write lines
        ps = fm.lineSpacing();

        if ((align & ::Qt::AlignVCenter) != 0) {
            int dy = h - l.count() * ps;

            if (dy > 0)
                y += dy / 2;
        }

        y -= fm.leading();

        if ((align & ::Qt::AlignHCenter) != 0) {
            x += w / 2;

            for (iter = l.begin(); iter != l.end(); ++iter) {
                y += ps;
                (void) fputs(header, fp);
                (void) fputs(" text-anchor=\"middle\"", fp);
                (void) fprintf(fp, " x=\"%d\" y=\"%d\">", x, y);
                xml_text(fp, *iter);
                (void) fputs("</text>\n", fp);
            }
        }
        else {
            for (iter = l.begin(); iter != l.end(); ++iter) {
                y += ps;
                (void) fputs(header, fp);
                (void) fprintf(fp, " x=\"%d\" y=\"%d\">", x, y);
                xml_text(fp, *iter);
                (void) fputs("</text>\n", fp);
            }
        }
    }
}

void draw_rotate_text(int cx, int cy, int angle, const QString & s,
                      const QFont & fn, FILE * fp, UmlColor fg)
{
    if (s.isEmpty())
        return;

    int ps = fn.pixelSize();
    int pts = fn.pointSize();

    (void) fprintf(fp, "\t<text font-family=\"%s\" font-size=\"%d\" fill=\"%s\" xml:space=\"preserve\"",
                   (const char *) fn.family().toLatin1().constData(),
                   // decrease size to help to have enough area
                   (ps == -1) ? /* Qt3 */ pts : ps - 1,
                   svg_color(fg));

    if (ps == -1)
        /* Qt3 */
        ps = pts;

    cx += ps / 2;

    if (fn.bold())
        (void) fputs(" font-weight=\"bold\"", fp);

    if (fn.italic())
        (void) fputs(" font-style=\"italic\"", fp);

    if (fn.underline())
        (void) fputs(" text-decoration=\"underline\"", fp);

    (void) fputs(" text-anchor=\"middle\"", fp);

    (void) fprintf(fp, " x=\"%d\" y=\"%d\" transform=\"rotate(%d %d %d)\">",
                   cx, cy, angle, cx, cy);

    xml_text(fp, s);

    (void) fputs("</text>\n", fp);
}

//

static QString Last_Used_Directory;

QString last_used_directory()
{
    return Last_Used_Directory;
}

void set_last_used_directory(const QString & s)
{
    QFileInfo fi(s);

    Last_Used_Directory = fi.dir().path();
}

unsigned api_format(bool useTrueFormat)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "DoUML", "settings");
    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
    int compat = settings.value("Main/compatibility_save").toInt();
    if((compat != 1) || useTrueFormat)
    {
        int fileFormat = settings.value("Main/fileformat").toInt();
        return fileFormat;
    }
    else
        return 75;
}
