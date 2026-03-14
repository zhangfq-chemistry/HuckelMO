/********************************************************************************
** Form generated from reading UI file 'bond.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOND_H
#define UI_BOND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog_bond
{
public:
    QLineEdit *lineEdit;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_ok;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_cancel;
    QWidget *widget;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label1;
    QComboBox *comboBox_type1;
    QSpacerItem *horizontalSpacer_6;
    QPlainTextEdit *textEdit_idList1;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label2;
    QComboBox *comboBox_type2;
    QSpacerItem *horizontalSpacer_5;
    QPlainTextEdit *textEdit_idList2;
    QGridLayout *gridLayout;
    QRadioButton *radioButton0;
    QRadioButton *radioButton1;
    QRadioButton *radioButton6;
    QRadioButton *radioButton2;
    QRadioButton *radioButton3;
    QRadioButton *radioButton4;
    QRadioButton *radioButton5;
    QRadioButton *radioButton7;
    QVBoxLayout *verticalLayout_3;
    QSlider *horizontalSlider;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label3;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label4;

    void setupUi(QDialog *Dialog_bond)
    {
        if (Dialog_bond->objectName().isEmpty())
            Dialog_bond->setObjectName(QStringLiteral("Dialog_bond"));
        Dialog_bond->resize(605, 522);
        lineEdit = new QLineEdit(Dialog_bond);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(230, 420, 111, 31));
        QFont font;
        font.setFamily(QStringLiteral("Sans Serif"));
        font.setPointSize(12);
        lineEdit->setFont(font);
        layoutWidget = new QWidget(Dialog_bond);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(110, 460, 341, 34));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_ok = new QPushButton(layoutWidget);
        pushButton_ok->setObjectName(QStringLiteral("pushButton_ok"));
        QFont font1;
        font1.setFamily(QStringLiteral("Sans Serif"));
        font1.setPointSize(10);
        pushButton_ok->setFont(font1);

        horizontalLayout->addWidget(pushButton_ok);

        horizontalSpacer = new QSpacerItem(88, 18, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_cancel = new QPushButton(layoutWidget);
        pushButton_cancel->setObjectName(QStringLiteral("pushButton_cancel"));
        pushButton_cancel->setFont(font1);

        horizontalLayout->addWidget(pushButton_cancel);

        widget = new QWidget(Dialog_bond);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(20, 20, 561, 391));
        verticalLayout_4 = new QVBoxLayout(widget);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(32, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        label1 = new QLabel(widget);
        label1->setObjectName(QStringLiteral("label1"));
        label1->setFont(font1);

        horizontalLayout_4->addWidget(label1);

        comboBox_type1 = new QComboBox(widget);
        comboBox_type1->setObjectName(QStringLiteral("comboBox_type1"));
        QFont font2;
        font2.setPointSize(10);
        comboBox_type1->setFont(font2);

        horizontalLayout_4->addWidget(comboBox_type1);

        horizontalSpacer_6 = new QSpacerItem(32, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);


        verticalLayout_2->addLayout(horizontalLayout_4);

        textEdit_idList1 = new QPlainTextEdit(widget);
        textEdit_idList1->setObjectName(QStringLiteral("textEdit_idList1"));
        textEdit_idList1->setStyleSheet(QStringLiteral(""));

        verticalLayout_2->addWidget(textEdit_idList1);


        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        label2 = new QLabel(widget);
        label2->setObjectName(QStringLiteral("label2"));
        label2->setFont(font1);

        horizontalLayout_5->addWidget(label2);

        comboBox_type2 = new QComboBox(widget);
        comboBox_type2->setObjectName(QStringLiteral("comboBox_type2"));
        comboBox_type2->setFont(font2);

        horizontalLayout_5->addWidget(comboBox_type2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_5);

        textEdit_idList2 = new QPlainTextEdit(widget);
        textEdit_idList2->setObjectName(QStringLiteral("textEdit_idList2"));

        verticalLayout->addWidget(textEdit_idList2);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_4->addLayout(horizontalLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        radioButton0 = new QRadioButton(widget);
        radioButton0->setObjectName(QStringLiteral("radioButton0"));
        radioButton0->setFont(font1);

        gridLayout->addWidget(radioButton0, 3, 1, 1, 1);

        radioButton1 = new QRadioButton(widget);
        radioButton1->setObjectName(QStringLiteral("radioButton1"));
        radioButton1->setFont(font1);

        gridLayout->addWidget(radioButton1, 0, 0, 1, 1);

        radioButton6 = new QRadioButton(widget);
        radioButton6->setObjectName(QStringLiteral("radioButton6"));
        radioButton6->setFont(font1);

        gridLayout->addWidget(radioButton6, 3, 0, 1, 1);

        radioButton2 = new QRadioButton(widget);
        radioButton2->setObjectName(QStringLiteral("radioButton2"));
        radioButton2->setFont(font1);

        gridLayout->addWidget(radioButton2, 0, 1, 1, 1);

        radioButton3 = new QRadioButton(widget);
        radioButton3->setObjectName(QStringLiteral("radioButton3"));
        radioButton3->setFont(font1);

        gridLayout->addWidget(radioButton3, 0, 2, 1, 1);

        radioButton4 = new QRadioButton(widget);
        radioButton4->setObjectName(QStringLiteral("radioButton4"));

        gridLayout->addWidget(radioButton4, 1, 0, 1, 1);

        radioButton5 = new QRadioButton(widget);
        radioButton5->setObjectName(QStringLiteral("radioButton5"));
        radioButton5->setFont(font1);

        gridLayout->addWidget(radioButton5, 1, 1, 1, 1);

        radioButton7 = new QRadioButton(widget);
        radioButton7->setObjectName(QStringLiteral("radioButton7"));
        radioButton7->setFont(font1);

        gridLayout->addWidget(radioButton7, 1, 2, 1, 1);


        verticalLayout_4->addLayout(gridLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalSlider = new QSlider(widget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setFont(font);
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout_3->addWidget(horizontalSlider);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label3 = new QLabel(widget);
        label3->setObjectName(QStringLiteral("label3"));
        label3->setFont(font1);

        horizontalLayout_3->addWidget(label3);

        horizontalSpacer_2 = new QSpacerItem(88, 18, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        label4 = new QLabel(widget);
        label4->setObjectName(QStringLiteral("label4"));
        label4->setFont(font1);

        horizontalLayout_3->addWidget(label4);


        verticalLayout_3->addLayout(horizontalLayout_3);


        verticalLayout_4->addLayout(verticalLayout_3);


        retranslateUi(Dialog_bond);

        QMetaObject::connectSlotsByName(Dialog_bond);
    } // setupUi

    void retranslateUi(QDialog *Dialog_bond)
    {
        Dialog_bond->setWindowTitle(QApplication::translate("Dialog_bond", "Dialog", Q_NULLPTR));
        pushButton_ok->setText(QApplication::translate("Dialog_bond", "OK", Q_NULLPTR));
        pushButton_cancel->setText(QApplication::translate("Dialog_bond", "Cancel", Q_NULLPTR));
        label1->setText(QApplication::translate("Dialog_bond", "Atom1", Q_NULLPTR));
        comboBox_type1->clear();
        comboBox_type1->insertItems(0, QStringList()
         << QApplication::translate("Dialog_bond", "Fixed", Q_NULLPTR)
         << QApplication::translate("Dialog_bond", "Move atom", Q_NULLPTR)
         << QApplication::translate("Dialog_bond", "Move group", Q_NULLPTR)
        );
        label2->setText(QApplication::translate("Dialog_bond", "Atom2", Q_NULLPTR));
        comboBox_type2->clear();
        comboBox_type2->insertItems(0, QStringList()
         << QApplication::translate("Dialog_bond", "Fixed", Q_NULLPTR)
         << QApplication::translate("Dialog_bond", "Move atom", Q_NULLPTR)
         << QApplication::translate("Dialog_bond", "Move group", Q_NULLPTR)
        );
        radioButton0->setText(QApplication::translate("Dialog_bond", "None", Q_NULLPTR));
        radioButton1->setText(QApplication::translate("Dialog_bond", "Single", Q_NULLPTR));
        radioButton6->setText(QApplication::translate("Dialog_bond", "Hydrogen Bond", Q_NULLPTR));
        radioButton2->setText(QApplication::translate("Dialog_bond", "Double", Q_NULLPTR));
        radioButton3->setText(QApplication::translate("Dialog_bond", "Triple", Q_NULLPTR));
        radioButton4->setText(QApplication::translate("Dialog_bond", "Quadruple", Q_NULLPTR));
        radioButton5->setText(QApplication::translate("Dialog_bond", "Aromatic", Q_NULLPTR));
        radioButton7->setText(QApplication::translate("Dialog_bond", "Weak", Q_NULLPTR));
        label3->setText(QApplication::translate("Dialog_bond", "0.5000", Q_NULLPTR));
        label4->setText(QApplication::translate("Dialog_bond", "10.0000", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog_bond: public Ui_Dialog_bond {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOND_H
