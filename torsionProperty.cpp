#include "torsionProperty.h"
#include <QMessageBox>
#include "mainwindow.h"

static const QRegularExpression RE_WHITESPACE("\\s+");

TorsionProperty::TorsionProperty(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
}

TorsionProperty::~TorsionProperty()
{

}

void TorsionProperty::setParent(MainWindow * p)
{
    m_parent=p;
    mol=m_parent->getMol();
    mol->backupAtoms();
    view3d=m_parent->getView3D();
    initial();
}

void TorsionProperty::initial()
{
    times=0;

    ui.comboBox1->setCurrentIndex (2);
    ui.comboBox2->setCurrentIndex (2);

    ui.horizontalSlider->setTickInterval(10);
    ui.horizontalSlider->setRange(-180000,180000);

    idx0=view3d->getSelectedAtomId(0);
    idx1=view3d->getSelectedAtomId(1);
    idx2=view3d->getSelectedAtomId(2);
    idx3=view3d->getSelectedAtomId(3);

    torsionAngle=mol->dihedral(idx0,idx1,idx2,idx3);


    ui.textEdit1->setFont(QFont("Courier", 12));
    ui.textEdit2->setFont(QFont("Courier", 12));
    ui.lineEdit->setAlignment(Qt::AlignCenter);
    ui.lineEdit->setText(QString::number(torsionAngle, 6, 4));


    mol->findChildren(idList1,idx1,idx0);
    mol->findChildren(idList2,idx2,idx3);

    for(uint i=0;i<mol->getAtombyIndex(idx1)->numNeighbors();i++) {
        if( mol->getAtombyIndex(idx1)->getNeighborId(i)==idx0) continue;
        if( mol->getAtombyIndex(idx1)->getNeighborId(i)==idx2) continue;
        idList1.push_back(mol->getAtombyIndex(idx1)->getNeighborId(i));
    }


    for(uint i=0;i<mol->getAtombyIndex(idx2)->numNeighbors();i++) {
        if( mol->getAtombyIndex(idx2)->getNeighborId(i)==idx1) continue;
        if( mol->getAtombyIndex(idx2)->getNeighborId(i)==idx3) continue;
        idList2.push_back(mol->getAtombyIndex(idx2)->getNeighborId(i));
    }

    vector <int> :: iterator iter;
    for (iter=idList1.begin();iter!=idList1.end();iter++)
    {
        if ( (*iter)==idx1) {
            idList1.erase(iter); break;
        }

    }

    for (iter=idList2.begin();iter!=idList2.end();iter++)
    {
        if ( (*iter)==idx2) {
            idList2.erase(iter);
            break;
        }
    }
/*
    qSort(idList1.begin()+1, idList1.end());
    sort (idList1.begin(), idList1.end());
    iter =  unique ( idList1.begin(), idList1.end());
    idList1.erase(iter,idList1.end());

    qSort(idList2.begin()+1, idList2.end());
    sort (idList2.begin(), idList2.end());
    iter =  unique ( idList2.begin(), idList2.end());
    idList2.erase(iter,idList2.end());
*/

    QString text1=QString::number(idx0+1)+":  ";
    for(int i=0;i<idList1.size();i++)
        text1+= QString::number(1+idList1[i])+"  ";
        ui.textEdit1->appendPlainText(text1);

    QString text2=QString::number(idx3+1)+":  ";
    for(int i=0;i<idList2.size();i++)
        text2+= QString::number(1+idList2[i])+"  ";
        ui.textEdit2->appendPlainText(text2);
}

void TorsionProperty::setTorsion()
{
    if( torsionAngle<-179.99 || fabs(torsionAngle-180.0)<0.01 ) return;

    times++;
    if(times<2) return;


    QString selectText1=ui.comboBox1->currentText(),
            selectText2=ui.comboBox2->currentText();

    if(selectText1.contains("Fixed") && selectText2.contains("Fixed") )
    {
        QMessageBox::about(NULL, "Warning!","Both two atoms are fixed!");
        return;
    }


    //get text from index0 list
    QString list1_text=ui.textEdit1->toPlainText ();
    list1_text.replace(',',' '); list1_text.replace(':',' ');
    vector <uint> mlist1;

    int m_index;
    QStringList lines=list1_text.split(RE_WHITESPACE);
    for (int i=0; i<lines.count(); i++) {
        m_index=lines[i].toInt()-1;
        if(m_index>=0 && m_index<10000)  mlist1.push_back(m_index);
    }

    sort ( mlist1.begin(), mlist1.end());
    vector <uint> :: iterator iter = unique ( mlist1.begin(), mlist1.end());
    mlist1.erase(iter,mlist1.end());


    //get text from index1 list
    QString list2_text=ui.textEdit2->toPlainText ();
    list2_text.replace(',',' '); list2_text.replace(':',' ');

    vector <uint> mlist2;
    lines.clear();
    lines=list2_text.split(RE_WHITESPACE);
    for (int i=0; i<lines.count(); i++) {
        m_index=lines[i].toInt()-1;
        if(m_index>=0 && m_index<10000)  mlist2.push_back(m_index);
    }
    sort ( mlist2.begin(), mlist2.end());
    iter = unique ( mlist2.begin(), mlist2.end());
    mlist1.erase(iter,mlist2.end());



    //fix atom or not
    //---------------------------------------------------------------------------------
    if( selectText1.contains("Fixed") &&  selectText2.contains("atom") )
    {
        mol->setTorsionAtoms(idx0, idx1, idx2, idx3, torsionAngle,
                              true, false);
        view3d->renderMol(mol); return;

    }

    if( selectText1.contains("atom")  && selectText2.contains("Fixed")  )
    {
        mol->setTorsionAtoms(idx0, idx1, idx2,  idx3, torsionAngle,
                                          false, true);
        view3d->renderMol(mol); return;
        return;
    }

    if( selectText1.contains("atom") &&  selectText2.contains("atom") )
    {
        mol->setTorsionAtoms(idx0, idx1, idx2,  idx3,  torsionAngle,
                              false, false);
        view3d->renderMol(mol); return;
        return;
    }



    //fixed group or not, using OpenBabel functions
    if( selectText1.contains("Fixed")  && selectText2.contains("group") )
    {
        mol->setTorsionFragments (idx0, idx1, idx2,idx3,  torsionAngle,
                                   mlist1, mlist2,
                                   true, false);
        view3d->renderMol(mol); return;
        return;
    }
    if( selectText1.contains("group") && selectText2.contains("Fixed")  )
    {
        mol->setTorsionFragments (idx0, idx1, idx2, idx3, torsionAngle,
                                   mlist1, mlist2,
                                   false,true);
        view3d->renderMol(mol); return;
        return;
    }
    if( selectText1.contains("group")  && selectText2.contains("group")  )
    {
        mol->setTorsionFragments (idx0, idx1, idx2, idx3, torsionAngle,
                                    mlist1, mlist2,
                                    false,false);
        view3d->renderMol(mol);
        return;
    }
    //move group and one atom simutaneously
    if( selectText1.contains("group")  && selectText2.contains("atom")  )
    {
        vector <uint > list0; list0.push_back(idx2);
        mol->setTorsionFragments (idx0, idx1, idx2, idx3, torsionAngle,
                                    mlist1, list0,
                                    false,false);
        view3d->renderMol(mol);
        return;
    }

    //move group and one atom simutaneously
    if( selectText1.contains("atom")  && selectText2.contains("group")  )
    {
        vector <uint > list0; list0.push_back(idx0);
        mol->setTorsionFragments (idx0, idx1, idx2,idx3,  torsionAngle,
                                    list0, mlist2,
                                    false,false);
        view3d->renderMol(mol);
        return;
    }
}




void TorsionProperty::refreshEditor()
{
    torsionAngle = ui.horizontalSlider->value ()/1000.0;
    ui.lineEdit->setText(QString::number (torsionAngle));
    setTorsion();
}

void TorsionProperty::refreshSlider()
{
    torsionAngle = ui.lineEdit->text().toDouble();
    int scale = (int) (torsionAngle*1000.0);
    ui.horizontalSlider->setValue(scale);

    setTorsion();
}


void TorsionProperty::on_horizontalSlider_valueChanged(int value)
{
    refreshEditor();
}


void TorsionProperty::on_lineEdit_textChanged(const QString &arg1)
{
    refreshSlider();
}


void TorsionProperty::on_pushButton_cancel_clicked()
{
    view3d->releaseAllActors();
    mol->recoverAtomCoordinateFromBackup();
    mol->removeBackupAtoms();
    view3d->renderMol(mol);
    close();
}


void TorsionProperty::on_pushButton_ok_clicked()
{
    view3d->releaseAllActors();
    mol->removeBackupAtoms();
    view3d->renderMol(mol);
    view3d->push2Stack("ChangTorsion");
    close();
}
