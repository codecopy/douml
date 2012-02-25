#ifndef _UMLBASEPACKAGE_H
#define _UMLBASEPACKAGE_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlPackage;
class UmlDiagram;

//  Manage the packages
class UmlBasePackage : public UmlItem {
  public:
    // returns a new package named 'name' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlPackage * create(UmlPackage * parent, const char * name);

    // returns the kind of the item
    virtual anItemKind kind();

    // returns the optional associated diagram
    UmlDiagram * associatedDiagram();

    // sets the associated diagram, arg may be null to unset it
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_AssociatedDiagram(UmlDiagram * d);

#ifdef WITHCPP
    // returns the path where the C++ source files are generated by the
    // c++ generator.
    const QCString & cppSrcDir();

    // to set the path where the C++ source files are generated by the
    // C++ generator, may be relative even the 'root' path is not
    // (already) an absolute path
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppSrcDir(const QCString & s);

    // returns the path where the C++ header files are generated by the
    // C++ generator.
    const QCString & cppHDir();

    // to set the path where the C++ header files are generated by the
    // C++ generator, may be relative even the 'root' path is not
    // (already) an absolute path
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppHDir(const QCString & s);

    //returns the namespace name associed to the package
    QCString cppNamespace();

    //to set the namespace name associed to the package.
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppNamespace(const QCString & s);

    // returns a sub package of the current one having the c++ namespace 'n'
    // (including the current one), else 0/null
    UmlPackage * findCppNamespace(const QCString & n) const;
#endif

#ifdef WITHJAVA
    // returns the path where the JAVA files are generated by the
    // JAVA generator.
    const QCString & javaDir();

    // to set the path where the JAVA files are generated by the
    // JAVA generator, may be relative even the 'root' path is not
    // (already) an absolute path
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaDir(const QCString & s);

    //returns the java package name associed to the package
    QCString javaPackage();

    // to set the java package name associed to the package.
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaPackage(const QCString & s);

    // returns a sub package of the current one having the Java package 'n'
    // (including the current one), else 0/null
    UmlPackage * findJavaPackage(const QCString & n) const;
#endif

#ifdef WITHPHP
    // returns the path where the PHP files are generated by the
    // PHP generator.
    const QCString & phpDir();

    // to set the path where the PHP files are generated by the
    // PHP generator, may be relative even the 'root' path is not
    // (already) an absolute path
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_PhpDir(const QCString & s);
#endif

#ifdef WITHPYTHON
    // returns the path where the PYTHON files are generated by the
    // PYTHON generator.
    const QCString & pythonDir();

    // to set the path where the PYTHON files are generated by the
    // PYTHON generator, may be relative even the 'root' path is not
    // (already) an absolute path
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_PythonDir(const QCString & s);

    //returns the python package name associed to the package
    QCString pythonPackage();

    // to set the python package name associed to the package.
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_PythonPackage(const QCString & s);

    // returns a sub package of the current one having the Python package 'n'
    // (including the current one), else 0/null
    UmlPackage * findPythonPackage(const QCString & n) const;
#endif

#ifdef WITHIDL
    // returns the path where the IDL files are generated by the
    // IDL generator.
    const QCString & idlDir();

    // to set the path where the IDL files are generated by the
    // IDL generator, may be relative even the 'root' path is not
    // (already) an absolute path
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IdlDir(const QCString & s);

    //returns the module name associed to the package
    QCString idlModule();

    // to set the module name associed to the package.
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IdlModule(const QCString & s);

    // returns a sub package of the current one having the IDL module 'n'
    // (including the current one), else 0/null
    UmlPackage * findIdlModule(const QCString & n) const;
#endif

    //  Returns the project
    static UmlPackage * getProject();

    //return TRUE in case something is modified (i.e. the project must be saved)
    
    static bool isProjectModified();

    //save the project if needed
    static void saveProject();

    //Does nothing in case an edition is on going under BOUML. Else :
    //close the current project (in case it is not saved the last modifications are lost),
    //load the specified one, and all the communications with the plug-outs including the
    //current one are closed.
    
    static void loadProject(QCString p);

    // Force consistency concerning the profiles and their stereotypes
    
    static void updateProfiles();

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE. 
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool rec = FALSE, bool del = FALSE);


  private:
    UmlDiagram * _assoc_diagram;

#ifdef WITHCPP
    QCString _cpp_src_dir;

    QCString _cpp_h_dir;

    QCString _cpp_namespace;
#endif
    

#ifdef WITHJAVA
    QCString _java_dir;

    QCString _java_package;
#endif
    

#ifdef WITHPHP
    QCString _php_dir;
#endif

#ifdef WITHPYTHON
    QCString _python_dir;

    QCString _python_package;
#endif

#ifdef WITHIDL
    QCString _idl_dir;

    QCString _idl_module;
#endif
    


  protected:
    //internal, do NOT use it
    
    virtual void read_uml_();

#ifdef WITHCPP
    //internal, do NOT use it
    
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    //internal, do NOT use it
    
    virtual void read_java_();
#endif

#ifdef WITHPHP
    //internal, do NOT use it
    
    virtual void read_php_();
#endif

#ifdef WITHPYTHON
    //internal, do NOT use it
    
    virtual void read_python_();
#endif

#ifdef WITHIDL
    //internal, do NOT use it
    
    virtual void read_idl_();
#endif

    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBasePackage(void * id, const QCString & n);

};

inline UmlBasePackage::UmlBasePackage(void * id, const QCString & n) : UmlItem(id, n) {
  _assoc_diagram = 0;
}

#endif
