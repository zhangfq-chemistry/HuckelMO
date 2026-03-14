/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *textLabel2_4;

    void setupUi(QWidget *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QStringLiteral("AboutDialog"));
        AboutDialog->resize(612, 488);
        gridLayout = new QGridLayout(AboutDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        textLabel2_4 = new QLabel(AboutDialog);
        textLabel2_4->setObjectName(QStringLiteral("textLabel2_4"));
        QFont font;
        textLabel2_4->setFont(font);
        textLabel2_4->setWordWrap(false);

        gridLayout->addWidget(textLabel2_4, 3, 0, 1, 1);


        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QWidget *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "About", Q_NULLPTR));
        textLabel2_4->setText(QApplication::translate("AboutDialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt; font-weight:600;\">HuckelOrbital-1.0</span></p><p align=\"center\"><br/><span style=\" font-size:12pt;\">Authors: </span></p><p align=\"center\"><span style=\" font-size:14pt; font-weight:600;\">GitHub: </span><a href=\"https://github.com/parisolab/mathmod\"><span style=\" font-size:14pt; font-weight:600; text-decoration: underline; color:#0000ff;\">https://github.com/zhangfq/huckelorbital</span></a></p><p align=\"center\"><span style=\" font-size:14pt; font-weight:600;\">Home:</span><a href=\"https://sourceforge.net/projects/mathmod\"><span style=\" font-size:14pt; font-weight:600; text-decoration: underline; color:#0000ff;\">https://www.sxnu.edu.cn/projects/</span></a><a href=\"https://github.com/parisolab/mathmod\"><span style=\" font-size:14pt; font-weight:600; text-decoration: underline; color:#0000ff;\">huckelorbital</span></a></p><p align=\"center\"><br/></p><p align=\"center\"><br/></p></body></html>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
