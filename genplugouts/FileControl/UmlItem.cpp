
#include "UmlItem.h"

#include <qfileinfo.h>
#include "UmlCom.h"
UmlItem::~UmlItem()
{
}

void UmlItem::fileControl(bool)
{
    UmlCom::trace("Error : must be applied on a <i>Package</i>");
}

void UmlItem::getFiles(QHash<QString, void *> &files, unsigned rec)
{
    QFileInfo fi(supportFile());

    if (fi.exists())
        files.insert(fi.fileName(), (void *) 1);

    const QVector<UmlItem*> ch = children();
    int index;

    for (index = 0; index != ch.size(); index += 1)
        ch[index]->getFiles(files, rec);
}

