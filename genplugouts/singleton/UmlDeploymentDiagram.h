#ifndef _UMLDEPLOYMENTDIAGRAM_H
#define _UMLDEPLOYMENTDIAGRAM_H


#include "UmlBaseDeploymentDiagram.h"
#include <QByteArray>

// This class manages 'deployment diagrams', notes that the class 'UmlDiagram'
// is a mother class of all the diagrams, allowing to generalize their
// management
//
// You can modify it as you want (except the constructor)
class UmlDeploymentDiagram : public UmlBaseDeploymentDiagram
{
public:
    UmlDeploymentDiagram(void * id, const QByteArray & n) : UmlBaseDeploymentDiagram(id, n) {};

};

#endif
