/********************************************************************************
** Form generated from reading UI file 'dialogAtom.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGATOM_H
#define UI_DIALOGATOM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDial>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogAtom
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_cancel;
    QPushButton *pushButton_ok;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QSpinBox *spinBox_green;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QSpinBox *spinBox_blue;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpinBox *spinBox_red;
    QPushButton *pushButton_color;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBoxRadius;
    QDial *dialAtomRadius;

    void setupUi(QDialog *DialogAtom)
    {
        if (DialogAtom->objectName().isEmpty())
            DialogAtom->setObjectName(QStringLiteral("DialogAtom"));
        DialogAtom->resize(272, 244);
        layoutWidget = new QWidget(DialogAtom);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(50, 200, 181, 34));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_cancel = new QPushButton(layoutWidget);
        pushButton_cancel->setObjectName(QStringLiteral("pushButton_cancel"));

        horizontalLayout->addWidget(pushButton_cancel);

        pushButton_ok = new QPushButton(layoutWidget);
        pushButton_ok->setObjectName(QStringLiteral("pushButton_ok"));

        horizontalLayout->addWidget(pushButton_ok);

        layoutWidget1 = new QWidget(DialogAtom);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(30, 15, 84, 161));
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_4->addWidget(label_2);

        spinBox_green = new QSpinBox(layoutWidget1);
        spinBox_green->setObjectName(QStringLiteral("spinBox_green"));
        spinBox_green->setMaximum(255);
        spinBox_green->setSingleStep(3);

        horizontalLayout_4->addWidget(spinBox_green);


        gridLayout->addLayout(horizontalLayout_4, 1, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_5->addWidget(label_3);

        spinBox_blue = new QSpinBox(layoutWidget1);
        spinBox_blue->setObjectName(QStringLiteral("spinBox_blue"));
        spinBox_blue->setMaximum(255);
        spinBox_blue->setSingleStep(3);

        horizontalLayout_5->addWidget(spinBox_blue);


        gridLayout->addLayout(horizontalLayout_5, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        spinBox_red = new QSpinBox(layoutWidget1);
        spinBox_red->setObjectName(QStringLiteral("spinBox_red"));
        QFont font;
        font.setPointSize(11);
        spinBox_red->setFont(font);
        spinBox_red->setMaximum(255);
        spinBox_red->setSingleStep(3);

        horizontalLayout_2->addWidget(spinBox_red);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        pushButton_color = new QPushButton(layoutWidget1);
        pushButton_color->setObjectName(QStringLiteral("pushButton_color"));

        gridLayout->addWidget(pushButton_color, 3, 0, 1, 1);

        layoutWidget2 = new QWidget(DialogAtom);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(130, 140, 112, 35));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(layoutWidget2);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_3->addWidget(label_4);

        doubleSpinBoxRadius = new QDoubleSpinBox(layoutWidget2);
        doubleSpinBoxRadius->setObjectName(QStringLiteral("doubleSpinBoxRadius"));
        doubleSpinBoxRadius->setMaximum(5);
        doubleSpinBoxRadius->setSingleStep(0.1);

        horizontalLayout_3->addWidget(doubleSpinBoxRadius);

        dialAtomRadius = new QDial(DialogAtom);
        dialAtomRadius->setObjectName(QStringLiteral("dialAtomRadius"));
        dialAtomRadius->setGeometry(QRect(100, 0, 171, 151));
        dialAtomRadius->setMaximum(200);
        dialAtomRadius->setSingleStep(5);
        dialAtomRadius->setPageStep(20);
        dialAtomRadius->setValue(100);

        retranslateUi(DialogAtom);

        QMetaObject::connectSlotsByName(DialogAtom);
    } // setupUi

    void retranslateUi(QDialog *DialogAtom)
    {
        DialogAtom->setWindowTitle(QApplication::translate("DialogAtom", "Color and Radius of Atom", Q_NULLPTR));
        pushButton_cancel->setText(QApplication::translate("DialogAtom", "Cancel", Q_NULLPTR));
        pushButton_ok->setText(QApplication::translate("DialogAtom", "OK", Q_NULLPTR));
        label_2->setText(QApplication::translate("DialogAtom", "\347\273\277", Q_NULLPTR));
        label_3->setText(QApplication::translate("DialogAtom", "\350\223\235", Q_NULLPTR));
        label->setText(QApplication::translate("DialogAtom", "\347\272\242", Q_NULLPTR));
        pushButton_color->setText(QApplication::translate("DialogAtom", "Color", Q_NULLPTR));
        label_4->setText(QApplication::translate("DialogAtom", "\345\215\212\345\276\204", Q_NULLPTR));
        dialAtomRadius->setProperty("view_option_name", QVariant(QApplication::translate("DialogAtom", "atom_scale", Q_NULLPTR)));
    } // retranslateUi

};

namespace Ui {
    class DialogAtom: public Ui_DialogAtom {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGATOM_H
