/********************************************************************************
** Form generated from reading UI file 'torsion.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TORSION_H
#define UI_TORSION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog_torsion
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_cancel;
    QSpacerItem *sf;
    QPushButton *pushButton_ok;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_3;
    QSlider *horizontalSlider;
    QHBoxLayout *_10;
    QLabel *label21;
    QSpacerItem *spacerItem;
    QLineEdit *lineEdit;
    QSpacerItem *spacerItem1;
    QLabel *label22;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label1;
    QComboBox *comboBox1;
    QSpacerItem *horizontalSpacer_7;
    QPlainTextEdit *textEdit1;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label2;
    QComboBox *comboBox2;
    QSpacerItem *horizontalSpacer_5;
    QPlainTextEdit *textEdit2;

    void setupUi(QDialog *Dialog_torsion)
    {
        if (Dialog_torsion->objectName().isEmpty())
            Dialog_torsion->setObjectName(QStringLiteral("Dialog_torsion"));
        Dialog_torsion->resize(624, 342);
        layoutWidget = new QWidget(Dialog_torsion);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(110, 280, 361, 34));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_cancel = new QPushButton(layoutWidget);
        pushButton_cancel->setObjectName(QStringLiteral("pushButton_cancel"));
        QFont font;
        font.setPointSize(10);
        pushButton_cancel->setFont(font);

        horizontalLayout->addWidget(pushButton_cancel);

        sf = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(sf);

        pushButton_ok = new QPushButton(layoutWidget);
        pushButton_ok->setObjectName(QStringLiteral("pushButton_ok"));
        pushButton_ok->setFont(font);

        horizontalLayout->addWidget(pushButton_ok);

        layoutWidget1 = new QWidget(Dialog_torsion);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(50, 210, 511, 57));
        verticalLayout_3 = new QVBoxLayout(layoutWidget1);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalSlider = new QSlider(layoutWidget1);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout_3->addWidget(horizontalSlider);

        _10 = new QHBoxLayout();
        _10->setObjectName(QStringLiteral("_10"));
        label21 = new QLabel(layoutWidget1);
        label21->setObjectName(QStringLiteral("label21"));
        QFont font1;
        font1.setFamily(QStringLiteral("Sans Serif"));
        font1.setPointSize(10);
        label21->setFont(font1);

        _10->addWidget(label21);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        _10->addItem(spacerItem);

        lineEdit = new QLineEdit(layoutWidget1);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setMaximumSize(QSize(200, 16777215));
        QFont font2;
        font2.setPointSize(12);
        lineEdit->setFont(font2);

        _10->addWidget(lineEdit);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        _10->addItem(spacerItem1);

        label22 = new QLabel(layoutWidget1);
        label22->setObjectName(QStringLiteral("label22"));
        label22->setFont(font1);

        _10->addWidget(label22);


        verticalLayout_3->addLayout(_10);

        layoutWidget2 = new QWidget(Dialog_torsion);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(30, 20, 566, 171));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);

        label1 = new QLabel(layoutWidget2);
        label1->setObjectName(QStringLiteral("label1"));
        label1->setFont(font1);

        horizontalLayout_6->addWidget(label1);

        comboBox1 = new QComboBox(layoutWidget2);
        comboBox1->setObjectName(QStringLiteral("comboBox1"));
        comboBox1->setFont(font1);

        horizontalLayout_6->addWidget(comboBox1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);


        verticalLayout_2->addLayout(horizontalLayout_6);

        textEdit1 = new QPlainTextEdit(layoutWidget2);
        textEdit1->setObjectName(QStringLiteral("textEdit1"));

        verticalLayout_2->addWidget(textEdit1);


        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        label2 = new QLabel(layoutWidget2);
        label2->setObjectName(QStringLiteral("label2"));
        label2->setFont(font1);

        horizontalLayout_5->addWidget(label2);

        comboBox2 = new QComboBox(layoutWidget2);
        comboBox2->setObjectName(QStringLiteral("comboBox2"));
        comboBox2->setFont(font1);

        horizontalLayout_5->addWidget(comboBox2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_5);

        textEdit2 = new QPlainTextEdit(layoutWidget2);
        textEdit2->setObjectName(QStringLiteral("textEdit2"));

        verticalLayout->addWidget(textEdit2);


        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(Dialog_torsion);

        QMetaObject::connectSlotsByName(Dialog_torsion);
    } // setupUi

    void retranslateUi(QDialog *Dialog_torsion)
    {
        Dialog_torsion->setWindowTitle(QApplication::translate("Dialog_torsion", "Dialog", Q_NULLPTR));
        pushButton_cancel->setText(QApplication::translate("Dialog_torsion", "Cancel", Q_NULLPTR));
        pushButton_ok->setText(QApplication::translate("Dialog_torsion", "OK", Q_NULLPTR));
        label21->setText(QApplication::translate("Dialog_torsion", "-180.00", Q_NULLPTR));
        label22->setText(QApplication::translate("Dialog_torsion", "180.00", Q_NULLPTR));
        label1->setText(QApplication::translate("Dialog_torsion", "Atom1", Q_NULLPTR));
        comboBox1->clear();
        comboBox1->insertItems(0, QStringList()
         << QApplication::translate("Dialog_torsion", "Fixed", Q_NULLPTR)
         << QApplication::translate("Dialog_torsion", "Rotate atom", Q_NULLPTR)
         << QApplication::translate("Dialog_torsion", "Rotate group", Q_NULLPTR)
        );
        label2->setText(QApplication::translate("Dialog_torsion", "Atom4", Q_NULLPTR));
        comboBox2->clear();
        comboBox2->insertItems(0, QStringList()
         << QApplication::translate("Dialog_torsion", "Fixed", Q_NULLPTR)
         << QApplication::translate("Dialog_torsion", "Rotate atom", Q_NULLPTR)
         << QApplication::translate("Dialog_torsion", "Rotate group", Q_NULLPTR)
        );
    } // retranslateUi

};

namespace Ui {
    class Dialog_torsion: public Ui_Dialog_torsion {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TORSION_H
