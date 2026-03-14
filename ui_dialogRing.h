/********************************************************************************
** Form generated from reading UI file 'dialogRing.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGRING_H
#define UI_DIALOGRING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogRing
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout_4;
    QDial *dialRadius;
    QDial *dialSize;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_cancel;
    QPushButton *pushButton_ok;
    QWidget *widget2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpinBox *spinBox_red;
    QSpinBox *spinBox_green;
    QSpinBox *spinBox_blue;
    QSpinBox *spinBox_opacity;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_color;
    QCheckBox *checkBox_type;
    QCheckBox *checkBox_aromatic;

    void setupUi(QDialog *DialogRing)
    {
        if (DialogRing->objectName().isEmpty())
            DialogRing->setObjectName(QString::fromUtf8("DialogRing"));
        DialogRing->resize(273, 315);
        widget = new QWidget(DialogRing);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(11, 11, 251, 151));
        horizontalLayout_4 = new QHBoxLayout(widget);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        dialRadius = new QDial(widget);
        dialRadius->setObjectName(QString::fromUtf8("dialRadius"));
        dialRadius->setMaximum(200);
        dialRadius->setSingleStep(5);
        dialRadius->setPageStep(20);
        dialRadius->setValue(100);

        horizontalLayout_4->addWidget(dialRadius);

        dialSize = new QDial(widget);
        dialSize->setObjectName(QString::fromUtf8("dialSize"));
        dialSize->setMaximum(200);
        dialSize->setSingleStep(5);
        dialSize->setPageStep(20);
        dialSize->setValue(100);

        horizontalLayout_4->addWidget(dialSize);

        widget1 = new QWidget(DialogRing);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(40, 270, 168, 34));
        horizontalLayout = new QHBoxLayout(widget1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_cancel = new QPushButton(widget1);
        pushButton_cancel->setObjectName(QString::fromUtf8("pushButton_cancel"));

        horizontalLayout->addWidget(pushButton_cancel);

        pushButton_ok = new QPushButton(widget1);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));

        horizontalLayout->addWidget(pushButton_ok);

        widget2 = new QWidget(DialogRing);
        widget2->setObjectName(QString::fromUtf8("widget2"));
        widget2->setGeometry(QRect(10, 180, 252, 77));
        verticalLayout = new QVBoxLayout(widget2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        spinBox_red = new QSpinBox(widget2);
        spinBox_red->setObjectName(QString::fromUtf8("spinBox_red"));
        QFont font;
        font.setPointSize(11);
        spinBox_red->setFont(font);
        spinBox_red->setWrapping(false);
        spinBox_red->setFrame(true);
        spinBox_red->setMaximum(255);
        spinBox_red->setSingleStep(3);

        horizontalLayout_2->addWidget(spinBox_red);

        spinBox_green = new QSpinBox(widget2);
        spinBox_green->setObjectName(QString::fromUtf8("spinBox_green"));
        spinBox_green->setMaximum(255);
        spinBox_green->setSingleStep(3);

        horizontalLayout_2->addWidget(spinBox_green);

        spinBox_blue = new QSpinBox(widget2);
        spinBox_blue->setObjectName(QString::fromUtf8("spinBox_blue"));
        spinBox_blue->setMaximum(255);
        spinBox_blue->setSingleStep(3);

        horizontalLayout_2->addWidget(spinBox_blue);

        spinBox_opacity = new QSpinBox(widget2);
        spinBox_opacity->setObjectName(QString::fromUtf8("spinBox_opacity"));
        spinBox_opacity->setMaximum(255);
        spinBox_opacity->setSingleStep(3);

        horizontalLayout_2->addWidget(spinBox_opacity);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushButton_color = new QPushButton(widget2);
        pushButton_color->setObjectName(QString::fromUtf8("pushButton_color"));

        horizontalLayout_3->addWidget(pushButton_color);

        checkBox_type = new QCheckBox(widget2);
        checkBox_type->setObjectName(QString::fromUtf8("checkBox_type"));

        horizontalLayout_3->addWidget(checkBox_type);

        checkBox_aromatic = new QCheckBox(widget2);
        checkBox_aromatic->setObjectName(QString::fromUtf8("checkBox_aromatic"));

        horizontalLayout_3->addWidget(checkBox_aromatic);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(DialogRing);

        QMetaObject::connectSlotsByName(DialogRing);
    } // setupUi

    void retranslateUi(QDialog *DialogRing)
    {
        DialogRing->setWindowTitle(QCoreApplication::translate("DialogRing", "Color and Radius of Ring", nullptr));
        dialRadius->setProperty("view_option_name", QVariant(QCoreApplication::translate("DialogRing", "atom_scale", nullptr)));
        dialSize->setProperty("view_option_name", QVariant(QCoreApplication::translate("DialogRing", "atom_scale", nullptr)));
        pushButton_cancel->setText(QCoreApplication::translate("DialogRing", "Cancel", nullptr));
        pushButton_ok->setText(QCoreApplication::translate("DialogRing", "OK", nullptr));
        pushButton_color->setText(QCoreApplication::translate("DialogRing", "Color", nullptr));
        checkBox_type->setText(QCoreApplication::translate("DialogRing", "Type", nullptr));
        checkBox_aromatic->setText(QCoreApplication::translate("DialogRing", "Aromatic", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogRing: public Ui_DialogRing {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGRING_H
