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

#ifndef REVSETTINGSDIALOG_H
#define REVSETTINGSDIALOG_H

#include <qdialog.h>

class QCheckBox;
class QComboBox;
class LineEdit;

class RevSettingsDialog : public QDialog
{
    Q_OBJECT

protected:
    LineEdit * cpp_dir;
    LineEdit * cpp_file;
    QCheckBox * cpp_dir_case_sensitive;
    QCheckBox * cpp_file_case_sensitive;
    QComboBox * edcpp_h_extension;
    QComboBox * edcpp_src_extension;

    LineEdit * java_dir;
    LineEdit * java_file;
    QCheckBox * java_dir_case_sensitive;
    QCheckBox * java_file_case_sensitive;
    QComboBox * edjava_extension;

    LineEdit * php_dir;
    LineEdit * php_file;
    QCheckBox * php_dir_case_sensitive;
    QCheckBox * php_file_case_sensitive;
    QComboBox * edphp_extension;

    static QSize previous_size;

public:
    RevSettingsDialog();
    virtual ~RevSettingsDialog();

protected slots:
    virtual void polish();
    void accept();
    void show_regexp_help();
};

#endif
