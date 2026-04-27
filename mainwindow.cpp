
#include <QtWidgets>
#include <QRegularExpression>

#include "mainwindow.h"
#include "math/vector3.h"
#include "aboutDialog.h"

static const QRegularExpression RE_WHITESPACE("\\s+");


#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkDataSetMapper.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkDataSet.h>

#include <vtkPDBReader.h>
#include <vtkXYZMolReader2.h>
#include <vtkGaussianCubeReader2.h>
#include <vtkCMLMoleculeReader.h>
#include <vtkAnimationScene.h>


#include <QIcon>
#include <QDialog>
#include <QColorDialog>

#include "bondProperty.h"

#include "templateMol.h"
#include "ringTemplate.h"
#include "templateView3d.h"
#include "rTemplate.h"
#include "atomTemplate.h"
#include "angleProperty.h"
#include "ringProperty.h"
#include "torsionProperty.h"

#include "EHMO.h"
#include "math/symmetry.h"


#include "periodicTable.h"

#include <QWidget>
/*
#include "color_2d_slider.hpp"
#include "color_delegate.hpp"
#include "color_dialog.hpp"
#include "color_line_edit.hpp"
#include "color_list_widget.hpp"
#include "color_palette_widget.hpp"
#include "color_preview.hpp"
#include "color_wheel.hpp"
#include "hue_slider.hpp"
*/
#include <QColorDialog>
#include <QTimer>

#include <openbabel/mol.h>
#include <openbabel/atom.h>
#include <openbabel/bond.h>
#include <openbabel/generic.h>
#include <openbabel/obconversion.h>

#include "codeEditor.h"

#include "huckelTextForm.h"
#include "orbitalProperty.h"


MainWindow::MainWindow(): mdiArea(new QMdiArea)
{
    ui.setupUi (this);

    statusBar()->showMessage("Ready");

    ui.view3d->mainWindow=this;
    view3d=ui.view3d;

    ui.tabWidget->setCurrentWidget(ui.tab_3d);
    ui.view3d->show();

    ui.view3d->setTemplateView3d(ui.openGLWidget_template);
    ui.openGLWidget_template->setMainWindow(this);

    ui.openGLWidget_template->setStyleSheet("#frame{border:2px solid #014F84; background-color:#00d8ff}");


    mol = new HMol(this);
    ui.view3d->mol=mol;

    //ui.mdiArea->tileSubWindows();
    ui.view3d->showMaximized();


    ui.textEdit_dataMol->setParent(this);
    ui.textEdit_dataMol->setView3D(view3d);
    ui.textEdit_dataMol->setMol(mol);


    isMOExisted=false;

    isFirstTime=true;
    needFreshView=true;

    FileName="UnTitle";

    ui.dialAtomScale->setRange(1,1000);
    ui.dialBondScale->setRange(1,1000);

    ui.huckelEnergyView->verticalHeader()->hide();
    ui.huckelEnergyView->horizontalHeader()->hide();
    ui.huckelEnergyView->setShowGrid(false);


    ui.checkBox_posLobe->setCheckState(Qt::Checked);
    ui.checkBox_negLobe->setCheckState(Qt::Checked);


    ui.checkBox_surface->setCheckState(Qt::Unchecked);
    ui.checkBox_mesh->setCheckState(Qt::Checked);

    ui.checkBox_posLobe->setCheckState(Qt::Checked);
    ui.checkBox_negLobe->setCheckState(Qt::Checked);

    ui.checkBox_MOType->setCheckState(Qt::Checked);
    ui.checkBox_hideHuckelOrbital->setCheckState(Qt::Unchecked);
    ui.checkBox_AOType->setCheckState(Qt::Unchecked);


    updateHuckelAO=false;
    ui.horizontalSlider->setRange(0,100);
    ui.horizontalSlider->setValue(10);

    ehmo=nullptr;
    hmo=nullptr;

    ui.tabWidget->setCurrentIndex(0);
    ui.tabWidget_2->setCurrentIndex(0);

}



void MainWindow::onTextEditPaste()
{

}



void MainWindow::sendMessage(QString  m_message)
{
    //ui.textEdit_MessageDisplay->append(m_message);
}



void MainWindow::clearAllViews()
{
    view3d->removeAllActors();
    view3d->updateView();
}




void MainWindow::onInformActive()
{


}










void MainWindow::closeEvent(QCloseEvent *event)
{


}


/*
void MainWindow::on_pushButton_XY_clicked()
{
    view3d->setDefaultOperationMode();
    view3d->onViewXY();
}


void MainWindow::on_pushButton_XZ_clicked()
{
    view3d->setDefaultOperationMode();
    view3d->onViewXZ();
}


void MainWindow::on_pushButton_YZ_clicked()
{
    view3d->onViewYZ();
}
*/

void MainWindow::on_pushButton_clockwise_clicked()
{
    view3d->setDefaultOperationMode();
    view3d->onView_rotateClockwise(10.0);
}

void MainWindow::on_pushButton_counterclockwise_clicked()
{
    view3d->setDefaultOperationMode();
    view3d->onView_rotateCounterClockwise(10.0);
}

void MainWindow::on_pushButton_Up_clicked()
{
    view3d->setDefaultOperationMode();
    view3d->onView_rotateUp(10.0);
}

void MainWindow::on_pushButton_Down_clicked()
{
    view3d->setDefaultOperationMode();
    view3d->onView_rotateDown(10.0);
}

void MainWindow::on_pushButton_left_clicked()
{
    view3d->setDefaultOperationMode();
    view3d->onView_rotateLeft(10.0);
}

void MainWindow::on_pushButton_right_clicked()
{
    view3d->setDefaultOperationMode();
    view3d->onView_rotateRight(10.0);
}


void MainWindow::on_actionFileNew_triggered()
{
    qDebug() << "FileNew: start";
    undoList.clear();
    redoList.clear();

    qDebug() << "FileNew: clearing mol";
    mol->clearAll();
    qDebug() << "FileNew: clearing hmo";
    if(hmo) hmo->clearAll();
    qDebug() << "FileNew: clearing ehmo";
    if(ehmo) {
        qDebug() << "FileNew: calling ehmo->clearAll()";
        ehmo->clearAll();
        qDebug() << "FileNew: ehmo->clearAll() done";
    }
    qDebug() << "FileNew: clearing view3d";
    view3d->clearAll();
    qDebug() << "FileNew: renderMol";
    view3d->renderMol();
    qDebug() << "FileNew: onViewReset";
    view3d->onViewReset();

    qDebug() << "FileNew: clearing data";
    dataMol0="";
    ui.textEdit_dataMol->setText("");

    ui.huckelEnergyView->setModel(nullptr);
    ui.huckelEnergyView->reset();
    qDebug() << "FileNew: done";
}


void MainWindow::on_actionFileOpen_triggered()
{
    QStringList filters;
    filters << tr("XYZ")+ "(*.xyz)"
            << tr("MOL") + "(*.mol)"
            << tr("PDB") + "(*.pdb)"
            << tr("CML") + "(*.cml)"
               //<< tr("CIF") + "(*.cif)"
            //<< tr("Cube")+ "(*.cub)"
               //<< tr("Vasp")+ "(*.vasp)"
            << tr("All files") + "(* *.*)";

    FileName = QFileDialog::getOpenFileName( this,
                                             tr( "Open File" ),
                                             QDir::currentPath(),
                                             filters.join(";;"));
    if (FileName.isEmpty()) return;

    on_actionFileNew_triggered();

    loadFile(FileName);
}



void MainWindow::loadFile(QString filename)
{
    if (filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    mol->clearAll();
    mol->setParent(this);
    mol->setView3D(view3d);

    mol->loadFile(filename);

    if (mol->NumAtoms()<1) {
        sendMessage("Cannot open file");
        return;
    }

    view3d->renderMol();

    push2Stack("newFile");

    isFirstTime=false;

    QString s="Load "+filename;
    sendMessage(s);
}






void MainWindow::saveFile(QString filename)
{

}





void MainWindow::on_actionFileSave2PNG_triggered()
{
    QStringList filters;
    filters << tr("Atomic Orbital Input") + "(*.orb)"
            << tr("png")+ "(*.png)"
            << tr("All files") + "(* *.*)";


    QString FileName = QFileDialog::getSaveFileName(this,
                                                    tr( "Save File" ), QDir::currentPath(), filters.join(";;"));

    if (FileName.isEmpty()) return;
    if (!FileName.endsWith("png"))  FileName+=".png";
    view3d-> writePNG(FileName);
}








void MainWindow::displayMolCoordinates(HMol * mol)
{
    uint NAtoms=mol->NumAtoms();
    if (NAtoms<1) return;

    QString single="           Atom  Coordinates";
    sendMessage(single);
    single="--------------------------------------------------";
    sendMessage(single);

    double x,y,z;
    for (uint i=0;i<NAtoms;i++) {
        QString a=mol->atomSymbol(i) + "    ";
        x=mol->atomPos(i).x();
        if(x<0.)   a+=QString::number(x,'f',4)+ " ";
        else       a+=QString::number(x,'f',4)+ " ";

        y=mol->atomPos(i).y();
        if(y<0.)   a+=QString::number(y,'f',4)+ " ";
        else       a+=QString::number(y,'f',4)+ " ";

        z=mol->atomPos(i).z();
        if(z<0.)   a+=QString::number(z,'f',4)+ " ";
        else       a+=QString::number(z,'f',4)+ " ";

        sendMessage(a);
    }
    single="--------------------------------------------------\n\n";
    sendMessage(single);
}



void MainWindow::setTemplateName(QString name)
{
    nameTMol=name;
}


void MainWindow::loadTemplateMol ()
{
    QString fileName = nameTMol+".frg";
    QString iconName = nameTMol+".xbm";

    QString path=QCoreApplication::applicationDirPath()+"/template/"+fileName;

    qDebug() << path.toStdString().c_str()<<"SFFFFFFFFFFFFFFFFF";

    view3d->loadTemplateMol(path);
    view3d->setLinkTemplateMode();
    isFirstTime=false;
}

QString MainWindow::getExecutePath ()
{
    return QCoreApplication::applicationDirPath();
}




void MainWindow::on_pushButton_clean_clicked()
{
    on_actionMMFF94_triggered();
    return;

    /*
    view3d->setDefaultOperationMode();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    mol->cleanMol();

    view3d->releaseAllActors();
    view3d->renderMol(mol);
    QApplication::restoreOverrideCursor();
    push2Stack("cleanMol");
    */
}





void MainWindow::on_pushButton_addHydrogen_clicked()
{
    if(view3d->numSelectedAtoms()<1)
    {
        QMessageBox::warning(this, "Warning","Please selected one atom!");
        return;
    }


    if (mol==nullptr) return;
    uint numAtoms=mol->NumAtoms();
    if (numAtoms<1)
    {
        QMessageBox::warning(this, "Warning","Empty molecule!");
        return;
    }

    if(mol->getAtomSymbol(view3d->getSelectedAtomId(0))=="X")     {
        QMessageBox::warning(this, "Warning","Cannot add hydrogen to X!");
        return;
    }

    if (view3d->numSelectedAtoms()<1) return;

    mol->addHydrogen(view3d->getSelectedAtomId(0));
    view3d->releaseAllActors();
    view3d->renderMol(mol);

    push2Stack("addHydrogen");
}




void MainWindow::on_pushButton_translate_clicked()
{
    if (mol==nullptr) return;

    float x,y,z, err=0.0000001;
    x=(ui.lineEdit_X->text()).toFloat();
    y=(ui.lineEdit_Y->text()).toFloat();
    z=(ui.lineEdit_Z->text()).toFloat();

    if ( abs(x) < err && abs(y) < err && abs(z) < err) {
        QMessageBox::information(0, "Warning","Please input rotation degree!");
        return;
    }

    if ( abs(x) > err && abs(y) > err) {
        QMessageBox::information(0, "Warning","Only one axis can be defined!");
        return;
    }

    if ( abs(y) > err && abs(z) > err) {
        QMessageBox::information(0, "Warning","Only one axis can be defined!");
        return;
    }

    if ( abs(x) > err && abs(z) > err) {
        QMessageBox::information(0, "Warning","Only one axis can be defined!");
        return;
    }

    mol->translate(x,y,z);

    view3d->setSelectedUpdateFromRendering(true);
    view3d->renderMol(mol);
    view3d->setSelectedUpdateFromRendering(false);

    push2Stack("translateMol");

    view3d->setDefaultOperationMode();
}






void MainWindow::on_pushButton_rotate_clicked()
{
    if (!mol) return;

    float x,y,z, err=0.01;
    x=(ui.lineEdit_X->text()).toFloat();
    y=(ui.lineEdit_Y->text()).toFloat();
    z=(ui.lineEdit_Z->text()).toFloat();

    if ( abs(x) < err && abs(y) < err && abs(z) < err) {
        QMessageBox::information(0, "Warning","Please input rotation degree!");
        return;
    }

    if ( abs(x) > err && abs(y) > err) {
        QMessageBox::information(0, "Warning","Only one axis can be defined!");
        return;
    }

    if ( abs(y) > err && abs(z) > err) {
        QMessageBox::information(0, "Warning","Only one axis can be defined!");
        return;
    }

    if ( abs(x) > err && abs(z) > err) {
        QMessageBox::information(0, "Warning","Only one axis can be defined!");
        return;
    }


    vector3 v(0.0,0.0,0.0);



    if ( abs(x) > err) {
        v.SetX(1.0);
        mol->rotate(v,x);
    }

    if ( abs(y) > err) {
        v.SetY(1.0);
        mol->rotate(v,y);
    }

    if ( abs(z) > err) {

        v.SetZ(1.0);
        mol->rotate(v,z);
    }

    view3d->setSelectedUpdateFromRendering(true);
    view3d->renderMol();
    view3d->setSelectedUpdateFromRendering(false);
    push2Stack("rotateMol");

    view3d->setDefaultOperationMode();
}


void MainWindow::on_pushButton_cleardata_clicked()
{
    ui.lineEdit_X->clear();
    ui.lineEdit_Y->clear();
    ui.lineEdit_Z->clear();
}



//store and push
void MainWindow::push2Stack (QString command)
{
    sendMessage(command);

    buildMolData ();
    dataMol = command+ "\n" +dataMol;
    undoList.push_back(dataMol);
    //cout << undoList.size()<< "     sfdsdsssd"<<endl;

    ui.textEdit_dataMol->setText(dataMol0);
    isFirstTime=false;
}


void MainWindow::push2Stack_selection (QString command)
{
    updateLastStackOne();
    push2Stack(command);
}

void MainWindow::updateLastStackOne ()
{
    dataMol = undoList.pop();
    dataMol+=buildSelectedAtomBondInfo();
    undoList.push(dataMol);
}

void MainWindow::buildMolData ()
{
    //commands=     newFile     cleanMol     runXTB     centralizeMol     addHydrogen
    //              translateMol     rotateMol     linkTemplate     newTemplate     removeSelectedAtomBond
    //              removeSelectedBonds     removeSelectedAtoms     pastefromClipboard


    if(mol==nullptr ) return;
    // cout << mol->NumAtoms() << endl;
    if (mol->NumAtoms()<1) return;

    //if(!redoList.isEmpty()) redoList.clear();

    // atoms ---------------------------------------
    // Symbol x     y    z  radius  color linked-atoms
    // C   0.0  0.0  0.0  0.3  uint32_t  1 2 3 4
    dataMol="";
    dataMol+="Atoms  "+ QString::number(mol->NumAtoms()) + "\n";

    dataMol0="";
    QString d="",s;

    for (uint i=0;i<mol->NumAtoms();i++)
    {
        s = QString("%1     %2     %3    %4\n")
            .arg(mol->atomSymbol(i))
            .arg(mol->atomPos(i).x(), 10, 'f', 6)
            .arg(mol->atomPos(i).y(), 10, 'f', 6)
            .arg(mol->atomPos(i).z(), 10, 'f', 6);
        dataMol0+=s;

        QString a=mol->atomSymbol(i) + "    "
                +QString::number(mol->atomPos(i).x(),'f',6)+ "    "
                +QString::number(mol->atomPos(i).y(),'f',6)+ "    "
                +QString::number(mol->atomPos(i).z(),'f',6)+ "    "

                + QString::number(mol->getAtombyIndex(i)->radius(),'f',2)+ "    "
                + QString::number(mol->atomList[i]->Color())+ "    ";


        QString b="";
        for (uint j=0;j<mol->getAtombyIndex(i)->numLinkdedAtoms();j++)
            b+=QString::number(mol->getAtombyIndex(i)->getLinkdedAtomId(j))+ " ";
        b+="\n";
        d+=a;
        d+=b;
    }
    dataMol += d;


    //bonds
    QString title="Bonds  "+ QString::number(mol->NumBonds()) + "\n";
    dataMol+=title;
    d="";

    // id-beg id-end  order  plane <x  y  z>=VZero  <idRing>=100001
    for (uint i=0;i<mol->NumBonds();i++ )
    {
        QString a="",b="",c="";
        a=QString::number(mol->getBondbyIndex(i)->atomIndex0()) + " "
                +QString::number(mol->getBondbyIndex(i)->atomIndex1())+ " "
                +QString::number(mol->getBondbyIndex(i)->getBondOrder())+ " ";

        vector3 norm=VZero;
        if (mol->getBondbyIndex(i)->getPlane().length()>0.1)
            norm=mol->getBondbyIndex(i)->getPlane();


        b=QString::number(mol->getBondbyIndex(i)->getPlane().x(),'f',2)+ " "
                +QString::number(mol->getBondbyIndex(i)->getPlane().y(),'f',2)+ " "
                +QString::number(mol->getBondbyIndex(i)->getPlane().z(),'f',2) +" ";

        c=a+b; d+=c;

        //id ring?
        uint idRing=100001;
        if(mol->getBondbyIndex(i)->isInsideRing())
            idRing=mol->getBondbyIndex(i)->getRingId();
        d+=QString::number(idRing)+ "\n";
    }
    dataMol+=d;



    //rings data stored:
    //size  < id of atoms> , <center>     < normal >,       <radius> ,  scale, color  ,  isAromatic
    //6 :  1 0 5 4 3 2 , -1.06 -0.62 0.00 , 0.00 -0.00 1.00, 1.40 ,  234234234, 0.5,       1
    title="Rings  "+ QString::number(mol->numRings()) + "\n";
    dataMol+=title;
    d="";
    for (uint i=0;i<mol->numRings();i++)
    {
        QString a="",b="";
        uint size=mol->getRingbyId(i)->atomIdList.size();
        a=QString::number(size)+ " :  ";

        for (uint j=0;j<size;j++)
            b+=QString::number(mol->getRingbyId(i)->atomIdList[j])+ " ";
        a+=b;

        //center
        vector3 v=mol->getRingbyId(i)->center;
        QString center = QString::number(v.x(),'f',2)+ " "
                + QString::number(v.y(),'f',2)+ " "
                + QString::number(v.z(),'f',2)+ ", ";
        a +=  center;

        //normal
        v=mol->getRingbyId(i)->norm;
        QString norm = QString::number(v.x(),'f',2)+ " "
                + QString::number(v.y(),'f',2)+ " "
                + QString::number(v.z(),'f',2)+ ", ";
        a += norm;


        //radius
        QString radius = QString::number(mol->getRingbyId(i)->radius,'f',2)+ ", ";
        a += radius;


        //scale
        QString scale = QString::number(mol->getRingbyId(i)->scale,'f',2)+ ", ";
        a += scale;

        //color
        QString color= QString::number(mol->getRingbyId(i)->color.x(),'f',2)+ " "
                + QString::number(mol->getRingbyId(i)->color.y(),'f',2)+ " "
                + QString::number(mol->getRingbyId(i)->color.z(),'f',2)+ ", ";
        a += color;

        //aromatic?
        if (mol->getRingbyId(i)->isAromatic)
            a+=QString::number(1) + "\n";
        else
            a+=QString::number(0)+ "\n";

        d+=a;
    }
    dataMol+=d;

    //cout << "\n\nsaved data:"<<endl;
    //cout << dataMol.toStdString().c_str()<<endl;
    //cout << "\n\n";
}



//help function
QString  MainWindow::buildSelectedAtomBondInfo ()
{
    QString All="";

    //selected Atoms
    uint nAtoms=view3d->numSelectedAtoms();
    if(nAtoms>1)
    {
        QString title="SelectedAtoms  "+ QString::number(nAtoms) + "\n";
        QString atomsList="";
        for (uint i=0;i<view3d->numSelectedAtoms();i++)
            atomsList+=QString::number(view3d->getSelectedAtomId(i)) + " ";
        atomsList=title+ atomsList+ " \n";
        All+=atomsList;
    }

    //selected Bonds
    uint nBonds=view3d->numSelectedBonds();
    if(nBonds>1)
    {
        QString bondsList="";
        QString title="SelectedBonds  "+ QString::number(nBonds) + "\n";
        for (uint i=0;i<view3d->numSelectedBonds();i++) {
            uint ibeg=0,iend=0;
            view3d->getSelectedBondIds(i,ibeg,iend);

            //only the index is stored
            bondsList += QString::number(i) + "  ";
        }
        bondsList=title+ bondsList+ " \n";
        All+=bondsList;
    }

    return All;
}

void MainWindow::recoverStackData (QString & All)
{
    view3d->releaseAllActors();
    mol->clearAll();

    QStringList Lines = All.split("\n");
    if (Lines.size()<1) return;

    QString single;
    QStringList ls;
    uint nAtoms=0,nBonds=0,nRings=0;

    QString command=Lines.at(0);

    cout << "recover :  "<<endl;
    cout << All.toStdString().c_str()<<endl;

    vector <uint > selAtoms,selBonds;
    for (uint i=1; i < Lines.size(); i++)
    {
        single = Lines.at(i);

        uint nAtoms0=0;
        // atoms ---------------------------------------
        // Symbol x     y    z   radius  color neighbors;
        // C   0.0  0.0  0.0  0.3  uint32_t  1 2 3 4
         if (single.contains("Atoms") )
         {
             ls = single.trimmed().split(RE_WHITESPACE);
            nAtoms=ls[1].toInt();

            if(nAtoms<1) return;
            nAtoms0=nAtoms;

            dataMol0="";
            QString s;
            double x,y,z;

            while (nAtoms--)
            {
                i=i+1;
                single = Lines.at(i);
                //single.replace(":", " ");
                //cout <<single .toStdString().c_str()<< "    sssssssssssss"<< endl;
                ls = single.trimmed().split(RE_WHITESPACE);

                x=ls[1].toDouble();  y=ls[2].toDouble();  z=ls[3].toDouble();

                s = QString("%1     %2     %3    %4\n")
                    .arg(ls[0])
                    .arg(x, 10, 'f', 6)
                    .arg(y, 10, 'f', 6)
                    .arg(z, 10, 'f', 6);
                dataMol0+=s;

                mol->addAtom(ls[0],x,y,z);

                //radius and color
                mol->getLastAtom()->setRadius(ls[4].toDouble());
                mol->getLastAtom()->setColor(ls[5].toULong());

                //cout << ls[0].toStdString().c_str() <<" "<<ls[1].toDouble() <<" "<< ls[2].toDouble()<<" "<<ls[3].toDouble() <<" ";
                //cout << ls[4].toDouble() << " "<<ls[5].toULong() ;

                if(ls.size()>6)
                    for(uint k=6;k<ls.size();k++)
                        mol->getLastAtom()->appendNeighbor(ls[k].toInt());
            }

            if(nAtoms0<2) {
                view3d->renderMol(mol);
                return;
            }

            continue;
        }

        //beg end order plane ringId
        uint nBonds0=0;
         if (single.contains("Bonds"))
         {
             ls = single.trimmed().split(RE_WHITESPACE);
            nBonds=ls[1].toInt();
            if(nBonds<1) return;

            nBonds0=nBonds;

            //bond
            //cout << "recovered bond infor:"<<endl;
            while (nBonds--)
            {
                i=i+1;
                single = Lines.at(i);
                //cout <<single .toStdString().c_str()<<endl;
                ls = single.trimmed().split(RE_WHITESPACE);
                mol->addBond(ls[0].toInt(),ls[1].toInt(),ls[2].toInt());
                mol->getLastBond()->setPlane(ls[3].toDouble(),ls[4].toDouble(),ls[5].toDouble());
                mol->getLastBond()->setRingId(ls[6].toInt());
            }

            if(nBonds0<2) {
                view3d->renderMol(mol);
                return;
            }

            continue;
        }

        //rings data stored:
        //size  < id of atoms> , <center>     < normal >,       <radius> ,  scale,  color  ,  isAromatic
        //6 :  1 0 5 4 3 2 , -1.06 -0.62 0.00 , 0.00 -0.00 1.00, 1.40 ,   0.00 -0.00 1.00 , 0.5,       1
        if (single.contains("Rings"))
        {
            ls = single.trimmed().split(RE_WHITESPACE);
            nRings=ls[1].toInt();

            vector <uint > idList;
            while (nRings--)
            {
                i=i+1;
                single = Lines.at(i);
                single.replace(",", " ");
                single.replace(":", " ");

                ls = single.trimmed().split(RE_WHITESPACE);

                idList.clear();
                uint size=ls[0].toInt();
                for(uint j=0;j<size;j++)
                    idList.push_back(ls[j+1].toInt());

                vector3 ringCenter(ls[size+1].toDouble(),
                        ls[size+2].toDouble(),
                        ls[size+3].toDouble());

                vector3 ringNormal(ls[size+4].toDouble(),
                        ls[size+5].toDouble(),
                        ls[size+6].toDouble());


                double ringRadius=ls[size+7].toDouble();
                double ringScale=ls[size+8].toDouble();

                vector3 color(ls[size+9].toDouble(), ls[size+10].toDouble(),ls[size+11].toDouble());


                uint   ringAromatic=ls[size+12].toInt();

                mol->addRing (idList, ringCenter, ringNormal, ringRadius, ringScale, color, ringAromatic);
            }
        }

        /*
        if (single.contains("SelectedAtoms"))
        {
            ls = single.trimmed().split(RE_WHITESPACE);
            nAtoms=ls[1].toInt();

            if(nAtoms<1) continue;
            while (nAtoms--)
            {
                i=i+1;
                single = Lines.at(i).simplified();
                if (single.size()<1) continue;
                ls = single.trimmed().split(RE_WHITESPACE);

                vector <uint > idList;

                for(uint j=0;j<ls.size();j++) {
                    idList.push_back(ls[j].toInt());
                    //cout <<ls[j].toInt() <<endl;
                }
            }
        }


        if (single.contains("SelectedBonds"))
        {
            ls = single.trimmed().split(RE_WHITESPACE);
            nBonds=ls[1].toInt();

            if(nBonds<1) continue;

            while (nBonds--)
            {
                i=i+1;
                single = Lines.at(i);
                //single.replace(":", " ");
                ls = single.trimmed().split(RE_WHITESPACE);

                vector <uint > idList;

                for(uint j=0;j<ls.size();j++) {
                    idList.push_back(ls[j].toInt());
                    //cout <<ls[j].toInt() <<endl;
                }
            }
        }*/

    }

    view3d->renderMol(mol);
    //view3d->setSelectedUpdateFromRendering(false);
}


void MainWindow::on_dialAtomScale_valueChanged(int value)
{
    view3d->setAtomScale(double (ui.dialAtomScale->value())/200.0);
    if(isFirstTime) return;
    view3d->renderMol();
}

void MainWindow::on_dialBondScale_valueChanged(int value)
{
    view3d->setBondScale(double (ui.dialBondScale->value())/200.0);
    if(isFirstTime) return;
    view3d->renderMol();
}


void MainWindow::on_actionSave_Png_triggered()
{

    QStringList filters;
    filters << tr("png")+ "(*.png)"
            << tr("All files") + "(* *.*)";

    QString FileNames = QFileDialog::getSaveFileName(this,
                                                     tr( "Save File" ), QDir::currentPath(), filters.join(";;"));

    if (FileNames.isEmpty()) return;
    view3d-> writePNG(FileNames);
    view3d->updateView();
}





void MainWindow::on_actionReset_triggered()
{
    if(hmo) {
        if (hmo->isSurfaceOK()) {
            view3d->onViewReset();
            return;
        }
    }
    if(ehmo) {
        if (ehmo->isSurfaceOK()) {
            view3d->onViewReset();
            return;
        }
    }

    //please percept symmetry and put the axis of plane molecule along z-axis
    {
        msym_context ctx = msymCreateContext();
        if (!ctx) {
            std::cerr << "Error: Failed to create libmsym context" << std::endl;
            return ;
        }

        size_t nAtoms = mol->NumAtoms();
        vector<msym_element_t> elements(nAtoms);
        for (size_t i = 0; i < nAtoms; i++) {
            vector3 pos = mol->atomPos(i);
            elements[i].id = reinterpret_cast<void*>(static_cast<size_t>(i));
            elements[i].m = mol->atomicMass(i);
            elements[i].v[0] = pos.x();
            elements[i].v[1] = pos.y();
            elements[i].v[2] = pos.z();
            elements[i].n = mol->atomicNum(i);
            string sym = mol->atomSymbol(i).toStdString();
            strncpy(elements[i].name, sym.c_str(), 3);
            elements[i].name[3] = '\0';
        }

        if (msymSetElements(ctx, nAtoms, elements.data()) != 0) {
            cerr << "Error: msymSetElements failed" << endl;
            msymReleaseContext(ctx);
            return ;
        }

        if (msymFindSymmetry(ctx) != 0) {
            cerr << "Error: msymFindSymmetry failed" << endl;
            msymReleaseContext(ctx);
            return ;
        }

        char pg_name[16];
        msymGetPointGroupName(ctx, 16, pg_name);
        auto symPointGroup = QString(pg_name);
        mol->setPointGroup(symPointGroup);
        cout << "Point group: " << symPointGroup.toStdString() << endl;

        // Get principal axes and align so highest moment axis is along z
        double eigvec[3][3];
        if (msymGetPrincipalAxes(ctx, eigvec) == 0) {
            // eigvec columns are the principal axes, sorted by moment (largest first)
            // We want the highest symmetry axis (largest moment) along z-axis (0,0,1)
            // The rotation matrix to map eigvec to z-axis is: R = [eigvec | z] but we need inverse
            // For column vectors, the rotation to align first eigenvector to z is:
            // R = [ex, ey, ez] where ex, ey, ez are eigenvectors in columns

            // Build rotation matrix (transpose of eigvec since eigvec columns are axes)
            // R maps from standard frame to molecule frame
            // We need R such that R * z_axis = highest_symmetry_axis
            // Since eigvec column 0 is highest moment axis, we rotate to align it with z

            // Get the rotation matrix that aligns principal axes to standard (z is 3rd column)
            // The transform matrix from msym has principal axes as columns
            // We need to transpose to get rotation from standard to principal

            // Find which eigvec corresponds to highest symmetry axis (usually z for planar mol)
            // For planar molecules, z (2nd axis) is usually highest symmetry
            // But let's just put largest moment axis along z

            // Create rotation matrix: columns are the target directions for x,y,z axes
            // We want: R * (0,0,1) = eigvec[:,0] (largest moment axis)
            // R * (1,0,0) = eigvec[:,1], R * (0,1,0) = eigvec[:,2]
            // So R = [eigvec[:,1], eigvec[:,2], eigvec[:,0]] (columns are new axis directions)

            double R[3][3];
            for (int j = 0; j < 3; j++) {
                for (int i = 0; i < 3; i++) {
                    R[i][j] = eigvec[i][j];  // column j = eigvec[:,j]
                }
            }

            // Apply rotation to all atoms: new_pos = R * old_pos
            vector<vector3> new_positions(nAtoms);
            for (size_t i = 0; i < nAtoms; i++) {
                vector3 pos = mol->atomPos(i);
                new_positions[i].Set(
                    R[0][0]*pos.x() + R[0][1]*pos.y() + R[0][2]*pos.z(),
                    R[1][0]*pos.x() + R[1][1]*pos.y() + R[1][2]*pos.z(),
                    R[2][0]*pos.x() + R[2][1]*pos.y() + R[2][2]*pos.z()
                );
            }

            // Update atom positions
            for (size_t i = 0; i < nAtoms; i++) {
                mol->setAtomPos(static_cast<uint>(i), new_positions[i]);
            }

            mol->updateRingPositions();

            cout << "Aligned principal axis to z-axis" << endl;
        } else {
            // Fallback: just use msymAlignAxes
            msymAlignAxes(ctx);

            // Get updated coordinates from elements
            for (size_t i = 0; i < nAtoms; i++) {
                mol->setAtomPos(static_cast<uint>(i),
                    elements[i].v[0], elements[i].v[1], elements[i].v[2]);
            }
            mol->updateRingPositions();
            cout << "Aligned using msymAlignAxes" << endl;
        }

        msymReleaseContext(ctx);
    }


    mol->orient2PlaneXOY();
    push2Stack("Rotate");


    view3d->renderMol(mol);
    view3d->onViewReset();
}



void MainWindow::on_actionEditUndo_triggered()
{
    if(undoList.size()<1)
        return;

    //cout << "Undo :"<<endl;
    //cout << "size of undo and redo " <<undoList.size() << " " << redoList.size()<<endl;

    QString All=undoList.pop();
    redoList.push_back(All);

    cout << "Recoved data:\n";
    cout << All.toStdString().c_str();
    cout << undoList.size() <<endl;

    if(undoList.size()>0)
        All=undoList.last();
    else {
        mol->clearAll();
        view3d->renderMol(mol);
        return;
    }
    recoverStackData(All);
    ui.textEdit_dataMol->setText(dataMol0);

    cout << mol->NumAtoms()<<endl;
}

void MainWindow::on_actionEditRedo_triggered()
{
    if(redoList.size()<1)
        return;

    QString All=redoList.pop();
    undoList.push_back(All);

    recoverStackData(All);
}


void MainWindow::on_actionToolscentralize_triggered()
{
    view3d->setDefaultOperationMode();

    if (mol==nullptr) return;
    if (mol->NumAtoms()>1)
    {
        mol->centeralize();

        view3d->renderMol(mol);
        push2Stack("centralizeMol");
    }
}


void MainWindow::on_actionFileExport_triggered()
{
    if(mol->NumAtoms()<1) return;

    QStringList filters;
    filters << tr("XYZ") + " (*.xyz)"
            << tr("MDL Mol") + " (*.mol)"
            << tr("PDB") + " (*.pdb)"
            << tr("Sybyl Mol2") + " (*.mol2)";

    QString currentSuffix= QFileInfo(FileName).suffix();
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr( "Save File" ), QDir::currentPath(), filters.join(";;"));
    if(currentSuffix.size()<1)
        currentSuffix="xyz";

    if(fileName.isEmpty())  return;

    OpenBabel::OBConversion conv;
    OpenBabel::OBFormat *format=conv.FindFormat(QFileInfo(fileName).suffix ().toStdString().c_str());
    QString fileType =format->GetID();

    if(mol->writeMol(fileName,fileType))
    {
        return;
    }
    else {
        QMessageBox::information(0, "Warning","Cannot export moleclue!");
    }

}



void MainWindow::on_actionEditbond_triggered()
{
    if( (view3d->numSelectedAtoms()==2) || (view3d->numSelectedBonds()==1) )
    {

        bondProperty * form = new bondProperty ();
        if (form!=nullptr) {
            form->setParent(this);
            form->setAttribute( Qt::WA_DeleteOnClose, true );
            form->exec();
        }
    }

    //view3d->setDefaultOperationMode();
}





void MainWindow::on_actionSingleBond_triggered()
{
    if(view3d->numSelectedBonds()<1) return;
    view3d->setSelectedBondOrder(1);
    view3d->refresh();
}

void MainWindow::on_actionDoubleBond_triggered()
{
    if(view3d->numSelectedBonds()>0)
    {
        view3d->setSelectedBondOrder(2);
        view3d->refresh();
    }

}

void MainWindow::on_actionTripleBond_triggered()
{
    if(view3d->numSelectedBonds()>0) {
        view3d->setSelectedBondOrder(3);
        view3d->refresh();
    }
}

void MainWindow::on_actionQuadrupleBond_triggered()
{
    if(view3d->numSelectedBonds()<1) return;
    view3d->setSelectedBondOrder(4);
    view3d->refresh();

}

void MainWindow::on_actionWeakBond_triggered()
{
    if(view3d->numSelectedBonds()<1) return;
    view3d->setSelectedBondOrder(7);
    view3d->refresh();
}

void MainWindow::on_actionHydrogenbond_triggered()
{
    if(view3d->numSelectedBonds()<1) return;
    view3d->setSelectedBondOrder(5);
    view3d->refresh();
}

void MainWindow::on_actionAromaticBond_triggered()
{
    if(view3d->numSelectedBonds()<1) return;
    view3d->setSelectedBondOrder(6);
    view3d->updateMol();
}

void MainWindow::on_actionToolsAddHydrogenAuto_triggered()
{
    view3d->setDefaultOperationMode();
    if(view3d->numSelectedAtoms()<1) {
        QMessageBox::warning(this, "Warning","请选择原子!");
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);


    for(uint i=0;i<view3d->selectedAtomList.size();i++)
    {
        mol->autoAddHydrogen(view3d->selectedAtomList[i]);
    }



    view3d->releaseAllActors();
    view3d->renderMol(mol);
    QApplication::restoreOverrideCursor();
}


void MainWindow::on_actionMMFF94_triggered()
{
    view3d->setDefaultOperationMode();

    if(mol->isXExisted()) {
        QMessageBox::warning(this, "Warning","请删除X原子!");
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    mol->runMolecularMechanics("MMFF94");

    view3d->releaseAllActors();
    view3d->renderMol(mol);
    QApplication::restoreOverrideCursor();
    push2Stack("cleanMol");
}


void MainWindow::on_actionGhemical_triggered()
{
    //view3d->setDefaultOperationMode();
    if(mol->isXExisted()) {
        QMessageBox::warning(this, "Warning","请删除X原子!");
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    mol->runMolecularMechanics("Ghemical");

    view3d->releaseAllActors2();

    view3d->renderMol(mol);
    QApplication::restoreOverrideCursor();
    push2Stack("cleanMol");
}


void MainWindow::on_actionUFF_triggered()
{
    //view3d->setDefaultOperationMode();
    if(mol->isXExisted()) {
        QMessageBox::warning(this, "Warning","请删除X原子!");
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    mol->runMolecularMechanics("UFF");

    view3d->releaseAllActors2();
    view3d->renderMol(mol);
    QApplication::restoreOverrideCursor();
    push2Stack("cleanMol");
}

void MainWindow::on_actionMM2_triggered()
{
    //view3d->setDefaultOperationMode();
    if(mol->isXExisted()) {
        QMessageBox::warning(this, "Warning","请删除X原子!");
        return;
    }
    return;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    mol->runMolecularMechanics("MM2");

    view3d->releaseAllActors2();
    view3d->renderMol(mol);
    QApplication::restoreOverrideCursor();
    push2Stack("cleanMol");
}

void MainWindow::on_actionGaff_triggered()
{
    //view3d->setDefaultOperationMode();
    if(mol->isXExisted()) {
        QMessageBox::warning(this, "Warning","请删除X原子!");
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    mol->runMolecularMechanics("GAFF");

    view3d->releaseAllActors2();
    view3d->renderMol(mol);
    QApplication::restoreOverrideCursor();
    push2Stack("cleanMol");
}



void MainWindow::on_actionreCalculateBonds_triggered()
{
    on_pushButton_rebond_clicked();
}

void MainWindow::on_actionPeriodicTable_triggered()
{
    PeriodicTable * pt = new PeriodicTable();
    pt->setView3d(view3d);
    pt->show();
}

void MainWindow::on_pushButton_rebond_clicked()
{
    mol->reCalculateBonds();
    view3d->renderMol();
}

void MainWindow::on_pushButton_UFF_clicked()
{
    on_actionUFF_triggered();
}

void MainWindow::on_pushButton_Ghemical_clicked()
{
    on_actionGhemical_triggered();
}




void MainWindow::on_actionXTB_optimize_triggered()
{
    view3d->setDefaultOperationMode();

    uint numAtoms=mol->NumAtoms();
    if (numAtoms<1)
    {
        QMessageBox::warning(this, "Warning","Empty molecue!");
        return;
    }

    if(mol->isXExisted()) {
        QMessageBox::warning(this, "Warning","请删除X原子!");
        return;
    }

    auto form = new HuckelTextForm ("XTB calculation",nullptr);
    form->setWindowTitle("XTB-Grimme");

    if (form!=nullptr)
    {
        form->setMinimumWidth(1000);
        form->update();
        form->setCalXTB();
        form->setParent(this);
        form->setAttribute( Qt::WA_DeleteOnClose, true );
        form->exec();
    }
    push2Stack("running XTB");

    view3d->releaseAllActors();
    view3d->setDefaultOperationMode();
}

void MainWindow::on_pushButton_GAFF_clicked()
{
    on_actionGaff_triggered();
}

void MainWindow::on_pushButton_Atom_clicked()
{
    //if(view3d->numSelectedAtoms()<1) return;

    atomProperty * form = new atomProperty (nullptr);
    if (form!=nullptr) {
        form->setParent(this);
        form->setAttribute( Qt::WA_DeleteOnClose, true );
        form->exec();
    }
}




void MainWindow::on_pushButton_Ring_clicked()
{
    if(view3d->numSelectedRings()!=1) return;

    ringProperty * form = new ringProperty (nullptr);
    if (form!=nullptr) {
        form->setParent(this);
        form->setAttribute( Qt::WA_DeleteOnClose, true );
        form->exec();
    }
}



void MainWindow::on_actionToolsPencil_triggered()
{
    view3d->setPencilDrawMode();
    view3d->removeDynamicLineActor();
    view3d->releaseAllActors();
}

void MainWindow::on_actionEditCopy_triggered()
{
    view3d->setDefaultOperationMode();
    view3d->copySeleted();
    //view3d->releaseAllActors();
}

void MainWindow::on_actionToolsRotate_triggered()
{
    view3d->setDefaultOperationMode();
    view3d->releaseAllActors();
    view3d->updateView();
}

void MainWindow::on_actionEditCut_triggered()
{
    view3d->cutSelected();
}
void MainWindow::on_actionEditPaste_triggered()
{
    view3d->paste();
}

void MainWindow::on_actionxyz_triggered()
{
    view3d->setXYZAxisVisible(!view3d->isXYZAxisVisible());
    //view3d->updateView();
    view3d->renderMol();
}



void MainWindow::on_actionXOY_triggered()
{
    view3d->setDefaultOperationMode();
    view3d->onViewXY();
}

void MainWindow::on_actionXOZ_triggered()
{
    view3d->setDefaultOperationMode();
    view3d->onViewXZ();
}

void MainWindow::on_actionYOZ_triggered()
{
    view3d->onViewYZ();
}

void MainWindow::on_actionEditangle_triggered()
{
    uint num=view3d->numSelectedAtoms();
    if(num!=3) return;

    AngleProperty * form = new AngleProperty ();
    if (form!=nullptr) {
        form->setParent(this);
        form->setAttribute( Qt::WA_DeleteOnClose, true );
        form->exec();
    }

    //view3d->setDefaultOperationMode();
}

void MainWindow::on_actionEditdihedral_triggered()
{
    // view3d->setDefaultOperationMode();

    uint num=view3d->numSelectedAtoms();
    if(num!=4) return;

    TorsionProperty * form = new TorsionProperty ();
    if (form!=nullptr) {
        form->setParent(this);
        form->setAttribute( Qt::WA_DeleteOnClose, true );
        form->exec();
    }

    //view3d->setDefaultOperationMode();
}





void MainWindow::on_pushButton_templateMol_clicked()
{
    auto form = new RingTemplate (this);
    form->setParent(this);

    if (form!=nullptr) {
        form->setAttribute( Qt::WA_DeleteOnClose, true );
        form->setParent(this);
        form->exec();
    }

    loadTemplateMol ();
}


void MainWindow::on_pushButton_hedral_clicked()
{
    on_actionEditdihedral_triggered();
}


void MainWindow::on_pushButton_angle_clicked()
{
    on_actionEditangle_triggered();
}

void MainWindow::on_pushButton_bond_clicked()
{
    on_actionEditbond_triggered();
}



void MainWindow::on_pushButton_pencil_clicked()
{
    view3d->setPencilDrawMode();
    view3d->removeDynamicLineActor();
    view3d->releaseAllActors();
}

void MainWindow::on_checkBox_kekule_stateChanged(int arg1)
{
    view3d->setKekuleStyle(arg1);
    view3d->setSelectedUpdateFromRendering(true);
    view3d->updateMol();
    view3d->setSelectedUpdateFromRendering(false);
}


void MainWindow::on_pushButton_calcHuckel_clicked()
{
    isHMO=true;
    isEHMO=false;
    if (mol==nullptr) return;

    view3d->releaseAllActors();
    view3d->setDefaultOperationMode();

    uint numAtoms=mol->NumAtoms();
    if (numAtoms<1)
    {
        QMessageBox::warning(this, "Warning","Empty molecue!");
        return;
    }

    //percept the shape of the molecule
    //mol->perceptMoleculeShape();
    //QString shape=mol->getMolShape();

    //standard orientation
    on_actionReset_triggered();


    //make a detailed check here
    //can tackel pure carbon system
    //cout << shape.toStdString()<<endl;
    //if(shape!="Plane" && shape!="Polygon" && shape!="Sphere") {
    //    QMessageBox::warning(this, "Warning","Huckel方法只能处理平面分子！");
    //    return;
    //}

    if(ehmo!=nullptr)
        ehmo->clearAll();

    if(hmo == nullptr)
        hmo = new HMO();
    else
        hmo->clearAll();


    if(hmo==nullptr) {
        QMessageBox::warning(this, "Warning","HMO cannot initialized!");
        return;
    }

    view3d->setHMO(hmo);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    view3d->updateMol();
    auto form = new HuckelTextForm ();
    form->setWindowTitle("Simple-HMO method");

    if (form==nullptr) return;
    form->setCalHuckel();
    form->setParent(this);
    form->setAttribute( Qt::WA_DeleteOnClose, true );
    QApplication::restoreOverrideCursor();

    form->exec();

    ui.checkBox_AOType->setEnabled(true);
    ui.checkBox_MOType->setEnabled(true);
    ui.tabWidget_2->setTabText(0,"HMO");
}



void MainWindow::on_pushButton_calcExtendedHuckel_clicked()
{
    if (mol==nullptr) {
        qDebug() << "DEBUG: mol is nullptr, returning";
        return;
    }

    uint numAtoms=mol->NumAtoms();

    if (numAtoms<1)
    {
        QMessageBox::warning(this, "Warning","Empty molecule!");
        return;
    }

    isEHMO=true;
    isHMO=false;
    view3d->releaseAllActors();
    view3d->setDefaultOperationMode();

    if(hmo!=nullptr)
        hmo->clearAll();

    if(ehmo != nullptr) {
        ehmo->clearAll();
    }
    else {
        ehmo = new EHMO();
    }


    if(ehmo==nullptr) {
        QMessageBox::warning(this, "Warning","EHMO cannot initialized!");
        return;
    }


    QApplication::setOverrideCursor(Qt::WaitCursor);
    view3d->updateMol();
    view3d->setEHMO(ehmo);

    auto form = new HuckelTextForm ();
    form->setWindowTitle("Extend-HMO method");


    if (form==nullptr) return;
    form->setCalExtendedHuckel();
    form->setParent(this);
    form->setAttribute( Qt::WA_DeleteOnClose, true );

    QApplication::restoreOverrideCursor();

    form->exec();

    ui.checkBox_AOType->setDisabled(true);
    ui.checkBox_MOType->setDisabled(true);
    ui.tabWidget_2->setTabText(1,"扩展HMO");
}


void MainWindow::on_checkBox_posLobe_stateChanged(int arg1)
{
    if  (arg1>0)
        view3d->setPosLobeVisible();
    else
        view3d->setPosLobeInvisible();

    view3d->renderMol(mol);
}

void MainWindow::on_checkBox_negLobe_stateChanged(int arg1)
{
    if  (arg1>0)
        view3d->setNegLobeVisible();
    else
        view3d->setNegLobeInvisible();

    view3d->renderMol(mol);
}



void MainWindow::on_checkBox_mesh_stateChanged(int arg1)
{
    if  (ui.checkBox_mesh->checkState()){
        ui.checkBox_surface->setCheckState(Qt::Unchecked);
        view3d->setLobeMesh();
    }
    else {
        ui.checkBox_surface->setCheckState(Qt::Checked);
        view3d->setLobeSmooth();
    }
    view3d->renderMol(mol);
}



void MainWindow::on_checkBox_surface_stateChanged(int arg1)
{
    if  (ui.checkBox_surface->checkState()){
        ui.checkBox_mesh->setCheckState(Qt::Unchecked);
        view3d->setLobeSmooth();
    }
    else {
        ui.checkBox_mesh->setCheckState(Qt::Checked);
        view3d->setLobeMesh();
    }

    view3d->renderMol(mol);
}


void MainWindow::on_checkBox_AOType_stateChanged(int arg1)
{
    if (ui.checkBox_AOType->checkState()){
        ui.checkBox_MOType->setCheckState(Qt::Unchecked);
        view3d->setAOType(true);
    }
    else {
        ui.checkBox_MOType->setCheckState(Qt::Checked);
        view3d->setAOType(false);
    }
    view3d->renderMol(mol);
}

void MainWindow::on_checkBox_MOType_stateChanged(int arg1)
{
    if (ui.checkBox_MOType->checkState()){
        ui.checkBox_AOType->setCheckState(Qt::Unchecked);
        view3d->setAOType(false);
    }
    else {
        ui.checkBox_AOType->setCheckState(Qt::Checked);
        view3d->setAOType(true);
    }
    view3d->renderMol(mol);
}


void MainWindow::on_checkBox_hideNode_stateChanged(int arg1)
{
    if  (arg1>0)
        view3d->setNodeVisible();
    else
        view3d->setNodeInvisible();

    view3d->renderMol(mol);
}


void MainWindow::on_checkBox_hideHuckelOrbital_stateChanged(int arg1)
{
    if  (arg1>0)
        view3d->setMOInvisible();
    else
        view3d->setMOVisible();

    view3d->renderMol(mol);
}



void MainWindow::on_huckelEnergyView_clicked(const QModelIndex &index)
{
    if (mol==nullptr) return;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    if(isHMO)  {
        scaleAO = hmo->buildHuckelMO(index.row());
        updateHuckelAO=false;
        ui.horizontalSlider->setValue(round(scaleAO*10));
    }
    else if(isEHMO)   {
        ehmo->buildExtendedHuckelMO(index.row());
    }

    view3d->renderMol();

    QColor calculatedColor(100, 200, 100);
    QFont calculatedFont = ui.huckelEnergyView->font();
    calculatedFont.setBold(true);
    auto model = qobject_cast<QStandardItemModel*>(ui.huckelEnergyView->model());
    if (model && index.row() < model->rowCount()) {
        auto item = model->item(index.row());
        item->setForeground(calculatedColor);
        item->setFont(calculatedFont);
    }

    updateHuckelAO=true;
    QApplication::restoreOverrideCursor();
}



void MainWindow::updateExtendedHuckelMOs()
{
    uint nCount=ehmo->huckelEigValues.size();

    QStringList strList;
    QString str;

    auto model = new QStandardItemModel(this);
    ui.huckelEnergyView->setModel(model);
    model->setColumnCount(1);

    // Beautiful color for calculated orbitals
    //QColor calculatedColor(100, 200, 100);
    QFont calculatedFont = ui.huckelEnergyView->font();
    calculatedFont.setBold(true);

    for(uint  i = 0; i < nCount; ++i)
    {
        double d=ehmo->huckelEigValues[i];
        str = QString("%1").arg(d, 10, 'f', 4);

        if(d > 0.0) str="+"+str;

        // Append irrep label if available
        if (i < (uint)ehmo->moIrrepLabels.size() && ehmo->moIrrepLabels[i].size() > 0)
            str += "-" + ehmo->moIrrepLabels[i];

        str.remove(RE_WHITESPACE);

        auto item = new QStandardItem(str);
        model->appendRow(item);
    }


    ui.huckelEnergyView->setModel(model);
    ui.huckelEnergyView->resizeColumnsToContents();
}


void MainWindow::updateHuckelMOs()
{
    isEHMO=isXTB=false;
    isHMO=true;

    unsigned nCount=hmo->huckelEigValues.size();

    QStringList strList;
    QString str,str_;

    auto model = new QStandardItemModel(this);
    ui.huckelEnergyView->setModel(model);
    model->setColumnCount(1);


    QChar  alpha=QChar(0xb1,0x03);
    QChar  beta=QChar(0xb2,0x03);


    QChar pair=QChar(0x21F5);
    QChar spinUp=QChar(0x2191);
    int numElectrons;

    for(auto i = 0; i < nCount; i++)
    {
        double d=hmo->huckelEigValues[i];
        double dn=hmo->huckelEigValues[i+1];
        double dp;

        str = QString("%1").arg(d, 8, 'f', 4);

        if(d > 0.0) str="+"+str;

        numElectrons=hmo->numElectrons()-2*(nCount-1-i);

        if(abs(d-dn)<0.01 && numElectrons==0 )
            numElectrons=1;
        if(i>0 ){
            dp=hmo->huckelEigValues[i-1];
            if(abs(d-dp)<0.01 && numElectrons==2)
            numElectrons=1;
        }
        if(numElectrons<0)
            numElectrons=0;

        str_="(";
        if(numElectrons==0) {
            str_ += "--";
        }
        if(numElectrons>1) {
            str_ += pair;
        }
        if(numElectrons==1) {
            str_ += spinUp;
        }        
        str_+=")";

        //zero
        if(fabs(d) <0.001) str=alpha;
        else  str=alpha+str+beta;

        str+=str_;

        str.remove(RE_WHITESPACE);

        auto item = new QStandardItem(str);
        model->appendRow(item);
    }
    cout <<"end"<<endl;

    ui.huckelEnergyView->setModel(model);
    ui.huckelEnergyView->resizeColumnsToContents();
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if(!updateHuckelAO) return;

    if (hmo==nullptr) return;

    hmo->reBuildHuckelAO(value/10.0);
    view3d->renderMol(mol);
}



void MainWindow::on_pushButton_colorOrbital_clicked()
{
    OrbitalProperty * form = new OrbitalProperty (this);
    if (form!=nullptr) {
        form->setAttribute( Qt::WA_DeleteOnClose, true );
        form->exec();
    }
}

void MainWindow::on_action_about_triggered()
{
    AboutDialog * aboutdig = new AboutDialog (this);
      if (aboutdig!=nullptr) {
          aboutdig->setAttribute( Qt::WA_DeleteOnClose, true );
          aboutdig->exec();
      }
}

void MainWindow::on_actionLabel_triggered()
{
    view3d->setAtomLabelVisible(!view3d->isAtomLabelVisible());
    view3d->refresh();
}
