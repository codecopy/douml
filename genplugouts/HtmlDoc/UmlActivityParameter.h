#ifndef _UMLACTIVITYPARAMETER_H
#define _UMLACTIVITYPARAMETER_H


#include "UmlBaseActivityParameter.h"


class UmlActivityParameter : public UmlBaseActivityParameter
{
public:
    //  the constructor, do not call it yourself !!!!!!!!!!
    UmlActivityParameter(void * id, const QByteArray & s) : UmlBaseActivityParameter(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QByteArray sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QByteArray pfix, unsigned int rank, unsigned int level);

};

#endif
