#ifndef _UMLUSECASEVIEW_H
#define _UMLUSECASEVIEW_H


#include "UmlBaseUseCaseView.h"


// This class manages 'use case view'
//
// You can modify it as you want (except the constructor)
class UmlUseCaseView : public UmlBaseUseCaseView
{
public:
    UmlUseCaseView(void * id, const QByteArray & n) : UmlBaseUseCaseView(id, n) {};

};

#endif
