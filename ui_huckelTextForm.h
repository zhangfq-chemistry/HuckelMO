/********************************************************************************
** Form generated from reading UI file 'huckelTextForm.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HUCKELTEXTFORM_H
#define UI_HUCKELTEXTFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
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
            ShowTextForm->setObjectName(QStringLiteral("ShowTextForm"));
        ShowTextForm->resize(730, 599);
        gridLayout = new QGridLayout(ShowTextForm);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(ShowTextForm);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        textEdit = new QTextEdit(ShowTextForm);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setTabChangesFocus(true);
        textEdit->setAcceptRichText(false);

        gridLayout->addWidget(textEdit, 0, 0, 1, 8);

        pushButton_Close = new QPushButton(ShowTextForm);
        pushButton_Close->setObjectName(QStringLiteral("pushButton_Close"));
        QIcon icon;
        QString iconThemeName = QStringLiteral("edit-copy");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QStringLiteral("../../../../bin/iboview/src/UI"), QSize(), QIcon::Normal, QIcon::Off);
        }
        pushButton_Close->setIcon(icon);

        gridLayout->addWidget(pushButton_Close, 2, 5, 1, 1);

        pushButton_run = new QPushButton(ShowTextForm);
        pushButton_run->setObjectName(QStringLiteral("pushButton_run"));

        gridLayout->addWidget(pushButton_run, 1, 5, 1, 1);

        label_2 = new QLabel(ShowTextForm);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        spinBox_charge = new QSpinBox(ShowTextForm);
        spinBox_charge->setObjectName(QStringLiteral("spinBox_charge"));

        gridLayout->addWidget(spinBox_charge, 1, 1, 1, 1);

        label_3 = new QLabel(ShowTextForm);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 3, 1, 1);

        label_4 = new QLabel(ShowTextForm);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 3, 1, 1);

        lineEdit_commands = new QLineEdit(ShowTextForm);
        lineEdit_commands->setObjectName(QStringLiteral("lineEdit_commands"));

        gridLayout->addWidget(lineEdit_commands, 2, 4, 1, 1);

        spinBox_mulplicity = new QSpinBox(ShowTextForm);
        spinBox_mulplicity->setObjectName(QStringLiteral("spinBox_mulplicity"));

        gridLayout->addWidget(spinBox_mulplicity, 2, 1, 1, 1);

        lineEdit_path = new QLineEdit(ShowTextForm);
        lineEdit_path->setObjectName(QStringLiteral("lineEdit_path"));

        gridLayout->addWidget(lineEdit_path, 1, 4, 1, 1);


        retranslateUi(ShowTextForm);

        QMetaObject::connectSlotsByName(ShowTextForm);
    } // setupUi

    void retranslateUi(QDialog *ShowTextForm)
    {
        ShowTextForm->setWindowTitle(QApplication::translate("ShowTextForm", "Show Text", Q_NULLPTR));
        label->setText(QApplication::translate("ShowTextForm", "Charge of molecule", Q_NULLPTR));
        pushButton_Close->setText(QApplication::translate("ShowTextForm", "Close", Q_NULLPTR));
        pushButton_run->setText(QApplication::translate("ShowTextForm", "run", Q_NULLPTR));
        label_2->setText(QApplication::translate("ShowTextForm", "Multiplicity", Q_NULLPTR));
        label_3->setText(QApplication::translate("ShowTextForm", "Commands", Q_NULLPTR));
        label_4->setText(QApplication::translate("ShowTextForm", "Excute Path", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ShowTextForm: public Ui_ShowTextForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HUCKELTEXTFORM_H
