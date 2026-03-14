#ifndef ORBITALPROPERTY_H
#define ORBITALPROPERTY_H

#include <QWidget>
#include <QtColorWidgets/color_2d_slider.hpp>
#include <QtColorWidgets/color_delegate.hpp>
#include <QtColorWidgets/color_dialog.hpp>
#include <QtColorWidgets/color_line_edit.hpp>
#include <QtColorWidgets/color_list_widget.hpp>
#include <QtColorWidgets/color_palette_widget.hpp>
#include <QtColorWidgets/color_preview.hpp>
#include <QtColorWidgets/color_wheel.hpp>
#include <QtColorWidgets/hue_slider.hpp>
#include <QColorDialog>
#include "math/vector3.h"

#include "ui_MOProperty.h"

using namespace std;

class MainWindow;
class View3D;



class OrbitalProperty : public QDialog
{
    Q_OBJECT
public:
    Ui::OrbitalProperty   ui;
    explicit OrbitalProperty(QWidget *parent = nullptr);

    MainWindow * m_parent;

    View3D * view3d;


    void initial();
    void refreshView();
    void getDataFromCurrentPanel();


signals:


private slots:
    void on_pushButton_posLobe_clicked();
    void on_pushButton_negLobe_clicked();
    void on_pushButton_Node_clicked();
    void on_pushButton_flipPhase_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();
    void on_spinBox_posLobe_red_valueChanged(int arg1);
    void on_spinBox_posLobe_green_valueChanged(int arg1);
    void on_spinBox_posLobe_blue_valueChanged(int arg1);
    void on_spinBox_posLobe_opacity_valueChanged(int arg1);
    void on_spinBox_negLobe_red_valueChanged(int arg1);

    void on_spinBox_negLobe_green_valueChanged(int arg1);

    void on_spinBox_negLobe_blue_valueChanged(int arg1);

    void on_spinBox_negLobe_opacity_valueChanged(int arg1);

    void on_spinBox_Node_red_valueChanged(int arg1);

    void on_spinBox_Node_green_valueChanged(int arg1);

    void on_spinBox_Node_blue_valueChanged(int arg1);

    void on_spinBox_Node_opacity_valueChanged(int arg1);

    void on_pushButton_Line_clicked();

    void on_pushButton_refresh_clicked();

private:
    vector3 LobeColor_pos,LobeColor_neg,orbitalLineColor;
    vector3 nodeColor;

    int  nodeOpacity;
    int  Lobe_opacity_pos, Lobe_opacity_neg, orbitalLine_opacity;

    vector3 backgroundcolor;

    double  orbitalLineWidth,nodeThickness;


};

#endif // ORBITALPROPERTY_H
