/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stack>
#include <QStack>


QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
QT_END_NAMESPACE


#include "View3D.h"
#include "textEditor.h"

#include "atomProperty.h"
#include "ui_mainwindow.h"
#include "Mol.h"
#include "EHMO.h"

#include "HMO.h"


class QStandardItemModel;

class HuckelTextForm;
class templateMol;
class TemplateView3d;
class AtomTemplate;
class ringTemplate;


struct MolData
{
    vector < QString > atomSymbols;   //H, He, Li
    vector <vector3> atom3d;          //x,y,z
    vector < vector < uint > > bonds; //beg,end, order

    vector < vector < uint > > rings; //idx
    vector < vector3 > ringCenter;
    vector < vector3 > ringRadius;
    vector < vector3 > ringNormal;
    vector < bool >    ringAromatic;
};

#include <QProgressBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() { if(ehmo) delete ehmo; }

     void updateExtendedHuckelMOs();
     void updateHuckelMOs();
     QString getExecutePath ();

     void recoverfromStack();


protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    //void on_pushButton_XY_clicked();
    //void on_pushButton_XZ_clicked();
    //void on_pushButton_YZ_clicked();


    void on_actionFileNew_triggered();
    void on_actionFileOpen_triggered();
    void on_actionFileSave2PNG_triggered();
    void on_pushButton_clean_clicked();

    void on_pushButton_addHydrogen_clicked();

    void on_pushButton_rotate_clicked();

    void on_pushButton_translate_clicked();

    void on_pushButton_cleardata_clicked();

    //void on_pushButton_reset_clicked();



    void onTextEditPaste();


    void on_dialAtomScale_valueChanged(int value);
    void on_dialBondScale_valueChanged(int value);


    void on_actionSave_Png_triggered();
    void on_actionReset_triggered();

    void on_actionEditUndo_triggered();
    void on_actionEditRedo_triggered();
    void on_actionToolscentralize_triggered();

    void on_actionFileExport_triggered();


    void on_actionEditCut_triggered();


    void on_actionSingleBond_triggered();

    void on_actionDoubleBond_triggered();

    void on_actionTripleBond_triggered();

    void on_actionQuadrupleBond_triggered();

    void on_actionWeakBond_triggered();

    void on_actionHydrogenbond_triggered();

    void on_actionAromaticBond_triggered();

    void on_actionToolsAddHydrogenAuto_triggered();

    void on_actionMMFF94_triggered();

    void on_actionGhemical_triggered();

    void on_actionUFF_triggered();

    void on_actionMM2_triggered();

    void on_actionGaff_triggered();

    void on_actionreCalculateBonds_triggered();

    void on_actionPeriodicTable_triggered();

    void on_pushButton_rebond_clicked();

    void on_pushButton_UFF_clicked();
    void on_pushButton_Ghemical_clicked();
    void on_pushButton_GAFF_clicked();



    void on_actionXTB_optimize_triggered();

    void on_pushButton_Atom_clicked();
    void on_pushButton_Ring_clicked();

public:
    void on_pushButton_left_clicked();
    void on_pushButton_right_clicked();
    void on_pushButton_Up_clicked();
    void on_pushButton_Down_clicked();
    void on_pushButton_clockwise_clicked();
    void on_pushButton_counterclockwise_clicked();

public:
    void sendMessage(QString  );

    void clearAllViews();
    void loadFile(QString );
    void saveFile(QString );

    void displayMolCoordinates(HMol * );

    void setFreshView (bool s) {needFreshView=s;}

    View3D * getView3D () {return view3d;}

    HMol * getMol () {return mol;}


    void  loadTemplateMol ();
    void  clearTemplate() ;
    void  setTemplateName(QString );


    void updateLastStackOne();
    void buildMolData ();
    QString buildSelectedAtomBondInfo();

    void push2Stack (QString );
    void push2Stack_selection (QString ); //special case for selection
    void recoverStackData (QString & );

    void undo () {on_actionEditUndo_triggered();}
    void redo () {on_actionEditRedo_triggered();}

    void setLinkedMode() {view3d->setLinkTemplateMode();}


    void loadDefaultColorSets();


public:
    QString nameTMol;
    HMol * mol;
    View3D  *  view3d;
    TextEditor *  infoview;


    TemplateView3d  * view3dT;

    QStack < QString  > undoList;
    QStack < QString  > redoList;
    QString dataMol,dataMol0;
    bool isFirstTime;
    void setFirstTime(bool b){isFirstTime=b;}
    

private:
    enum { MaxRecentFiles = 5 };



    QMdiArea *mdiArea;
    QString FileName;

   // QMenu *windowMenu;
   // QAction *newAct;
  //  QAction *saveAct;
   // QAction *saveAsAct;
  //  QAction *recentFileActs[MaxRecentFiles];
  //  QAction *recentFileSeparator;
  //  QAction *recentFileSubMenuAct;



public slots:

  void onInformActive();

  void on_actionToolsPencil_triggered();
  void on_actionEditCopy_triggered();
  void on_actionToolsRotate_triggered();


private slots:
  void on_actionEditPaste_triggered();

  void on_actionxyz_triggered();

  void on_actionEditbond_triggered();

  void on_actionEditangle_triggered();

  void on_actionEditdihedral_triggered();





  

  void on_actionXOY_triggered();

  void on_actionXOZ_triggered();

  void on_actionYOZ_triggered();

  void on_pushButton_templateMol_clicked();

  void on_pushButton_hedral_clicked();

  void on_pushButton_angle_clicked();

  void on_pushButton_bond_clicked();

  void on_pushButton_pencil_clicked();

  void on_checkBox_kekule_stateChanged(int arg1);

  void on_pushButton_calcHuckel_clicked();

  void on_pushButton_calcExtendedHuckel_clicked();

  void on_checkBox_posLobe_stateChanged(int arg1);

  void on_checkBox_negLobe_stateChanged(int arg1);

  void on_checkBox_mesh_stateChanged(int arg1);

  void on_checkBox_AOType_stateChanged(int arg1);

  void on_checkBox_MOType_stateChanged(int arg1);

  void on_checkBox_hideNode_stateChanged(int arg1);

  void on_checkBox_hideHuckelOrbital_stateChanged(int arg1);

  void on_checkBox_surface_stateChanged(int arg1);

  void on_huckelEnergyView_clicked(const QModelIndex &index);

  void on_horizontalSlider_valueChanged(int value);

  void on_pushButton_colorOrbital_clicked();

  void on_action_about_triggered();

  void on_actionLabel_triggered();

public:
  EHMO * getEHMO () {return ehmo;}
  HMO  * getHMO ()  {return hmo;}


private:
  Ui::MainWindow ui;
  bool needFreshView;
  bool isMOExisted;
  //HuckelTextForm * huckelTextForm;

  double scaleAO;
  bool updateHuckelAO;
  EHMO * ehmo;
  HMO * hmo;

  //calculation type
  bool isHMO,isEHMO,isXTB;
};

#endif
