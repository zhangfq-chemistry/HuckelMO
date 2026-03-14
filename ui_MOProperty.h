/********************************************************************************
** Form generated from reading UI file 'MOProperty.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOPROPERTY_H
#define UI_MOPROPERTY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_OrbitalProperty
{
public:
    QFormLayout *formLayout_2;
    QFrame *frame;
    QFormLayout *formLayout;
    QGridLayout *gridLayout;
    QPushButton *pushButton_ok;
    QLabel *label;
    QSpinBox *spinBox_posLobe_red;
    QSpinBox *spinBox_Line_opacity;
    QPushButton *pushButton_negLobe;
    QSpinBox *spinBox_posLobe_blue;
    QSpinBox *spinBox_Node_blue;
    QPushButton *pushButton_refresh;
    QLabel *label_4;
    QSpinBox *spinBox_Line_blue;
    QSpinBox *spinBox_negLobe_red;
    QPushButton *pushButton_Line;
    QSpinBox *spinBox_Line_red;
    QSpinBox *spinBox_negLobe_opacity;
    QSpinBox *spinBox_Node_green;
    QPushButton *pushButton_cancel;
    QLabel *label_7;
    QSpinBox *spinBox_Line_green;
    QPushButton *pushButton_flipPhase;
    QSpinBox *spinBox_negLobe_green;
    QSpinBox *spinBox_Node_opacity;
    QSpinBox *spinBox_negLobe_blue;
    QSpinBox *spinBox_posLobe_green;
    QPushButton *pushButton_posLobe;
    QSpinBox *spinBox_posLobe_opacity;
    QSpinBox *spinBox_Node_red;
    QPushButton *pushButton_Node;
    QLabel *label_3;
    QLabel *label_2;

    void setupUi(QDialog *OrbitalProperty)
    {
        if (OrbitalProperty->objectName().isEmpty())
            OrbitalProperty->setObjectName(QStringLiteral("OrbitalProperty"));
        OrbitalProperty->resize(441, 216);
        formLayout_2 = new QFormLayout(OrbitalProperty);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        frame = new QFrame(OrbitalProperty);
        frame->setObjectName(QStringLiteral("frame"));
        formLayout = new QFormLayout(frame);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton_ok = new QPushButton(frame);
        pushButton_ok->setObjectName(QStringLiteral("pushButton_ok"));

        gridLayout->addWidget(pushButton_ok, 5, 4, 1, 1);

        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        spinBox_posLobe_red = new QSpinBox(frame);
        spinBox_posLobe_red->setObjectName(QStringLiteral("spinBox_posLobe_red"));
        spinBox_posLobe_red->setMaximum(255);

        gridLayout->addWidget(spinBox_posLobe_red, 1, 1, 1, 1);

        spinBox_Line_opacity = new QSpinBox(frame);
        spinBox_Line_opacity->setObjectName(QStringLiteral("spinBox_Line_opacity"));
        spinBox_Line_opacity->setSingleStep(5);

        gridLayout->addWidget(spinBox_Line_opacity, 4, 4, 1, 1);

        pushButton_negLobe = new QPushButton(frame);
        pushButton_negLobe->setObjectName(QStringLiteral("pushButton_negLobe"));

        gridLayout->addWidget(pushButton_negLobe, 0, 2, 1, 1);

        spinBox_posLobe_blue = new QSpinBox(frame);
        spinBox_posLobe_blue->setObjectName(QStringLiteral("spinBox_posLobe_blue"));
        spinBox_posLobe_blue->setMaximum(255);

        gridLayout->addWidget(spinBox_posLobe_blue, 3, 1, 1, 1);

        spinBox_Node_blue = new QSpinBox(frame);
        spinBox_Node_blue->setObjectName(QStringLiteral("spinBox_Node_blue"));
        spinBox_Node_blue->setMaximum(255);

        gridLayout->addWidget(spinBox_Node_blue, 3, 3, 1, 1);

        pushButton_refresh = new QPushButton(frame);
        pushButton_refresh->setObjectName(QStringLiteral("pushButton_refresh"));

        gridLayout->addWidget(pushButton_refresh, 5, 1, 1, 1);

        label_4 = new QLabel(frame);
        label_4->setObjectName(QStringLiteral("label_4"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        spinBox_Line_blue = new QSpinBox(frame);
        spinBox_Line_blue->setObjectName(QStringLiteral("spinBox_Line_blue"));

        gridLayout->addWidget(spinBox_Line_blue, 3, 4, 1, 1);

        spinBox_negLobe_red = new QSpinBox(frame);
        spinBox_negLobe_red->setObjectName(QStringLiteral("spinBox_negLobe_red"));
        spinBox_negLobe_red->setMaximum(255);

        gridLayout->addWidget(spinBox_negLobe_red, 1, 2, 1, 1);

        pushButton_Line = new QPushButton(frame);
        pushButton_Line->setObjectName(QStringLiteral("pushButton_Line"));

        gridLayout->addWidget(pushButton_Line, 0, 4, 1, 1);

        spinBox_Line_red = new QSpinBox(frame);
        spinBox_Line_red->setObjectName(QStringLiteral("spinBox_Line_red"));

        gridLayout->addWidget(spinBox_Line_red, 1, 4, 1, 1);

        spinBox_negLobe_opacity = new QSpinBox(frame);
        spinBox_negLobe_opacity->setObjectName(QStringLiteral("spinBox_negLobe_opacity"));
        spinBox_negLobe_opacity->setMaximum(255);
        spinBox_negLobe_opacity->setSingleStep(5);

        gridLayout->addWidget(spinBox_negLobe_opacity, 4, 2, 1, 1);

        spinBox_Node_green = new QSpinBox(frame);
        spinBox_Node_green->setObjectName(QStringLiteral("spinBox_Node_green"));
        spinBox_Node_green->setMaximum(255);

        gridLayout->addWidget(spinBox_Node_green, 2, 3, 1, 1);

        pushButton_cancel = new QPushButton(frame);
        pushButton_cancel->setObjectName(QStringLiteral("pushButton_cancel"));

        gridLayout->addWidget(pushButton_cancel, 5, 3, 1, 1);

        label_7 = new QLabel(frame);
        label_7->setObjectName(QStringLiteral("label_7"));
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_7, 4, 0, 1, 1);

        spinBox_Line_green = new QSpinBox(frame);
        spinBox_Line_green->setObjectName(QStringLiteral("spinBox_Line_green"));

        gridLayout->addWidget(spinBox_Line_green, 2, 4, 1, 1);

        pushButton_flipPhase = new QPushButton(frame);
        pushButton_flipPhase->setObjectName(QStringLiteral("pushButton_flipPhase"));

        gridLayout->addWidget(pushButton_flipPhase, 5, 2, 1, 1);

        spinBox_negLobe_green = new QSpinBox(frame);
        spinBox_negLobe_green->setObjectName(QStringLiteral("spinBox_negLobe_green"));
        spinBox_negLobe_green->setMaximum(255);

        gridLayout->addWidget(spinBox_negLobe_green, 2, 2, 1, 1);

        spinBox_Node_opacity = new QSpinBox(frame);
        spinBox_Node_opacity->setObjectName(QStringLiteral("spinBox_Node_opacity"));
        spinBox_Node_opacity->setMaximum(255);
        spinBox_Node_opacity->setSingleStep(5);

        gridLayout->addWidget(spinBox_Node_opacity, 4, 3, 1, 1);

        spinBox_negLobe_blue = new QSpinBox(frame);
        spinBox_negLobe_blue->setObjectName(QStringLiteral("spinBox_negLobe_blue"));
        spinBox_negLobe_blue->setMaximum(255);

        gridLayout->addWidget(spinBox_negLobe_blue, 3, 2, 1, 1);

        spinBox_posLobe_green = new QSpinBox(frame);
        spinBox_posLobe_green->setObjectName(QStringLiteral("spinBox_posLobe_green"));
        spinBox_posLobe_green->setMaximum(255);

        gridLayout->addWidget(spinBox_posLobe_green, 2, 1, 1, 1);

        pushButton_posLobe = new QPushButton(frame);
        pushButton_posLobe->setObjectName(QStringLiteral("pushButton_posLobe"));

        gridLayout->addWidget(pushButton_posLobe, 0, 1, 1, 1);

        spinBox_posLobe_opacity = new QSpinBox(frame);
        spinBox_posLobe_opacity->setObjectName(QStringLiteral("spinBox_posLobe_opacity"));
        spinBox_posLobe_opacity->setMaximum(255);
        spinBox_posLobe_opacity->setSingleStep(5);

        gridLayout->addWidget(spinBox_posLobe_opacity, 4, 1, 1, 1);

        spinBox_Node_red = new QSpinBox(frame);
        spinBox_Node_red->setObjectName(QStringLiteral("spinBox_Node_red"));
        spinBox_Node_red->setMaximum(255);

        gridLayout->addWidget(spinBox_Node_red, 1, 3, 1, 1);

        pushButton_Node = new QPushButton(frame);
        pushButton_Node->setObjectName(QStringLiteral("pushButton_Node"));

        gridLayout->addWidget(pushButton_Node, 0, 3, 1, 1);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);


        formLayout->setLayout(0, QFormLayout::LabelRole, gridLayout);


        formLayout_2->setWidget(0, QFormLayout::FieldRole, frame);


        retranslateUi(OrbitalProperty);

        QMetaObject::connectSlotsByName(OrbitalProperty);
    } // setupUi

    void retranslateUi(QDialog *OrbitalProperty)
    {
        OrbitalProperty->setWindowTitle(QApplication::translate("OrbitalProperty", "Dialog", Q_NULLPTR));
        pushButton_ok->setText(QApplication::translate("OrbitalProperty", "OK", Q_NULLPTR));
        label->setText(QApplication::translate("OrbitalProperty", "Red", Q_NULLPTR));
        pushButton_negLobe->setText(QApplication::translate("OrbitalProperty", "\342\210\222", Q_NULLPTR));
        pushButton_refresh->setText(QApplication::translate("OrbitalProperty", "Refresh", Q_NULLPTR));
        label_4->setText(QApplication::translate("OrbitalProperty", "Color", Q_NULLPTR));
        pushButton_Line->setText(QApplication::translate("OrbitalProperty", "Line", Q_NULLPTR));
        pushButton_cancel->setText(QApplication::translate("OrbitalProperty", "Cancel", Q_NULLPTR));
        label_7->setText(QApplication::translate("OrbitalProperty", "Opacity", Q_NULLPTR));
        pushButton_flipPhase->setText(QApplication::translate("OrbitalProperty", "Flip Phase", Q_NULLPTR));
        pushButton_posLobe->setText(QApplication::translate("OrbitalProperty", "+", Q_NULLPTR));
        pushButton_Node->setText(QApplication::translate("OrbitalProperty", "Node", Q_NULLPTR));
        label_3->setText(QApplication::translate("OrbitalProperty", "Blue", Q_NULLPTR));
        label_2->setText(QApplication::translate("OrbitalProperty", "Green", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class OrbitalProperty: public Ui_OrbitalProperty {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOPROPERTY_H
