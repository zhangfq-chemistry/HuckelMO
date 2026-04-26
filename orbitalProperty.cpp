

#include <QMessageBox>
#include <iostream>
#include <QFileDialog>

#include <QtColorWidgets/color_2d_slider.hpp>
#include <QtColorWidgets/color_delegate.hpp>
#include <QtColorWidgets/color_dialog.hpp>
#include <QtColorWidgets/color_line_edit.hpp>
#include <QtColorWidgets/color_list_widget.hpp>
#include <QtColorWidgets/color_palette_widget.hpp>
#include <QtColorWidgets/color_preview.hpp>
#include <QtColorWidgets/color_wheel.hpp>
#include <QtColorWidgets/hue_slider.hpp>


#include "orbitalProperty.h"
#include "mainwindow.h"



OrbitalProperty::OrbitalProperty(QWidget *parent): QDialog(parent)
{
    m_parent=(MainWindow *)parent;

    view3d=m_parent->view3d;

    ui.setupUi(this);

    initial();
}

//read from View3d
void OrbitalProperty::initial()
{
    QString s;

    //color
    backgroundcolor=255*view3d->getBackgroundColor();
    LobeColor_pos=255*view3d->getPosLobeColor();
    LobeColor_neg=255*view3d->getNegLobeColor();


    ui.spinBox_posLobe_red->setStyleSheet("backgroundcolor:red");
    ui.spinBox_posLobe_green->setStyleSheet("backgroundcolor:green");
    ui.spinBox_posLobe_blue->setStyleSheet("backgroundcolor:blue");

    ui.spinBox_posLobe_red->setValue(LobeColor_pos.x());
    ui.spinBox_posLobe_green->setValue(LobeColor_pos.y());
    ui.spinBox_posLobe_blue->setValue(LobeColor_pos.z());

    ui.spinBox_negLobe_red->setValue(LobeColor_neg.x());
    ui.spinBox_negLobe_green->setValue(LobeColor_neg.y());
    ui.spinBox_negLobe_blue->setValue(LobeColor_neg.z());

    s=tr("background-color: rgb(%1,%2,%3);").arg(LobeColor_pos.x()).arg(LobeColor_pos.y()).arg(LobeColor_pos.z());
    ui.pushButton_posLobe->setStyleSheet(s);

    s=tr("background-color: rgb(%1,%2,%3);").arg(LobeColor_neg.x()).arg(LobeColor_neg.y()).arg(LobeColor_neg.z());
    ui.pushButton_negLobe->setStyleSheet(s);


    nodeColor=255*view3d->getNodecolor();


    ui.spinBox_Node_red->setValue(nodeColor.x());
    ui.spinBox_Node_green->setValue(nodeColor.y());
    ui.spinBox_Node_blue->setValue(nodeColor.z());

    s=tr("background-color: rgb(%1,%2,%3);").arg(nodeColor.x()).arg(nodeColor.y()).arg(nodeColor.z());
    ui.pushButton_Node->setStyleSheet(s);



    //opacity
    nodeOpacity=255*view3d->getNodeOpicity();
    ui.spinBox_Node_opacity->setValue(nodeOpacity);


    Lobe_opacity_pos=255*view3d->getPosLobeOpacity();
    Lobe_opacity_neg=255*view3d->getNegLobeOpacity();

    ui.spinBox_posLobe_opacity->setValue(Lobe_opacity_pos);
    ui.spinBox_negLobe_opacity->setValue(Lobe_opacity_neg);

    orbitalLineWidth=view3d->getOrbitalLineWidth();
   // nodeThickness=view3d->getNodeThickness();
}


void OrbitalProperty::getDataFromCurrentPanel()
{
    LobeColor_pos.SetX(ui.spinBox_posLobe_red->value());
    LobeColor_pos.SetY(ui.spinBox_posLobe_green->value());
    LobeColor_pos.SetZ(ui.spinBox_posLobe_blue->value());


    LobeColor_neg.SetX(ui.spinBox_negLobe_red->value());
    LobeColor_neg.SetY(ui.spinBox_negLobe_green->value());
    LobeColor_neg.SetZ(ui.spinBox_negLobe_blue->value());


    nodeColor.SetX(ui.spinBox_Node_red->value());
    nodeColor.SetY(ui.spinBox_Node_green->value());
    nodeColor.SetZ(ui.spinBox_Node_blue->value());

    nodeOpacity=ui.spinBox_Node_opacity->value();

    Lobe_opacity_pos=ui.spinBox_posLobe_opacity->value();
    Lobe_opacity_neg=ui.spinBox_negLobe_opacity->value();
}


void OrbitalProperty::refreshView()
{
    getDataFromCurrentPanel();

    double a=255.0;

    //lobe color
    view3d->setPosLobeColor(LobeColor_pos/a);
    view3d->setNegLobeColor(LobeColor_neg/a);


    view3d->setPosLobeOpacity(Lobe_opacity_pos/a);
    view3d->setNegLobeOpacity(Lobe_opacity_neg/a);


    view3d->setNodecolor(nodeColor/a);

    //nodal opacity
    view3d->setNodeOpicity(nodeOpacity/a);

    //update view
    view3d->updateMol();
}



void OrbitalProperty::on_pushButton_posLobe_clicked()
{
    QColor color(LobeColor_pos.x(),
                 LobeColor_pos.y(),
                 LobeColor_pos.z(),
                 Lobe_opacity_pos);


    QColorDialog dialog(color);
    dialog.setOption(QColorDialog::ShowAlphaChannel);


    if(dialog.exec() == QDialog::Accepted)
    {
        QColor c=dialog.currentColor();
        LobeColor_pos.Set(c.red(),c.green(),c.blue());
        Lobe_opacity_pos=c.alpha();
    }


    ui.spinBox_posLobe_red->setValue(LobeColor_pos.x());
    ui.spinBox_posLobe_green->setValue(LobeColor_pos.y());
    ui.spinBox_posLobe_blue->setValue(LobeColor_pos.z());
    ui.spinBox_posLobe_opacity->setValue(Lobe_opacity_pos);

}

void OrbitalProperty::on_pushButton_negLobe_clicked()
{
    QColor color(LobeColor_neg.x(),
                 LobeColor_neg.y(),
                 LobeColor_neg.z(),
                 Lobe_opacity_neg);


    QColorDialog dialog(color);
    dialog.setOption(QColorDialog::ShowAlphaChannel);


    if(dialog.exec() == QDialog::Accepted)
    {
        QColor c=dialog.currentColor();
        LobeColor_neg.Set(c.red(),c.green(),c.blue());
        Lobe_opacity_neg=c.alpha();
    }


    ui.spinBox_negLobe_red->setValue(LobeColor_neg.x());
    ui.spinBox_negLobe_green->setValue(LobeColor_neg.y());
    ui.spinBox_negLobe_blue->setValue(LobeColor_neg.z());
    ui.spinBox_negLobe_opacity->setValue(Lobe_opacity_neg);

}

void OrbitalProperty::on_pushButton_Node_clicked()
{
    QColor color(nodeColor.x(),
                       nodeColor.y(),
                       nodeColor.z(),
                       nodeOpacity);

    QColorDialog dialog(color);
    dialog.setOption(QColorDialog::ShowAlphaChannel);

    if(dialog.exec() == QDialog::Accepted)
    {
        QColor c=dialog.currentColor();
        nodeColor.Set(c.red(),c.green(),c.blue());
        nodeOpacity=c.alpha();
    }

    ui.spinBox_Node_red->setValue(nodeColor.x());
    ui.spinBox_Node_green->setValue(nodeColor.y());
    ui.spinBox_Node_blue->setValue(nodeColor.z());
    ui.spinBox_Node_opacity->setValue(nodeOpacity);


}

void OrbitalProperty::on_pushButton_flipPhase_clicked()
{
    //store
    int opacity=Lobe_opacity_neg;
    int red=LobeColor_neg.x();
    int blue=LobeColor_neg.y();
    int green=LobeColor_neg.z();

    Lobe_opacity_neg=Lobe_opacity_pos;
    LobeColor_neg.Set(LobeColor_pos.x(),LobeColor_pos.y(),LobeColor_pos.z());
    LobeColor_pos.Set(red,blue,green);
    Lobe_opacity_neg=opacity;


    ui.spinBox_negLobe_red->setValue(LobeColor_neg.x());
    ui.spinBox_negLobe_green->setValue(LobeColor_neg.y());
    ui.spinBox_negLobe_blue->setValue(LobeColor_neg.z());
    ui.spinBox_negLobe_opacity->setValue(Lobe_opacity_neg);


    ui.spinBox_posLobe_red->setValue(LobeColor_pos.x());
    ui.spinBox_posLobe_green->setValue(LobeColor_pos.y());
    ui.spinBox_posLobe_blue->setValue(LobeColor_pos.z());
    ui.spinBox_posLobe_opacity->setValue(Lobe_opacity_pos);

    QString s;
    s=tr("background-color: rgb(%1,%2,%3);").arg(LobeColor_pos.x()).arg(LobeColor_pos.y()).arg(LobeColor_pos.z());
    ui.pushButton_posLobe->setStyleSheet(s);

    s=tr("background-color: rgb(%1,%2,%3);").arg(LobeColor_neg.x()).arg(LobeColor_neg.y()).arg(LobeColor_neg.z());
    ui.pushButton_negLobe->setStyleSheet(s);
}


void OrbitalProperty::on_pushButton_cancel_clicked()
{
    initial();
    close();
}

void OrbitalProperty::on_pushButton_ok_clicked()
{
    refreshView();
    close();
}

void OrbitalProperty::on_spinBox_posLobe_red_valueChanged(int arg1)
{
    int r=ui.spinBox_posLobe_red->value(),
        g=ui.spinBox_posLobe_green->value(),
        b=ui.spinBox_posLobe_blue->value();

    QString s=tr("background-color: rgb(%1,%2,%3);").arg(r).arg(g).arg(b);
    ui.pushButton_posLobe->setStyleSheet(s);
}

void OrbitalProperty::on_spinBox_posLobe_green_valueChanged(int arg1)
{
    int r=ui.spinBox_posLobe_red->value(),
        g=ui.spinBox_posLobe_green->value(),
        b=ui.spinBox_posLobe_blue->value();

    QString s=tr("background-color: rgb(%1,%2,%3);").arg(r).arg(g).arg(b);
    ui.pushButton_posLobe->setStyleSheet(s);
}

void OrbitalProperty::on_spinBox_posLobe_blue_valueChanged(int arg1)
{
    int r=ui.spinBox_posLobe_red->value(),
        g=ui.spinBox_posLobe_green->value(),
        b=ui.spinBox_posLobe_blue->value();

    QString s=tr("background-color: rgb(%1,%2,%3);").arg(r).arg(g).arg(b);
    ui.pushButton_posLobe->setStyleSheet(s);
}

void OrbitalProperty::on_spinBox_posLobe_opacity_valueChanged(int arg1)
{
    Lobe_opacity_pos=arg1;
}

void OrbitalProperty::on_spinBox_negLobe_red_valueChanged(int arg1)
{
    int r=ui.spinBox_negLobe_red->value(),
        g=ui.spinBox_negLobe_green->value(),
        b=ui.spinBox_negLobe_blue->value();

    QString s=tr("background-color: rgb(%1,%2,%3);").arg(r).arg(g).arg(b);
    ui.pushButton_negLobe->setStyleSheet(s);
}

void OrbitalProperty::on_spinBox_negLobe_green_valueChanged(int arg1)
{
    int r=ui.spinBox_negLobe_red->value(),
        g=ui.spinBox_negLobe_green->value(),
        b=ui.spinBox_negLobe_blue->value();

    QString s=tr("background-color: rgb(%1,%2,%3);").arg(r).arg(g).arg(b);
    ui.pushButton_negLobe->setStyleSheet(s);
}

void OrbitalProperty::on_spinBox_negLobe_blue_valueChanged(int arg1)
{
    int r=ui.spinBox_negLobe_red->value(),
        g=ui.spinBox_negLobe_green->value(),
        b=ui.spinBox_negLobe_blue->value();

    QString s=tr("background-color: rgb(%1,%2,%3);").arg(r).arg(g).arg(b);
    ui.pushButton_negLobe->setStyleSheet(s);
}

void OrbitalProperty::on_spinBox_negLobe_opacity_valueChanged(int arg1)
{
    Lobe_opacity_neg=arg1;
}

void OrbitalProperty::on_spinBox_Node_red_valueChanged(int arg1)
{
    int r=ui.spinBox_Node_red->value(),
        g=ui.spinBox_Node_green->value(),
        b=ui.spinBox_Node_blue->value();

    QString s=tr("background-color: rgb(%1,%2,%3);").arg(r).arg(g).arg(b);
    ui.pushButton_Node->setStyleSheet(s);
}

void OrbitalProperty::on_spinBox_Node_green_valueChanged(int arg1)
{
    int r=ui.spinBox_Node_red->value(),
        g=ui.spinBox_Node_green->value(),
        b=ui.spinBox_Node_blue->value();

    QString s=tr("background-color: rgb(%1,%2,%3);").arg(r).arg(g).arg(b);
    ui.pushButton_Node->setStyleSheet(s);
}

void OrbitalProperty::on_spinBox_Node_blue_valueChanged(int arg1)
{
    int r=ui.spinBox_Node_red->value(),
        g=ui.spinBox_Node_green->value(),
        b=ui.spinBox_Node_blue->value();

    QString s=tr("background-color: rgb(%1,%2,%3);").arg(r).arg(g).arg(b);
    ui.pushButton_Node->setStyleSheet(s);
}

void OrbitalProperty::on_spinBox_Node_opacity_valueChanged(int arg1)
{
    int r=ui.spinBox_Node_red->value(),
        g=ui.spinBox_Node_green->value(),
        b=ui.spinBox_Node_blue->value();

    QString s=tr("background-color: rgb(%1,%2,%3);").arg(r).arg(g).arg(b);
    ui.pushButton_Node->setStyleSheet(s);
}

void OrbitalProperty::on_pushButton_Line_clicked()
{

}

void OrbitalProperty::on_pushButton_refresh_clicked()
{

}
