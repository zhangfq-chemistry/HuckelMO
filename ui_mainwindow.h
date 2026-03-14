/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <View3D.h>
#include <codeEditor.h>
#include <templateView3d.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave_as;
    QAction *actionAlignView_Xy;
    QAction *actionAlignView_Xz;
    QAction *actionAlignView_Yz;
    QAction *actionAbout;
    QAction *actionSave_Png;
    QAction *actionCascade;
    QAction *actionTitles;
    QAction *actionFileOpen;
    QAction *actionFileClose;
    QAction *actionBuilderShow;
    QAction *actionFileNew;
    QAction *actionFileSave2PNG;
    QAction *actionChangeColor;
    QAction *actionEditUndo;
    QAction *actionEditRedo;
    QAction *actionAddHydrogen;
    QAction *actionXTB_optimize;
    QAction *actionReset;
    QAction *actionXOY;
    QAction *actionXOZ;
    QAction *actionYOZ;
    QAction *action;
    QAction *actionSingleBond;
    QAction *actionDoubleBond;
    QAction *actionTripleBond;
    QAction *action_about;
    QAction *actionSymmetryView;
    QAction *actionEditbond;
    QAction *actionEditangle;
    QAction *actionEditdihedral;
    QAction *actionToolscentralize;
    QAction *actionEditPaste;
    QAction *actionEditCut;
    QAction *actionFileExport;
    QAction *actionQuadrupleBond;
    QAction *actionAromaticBond;
    QAction *actionWeakBond;
    QAction *actionHydrogenbond;
    QAction *actionToolsAddHydrogenAuto;
    QAction *actionMMFF94;
    QAction *actionGhemical;
    QAction *actionGaff;
    QAction *actionMM2;
    QAction *actionUFF;
    QAction *actionreCalculateBonds;
    QAction *actionPeriodicTable;
    QAction *actionToolsPencil;
    QAction *actionToolsSelect;
    QAction *actionToolsRotate;
    QAction *actionEditCopy;
    QAction *actionxyz;
    QAction *actionLabel;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab_3d;
    QGridLayout *gridLayout_2;
    View3D *view3d;
    QWidget *tab_coordinate;
    QGridLayout *gridLayout_4;
    QFrame *frame;
    QGridLayout *gridLayout_3;
    CodeEditor *textEdit_dataMol;
    QTabWidget *tabWidget_2;
    QWidget *tab_1;
    TemplateView3d *openGLWidget_template;
    QWidget *frame_21;
    QGridLayout *gridLayout_12;
    QLabel *label_32;
    QLabel *label_30;
    QLineEdit *lineEdit_Y;
    QPushButton *pushButton_translate;
    QPushButton *pushButton_cleardata;
    QPushButton *pushButton_rotate;
    QLineEdit *lineEdit_X;
    QLineEdit *lineEdit_Z;
    QLabel *label_31;
    QFrame *frame1;
    QGridLayout *gridLayout_15;
    QPushButton *pushButton_addHydrogen;
    QPushButton *pushButton_clean;
    QCheckBox *checkBox_kekule;
    QPushButton *pushButton_GAFF;
    QPushButton *pushButton_Ghemical;
    QPushButton *pushButton_UFF;
    QPushButton *pushButton_bond;
    QPushButton *pushButton_angle;
    QPushButton *pushButton_pencil;
    QPushButton *pushButton_hedral;
    QPushButton *pushButton_rebond;
    QPushButton *pushButton_templateMol;
    QWidget *tab_2;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_3;
    QTableView *huckelEnergyView;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame2;
    QGridLayout *gridLayout_9;
    QCheckBox *checkBox_hideNode;
    QCheckBox *checkBox_hideHuckelOrbital;
    QCheckBox *checkBox_negLobe;
    QPushButton *pushButton_calcExtendedHuckel;
    QCheckBox *checkBox_MOType;
    QCheckBox *checkBox_posLobe;
    QCheckBox *checkBox_mesh;
    QCheckBox *checkBox_AOType;
    QCheckBox *checkBox_surface;
    QPushButton *pushButton_calcHuckel;
    QSlider *horizontalSlider;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_16;
    QDial *dialAtomScale;
    QPushButton *pushButton_Bond;
    QPushButton *pushButton_Atom;
    QDial *dialBondScale;
    QPushButton *pushButton_Ring;
    QPushButton *pushButton_colorOrbital;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuwindows;
    QMenu *menuToosBar;
    QMenu *menu;
    QMenu *menuMM;
    QMenu *menuHelp;
    QMenu *menuEdit;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1142, 769);
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName(QStringLiteral("actionSave_as"));
        QIcon icon;
        icon.addFile(QStringLiteral("../images/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_as->setIcon(icon);
        actionAlignView_Xy = new QAction(MainWindow);
        actionAlignView_Xy->setObjectName(QStringLiteral("actionAlignView_Xy"));
        QFont font;
        font.setFamily(QStringLiteral("DejaVu Sans"));
        font.setPointSize(13);
        actionAlignView_Xy->setFont(font);
        actionAlignView_Xz = new QAction(MainWindow);
        actionAlignView_Xz->setObjectName(QStringLiteral("actionAlignView_Xz"));
        actionAlignView_Xz->setFont(font);
        actionAlignView_Yz = new QAction(MainWindow);
        actionAlignView_Yz->setObjectName(QStringLiteral("actionAlignView_Yz"));
        actionAlignView_Yz->setFont(font);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionAbout->setFont(font);
        actionSave_Png = new QAction(MainWindow);
        actionSave_Png->setObjectName(QStringLiteral("actionSave_Png"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/picture.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_Png->setIcon(icon1);
        actionCascade = new QAction(MainWindow);
        actionCascade->setObjectName(QStringLiteral("actionCascade"));
        QFont font1;
        font1.setPointSize(11);
        actionCascade->setFont(font1);
        actionTitles = new QAction(MainWindow);
        actionTitles->setObjectName(QStringLiteral("actionTitles"));
        actionTitles->setFont(font1);
        actionFileOpen = new QAction(MainWindow);
        actionFileOpen->setObjectName(QStringLiteral("actionFileOpen"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/fileOpen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFileOpen->setIcon(icon2);
        actionFileClose = new QAction(MainWindow);
        actionFileClose->setObjectName(QStringLiteral("actionFileClose"));
        actionBuilderShow = new QAction(MainWindow);
        actionBuilderShow->setObjectName(QStringLiteral("actionBuilderShow"));
        actionFileNew = new QAction(MainWindow);
        actionFileNew->setObjectName(QStringLiteral("actionFileNew"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/fileNew.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFileNew->setIcon(icon3);
        actionFileNew->setFont(font1);
        actionFileSave2PNG = new QAction(MainWindow);
        actionFileSave2PNG->setObjectName(QStringLiteral("actionFileSave2PNG"));
        actionFileSave2PNG->setIcon(icon1);
        actionChangeColor = new QAction(MainWindow);
        actionChangeColor->setObjectName(QStringLiteral("actionChangeColor"));
        actionEditUndo = new QAction(MainWindow);
        actionEditUndo->setObjectName(QStringLiteral("actionEditUndo"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/editUndo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEditUndo->setIcon(icon4);
        actionEditUndo->setFont(font1);
        actionEditRedo = new QAction(MainWindow);
        actionEditRedo->setObjectName(QStringLiteral("actionEditRedo"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/editRedo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEditRedo->setIcon(icon5);
        actionEditRedo->setFont(font1);
        actionAddHydrogen = new QAction(MainWindow);
        actionAddHydrogen->setObjectName(QStringLiteral("actionAddHydrogen"));
        actionAddHydrogen->setFont(font1);
        actionXTB_optimize = new QAction(MainWindow);
        actionXTB_optimize->setObjectName(QStringLiteral("actionXTB_optimize"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/UFF.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionXTB_optimize->setIcon(icon6);
        actionReset = new QAction(MainWindow);
        actionReset->setObjectName(QStringLiteral("actionReset"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReset->setIcon(icon7);
        actionReset->setFont(font1);
        actionXOY = new QAction(MainWindow);
        actionXOY->setObjectName(QStringLiteral("actionXOY"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/xoy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionXOY->setIcon(icon8);
        actionXOY->setFont(font1);
        actionXOZ = new QAction(MainWindow);
        actionXOZ->setObjectName(QStringLiteral("actionXOZ"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/images/yoz.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionXOZ->setIcon(icon9);
        actionXOZ->setFont(font1);
        actionYOZ = new QAction(MainWindow);
        actionYOZ->setObjectName(QStringLiteral("actionYOZ"));
        actionYOZ->setIcon(icon9);
        actionYOZ->setFont(font1);
        action = new QAction(MainWindow);
        action->setObjectName(QStringLiteral("action"));
        actionSingleBond = new QAction(MainWindow);
        actionSingleBond->setObjectName(QStringLiteral("actionSingleBond"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/images/singlebond.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionSingleBond->setIcon(icon10);
        actionDoubleBond = new QAction(MainWindow);
        actionDoubleBond->setObjectName(QStringLiteral("actionDoubleBond"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/images/doublebond.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionDoubleBond->setIcon(icon11);
        actionTripleBond = new QAction(MainWindow);
        actionTripleBond->setObjectName(QStringLiteral("actionTripleBond"));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/images/triplebond.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionTripleBond->setIcon(icon12);
        action_about = new QAction(MainWindow);
        action_about->setObjectName(QStringLiteral("action_about"));
        action_about->setFont(font1);
        actionSymmetryView = new QAction(MainWindow);
        actionSymmetryView->setObjectName(QStringLiteral("actionSymmetryView"));
        actionSymmetryView->setCheckable(true);
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/images/symmetry.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSymmetryView->setIcon(icon13);
        actionEditbond = new QAction(MainWindow);
        actionEditbond->setObjectName(QStringLiteral("actionEditbond"));
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/images/bond.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEditbond->setIcon(icon14);
        actionEditbond->setFont(font1);
        actionEditangle = new QAction(MainWindow);
        actionEditangle->setObjectName(QStringLiteral("actionEditangle"));
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/images/angle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEditangle->setIcon(icon15);
        actionEditangle->setFont(font1);
        actionEditdihedral = new QAction(MainWindow);
        actionEditdihedral->setObjectName(QStringLiteral("actionEditdihedral"));
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/images/dihedral.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEditdihedral->setIcon(icon16);
        actionEditdihedral->setFont(font1);
        actionToolscentralize = new QAction(MainWindow);
        actionToolscentralize->setObjectName(QStringLiteral("actionToolscentralize"));
        QIcon icon17;
        icon17.addFile(QStringLiteral(":/images/center.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionToolscentralize->setIcon(icon17);
        actionToolscentralize->setFont(font1);
        actionEditPaste = new QAction(MainWindow);
        actionEditPaste->setObjectName(QStringLiteral("actionEditPaste"));
        QIcon icon18;
        icon18.addFile(QStringLiteral(":/images/editPaste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEditPaste->setIcon(icon18);
        actionEditPaste->setFont(font1);
        actionEditCut = new QAction(MainWindow);
        actionEditCut->setObjectName(QStringLiteral("actionEditCut"));
        QIcon icon19;
        icon19.addFile(QStringLiteral(":/images/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEditCut->setIcon(icon19);
        actionEditCut->setFont(font1);
        actionFileExport = new QAction(MainWindow);
        actionFileExport->setObjectName(QStringLiteral("actionFileExport"));
        QIcon icon20;
        icon20.addFile(QStringLiteral(":/images/fileSave.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFileExport->setIcon(icon20);
        actionQuadrupleBond = new QAction(MainWindow);
        actionQuadrupleBond->setObjectName(QStringLiteral("actionQuadrupleBond"));
        QIcon icon21;
        icon21.addFile(QStringLiteral(":/images/quadruplebond.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionQuadrupleBond->setIcon(icon21);
        actionAromaticBond = new QAction(MainWindow);
        actionAromaticBond->setObjectName(QStringLiteral("actionAromaticBond"));
        QIcon icon22;
        icon22.addFile(QStringLiteral(":/images/aromaticbond.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionAromaticBond->setIcon(icon22);
        actionWeakBond = new QAction(MainWindow);
        actionWeakBond->setObjectName(QStringLiteral("actionWeakBond"));
        QIcon icon23;
        icon23.addFile(QStringLiteral(":/images/weakbond.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionWeakBond->setIcon(icon23);
        actionHydrogenbond = new QAction(MainWindow);
        actionHydrogenbond->setObjectName(QStringLiteral("actionHydrogenbond"));
        QIcon icon24;
        icon24.addFile(QStringLiteral(":/images/line.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHydrogenbond->setIcon(icon24);
        actionToolsAddHydrogenAuto = new QAction(MainWindow);
        actionToolsAddHydrogenAuto->setObjectName(QStringLiteral("actionToolsAddHydrogenAuto"));
        QIcon icon25;
        icon25.addFile(QStringLiteral(":/images/AddHydrogens.gif"), QSize(), QIcon::Normal, QIcon::Off);
        actionToolsAddHydrogenAuto->setIcon(icon25);
        actionToolsAddHydrogenAuto->setFont(font1);
        actionMMFF94 = new QAction(MainWindow);
        actionMMFF94->setObjectName(QStringLiteral("actionMMFF94"));
        actionGhemical = new QAction(MainWindow);
        actionGhemical->setObjectName(QStringLiteral("actionGhemical"));
        actionGaff = new QAction(MainWindow);
        actionGaff->setObjectName(QStringLiteral("actionGaff"));
        actionMM2 = new QAction(MainWindow);
        actionMM2->setObjectName(QStringLiteral("actionMM2"));
        actionUFF = new QAction(MainWindow);
        actionUFF->setObjectName(QStringLiteral("actionUFF"));
        actionreCalculateBonds = new QAction(MainWindow);
        actionreCalculateBonds->setObjectName(QStringLiteral("actionreCalculateBonds"));
        actionreCalculateBonds->setFont(font1);
        actionPeriodicTable = new QAction(MainWindow);
        actionPeriodicTable->setObjectName(QStringLiteral("actionPeriodicTable"));
        QIcon icon26;
        icon26.addFile(QStringLiteral(":/images/periodicTabel.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPeriodicTable->setIcon(icon26);
        actionPeriodicTable->setFont(font1);
        actionToolsPencil = new QAction(MainWindow);
        actionToolsPencil->setObjectName(QStringLiteral("actionToolsPencil"));
        QIcon icon27;
        icon27.addFile(QStringLiteral(":/images/draw.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionToolsPencil->setIcon(icon27);
        actionToolsPencil->setFont(font1);
        actionToolsSelect = new QAction(MainWindow);
        actionToolsSelect->setObjectName(QStringLiteral("actionToolsSelect"));
        QIcon icon28;
        icon28.addFile(QStringLiteral(":/images/select.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionToolsSelect->setIcon(icon28);
        actionToolsRotate = new QAction(MainWindow);
        actionToolsRotate->setObjectName(QStringLiteral("actionToolsRotate"));
        QIcon icon29;
        icon29.addFile(QStringLiteral(":/images/rotate.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionToolsRotate->setIcon(icon29);
        actionToolsRotate->setFont(font1);
        actionEditCopy = new QAction(MainWindow);
        actionEditCopy->setObjectName(QStringLiteral("actionEditCopy"));
        QIcon icon30;
        icon30.addFile(QStringLiteral(":/images/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEditCopy->setIcon(icon30);
        actionEditCopy->setFont(font1);
        actionxyz = new QAction(MainWindow);
        actionxyz->setObjectName(QStringLiteral("actionxyz"));
        QIcon icon31;
        icon31.addFile(QStringLiteral(":/images/axis.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionxyz->setIcon(icon31);
        actionLabel = new QAction(MainWindow);
        actionLabel->setObjectName(QStringLiteral("actionLabel"));
        actionLabel->setFont(font1);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(-1, -1, -1, 0);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setFont(font1);
        tab_3d = new QWidget();
        tab_3d->setObjectName(QStringLiteral("tab_3d"));
        gridLayout_2 = new QGridLayout(tab_3d);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        view3d = new View3D(tab_3d);
        view3d->setObjectName(QStringLiteral("view3d"));

        gridLayout_2->addWidget(view3d, 0, 0, 1, 1);

        tabWidget->addTab(tab_3d, QString());
        tab_coordinate = new QWidget();
        tab_coordinate->setObjectName(QStringLiteral("tab_coordinate"));
        gridLayout_4 = new QGridLayout(tab_coordinate);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        frame = new QFrame(tab_coordinate);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        textEdit_dataMol = new CodeEditor(frame);
        textEdit_dataMol->setObjectName(QStringLiteral("textEdit_dataMol"));
        textEdit_dataMol->setFrameShape(QFrame::Box);

        gridLayout_3->addWidget(textEdit_dataMol, 0, 0, 1, 1);


        gridLayout_4->addWidget(frame, 0, 0, 1, 1);

        tabWidget->addTab(tab_coordinate, QString());

        gridLayout->addWidget(tabWidget, 0, 1, 1, 1);

        tabWidget_2 = new QTabWidget(centralwidget);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setMinimumSize(QSize(278, 0));
        tabWidget_2->setMaximumSize(QSize(230, 16777215));
        tabWidget_2->setFont(font1);
        tab_1 = new QWidget();
        tab_1->setObjectName(QStringLiteral("tab_1"));
        openGLWidget_template = new TemplateView3d(tab_1);
        openGLWidget_template->setObjectName(QStringLiteral("openGLWidget_template"));
        openGLWidget_template->setGeometry(QRect(10, 150, 261, 231));
        frame_21 = new QWidget(tab_1);
        frame_21->setObjectName(QStringLiteral("frame_21"));
        frame_21->setGeometry(QRect(10, 390, 250, 71));
        frame_21->setMaximumSize(QSize(250, 16777215));
        gridLayout_12 = new QGridLayout(frame_21);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        label_32 = new QLabel(frame_21);
        label_32->setObjectName(QStringLiteral("label_32"));
        QFont font2;
        font2.setFamily(QStringLiteral("Noto Sans CJK SC"));
        font2.setPointSize(12);
        label_32->setFont(font2);

        gridLayout_12->addWidget(label_32, 0, 2, 1, 1);

        label_30 = new QLabel(frame_21);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setFont(font2);

        gridLayout_12->addWidget(label_30, 0, 4, 1, 1);

        lineEdit_Y = new QLineEdit(frame_21);
        lineEdit_Y->setObjectName(QStringLiteral("lineEdit_Y"));
        lineEdit_Y->setMinimumSize(QSize(0, 0));
        lineEdit_Y->setMaximumSize(QSize(100, 30));
        QFont font3;
        font3.setFamily(QStringLiteral("Cantarell"));
        font3.setPointSize(11);
        lineEdit_Y->setFont(font3);

        gridLayout_12->addWidget(lineEdit_Y, 0, 3, 1, 1);

        pushButton_translate = new QPushButton(frame_21);
        pushButton_translate->setObjectName(QStringLiteral("pushButton_translate"));
        QFont font4;
        font4.setFamily(QStringLiteral("Noto Sans CJK SC"));
        font4.setPointSize(11);
        pushButton_translate->setFont(font4);

        gridLayout_12->addWidget(pushButton_translate, 1, 3, 1, 1);

        pushButton_cleardata = new QPushButton(frame_21);
        pushButton_cleardata->setObjectName(QStringLiteral("pushButton_cleardata"));
        pushButton_cleardata->setFont(font4);

        gridLayout_12->addWidget(pushButton_cleardata, 1, 1, 1, 1);

        pushButton_rotate = new QPushButton(frame_21);
        pushButton_rotate->setObjectName(QStringLiteral("pushButton_rotate"));
        QFont font5;
        font5.setFamily(QStringLiteral("Sans Serif"));
        font5.setPointSize(11);
        pushButton_rotate->setFont(font5);

        gridLayout_12->addWidget(pushButton_rotate, 1, 5, 1, 1);

        lineEdit_X = new QLineEdit(frame_21);
        lineEdit_X->setObjectName(QStringLiteral("lineEdit_X"));
        lineEdit_X->setMaximumSize(QSize(100, 30));
        lineEdit_X->setFont(font3);
        lineEdit_X->setMaxLength(32);

        gridLayout_12->addWidget(lineEdit_X, 0, 1, 1, 1);

        lineEdit_Z = new QLineEdit(frame_21);
        lineEdit_Z->setObjectName(QStringLiteral("lineEdit_Z"));
        lineEdit_Z->setMaximumSize(QSize(100, 30));
        lineEdit_Z->setFont(font3);

        gridLayout_12->addWidget(lineEdit_Z, 0, 5, 1, 1);

        label_31 = new QLabel(frame_21);
        label_31->setObjectName(QStringLiteral("label_31"));
        label_31->setFont(font2);

        gridLayout_12->addWidget(label_31, 0, 0, 1, 1);

        frame1 = new QFrame(tab_1);
        frame1->setObjectName(QStringLiteral("frame1"));
        frame1->setGeometry(QRect(10, 0, 254, 151));
        frame1->setFrameShape(QFrame::NoFrame);
        gridLayout_15 = new QGridLayout(frame1);
        gridLayout_15->setObjectName(QStringLiteral("gridLayout_15"));
        pushButton_addHydrogen = new QPushButton(frame1);
        pushButton_addHydrogen->setObjectName(QStringLiteral("pushButton_addHydrogen"));
        pushButton_addHydrogen->setFont(font1);

        gridLayout_15->addWidget(pushButton_addHydrogen, 2, 1, 1, 1);

        pushButton_clean = new QPushButton(frame1);
        pushButton_clean->setObjectName(QStringLiteral("pushButton_clean"));
        pushButton_clean->setFont(font1);

        gridLayout_15->addWidget(pushButton_clean, 0, 2, 1, 1);

        checkBox_kekule = new QCheckBox(frame1);
        checkBox_kekule->setObjectName(QStringLiteral("checkBox_kekule"));
        checkBox_kekule->setFont(font1);

        gridLayout_15->addWidget(checkBox_kekule, 0, 0, 1, 1);

        pushButton_GAFF = new QPushButton(frame1);
        pushButton_GAFF->setObjectName(QStringLiteral("pushButton_GAFF"));
        pushButton_GAFF->setFont(font1);

        gridLayout_15->addWidget(pushButton_GAFF, 0, 1, 1, 1);

        pushButton_Ghemical = new QPushButton(frame1);
        pushButton_Ghemical->setObjectName(QStringLiteral("pushButton_Ghemical"));
        pushButton_Ghemical->setFont(font1);

        gridLayout_15->addWidget(pushButton_Ghemical, 1, 2, 1, 1);

        pushButton_UFF = new QPushButton(frame1);
        pushButton_UFF->setObjectName(QStringLiteral("pushButton_UFF"));
        pushButton_UFF->setFont(font1);

        gridLayout_15->addWidget(pushButton_UFF, 1, 1, 1, 1);

        pushButton_bond = new QPushButton(frame1);
        pushButton_bond->setObjectName(QStringLiteral("pushButton_bond"));
        pushButton_bond->setFont(font1);

        gridLayout_15->addWidget(pushButton_bond, 1, 0, 1, 1);

        pushButton_angle = new QPushButton(frame1);
        pushButton_angle->setObjectName(QStringLiteral("pushButton_angle"));
        pushButton_angle->setFont(font1);

        gridLayout_15->addWidget(pushButton_angle, 2, 0, 1, 1);

        pushButton_pencil = new QPushButton(frame1);
        pushButton_pencil->setObjectName(QStringLiteral("pushButton_pencil"));
        pushButton_pencil->setFont(font1);

        gridLayout_15->addWidget(pushButton_pencil, 3, 1, 1, 1);

        pushButton_hedral = new QPushButton(frame1);
        pushButton_hedral->setObjectName(QStringLiteral("pushButton_hedral"));
        pushButton_hedral->setFont(font1);

        gridLayout_15->addWidget(pushButton_hedral, 3, 0, 1, 1);

        pushButton_rebond = new QPushButton(frame1);
        pushButton_rebond->setObjectName(QStringLiteral("pushButton_rebond"));
        pushButton_rebond->setFont(font1);

        gridLayout_15->addWidget(pushButton_rebond, 2, 2, 1, 1);

        pushButton_templateMol = new QPushButton(frame1);
        pushButton_templateMol->setObjectName(QStringLiteral("pushButton_templateMol"));
        pushButton_templateMol->setFont(font1);

        gridLayout_15->addWidget(pushButton_templateMol, 3, 2, 1, 1);

        tabWidget_2->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        frame_2 = new QFrame(tab_2);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(0, 10, 277, 598));
        verticalLayout_3 = new QVBoxLayout(frame_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        huckelEnergyView = new QTableView(frame_2);
        huckelEnergyView->setObjectName(QStringLiteral("huckelEnergyView"));

        verticalLayout_3->addWidget(huckelEnergyView);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        frame2 = new QFrame(frame_2);
        frame2->setObjectName(QStringLiteral("frame2"));
        frame2->setFrameShape(QFrame::NoFrame);
        gridLayout_9 = new QGridLayout(frame2);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        checkBox_hideNode = new QCheckBox(frame2);
        checkBox_hideNode->setObjectName(QStringLiteral("checkBox_hideNode"));

        gridLayout_9->addWidget(checkBox_hideNode, 3, 0, 1, 1);

        checkBox_hideHuckelOrbital = new QCheckBox(frame2);
        checkBox_hideHuckelOrbital->setObjectName(QStringLiteral("checkBox_hideHuckelOrbital"));

        gridLayout_9->addWidget(checkBox_hideHuckelOrbital, 3, 1, 1, 1);

        checkBox_negLobe = new QCheckBox(frame2);
        checkBox_negLobe->setObjectName(QStringLiteral("checkBox_negLobe"));

        gridLayout_9->addWidget(checkBox_negLobe, 0, 1, 1, 1);

        pushButton_calcExtendedHuckel = new QPushButton(frame2);
        pushButton_calcExtendedHuckel->setObjectName(QStringLiteral("pushButton_calcExtendedHuckel"));

        gridLayout_9->addWidget(pushButton_calcExtendedHuckel, 5, 0, 1, 1);

        checkBox_MOType = new QCheckBox(frame2);
        checkBox_MOType->setObjectName(QStringLiteral("checkBox_MOType"));

        gridLayout_9->addWidget(checkBox_MOType, 2, 1, 1, 1);

        checkBox_posLobe = new QCheckBox(frame2);
        checkBox_posLobe->setObjectName(QStringLiteral("checkBox_posLobe"));

        gridLayout_9->addWidget(checkBox_posLobe, 0, 0, 1, 1);

        checkBox_mesh = new QCheckBox(frame2);
        checkBox_mesh->setObjectName(QStringLiteral("checkBox_mesh"));

        gridLayout_9->addWidget(checkBox_mesh, 1, 0, 1, 1);

        checkBox_AOType = new QCheckBox(frame2);
        checkBox_AOType->setObjectName(QStringLiteral("checkBox_AOType"));

        gridLayout_9->addWidget(checkBox_AOType, 2, 0, 1, 1);

        checkBox_surface = new QCheckBox(frame2);
        checkBox_surface->setObjectName(QStringLiteral("checkBox_surface"));

        gridLayout_9->addWidget(checkBox_surface, 1, 1, 1, 1);

        pushButton_calcHuckel = new QPushButton(frame2);
        pushButton_calcHuckel->setObjectName(QStringLiteral("pushButton_calcHuckel"));

        gridLayout_9->addWidget(pushButton_calcHuckel, 5, 1, 1, 1);

        horizontalSlider = new QSlider(frame2);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout_9->addWidget(horizontalSlider, 6, 0, 1, 2);


        verticalLayout_2->addWidget(frame2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout_16 = new QGridLayout();
        gridLayout_16->setObjectName(QStringLiteral("gridLayout_16"));
        dialAtomScale = new QDial(frame_2);
        dialAtomScale->setObjectName(QStringLiteral("dialAtomScale"));
        dialAtomScale->setMaximum(200);
        dialAtomScale->setSingleStep(5);
        dialAtomScale->setPageStep(20);
        dialAtomScale->setValue(100);

        gridLayout_16->addWidget(dialAtomScale, 1, 1, 1, 1);

        pushButton_Bond = new QPushButton(frame_2);
        pushButton_Bond->setObjectName(QStringLiteral("pushButton_Bond"));

        gridLayout_16->addWidget(pushButton_Bond, 2, 2, 1, 1);

        pushButton_Atom = new QPushButton(frame_2);
        pushButton_Atom->setObjectName(QStringLiteral("pushButton_Atom"));

        gridLayout_16->addWidget(pushButton_Atom, 2, 1, 1, 1);

        dialBondScale = new QDial(frame_2);
        dialBondScale->setObjectName(QStringLiteral("dialBondScale"));
        dialBondScale->setMaximum(200);
        dialBondScale->setSingleStep(5);
        dialBondScale->setPageStep(20);
        dialBondScale->setValue(100);

        gridLayout_16->addWidget(dialBondScale, 1, 2, 1, 1);

        pushButton_Ring = new QPushButton(frame_2);
        pushButton_Ring->setObjectName(QStringLiteral("pushButton_Ring"));

        gridLayout_16->addWidget(pushButton_Ring, 3, 2, 1, 1);

        pushButton_colorOrbital = new QPushButton(frame_2);
        pushButton_colorOrbital->setObjectName(QStringLiteral("pushButton_colorOrbital"));

        gridLayout_16->addWidget(pushButton_colorOrbital, 3, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_16);


        verticalLayout_2->addLayout(verticalLayout);


        verticalLayout_3->addLayout(verticalLayout_2);

        tabWidget_2->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget_2, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        tabWidget_2->raise();
        tabWidget->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1142, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuwindows = new QMenu(menuBar);
        menuwindows->setObjectName(QStringLiteral("menuwindows"));
        menuToosBar = new QMenu(menuBar);
        menuToosBar->setObjectName(QStringLiteral("menuToosBar"));
        menu = new QMenu(menuToosBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu->setFont(font1);
        menuMM = new QMenu(menuToosBar);
        menuMM->setObjectName(QStringLiteral("menuMM"));
        menuMM->setFont(font1);
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        QWidget::setTabOrder(tabWidget_2, tabWidget);
        QWidget::setTabOrder(tabWidget, lineEdit_Z);
        QWidget::setTabOrder(lineEdit_Z, pushButton_translate);
        QWidget::setTabOrder(pushButton_translate, lineEdit_Y);
        QWidget::setTabOrder(lineEdit_Y, lineEdit_X);
        QWidget::setTabOrder(lineEdit_X, pushButton_rotate);
        QWidget::setTabOrder(pushButton_rotate, pushButton_cleardata);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuToosBar->menuAction());
        menuBar->addAction(menuwindows->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionFileNew);
        menuFile->addAction(actionFileOpen);
        menuFile->addAction(actionFileClose);
        menuFile->addAction(actionFileSave2PNG);
        menuFile->addAction(actionFileExport);
        menuView->addSeparator();
        menuView->addAction(actionReset);
        menuView->addAction(actionXOY);
        menuView->addAction(actionXOZ);
        menuView->addAction(actionYOZ);
        menuView->addAction(actionLabel);
        menuwindows->addAction(actionCascade);
        menuwindows->addAction(actionTitles);
        menuToosBar->addAction(menuMM->menuAction());
        menuToosBar->addAction(menu->menuAction());
        menuToosBar->addAction(actionToolsAddHydrogenAuto);
        menuToosBar->addAction(actionreCalculateBonds);
        menuToosBar->addAction(actionPeriodicTable);
        menuToosBar->addAction(actionToolscentralize);
        menuToosBar->addAction(actionToolsPencil);
        menuToosBar->addAction(actionToolsRotate);
        menu->addAction(actionSingleBond);
        menu->addAction(actionDoubleBond);
        menu->addAction(actionTripleBond);
        menu->addAction(actionQuadrupleBond);
        menu->addAction(actionAromaticBond);
        menu->addAction(actionWeakBond);
        menuMM->addAction(actionUFF);
        menuMM->addAction(actionMMFF94);
        menuMM->addAction(actionGhemical);
        menuMM->addAction(actionGaff);
        menuMM->addAction(actionMM2);
        menuHelp->addAction(action_about);
        menuEdit->addAction(actionEditUndo);
        menuEdit->addAction(actionEditRedo);
        menuEdit->addAction(actionEditCut);
        menuEdit->addAction(actionEditCopy);
        menuEdit->addAction(actionEditPaste);
        menuEdit->addAction(actionEditbond);
        menuEdit->addAction(actionEditangle);
        menuEdit->addAction(actionEditdihedral);
        menuEdit->addAction(actionAddHydrogen);
        toolBar->addAction(actionFileOpen);
        toolBar->addAction(actionFileNew);
        toolBar->addAction(actionEditUndo);
        toolBar->addAction(actionEditRedo);
        toolBar->addAction(actionEditCut);
        toolBar->addAction(actionEditCopy);
        toolBar->addAction(actionEditPaste);
        toolBar->addSeparator();
        toolBar->addSeparator();
        toolBar->addAction(actionSingleBond);
        toolBar->addAction(actionAromaticBond);
        toolBar->addAction(actionDoubleBond);
        toolBar->addAction(actionTripleBond);
        toolBar->addAction(actionWeakBond);
        toolBar->addSeparator();
        toolBar->addAction(actionReset);
        toolBar->addAction(actionToolscentralize);
        toolBar->addAction(actionxyz);
        toolBar->addAction(actionXOY);
        toolBar->addAction(actionXOZ);
        toolBar->addAction(actionYOZ);
        toolBar->addAction(actionXTB_optimize);
        toolBar->addAction(actionToolsRotate);
        toolBar->addAction(actionToolsPencil);
        toolBar->addAction(actionToolsAddHydrogenAuto);
        toolBar->addAction(actionPeriodicTable);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\344\274\221\345\205\213\345\260\224\345\210\206\345\255\220\350\275\250\351\201\223(HMO-SXNU)", Q_NULLPTR));
        actionSave_as->setText(QApplication::translate("MainWindow", "Save as...", Q_NULLPTR));
        actionAlignView_Xy->setText(QApplication::translate("MainWindow", "Align to x/y", Q_NULLPTR));
        actionAlignView_Xz->setText(QApplication::translate("MainWindow", "Align to x/z", Q_NULLPTR));
        actionAlignView_Yz->setText(QApplication::translate("MainWindow", "Align to y/z", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("MainWindow", "About...", Q_NULLPTR));
        actionSave_Png->setText(QApplication::translate("MainWindow", "Save Png", Q_NULLPTR));
        actionCascade->setText(QApplication::translate("MainWindow", "Cascade", Q_NULLPTR));
        actionTitles->setText(QApplication::translate("MainWindow", "Titles", Q_NULLPTR));
        actionFileOpen->setText(QApplication::translate("MainWindow", "Open", Q_NULLPTR));
        actionFileClose->setText(QApplication::translate("MainWindow", "Close", Q_NULLPTR));
        actionBuilderShow->setText(QApplication::translate("MainWindow", "Builder", Q_NULLPTR));
        actionFileNew->setText(QApplication::translate("MainWindow", "New", Q_NULLPTR));
        actionFileSave2PNG->setText(QApplication::translate("MainWindow", "png\345\233\276\347\211\207", Q_NULLPTR));
        actionChangeColor->setText(QApplication::translate("MainWindow", "ChangeColor", Q_NULLPTR));
        actionEditUndo->setText(QApplication::translate("MainWindow", "Undo", Q_NULLPTR));
        actionEditRedo->setText(QApplication::translate("MainWindow", "Redo", Q_NULLPTR));
        actionAddHydrogen->setText(QApplication::translate("MainWindow", "\345\212\240\346\260\242", Q_NULLPTR));
        actionXTB_optimize->setText(QApplication::translate("MainWindow", "XTB \345\207\240\344\275\225\344\274\230\345\214\226", Q_NULLPTR));
        actionReset->setText(QApplication::translate("MainWindow", "Reset", Q_NULLPTR));
        actionXOY->setText(QApplication::translate("MainWindow", "XOY", Q_NULLPTR));
        actionXOZ->setText(QApplication::translate("MainWindow", "XOZ", Q_NULLPTR));
        actionYOZ->setText(QApplication::translate("MainWindow", "YOZ", Q_NULLPTR));
        action->setText(QApplication::translate("MainWindow", "\347\202\271\347\276\244\345\205\203\347\264\240", Q_NULLPTR));
        actionSingleBond->setText(QApplication::translate("MainWindow", "single", Q_NULLPTR));
        actionDoubleBond->setText(QApplication::translate("MainWindow", "double", Q_NULLPTR));
        actionTripleBond->setText(QApplication::translate("MainWindow", "triple", Q_NULLPTR));
        action_about->setText(QApplication::translate("MainWindow", "\344\275\234\350\200\205", Q_NULLPTR));
        actionSymmetryView->setText(QApplication::translate("MainWindow", "\345\257\271\347\247\260\345\205\203\347\264\240", Q_NULLPTR));
        actionEditbond->setText(QApplication::translate("MainWindow", "\346\224\271\345\217\230\351\224\256\351\225\277", Q_NULLPTR));
        actionEditangle->setText(QApplication::translate("MainWindow", "\346\224\271\345\217\230\351\224\256\350\247\222", Q_NULLPTR));
        actionEditdihedral->setText(QApplication::translate("MainWindow", "\344\272\214\351\235\242\350\247\222", Q_NULLPTR));
        actionToolscentralize->setText(QApplication::translate("MainWindow", "\345\210\206\345\255\220\345\261\205\344\270\255", Q_NULLPTR));
        actionEditPaste->setText(QApplication::translate("MainWindow", "Paste", Q_NULLPTR));
        actionEditCut->setText(QApplication::translate("MainWindow", "Cut", Q_NULLPTR));
        actionFileExport->setText(QApplication::translate("MainWindow", "\345\255\230\347\233\230", Q_NULLPTR));
        actionQuadrupleBond->setText(QApplication::translate("MainWindow", "quadruple", Q_NULLPTR));
        actionAromaticBond->setText(QApplication::translate("MainWindow", "aromatic", Q_NULLPTR));
        actionWeakBond->setText(QApplication::translate("MainWindow", "weak", Q_NULLPTR));
        actionHydrogenbond->setText(QApplication::translate("MainWindow", "hydrogen", Q_NULLPTR));
        actionToolsAddHydrogenAuto->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\345\212\240\346\260\242", Q_NULLPTR));
        actionMMFF94->setText(QApplication::translate("MainWindow", "MMFF94", Q_NULLPTR));
        actionGhemical->setText(QApplication::translate("MainWindow", "Ghemical", Q_NULLPTR));
        actionGaff->setText(QApplication::translate("MainWindow", "Gaff", Q_NULLPTR));
        actionMM2->setText(QApplication::translate("MainWindow", "MM2", Q_NULLPTR));
        actionUFF->setText(QApplication::translate("MainWindow", "UFF", Q_NULLPTR));
        actionreCalculateBonds->setText(QApplication::translate("MainWindow", "\351\207\215\346\226\260\350\256\241\347\256\227\345\214\226\345\255\246\351\224\256", Q_NULLPTR));
        actionPeriodicTable->setText(QApplication::translate("MainWindow", "\345\205\203\347\264\240\345\221\250\346\234\237\350\241\250", Q_NULLPTR));
        actionToolsPencil->setText(QApplication::translate("MainWindow", "\351\223\205\347\254\224", Q_NULLPTR));
        actionToolsSelect->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\346\250\241\345\274\217", Q_NULLPTR));
        actionToolsRotate->setText(QApplication::translate("MainWindow", "\346\227\213\350\275\254\346\250\241\345\274\217", Q_NULLPTR));
        actionEditCopy->setText(QApplication::translate("MainWindow", "Copy", Q_NULLPTR));
        actionxyz->setText(QApplication::translate("MainWindow", "\345\235\220\346\240\207\350\275\264", Q_NULLPTR));
        actionLabel->setText(QApplication::translate("MainWindow", "Label", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_3d), QApplication::translate("MainWindow", "3D\346\250\241\345\236\213", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_coordinate), QApplication::translate("MainWindow", "\345\216\237\345\255\220\345\235\220\346\240\207", Q_NULLPTR));
        label_32->setText(QApplication::translate("MainWindow", "Y", Q_NULLPTR));
        label_30->setText(QApplication::translate("MainWindow", "Z", Q_NULLPTR));
        pushButton_translate->setText(QApplication::translate("MainWindow", "\345\271\263\347\247\273", Q_NULLPTR));
        pushButton_cleardata->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", Q_NULLPTR));
        pushButton_rotate->setText(QApplication::translate("MainWindow", "\346\227\213\350\275\254", Q_NULLPTR));
        label_31->setText(QApplication::translate("MainWindow", "X", Q_NULLPTR));
        pushButton_addHydrogen->setText(QApplication::translate("MainWindow", "\345\212\240\346\260\242", Q_NULLPTR));
        pushButton_clean->setText(QApplication::translate("MainWindow", "MMFF94", Q_NULLPTR));
        checkBox_kekule->setText(QApplication::translate("MainWindow", "Kekule", Q_NULLPTR));
        pushButton_GAFF->setText(QApplication::translate("MainWindow", "GAFF", Q_NULLPTR));
        pushButton_Ghemical->setText(QApplication::translate("MainWindow", "Ghemical", Q_NULLPTR));
        pushButton_UFF->setText(QApplication::translate("MainWindow", "UFF", Q_NULLPTR));
        pushButton_bond->setText(QApplication::translate("MainWindow", "\351\224\256\351\225\277", Q_NULLPTR));
        pushButton_angle->setText(QApplication::translate("MainWindow", "\351\224\256\350\247\222", Q_NULLPTR));
        pushButton_pencil->setText(QApplication::translate("MainWindow", "\351\223\205\347\254\224", Q_NULLPTR));
        pushButton_hedral->setText(QApplication::translate("MainWindow", "\344\272\214\351\235\242\350\247\222", Q_NULLPTR));
        pushButton_rebond->setText(QApplication::translate("MainWindow", "\351\207\215\346\226\260\351\224\256\350\277\236", Q_NULLPTR));
        pushButton_templateMol->setText(QApplication::translate("MainWindow", "\345\210\206\345\255\220\346\250\241\346\235\277", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_1), QApplication::translate("MainWindow", "\346\236\204\345\273\272\345\210\206\345\255\220", Q_NULLPTR));
        checkBox_hideNode->setText(QApplication::translate("MainWindow", "\350\275\250\351\201\223\350\212\202\351\235\242", Q_NULLPTR));
        checkBox_hideHuckelOrbital->setText(QApplication::translate("MainWindow", "\351\232\220\350\227\217\350\275\250\351\201\223", Q_NULLPTR));
        checkBox_negLobe->setText(QApplication::translate("MainWindow", "\350\275\250\351\201\223\350\264\237\347\223\243(-)", Q_NULLPTR));
        pushButton_calcExtendedHuckel->setText(QApplication::translate("MainWindow", "\346\211\251\345\261\225HMO\346\263\225", Q_NULLPTR));
        checkBox_MOType->setText(QApplication::translate("MainWindow", "\345\210\206\345\255\220\350\275\250\351\201\223\345\236\213", Q_NULLPTR));
        checkBox_posLobe->setText(QApplication::translate("MainWindow", "\350\275\250\351\201\223\346\255\243\347\223\243(+)", Q_NULLPTR));
        checkBox_mesh->setText(QApplication::translate("MainWindow", "\347\275\221\346\240\274(Mesh)", Q_NULLPTR));
        checkBox_AOType->setText(QApplication::translate("MainWindow", "\345\216\237\345\255\220\350\275\250\351\201\223\345\236\213", Q_NULLPTR));
        checkBox_surface->setText(QApplication::translate("MainWindow", "\346\233\262\351\235\242(Surface)", Q_NULLPTR));
        pushButton_calcHuckel->setText(QApplication::translate("MainWindow", "HMO\346\263\225", Q_NULLPTR));
        dialAtomScale->setProperty("view_option_name", QVariant(QApplication::translate("MainWindow", "atom_scale", Q_NULLPTR)));
        pushButton_Bond->setText(QApplication::translate("MainWindow", "\351\224\256", Q_NULLPTR));
        pushButton_Atom->setText(QApplication::translate("MainWindow", "\345\216\237\345\255\220", Q_NULLPTR));
        dialBondScale->setProperty("view_option_name", QVariant(QApplication::translate("MainWindow", "bond_scale", Q_NULLPTR)));
        pushButton_Ring->setText(QApplication::translate("MainWindow", "\345\234\206\347\216\257", Q_NULLPTR));
        pushButton_colorOrbital->setText(QApplication::translate("MainWindow", "\350\275\250\351\201\223\351\242\234\350\211\262", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_2), QApplication::translate("MainWindow", "Huckel", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("MainWindow", "\350\247\206\345\233\276", Q_NULLPTR));
        menuwindows->setTitle(QApplication::translate("MainWindow", "\347\252\227\345\217\243", Q_NULLPTR));
        menuToosBar->setTitle(QApplication::translate("MainWindow", "\345\267\245\345\205\267", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\350\260\203\346\225\264\351\224\256\347\272\247", Q_NULLPTR));
        menuMM->setTitle(QApplication::translate("MainWindow", "MM \345\207\240\344\275\225\344\274\230\345\214\226", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("MainWindow", "\347\274\226\350\276\221", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
