#ifndef _UMLOPERATION_H
#define _UMLOPERATION_H


#include "UmlBaseOperation.h"

#include "Vector.h"

// This class manages 'operations', notes that the class 'UmlClassItem'
// is a mother class of all the class's children.
//
// You can modify it as you want (except the constructor)
class UmlOperation : public UmlBaseOperation
{
public:
    UmlOperation(void * id, const QByteArray & n) : UmlBaseOperation(id, n) {};

    //returns a string indicating the king of the element
    virtual QByteArray sKind();

    virtual void memo_ref();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QByteArray pfix, unsigned int rank, unsigned int level);

    static void ref_index();

    static void generate_index();


private:
    //produce the definition in Uml
    void gen_uml_decl();

    //produce the definition in C++
    void gen_cpp_decl(QByteArray s, bool descr);

    //produce the definition in Java
    void gen_java_decl(QByteArray s, bool descr);

    //produce the definition in Php
    void gen_php_decl(QByteArray s, bool descr);

    //produce the definition in Python
    void gen_python_decl(QByteArray s, bool descr);


public:
    QByteArray compute_name(QByteArray s);


protected:
    static Vector opers;

};

#endif
