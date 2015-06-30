TEMPLATE	= app
TARGET		= gpro
CONFIG        += debug warn_on qt
DEFINES        = WITHCPP WITHJAVA WITHPHP WITHPYTHON WITHIDL TRUE=true FALSE=false
HEADERS		= ./PackageGlobalCmd.h \
		  ./UmlBaseItem.h \
		  ./UmlBaseArtifact.h \
		  ./IdlSettingsCmd.h \
		  ./UmlBaseClassMember.h \
		  ./UmlBasePackage.h \
		  ./UmlSequenceDiagram.h \
		  ./UmlDiagram.h \
		  ./CmdFamily.h \
		  ./UmlNode.h \
		  ./UmlTypeSpec.h \
		  ./UmlParameter.h \
		  ./IdlSettings.h \
		  ./UmlBaseOperation.h \
		  ./UmlNcRelation.h \
		  ./aDirection.h \
		  ./JavaSettings.h \
		  ./UmlClassDiagram.h \
		  ./UmlFormalParameter.h \
		  ./UmlComponentView.h \
		  ./UmlUseCaseView.h \
		  ./UmlBaseActualParameter.h \
		  ./Dialog.h \
		  ./UmlBaseDiagram.h \
		  ./UmlBaseRelation.h \
		  ./UmlSettingsCmd.h \
		  ./UmlBaseUseCaseDiagram.h \
		  ./UmlBuiltin.h \
		  ./UmlBaseClass.h \
		  ./CppSettingsCmd.h \
		  ./UmlClassView.h \
		  ./UmlBaseNode.h \
		  ./UmlBaseComponentDiagram.h \
		  ./aVisibility.h \
		  ./UmlBaseAttribute.h \
		  ./UmlCom.h \
		  ./OnInstanceCmd.h \
		  ./UmlDeploymentView.h \
		  ./UmlCollaborationDiagram.h \
		  ./CppSettings.h \
		  ./UmlUseCaseDiagram.h \
		  ./UmlClassItem.h \
		  ./UmlExtraClassMember.h \
		  ./aRelationKind.h \
		  ./UmlBaseDeploymentDiagram.h \
		  ./UmlStereotype.h \
		  ./UmlBaseDeploymentView.h \
		  ./UmlItem.h \
		  ./UmlOperation.h \
		  ./JavaSettingsCmd.h \
		  ./anItemKind.h \
		  ./UmlBaseClassDiagram.h \
		  ./ClassGlobalCmd.h \
		  ./UmlPackage.h \
		  ./UmlActualParameter.h \
		  ./UmlBaseUseCaseView.h \
		  ./MiscGlobalCmd.h \
		  ./UmlComponentDiagram.h \
		  ./UmlArtifact.h \
		  ./UmlAttribute.h \
		  ./UmlClass.h \
		  ./SmallPushButton.h \
		  ./UmlBaseNcRelation.h \
		  ./UmlComponent.h \
		  ./UmlBaseUseCase.h \
		  ./UmlBaseComponentView.h \
		  ./UmlBaseClassItem.h \
		  ./UmlBaseComponent.h \
		  ./UmlSettings.h \
		  ./UmlBaseFormalParameter.h \
		  ./UmlBaseExtraClassMember.h \
		  ./UmlUseCase.h \
		  ./UmlClassMember.h \
		  ./UmlBaseCollaborationDiagram.h \
		  ./UmlBaseSequenceDiagram.h \
		  ./UmlDeploymentDiagram.h \
		  ./UmlBaseClassView.h \
		  ./UmlBaseDeployment.h \
          ./UmlRelation.h \
    gridbox.h \
    hhbox.h
SOURCES		= ./PackageGlobalCmd.cpp \
		  ./UmlBaseItem.cpp \
		  ./UmlBaseArtifact.cpp \
		  ./IdlSettingsCmd.cpp \
		  ./UmlBaseClassMember.cpp \
		  ./UmlBasePackage.cpp \
		  ./UmlSequenceDiagram.cpp \
		  ./UmlDiagram.cpp \
		  ./CmdFamily.cpp \
		  ./UmlNode.cpp \
		  ./UmlTypeSpec.cpp \
		  ./UmlParameter.cpp \
		  ./Main.cpp \
		  ./IdlSettings.cpp \
		  ./UmlBaseOperation.cpp \
		  ./UmlNcRelation.cpp \
		  ./aDirection.cpp \
		  ./JavaSettings.cpp \
		  ./UmlClassDiagram.cpp \
		  ./UmlFormalParameter.cpp \
		  ./UmlComponentView.cpp \
		  ./UmlUseCaseView.cpp \
		  ./UmlBaseActualParameter.cpp \
		  ./Dialog.cpp \
		  ./UmlBaseDiagram.cpp \
		  ./UmlBaseRelation.cpp \
		  ./UmlSettingsCmd.cpp \
		  ./UmlBaseUseCaseDiagram.cpp \
		  ./UmlBuiltin.cpp \
		  ./UmlBaseClass.cpp \
		  ./CppSettingsCmd.cpp \
		  ./UmlClassView.cpp \
		  ./UmlBaseNode.cpp \
		  ./UmlBaseComponentDiagram.cpp \
		  ./aVisibility.cpp \
		  ./UmlBaseAttribute.cpp \
		  ./UmlCom.cpp \
		  ./OnInstanceCmd.cpp \
		  ./UmlDeploymentView.cpp \
		  ./UmlCollaborationDiagram.cpp \
		  ./CppSettings.cpp \
		  ./UmlUseCaseDiagram.cpp \
		  ./UmlClassItem.cpp \
		  ./UmlExtraClassMember.cpp \
		  ./aRelationKind.cpp \
		  ./UmlBaseDeploymentDiagram.cpp \
		  ./UmlStereotype.cpp \
		  ./UmlBaseDeploymentView.cpp \
		  ./UmlItem.cpp \
		  ./UmlOperation.cpp \
		  ./JavaSettingsCmd.cpp \
		  ./anItemKind.cpp \
		  ./UmlBaseClassDiagram.cpp \
		  ./ClassGlobalCmd.cpp \
		  ./UmlPackage.cpp \
		  ./UmlActualParameter.cpp \
		  ./UmlBaseUseCaseView.cpp \
		  ./MiscGlobalCmd.cpp \
		  ./UmlComponentDiagram.cpp \
		  ./UmlArtifact.cpp \
		  ./UmlAttribute.cpp \
		  ./UmlClass.cpp \
		  ./SmallPushButton.cpp \
		  ./UmlBaseNcRelation.cpp \
		  ./UmlComponent.cpp \
		  ./UmlBaseUseCase.cpp \
		  ./UmlBaseComponentView.cpp \
		  ./UmlBaseClassItem.cpp \
		  ./UmlBaseComponent.cpp \
		  ./UmlSettings.cpp \
		  ./UmlBaseFormalParameter.cpp \
		  ./UmlBaseExtraClassMember.cpp \
		  ./UmlUseCase.cpp \
		  ./UmlClassMember.cpp \
		  ./UmlBaseCollaborationDiagram.cpp \
		  ./UmlBaseSequenceDiagram.cpp \
		  ./UmlDeploymentDiagram.cpp \
		  ./UmlBaseClassView.cpp \
		  ./UmlBaseDeployment.cpp \
          ./UmlRelation.cpp \
    gridbox.cpp \
    hhbox.cpp

#The following line was inserted by qt3to4
QT += network  widgets

INCLUDEPATH += ../../src
QT += testlib
Release{


    MOC_DIR = bin/douml/gpro/MOC_release
    OBJECTS_DIR = bin/douml/gpro/Obj_release
}

Debug{
    MOC_DIR = bin/douml/gpro/MOC_Debug
    OBJECTS_DIR = bin/douml/gpro/Obj_Debug

}
    UI_DIR = src/ui
    DESTDIR = ../../bin

QMAKE_CXXFLAGS += -std=gnu++11
mac:QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -stdlib=libc++
