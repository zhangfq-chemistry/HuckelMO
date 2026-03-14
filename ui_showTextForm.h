/********************************************************************************
** Form generated from reading UI file 'showTextForm.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWTEXTFORM_H
#define UI_SHOWTEXTFORM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ShowTextForm
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QTextEdit *textEdit;
    QPushButton *pushButton_Close;
    QPushButton *pushButton_run;
    QLabel *label_2;
    QSpinBox *spinBox_charge;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit_commands;
    QSpinBox *spinBox_mulplicity;
    QLineEdit *lineEdit_path;

    void setupUi(QDialog *ShowTextForm)
    {
        if (ShowTextForm->objectName().isEmpty())
            ShowTextForm->setObjectName(QString::fromUtf8("ShowTextForm"));
        ShowTextForm->resize(730, 599);
        gridLayout = new QGridLayout(ShowTextForm);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(ShowTextForm);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        textEdit = new QTextEdit(ShowTextForm);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setTabChangesFocus(true);
        textEdit->setAcceptRichText(false);

        gridLayout->addWidget(textEdit, 0, 0, 1, 8);

        pushButton_Close = new QPushButton(ShowTextForm);
        pushButton_Close->setObjectName(QString::fromUtf8("pushButton_Close"));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("edit-copy");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("../../../../bin/iboview/src/UI"), QSize(), QIcon::Normal, QIcon::Off);
        }
        pushButton_Close->setIcon(icon);

        gridLayout->addWidget(pushButton_Close, 2, 5, 1, 1);

        pushButton_run = new QPushButton(ShowTextForm);
        pushButton_run->setObjectName(QString::fromUtf8("pushButton_run"));

        gridLayout->addWidget(pushButton_run, 1, 5, 1, 1);

        label_2 = new QLabel(ShowTextForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        spinBox_charge = new QSpinBox(ShowTextForm);
        spinBox_charge->setObjectName(QString::fromUtf8("spinBox_charge"));

        gridLayout->addWidget(spinBox_charge, 1, 1, 1, 1);

        label_3 = new QLabel(ShowTextForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 3, 1, 1);

        label_4 = new QLabel(ShowTextForm);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 3, 1, 1);

        lineEdit_commands = new QLineEdit(ShowTextForm);
        lineEdit_commands->setObjectName(QString::fromUtf8("lineEdit_commands"));

        gridLayout->addWidget(lineEdit_commands, 2, 4, 1, 1);

        spinBox_mulplicity = new QSpinBox(ShowTextForm);
        spinBox_mulplicity->setObjectName(QString::fromUtf8("spinBox_mulplicity"));

        gridLayout->addWidget(spinBox_mulplicity, 2, 1, 1, 1);

        lineEdit_path = new QLineEdit(ShowTextForm);
        lineEdit_path->setObjectName(QString::fromUtf8("lineEdit_path"));

        gridLayout->addWidget(lineEdit_path, 1, 4, 1, 1);


        retranslateUi(ShowTextForm);

        QMetaObject::connectSlotsByName(ShowTextForm);
    } // setupUi

    void retranslateUi(QDialog *ShowTextForm)
    {
        ShowTextForm->setWindowTitle(QCoreApplication::translate("ShowTextForm", "Show Text", nullptr));
        label->setText(QCoreApplication::translate("ShowTextForm", "Charge of molecule", nullptr));
        pushButton_Close->setText(QCoreApplication::translate("ShowTextForm", "Close", nullptr));
        pushButton_run->setText(QCoreApplication::translate("ShowTextForm", "run", nullptr));
        label_2->setText(QCoreApplication::translate("ShowTextForm", "Multiplicity", nullptr));
        label_3->setText(QCoreApplication::translate("ShowTextForm", "Commands", nullptr));
        label_4->setText(QCoreApplication::translate("ShowTextForm", "Excute Path", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShowTextForm: public Ui_ShowTextForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWTEXTFORM_H
