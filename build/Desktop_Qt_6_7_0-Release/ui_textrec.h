/********************************************************************************
** Form generated from reading UI file 'textrec.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTREC_H
#define UI_TEXTREC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_textrec
{
public:

    void setupUi(QWidget *textrec)
    {
        if (textrec->objectName().isEmpty())
            textrec->setObjectName("textrec");
        textrec->resize(800, 600);

        retranslateUi(textrec);

        QMetaObject::connectSlotsByName(textrec);
    } // setupUi

    void retranslateUi(QWidget *textrec)
    {
        textrec->setWindowTitle(QCoreApplication::translate("textrec", "textrec", nullptr));
    } // retranslateUi

};

namespace Ui {
    class textrec: public Ui_textrec {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTREC_H
