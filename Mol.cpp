#include "Mol.h"
#include "templateMol.h"
#include <QFile>
#include <QMessageBox>
#include <QStringList>

#include <QString>
#include <QIODevice>

#include <vector>
//#include <Eigen/Core>
#include <QProcess>


#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/chains.h>
//#include <openbabel/babelconfig.h>
#include <openbabel/generic.h>
#include <openbabel/optransform.h>
#include <openbabel/plugin.h>

#include <openbabel/obutil.h>
#include <openbabel/base.h>

#include <openbabel/babelconfig.h>
#include <openbabel/data.h>
#include <openbabel/generic.h>
#include <openbabel/forcefield.h>
#include <openbabel/mol.h>
#include <openbabel/typer.h>
#include <openbabel/math/transform3d.h>

#include <fstream>




#include "mainwindow.h"

#include <cstdlib>
#include <cstdio>
#include <openbabel/ring.h>
#include <QApplication>
#include <QClipboard>


#include "math/matrix3x3.h"
#include "functionals.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <BaseMathOperators.h>
#include <MathOperators.h>
#include <Complex.h>
#include <Matrix.h>




using OpenBabel::OBConversion;
using OpenBabel::OBFormat;
using OpenBabel::OBMol;
using OpenBabel::OBAtom;
using OpenBabel::OBAtomIterator;
using OpenBabel::CSSRFormat;


using std::ifstream;
using std::ofstream;

using namespace hmma;





void HAtom::setSymbol ( QString s)
{
    _symbol=s;
}

void HAtom::setSymbol()
{
    if (_atomicNum>0)
        _symbol=QString::fromStdString(ElementNames[idx]);
}


void HAtom::modify2Symbol(QString s)
{
    _symbol=s;

    for (int i=0;i<119;i++) {
        if(QString::fromStdString(ElementNames[i])==_symbol)
        {
            _atomicNum=i;
            break;
        }
    }
    _mass=elementMasses[_atomicNum];
    _radius=CovalentRadii[_atomicNum];
    _color=ElementColors[_atomicNum];
    _selected=false;
}



QString HAtom::Symbol()
{
    return  _symbol;
}


QString HAtom::Symbol(uint idx)
{
    if (_symbol=="X" && _atomicNum>0)
        _symbol=QString::fromStdString(ElementNames[idx]);
    return  _symbol;
}



//assignment
HAtom & HAtom::operator= (const HAtom & other)
{
    _symbol=other._symbol;
    _atomicNum=other._atomicNum;
    _radius=other._radius;
    pos_cart=other.pos_cart;
    pos_frac=other.pos_frac;
    idx=other.idx;
    _color=other._color;
    _mass=other._mass;
    _scale=other._scale;

    return *this;
}




HAtom::HAtom(uint __atomicNum, double x, double y, double z)
{
    _atomicNum=__atomicNum;
    _symbol=QString::fromStdString(ElementNames[_atomicNum]);
    pos_cart.Set(x,y,z);

    _mass=elementMasses[_atomicNum];
    _radius=CovalentRadii[_atomicNum];
    _color=ElementColors[_atomicNum];
    _selected=false;

    _scale=0.3;
}


HAtom::HAtom(uint __atomicNum, vector3 p )
{
    _atomicNum=__atomicNum;
    _symbol=QString::fromStdString(ElementNames[_atomicNum]);
    pos_cart=p;

    _mass=elementMasses[_atomicNum];
    _radius=CovalentRadii[_atomicNum];
    _color=ElementColors[_atomicNum];
    _selected=false;
    _scale=0.3;
}


HAtom::HAtom(QString s, double x, double y, double z)
{
    _selected=false;
    _symbol=s;
    pos_cart.Set(x,y,z);

    for (int i=0;i<119;i++) {
        if(QString::fromStdString(ElementNames[i])==_symbol)
        {
            _atomicNum=i;
            break;
        }
    }


    _mass=elementMasses[_atomicNum];
    _radius=CovalentRadii[_atomicNum];
    _color=ElementColors[_atomicNum];
    _scale=0.3;

    // cout << atomicNum << " " <<radius <<endl;
}

HAtom::HAtom(QString s, vector3 p)
{
    _symbol=s;
    pos_cart=p;

    for (int i=0;i<119;i++) {
        if(QString::fromStdString(ElementNames[i])==_symbol) {_atomicNum=i; break; }
    }

    _mass=elementMasses[_atomicNum];
    _radius=CovalentRadii[_atomicNum];
    _color=ElementColors[_atomicNum];
    _selected=false;
    _scale=0.3;
}


void HAtom::update()
{
    //update atomicNum
    for (int i=0;i<119;i++) {
        if(QString::fromStdString(ElementNames[i])==_symbol) {_atomicNum=i; break; }
    }

    _mass=elementMasses[_atomicNum];
    _radius=CovalentRadii[_atomicNum];
    _color=ElementColors[_atomicNum];
}


double HAtom::Mass()
{
    if(_atomicNum<1) update();
    return _mass;
}

double HAtom::radius()
{
    if(_radius<0.1) update();
    return _radius;
}

void HAtom::removeLinkedAtomId(uint id)
{
    uint i;
    for ( i=0;i<LinkdedAtoms.size();i++)
    {
        if(LinkdedAtoms[i]==id)
        {
            LinkdedAtoms.removeAt(i);
            break;
        }
    }
}

void HAtom::removeNeighbor(uint id)
{
    uint i;
    for ( i=0;i<LinkdedAtoms.size();i++)
    {
        if(LinkdedAtoms[i]==id)
        {
            LinkdedAtoms.removeAt(i);
            break;
        }
    }
}




HMol::HMol()
{
    isAtomvisible=true;
    isAtomLabelVisible=false;

    xyzLength.Set(0.0,0.0,0.0);

    m_graph=nullptr;

    charge=0;
}


void HMol::centeralize()
{
    vector3 MassCenter=VZero;
    double totalMass=0.0;

    for (uint i=0;i<NumAtoms();i++) {
        double mass=atomicMass(i);
        MassCenter += atomPos(i)*mass;
        totalMass += mass;
    }

    MassCenter /= totalMass;

    cout << MassCenter<<endl;

    for (uint j=0;j<NumAtoms();j++){
        setAtomPos(j,atomPos(j)-MassCenter);
    }

    calcMolVolume();


    vector3 a, b;
    //update bond plane information
    for (uint i=0;i<NumBonds();i++)
    {
        if(getBondbyIndex(i)->getBondOrder()<2) continue;
        if(getBondbyIndex(i)->getBondOrder()>6) continue;

        if (getBondbyIndex(i)->getPlane().length() < 0.6) continue;

        a=getBondbyIndex(i)->getPlane()-MassCenter;

        getBondbyIndex(i)->setPlane(a);
    }

    //update ring information
    for (uint i=0;i<numRings();i++)
    {
        //a=getRingbyId(i)->norm-MassCenter;
        //getRingbyId(i)->norm=a;

        a=getRingbyId(i)->center-MassCenter;
        getRingbyId(i)->center=a;
    }
    //perceivePlaneRingforBonds();
}



QString HMol::getCoordinates()
{
    QString coord="";

    if (NumAtoms()<1)  return coord;

    char ss[256];

    for (uint i=0;i<atomList.size();i++)
    {
        std::sprintf (ss,"%s %10.6f  %10.6f  %10.6f\n",(atomList[i]->Symbol().toStdString()).c_str(),
                      atomList[i]->x(),atomList[i]->y(),atomList[i]->z());
        coord+=ss;
    }
}


bool HMol::hasSelectedAtoms()
{
    if(view3d==nullptr)
        view3d=m_parent->getView3D();

    if (view3d && view3d->numSelectedAtoms()>0)
        return true;
    return false;
}

//translate and rotation
void HMol::tranform(vector3 v, matrix3x3 m)
{
    vector3  a,b;

    if(!hasSelectedAtoms())
    {
        for (uint i=0;i<NumAtoms();i++)
        {
            a=atomPos(i)-v;
            setAtomPos(i,m*a+v);
        }

        //perceivePlaneRingforBonds();
        //update bond plane information
        for (uint i=0;i<NumBonds();i++)
        {
            if(getBondbyIndex(i)->getBondOrder()<2) continue;
            if(getBondbyIndex(i)->getBondOrder()>6) continue;

            if (getBondbyIndex(i)->getPlane().length() < 0.6) continue;

            a=getBondbyIndex(i)->getPlane();

            getBondbyIndex(i)->setPlane(m*a);
        }

        //update ring information
        for (uint i=0;i<numRings();i++)
        {
            a=getRingbyId(i)->norm;
            getRingbyId(i)->norm=m*a;

            a=getRingbyId(i)->center;
            getRingbyId(i)->center=m*a;
        }
        return;
    }

    //selected atoms existed
    if(hasSelectedAtoms())
    {
        for (uint i=0;i<NumAtoms();i++)
        {
            if(!getAtombyIndex(i)->isSelected())
                continue;
            a=atomPos(i)-v;
            setAtomPos(i,m*a+v);
            updateAtomInsideRingOrNot(i);
        }
    }
}

void HMol::rotate(matrix3x3 m)
{
    vector3  a,b;
    if(hasSelectedAtoms())
        cout << " hasSelectedAtoms" << " cannot rotate"<<endl;

    if(!hasSelectedAtoms())
    {
        for (uint i=0;i<NumAtoms();i++)
        {
            setAtomPos(i,m*atomPos(i));
        }

        //perceivePlaneRingforBonds();
        //update bond plane information
        for (uint i=0;i<NumBonds();i++)
        {
            if(getBondbyIndex(i)->getBondOrder()<2) continue;
            if(getBondbyIndex(i)->getBondOrder()>6) continue;

            if (getBondbyIndex(i)->getPlane().length() < 0.6) continue;

            a=getBondbyIndex(i)->getPlane();

            getBondbyIndex(i)->setPlane(m*a);
        }

        //update ring information
        for (uint i=0;i<numRings();i++)
        {
            a=getRingbyId(i)->norm;
            getRingbyId(i)->norm=m*a;

            a=getRingbyId(i)->center;
            getRingbyId(i)->center=m*a;
        }
        return;
    }

    //selected atoms existed
    if(hasSelectedAtoms())
    {
        for (uint i=0;i<NumAtoms();i++)
        {
            if(!getAtombyIndex(i)->isSelected())
                continue;

            a=atomPos(i);
            setAtomPos(i,m*a);
            updateAtomInsideRingOrNot(i);
        }
    }
}

void HMol::rotate(vector3  v, double angle )
{
    double  ele[3][3];

    double ang = angle*3.14159265359/180.0;

    double c = cos(ang), t=1-c,  s = sin(ang);


    vector3 vec = v;
    v.normalize();

    double   x = vec[0],  y = vec[1], z = vec[2];

    ele[0][0] = x*x*t+c ;       ele[0][1] = x*y*t-z*s;       ele[0][2] = x*z*t+y*s;
    ele[1][0] = x*y*t+z*s;      ele[1][1] = y*y*t+c;         ele[1][2] = y*z*t-x*s;
    ele[2][0] = x*z*t-y*s;      ele[2][1] = y*z*t+x*s;       ele[2][2] = z*z*t+c;

    vector3  a,b;


    if(!hasSelectedAtoms())
    {
        for (uint i=0;i<NumAtoms();i++)
        {
            a=atomPos(i);

            b.SetX(  a.x()*ele[0][0] + a.y()*ele[0][1] + a.z()*ele[0][2] );
            b.SetY(  a.x()*ele[1][0] + a.y()*ele[1][1] + a.z()*ele[1][2] );
            b.SetZ(  a.x()*ele[2][0] + a.y()*ele[2][1] + a.z()*ele[2][2]);

            setAtomPos(i,b);
        }

        //perceivePlaneRingforBonds();
        //update bond plane information
        for (uint i=0;i<NumBonds();i++)
        {
            if(getBondbyIndex(i)->getBondOrder()<2) continue;
            if(getBondbyIndex(i)->getBondOrder()>6) continue;

            if (getBondbyIndex(i)->getPlane().length() < 0.6) continue;

            a=getBondbyIndex(i)->getPlane();
            b.SetX(  a.x()*ele[0][0] + a.y()*ele[0][1] + a.z()*ele[0][2] );
            b.SetY(  a.x()*ele[1][0] + a.y()*ele[1][1] + a.z()*ele[1][2] );
            b.SetZ(  a.x()*ele[2][0] + a.y()*ele[2][1] + a.z()*ele[2][2] );

            getBondbyIndex(i)->setPlane(b);
        }

        //update ring information
        for (uint i=0;i<numRings();i++)
        {
            a=getRingbyId(i)->norm;
            b.SetX(  a.x()*ele[0][0] + a.y()*ele[0][1] + a.z()*ele[0][2] );
            b.SetY(  a.x()*ele[1][0] + a.y()*ele[1][1] + a.z()*ele[1][2] );
            b.SetZ(  a.x()*ele[2][0] + a.y()*ele[2][1] + a.z()*ele[2][2] );
            getRingbyId(i)->norm=b;

            a=getRingbyId(i)->center;
            b.SetX(  a.x()*ele[0][0] + a.y()*ele[0][1] + a.z()*ele[0][2] );
            b.SetY(  a.x()*ele[1][0] + a.y()*ele[1][1] + a.z()*ele[1][2] );
            b.SetZ(  a.x()*ele[2][0] + a.y()*ele[2][1] + a.z()*ele[2][2] );
            getRingbyId(i)->center=b;
        }
        return;
    }

    //selected atoms existed
    if(hasSelectedAtoms())
    {
        for (uint i=0;i<NumAtoms();i++)
        {
            if(!getAtombyIndex(i)->isSelected())
                continue;

            a=atomPos(i);
            b.SetX(  a.x()*ele[0][0] + a.y()*ele[0][1] + a.z()*ele[0][2] );
            b.SetY(  a.x()*ele[1][0] + a.y()*ele[1][1] + a.z()*ele[1][2] );
            b.SetZ(  a.x()*ele[2][0] + a.y()*ele[2][1] + a.z()*ele[2][2] );

            setAtomPos(i,b);
            updateAtomInsideRingOrNot(i);
        }
    }
}


void HMol::rotate(vector3  v1, vector3  v2)
{   
    matrix3x3 m(1.0);

    if (isEqual(IMomentMatrix.GetColumn(2),VZ)) {
        return;
    }
    else if (isNegativeEqual(IMomentMatrix.GetColumn(2),VZ)) {
        m.Set(0,0,-1.0);
        m.Set(1,1,-1.0);
        m.Set(1,1,-1.0);
    }
    else {
        m.SetupRotateMatrix(v1,v2);
    }



    vector3 a,b;
    if(!hasSelectedAtoms())
    {
        for (uint i=0;i<NumAtoms();i++)
            setAtomPos(i,m*atomPos(i));

        //update bond plane information
        for (uint i=0;i<NumBonds();i++)
        {
            if(getBondbyIndex(i)->getBondOrder()<2) continue;
            if(getBondbyIndex(i)->getBondOrder()>6) continue;

            if (getBondbyIndex(i)->getPlane().length() < 0.6) continue;

            a=getBondbyIndex(i)->getPlane();
            getBondbyIndex(i)->setPlane(m*a);
        }

        //update ring information
        for (uint i=0;i<numRings();i++)
        {
            a=getRingbyId(i)->norm;
            getRingbyId(i)->norm=m*a;

            a=getRingbyId(i)->center;
            getRingbyId(i)->center=m*a;
        }
        return;
    }


    //selected atoms existed
    if(hasSelectedAtoms())
    {
        for (uint i=0;i<NumAtoms();i++)
        {
            if(!getAtombyIndex(i)->isSelected())
                continue;

            a=atomPos(i);
            setAtomPos(i,m*a);
            updateAtomInsideRingOrNot(i);
        }
    }
}

//help function
void HMol::updateAtomInsideRingOrNot(uint id)
{
    uint idRing=1000001;

    if (numRings()<1) return;

    vector <uint > ringList;
    ringList.clear();
    for(uint i=0;i<numRings();i++)
    {
        if(isAtomInsideRing(id, i))
            ringList.push_back(i);
    }

    if(ringList.size()<1) return;
    uint num=ringList.size();


    vector3 pp=getAtomPosbyIndex(id);

    if(num>1)
        reverse(ringList.begin(),ringList.end());

    foreach (uint idRing,ringList)
    {
        //up-down
        double angle=vectorAngle(getRingbyId(idRing)->norm,getRingbyId(idRing)->center,pp);

        if( abs(angle-90.0) > 2.0 ) {
            removeRingbyId(idRing);
            continue;
        }

        //radius
        double r=getRingbyId(idRing)->radius;
        double ll=(pp-getRingbyId(idRing)->center).length()-r;

        if (abs(ll)>0.1) {
            removeRingbyId(idRing); continue;
        }

        //horizontal angle
        angle=360.0/getRingbyId(idRing)->atomIdList.size();
        uint neighId=getNeighborinsideRing(id,idRing);
        double ang=vectorAngle(getAtomPosbyIndex(neighId),getRingbyId(idRing)->center,getAtomPosbyIndex(id));

        if (abs(ang-angle)>2.0){
            removeRingbyId(idRing);
            continue;
        }
    }
}


uint HMol::getNeighborinsideRing(uint id, uint idRing)
{
    if(getRingbyId(idRing)==nullptr) return 100001;

    uint i=0;
    uint size=getRingbyId(idRing)->atomIdList.size();
    for(i=0;i<size;i++)
    {
        if(id==getRingbyId(idRing)->atomIdList[i]) break;
    }

    if(i<1) return getRingbyId(idRing)->atomIdList[1];
    else  return getRingbyId(idRing)->atomIdList[i-1];
}


void HMol::translate(vector3 p)
{
    if(!hasSelectedAtoms())
    {
        for (uint i=0;i<NumAtoms();i++) {
            setAtomPos(i,atomPos(i)+p);

        }

        vector3 a, b;
        //update bond plane information
        for (uint i=0;i<NumBonds();i++)
        {
            if(getBondbyIndex(i)->getBondOrder()<2) continue;
            if(getBondbyIndex(i)->getBondOrder()>6) continue;

            if (getBondbyIndex(i)->getPlane().length() < 0.6) continue;

            a=getBondbyIndex(i)->getPlane()+p;

            getBondbyIndex(i)->setPlane(a);
        }

        //update ring information
        for (uint i=0;i<numRings();i++)
        {
            a=getRingbyId(i)->center+p;
            getRingbyId(i)->center=a;
        }
    }
    else {
        //selected atoms existed
        for (uint i=0;i<NumAtoms();i++)
        {
            if(!getAtombyIndex(i)->isSelected()) continue;
            setAtomPos(i,atomPos(i)+p);
            //setAtomPos(i,p);
            updateAtomInsideRingOrNot(i);
        }
    }
}

void HMol::translate(double x, double y, double z)
{
    vector3 p(x,y,z);
    translate(p);
}




void HMol::clearAll()
{
    removeBackupAtoms();
    removeAllXAtoms();
    removeAllAtoms();
    removeAllBonds();
    removeAllRings();
    removeAllArcs();

    huckelCarbonList.clear();
}

void HMol::removeAllArcs()
{
    if (arcList.size() < 1) return;
    for (auto it=arcList.begin();it!=arcList.end();++it)
    {
        if(*it != nullptr) {
            delete (*it);
            (*it) = nullptr;
        }
    }
    arcList.clear();
    QVector<HArc*>().swap(arcList);
}


void HMol::removeAllRings()
{
    if (ringList.size() < 1) return;
    for (auto it=ringList.begin();it!=ringList.end();++it)
    {
        if(*it != nullptr) {
            delete (*it);
            (*it) = nullptr;
        }
    }
    ringList.clear();
    QVector<HRing*>().swap(ringList);
}


void HMol::removeAllAtoms()
{  
    //remove bond first;
    removeAllBonds();
    if (atomList.size() < 1) return;
    for (auto it=atomList.begin();it!=atomList.end();++it)
    {
        if(*it != nullptr) {
            delete (*it);
            (*it) = nullptr;
        }
    }
    atomList.clear();
    QVector<HAtom*>().swap(atomList);
}

void HMol::removeAllXAtoms()
{
    if (atomXList.size() < 1) return;
    for (auto it=atomXList.begin();it!=atomXList.end();++it)
    {
        if(*it != nullptr) {
            delete (*it);
            (*it) = nullptr;
        }
    }
    atomXList.clear();
    QVector<HXAtom*>().swap(atomXList);
}



void HMol::removeAllBonds()
{
    removeAllRings();

    if (bondList.size() < 1) return;
    for (auto it=bondList.begin();it!=bondList.end();++it)
    {
        if(*it != nullptr) {
            delete (*it);
            (*it) = nullptr;
        }
    }
    bondList.clear();
    QVector<HBond*>().swap(bondList);


    if(atomList.size()>0)
        for (auto iter=atomList.begin();iter!=atomList.end();iter++)
        {
            (*iter)->clearLinkedAtomId();
        }
}



double HMol::distance  (uint id1, uint id2)
{
    return vectorDistance(getAtomPosbyIndex(id1),getAtomPosbyIndex(id2));
}

double HMol::angle (uint id1, uint id2, uint id3)
{
    return vectorAngle(getAtomPosbyIndex(id1),
                       getAtomPosbyIndex(id2),
                       getAtomPosbyIndex(id3));
}



double HMol::dihedral  (uint id1, uint id2, uint id3, uint id4)
{
    return TorsionAngle(getAtomPosbyIndex(id1),
                        getAtomPosbyIndex(id2),
                        getAtomPosbyIndex(id3),
                        getAtomPosbyIndex(id4)
                        );
}




void HMol::setView3D (View3D * v)
{
    if(v!=nullptr)
        view3d=v;
}

void HMol::removeAtombyId(vector <uint> idList)
{
    sort ( idList.begin(), idList.end());
    vector <uint> :: iterator iter = unique ( idList.begin(), idList.end());
    idList.erase(iter,idList.end());
    sort(idList.rbegin(), idList.rend());

    //remove from the tail one-by-one
    for (uint i=0;i<idList.size();i++)
        removeAtombyId(idList[i]);
}

void HMol::replaceSelelctedAtomWith(uint idx,vector <uint> selectedAtomList)
{
    for (uint i=0;i<selectedAtomList.size();i++)
    {
        getAtombyIndex(selectedAtomList[i])->setSymbol(QString::fromStdString(ElementNames[idx]));
        getAtombyIndex(selectedAtomList[i])->update();
    }
}


bool HMol::isAtomInsideRing(uint idAtom, uint idRing)
{
    if(numRings()<1) return false;

    vector<uint>::iterator it = find(ringList[idRing]->atomIdList.begin(),
                                     ringList[idRing]->atomIdList.end(),
                                     idAtom);

    if (it != ringList[idRing]->atomIdList.end()) return true;
    return false;
}

bool HMol::isAtomInsideArc(uint idAtom, uint idArc)
{
    if(numArcs()<1) return false;

    vector<uint>::iterator it = find(ringList[idArc]->atomIdList.begin(),
                                     ringList[idArc]->atomIdList.end(),
                                     idAtom);

    if (it != arcList[idArc]->atomIdList.end()) return true;
    return false;
}


void HMol::removeArcbyId(uint id)
{
    delete arcList[id];
    arcList.remove(id);

    //refresh bond information
    uint j=NumBonds();
    while (j--)
    {
        HBond * bond= bondList[j];
        if(!bond->isInsideArc()) continue;
        uint id0=bond->getArcId();
        if(id0==id)  bond->setArcId(100001);
        if(id0>id)   bond->setArcId(id0-1);
    }
}

void HMol::removeRingbyId(uint id)
{
    delete ringList[id];
    ringList.remove(id);

    //refresh bond information
    uint j=NumBonds();
    while (j--)
    {
        HBond * bond= bondList[j];
        if(!bond->isInsideRing()) continue;
        uint id0=bond->getRingId();
        if(id0==id)  bond->setRingId(100001);
        if(id0>id)   bond->setRingId(id0-1);
    }
}


void HMol::removeSingleRingbyId(uint id)
{
    delete ringList[id];
    ringList.remove(id);

    //refresh bond information
    uint j=NumBonds();
    while (j--)
    {
        HBond * bond= bondList[j];
        if(!bond->isInsideRing()) continue;
        uint id0=bond->getRingId();
        if(id0==id)  bond->setRingId(100001);
        if(id0>id)   bond->setRingId(id0-1);
    }

    /*
    uint j=NumBonds();
    while (j--)
    {
        HBond * bond= bondList[j];
        bond->setRingId(100000001);
    }

    j=numRings();
    while (j--)
    {
        uint size=getRingbyId(j)->atomIdList.size();
        HBond * bond=nullptr;
        for (uint i=0;i<size-1;i++)
        {
            bond=getBondbyAtomIndex(getRingbyId(j)->atomIdList[i],getRingbyId(j)->atomIdList[i+1]);
            if (bond==nullptr) continue;
            bond->setRingId(j);
        }
        bond=getBondbyAtomIndex(getRingbyId(j)->atomIdList[0],getRingbyId(j)->atomIdList[size-1]);
        if (bond!=nullptr) bond->setRingId(j);
    }
*/
}


void HMol::removeLonelyHydrogen()
{
    //remove terminal Hydrogen
    uint id=NumAtoms();

    cout << id << " atoms left"<<endl;
    while(id--)
    {
        //Hydrogen?
        if(getAtombyIndex(id)->Symbol()!="H")    continue;

        //lonely?
        if(getAtombyIndex(id)->numNeighbors()>0) continue;


        cout << " wissssssss " <<id+1 <<endl;
        //continue;


        //refresh Bonds index
        uint j=NumBonds();
        while (j--)
        {
            HBond * bond= bondList[j];
            uint idx0=bond->atomIndex0();
            uint idx1=bond->atomIndex1();

            if (idx0 > id )   bond->setBeginIndex(idx0-1);
            if (idx1 > id )   bond->setEndIndex(idx1-1);
        }


        //update infomation of ring
        j=numRings();
        if(j>=1)
        {
            //update information of ring left
            while (j--)
            {
                uint k=ringList[j]->atomIdList.size();
                while(k--)
                {
                    uint ii=ringList[j]->atomIdList[k];
                    if(ii>id)
                        ringList[j]->atomIdList[k]=ii-1;
                }
            }
        }

        //update information of atoms left
        delete atomList[id];
        atomList.remove(id);


        j=NumAtoms();
        while (j--)  atomList[j]->clearNeighbors();

        //update neighor information of each atom
        j=NumBonds();
        if(j>0)
            while (j--)
            {
                uint idx0=bondList[j]->atomIndex0();
                uint idx1=bondList[j]->atomIndex1();

                getAtombyIndex(idx1)->appendNeighbor(idx0);
                getAtombyIndex(idx0)->appendNeighbor(idx1);
            }
    }
}


void HMol::removeTerminalHydrogen(uint id)
{
    uint j=NumBonds();
    while (j--)
    {
        HBond * bond= bondList[j];
        uint idx0=bond->atomIndex0();
        uint idx1=bond->atomIndex1();

        if (idx0 == id || id==idx1)  {
            delete bondList[j];
            bondList.remove(j);
            continue;
        }

        if (idx0 > id )
            bond->setBeginIndex(idx0-1);

        if (idx1 > id )
            bond->setEndIndex(idx1-1);
    }

    if(bondList.size()<1) {
        bondList.clear();
        QVector<HBond*>().swap(bondList);
    }


    //update infomation of rings
    j=numRings();
    if(j>=1)
    {
        while (j--)
        {
            if(!isAtomInsideRing(id, j))  continue;
            removeSingleRingbyId(j);
        }

        //update information of ring left
        uint k=numRings();
        while (k--)
        {
            uint size=ringList[k]->atomIdList.size();
            while(size--)
            {
                uint ii=ringList[k]->atomIdList[size];
                if(ii>id)
                    ringList[k]->atomIdList[size]=ii-1;
            }
        }
    }

    //remove this atom
    delete atomList[id];
    atomList.remove(id);

    //clear neighbor informations
    j=NumAtoms();
    while (j--)  atomList[j]->clearNeighbors();

    //update neighbor information of each atom
    j=NumBonds();
    while (j--)
    {
        uint idx0=bondList[j]->atomIndex0();
        uint idx1=bondList[j]->atomIndex1();

        getAtombyIndex(idx1)->appendNeighbor(idx0);
        getAtombyIndex(idx0)->appendNeighbor(idx1);
    }
}


void HMol::removeSingleAtombyId(uint id)
{
    //remove the last atom with out bonds!
    if(atomList.size()==1) {
        clearAll();
        return;
    }

    //removed bonds of this atom
    uint j=NumBonds();
    while (j--)
    {
        HBond * bond= bondList[j];
        uint idx0=bond->atomIndex0();
        uint idx1=bond->atomIndex1();

        if (idx0 == id || id==idx1)  {
            delete bondList[j];
            bondList.remove(j);
            continue;
        }

        if (idx0 > id )
            bond->setBeginIndex(idx0-1);

        if (idx1 > id )
            bond->setEndIndex(idx1-1);
    }

    if(bondList.size()<1) {
        bondList.clear();
        QVector<HBond*>().swap(bondList);
    }



    //update infomation of rings
    j=numRings();
    if(j>=1)
    {
        while (j--)
        {
            if(!isAtomInsideRing(id, j))  continue;
            removeSingleRingbyId(j);
        }

        //update information of ring left
        uint k=numRings();
        while (k--)
        {
            uint size=ringList[k]->atomIdList.size();
            while(size--)
            {
                uint ii=ringList[k]->atomIdList[size];
                if(ii>id)
                    ringList[k]->atomIdList[size]=ii-1;
            }
        }
    }


    //remove this atom
    delete atomList[id];
    atomList.remove(id);

    //clear neighbor informations
    j=NumAtoms();
    while (j--)  atomList[j]->clearNeighbors();

    //update neighbor information of each atom
    j=NumBonds();
    while (j--)
    {
        uint idx0=bondList[j]->atomIndex0();
        uint idx1=bondList[j]->atomIndex1();

        getAtombyIndex(idx1)->appendNeighbor(idx0);
        getAtombyIndex(idx0)->appendNeighbor(idx1);
    }

}


void HMol::removeAtombyId(uint id)
{
    //cout << "remove atom begin "<<endl;

    //remove the last atom with out bonds!
    if(atomList.size()==1) {
        clearAll();
        return;
    }

    //find the terminal hydrogen first
    uint size=getAtombyIndex(id)->numNeighbors();
    while (size--)
    {
        uint ii=getAtombyIndex(id)->getNeighborId(size);

        //Hydrogen?
        if(getAtombyIndex(ii)->Symbol()!="H")    continue;

        //lonely?
        if(getAtombyIndex(ii)->numNeighbors()>1) continue;

        removeTerminalHydrogen(ii);
    }


    //removed bonds of this atom
    uint j=NumBonds();
    while (j--)
    {
        HBond * bond= bondList[j];
        uint idx0=bond->atomIndex0();
        uint idx1=bond->atomIndex1();

        if (idx0 == id || id==idx1)  {
            delete bondList[j];
            bondList.remove(j);
            continue;
        }

        if (idx0 > id )
            bond->setBeginIndex(idx0-1);

        if (idx1 > id )
            bond->setEndIndex(idx1-1);
    }

    if(bondList.size()<1) {
        bondList.clear();
        QVector<HBond*>().swap(bondList);
    }



    //update infomation of rings
    j=numRings();
    if(j>=1)
    {
        while (j--)
        {
            if(!isAtomInsideRing(id, j))  continue;
            removeSingleRingbyId(j);
        }

        //update information of ring left
        uint k=numRings();
        while (k--)
        {
            uint size=ringList[k]->atomIdList.size();
            while(size--)
            {
                uint ii=ringList[k]->atomIdList[size];
                if(ii>id)
                    ringList[k]->atomIdList[size]=ii-1;
            }
        }
    }


    //remove this atom
    delete atomList[id];
    atomList.remove(id);

    //clear neighbor informations
    j=NumAtoms();
    while (j--)  atomList[j]->clearNeighbors();

    //update neighbor information of each atom
    j=NumBonds();
    while (j--)
    {
        uint idx0=bondList[j]->atomIndex0();
        uint idx1=bondList[j]->atomIndex1();

        getAtombyIndex(idx1)->appendNeighbor(idx0);
        getAtombyIndex(idx0)->appendNeighbor(idx1);
    }

    // displayBonds();
    // displayAtoms();


    //remove terminal Hydrogen
    //removeLonelyHydrogen();
}

void HMol::removeBondbyId(uint id)
{
    uint idx0=bondList[id]->atomIndex0();
    uint idx1=bondList[id]->atomIndex1();
    removeBondbyAtomIds(idx0,idx1);
}



void HMol::removeBondbyAtomIds(uint id0,uint id1)
{
    uint j=NumBonds();

    if(j<1) return;

    while (j--)
    {
        uint idx0=bondList[j]->atomIndex0();
        uint idx1=bondList[j]->atomIndex1();

        if ( id0==idx0 && id1==idx1)
        {
            getAtombyIndex(id0)->removeLinkedAtomId(id1);
            getAtombyIndex(id1)->removeLinkedAtomId(id0);

            delete bondList[j];
            bondList.remove(j);
            break;
        }

        if ( id1==idx0 && id0==idx1)
        {
            getAtombyIndex(id0)->removeLinkedAtomId(id1);
            getAtombyIndex(id1)->removeLinkedAtomId(id0);

            delete bondList[j];
            bondList.remove(j);
            break;
        }
    }
    perceivePlaneRingforBonds();
}



void HMol::removeBondbyAtomId(uint id)
{
    QVector<HBond *>::iterator iter;
    for (iter=bondList.begin();iter!=bondList.end();iter++)
    {
        if ( id==(*iter)->atomIndex0() || id==(*iter)->atomIndex1())
        {
            delete (*iter);
            (*iter) = nullptr;
            bondList.erase(iter);
        }
    }
    perceivePlaneRingforBonds();
}


void HMol::loadFile(QString filename)
{

    if( filename.endsWith("xyz") ||  filename.endsWith("XYZ") )    {
        load_xyz(filename);
        return;
    }


    if( filename.endsWith("mol") ||  filename.endsWith("MOL") )    {
        load_mol(filename);
        return;
    }

    if( filename.endsWith("pdb") ||  filename.endsWith("PDB") )    {
        load_other(filename);
        return;
    }
    
    if( filename.endsWith("fch") ||  filename.endsWith("fchk")
            ||  filename.endsWith("FCH") ||  filename.endsWith("FCHK") )
    {
        load_GaussianFchk(filename);
        return;
    }


    load_other(filename);
}


void HMol::load_GaussianFchk(QString filename)
{
    
}


void HMol::loadxyzfromQString(QString text)
{
    //cout << text.toStdString().c_str()<<endl;

    //if(!pastefromClipboard()) return;

    clearAll();

    double x, y, z;
    QString  atomLabel,line;
    QStringList lst=text.split("\n"),single;

    for (uint i=0;i<lst.size();i++)
    {
        line=lst[i].simplified();
        if (line.size()<5) continue;
        single=line.split(" ",QString::SkipEmptyParts);
        if (single.size()!=4) continue;

        if(isNumber(single[0]))
            atomLabel=QString::fromStdString(ElementNames[single[0].toInt()]);
        if(isEnglish(single[0]))
            atomLabel=single[0];

        x=single[1].toDouble();
        y=single[2].toDouble();
        z=single[3].toDouble();

        addAtom(atomLabel,x,y,z);
    }

    perceiveBondOrder();
    perceivePlaneRingforBonds();
}

void HMol::addatomfromQString(QString data)
{
    //cout << text.toStdString().c_str()<<endl;

    //if(!pastefromClipboard()) return;

    double x, y, z;
    QString  atomLabel,line;
    QStringList lst=data.split("\n"),single;

    for (uint i=0;i<lst.size();i++)
    {
        line=lst[i].simplified();
        if (line.size()<5) continue;
        single=line.split(" ",QString::SkipEmptyParts);
        if (single.size()!=4) continue;

        if(isNumber(single[0]))
            atomLabel=QString::fromStdString(ElementNames[single[0].toInt()]);
        if(isEnglish(single[0]))
            atomLabel=single[0];

        x=single[1].toDouble();
        y=single[2].toDouble();
        z=single[3].toDouble();

        addAtom(atomLabel,x,y,z);
    }

    //perceiveBondOrder();
    //perceivePlaneRingforBonds();
}


void HMol::load_xyz(QString filename)
{
    clearAll();

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(0, "Warning","Cannot Open File!");
    }

    double x, y, z;
    QString  atomLabel;
    QStringList lst;
    
    while(!file.atEnd())
    {
        QString line(file.readLine());
        line.chop(1);
        if (isNumber(line)) continue;
        lst=line.split(" ",QString::SkipEmptyParts);

        if (lst.size()!=4) continue;

        atomLabel=lst[0];
        x=lst[1].toDouble();
        y=lst[2].toDouble();
        z=lst[3].toDouble();

        addAtom(atomLabel,x,y,z);
    }
    perceiveBondOrder();
    cout << "read "<<filename.toStdString().c_str() << " done!"<<endl;
}



void HMol::addAtom(string atomLabel,vector3 p)
{
    if(atomExisted(QString::fromStdString(atomLabel),p)) return;

    HAtom * atom = new HAtom(QString::fromStdString(atomLabel),p);
    atomList.push_back(atom);

    getLastAtom()->setIndex(atomList.size());
    //getLastAtom()->setScale(0.3);
}

void HMol::addAtom(QString atomLabel,vector3 p)
{
    if(atomExisted(atomLabel,p)) return;

    HAtom * atom = new HAtom(atomLabel,p);
    atomList.push_back(atom);

    getLastAtom()->setIndex(atomList.size());
    //getLastAtom()->setScale(0.3);
}


void HMol::addAtom(QString atomLabel,double x,double y,double z)
{
    if(atomExisted(atomLabel,x,y,z)) return;

    HAtom * atom = new HAtom(atomLabel,x,y,z);
    atomList.push_back(atom);

    getLastAtom()->setIndex(atomList.size()-1);
    //getLastAtom()->setScale(0.3);
}


void HMol::addAtom(string atomLabel,double x,double y,double z)
{
    if(atomExisted(QString::fromStdString(atomLabel),x,y,z)) return;

    HAtom * atom = new HAtom(QString::fromStdString(atomLabel),x,y,z);
    atomList.push_back(atom);

    getLastAtom()->setIndex(atomList.size()-1);
    //getLastAtom()->setScale(0.3);
}

void HMol::addAtom(uint atomicNumber,double x,double y,double z)
{
    if(atomExisted(atomicNumber,x,y,z)) return;

    HAtom * atom = new HAtom(atomicNumber,x,y,z);
    atomList.push_back(atom);

    getLastAtom()->setIndex(atomList.size()-1);
    //getLastAtom()->setScale(0.3);
}

void HMol::addAtom(uint atomicNumber, vector3 p)
{
    if(atomExisted(atomicNumber,p)) return;

    HAtom * atom = new HAtom(atomicNumber,p);
    atomList.push_back(atom);

    getLastAtom()->setIndex(atomList.size()-1);
    //getLastAtom()->setScale(0.3);
}


bool HMol::atomExisted(uint atomicNum,vector3 p)
{
    if(atomList.size()<1) return false;
    foreach(HAtom * atom, atomList)
    {
        if(atom==nullptr) continue;
        double length=(p-atom->Pos()).length();
        if ( length<0.5 && atomicNum==atom->atomicNum())
            return true;
    }
    return false;
}

bool HMol::atomExisted(uint atomicNum,double x,double y,double z)
{
    vector3 p(x,y,z);
    return atomExisted(atomicNum,p);
}


bool HMol::atomExisted(QString atomSymbol, vector3 p)
{
    if(atomList.size()<1) return false;

    foreach(HAtom * atom, atomList)
    {
        if(atom==nullptr) continue;
        double length=(p-atom->Pos()).length();
        if ( length<0.5 && atomSymbol==atom->Symbol())
            return true;
    }
    return false;
}


bool HMol::atomExisted(QString atomSymbol,double x,double y,double z)
{
    vector3 p(x,y,z);
    return atomExisted(atomSymbol,p);
}


HAtom * HMol::getLastAtom()
{
    uint size=atomList.size();
    if (size>0)
        return atomList[size-1];
    return nullptr;
}

bool HMol::addAtom(HAtom * other)
{
    if(other==nullptr) return false;

    double eps=1e-6;

    double x=other->x();
    double y=other->y();
    double z=other->z();

    if(abs(x)<eps)        x=0.0;
    if(abs(y)<eps)        y=0.0;
    if(abs(z)<eps)        z=0.0;

    if(atomExisted(other->atomicNum(),other->cartesian()))
        return false;

    HAtom * atom = new HAtom();
    atom->setAtomicNum(other->atomicNum());
    atom->setSymbol(other->Symbol());

    // cout << "inside addAtom1" <<endl;
    // cout << other->atomicNum()<<endl;
    //  cout << atom->atomicNum()<<endl;

    //  cout << other->Symbol().toStdString().c_str()<<endl;
    //   cout << atom->Symbol().toStdString().c_str()<<endl;



    atom->setIndex(other->index());
    atom->setRadius (other->radius());
    atom->setMass(other->Mass());

    atom->setCartesian(x,y,z);


    atom->setColor (other->Color());


    atomList.push_back(atom);
    getLastAtom()->setIndex(atomList.size()-1);
    return true;
}



void HMol::appendAtom(QString s,double x,double y ,double z)
{
    if(atomExisted(s,x,y,z))
        return;

    HAtom * atom = new HAtom(s,x,y,z);
    atomList.push_back(atom);
    getLastAtom()->setIndex(atomList.size()-1);

    //refresh bond information

}

void HMol::appendAtom(uint atomicNum,double x,double y ,double z)
{
    if(atomExisted(atomicNum,x,y,z))
        return;

    HAtom * atom = new HAtom(atomicNum,x,y,z);
    atomList.push_back(atom);
    getLastAtom()->setIndex(atomList.size()-1);

    //refresh bond information
}



void HMol::insertAtom(uint idx, QString s,double x,double y ,double z)
{
    if(atomExisted(s,x,y,z))
        return;

    HAtom * atom = new HAtom(s,x,y,z);

    atomList.insert(idx-1,atom);


    //refresh bond information
    for (int i=0;i<NumBonds();i++)
    {
        HBond * bond= bondList[i];
        int idx1=bond->atomIndex0();
        int idx2=bond->atomIndex1();

        if (idx1 >= idx )
            bond->setBeginIndex(idx1+1);

        if (idx2 >= idx )
            bond->setEndIndex(idx2+1);
    }

    //refresh ring information
    for (int i=0;i<numRings();i++)
    {

    }

    //update neighor information of each atom
    int j=NumBonds();
    if(j>0)
        while (j--)
        {
            //uint idx0=bondList[j]->atomIndex0();
            //uint idx1=bondList[j]->atomIndex1();

            //getAtombyIndex(idx1)->appendNeighbor(idx0);
            //getAtombyIndex(idx0)->appendNeighbor(idx1);
        }


    //calculate new bonds
    for (uint i=0;i<NumAtoms();i++)
    {

    }
}

void HMol::insertAtom(uint idx, uint atomicNum, double x,double y ,double z)
{
    HAtom * atom = new HAtom(atomicNum,x,y,z);
    atomList.insert(idx-1,atom);
}

void HMol::swapTwoAtom(uint from, uint to)
{

}


void HMol::load_mol(QString filename)
{
    clearAll();

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(0, "Warning","Cannot Open File!");
    }

    double x, y, z;
    QString  atomLabel;
    QStringList lst;

    while(!file.atEnd()) {
        QString line(file.readLine());
        if (isNumber(line)) continue;
        lst=line.split(" ",QString::SkipEmptyParts);

        if (lst.size()==9)
        {
            atomLabel=lst[3];
            x=lst[0].toDouble();
            y=lst[1].toDouble();
            z=lst[2].toDouble();

            addAtom(atomLabel,x,y,z);
            continue;
        }

        if (lst.size()==6 && atomList.size()>0)
        {
            addBond(lst[0].toInt()-1,lst[1].toInt()-1,lst[2].toInt());
        }
    }
    file.close();



    cout << "read "<<filename.toStdString().c_str() << " done!"<<endl;
}


void HMol::addBond(uint beg, uint end, uint order)
{
    if(getBondOrderbyAtomIndex(beg,end)>0) return;

    HBond * bond = new HBond(beg,end,order);

    bondList.push_back(bond);

    getAtombyIndex(beg)->appendLinkdedAtomId(end);
    getAtombyIndex(end)->appendLinkdedAtomId(beg);
}

void HMol::addBond(uint beg, uint end)
{
    if(getBondOrderbyAtomIndex(beg,end)>0) return;
    addBond(beg,end,1);
}














void HMol::perceiveBonds()
{
    for (uint i=0; i<NumAtoms();i++)
    {
        double ri=getAtombyIndex(i)->radius();

        for (uint j=i+1; j<NumAtoms();j++)
        {
            double rij=distance(i,j);
            double rj=getAtombyIndex(j)->radius();

            if ( rij > 0.7 * (ri+rj))
                continue;
            addBond(i,j,1);
        }

    }
}


void HMol::perceiveBonds(int idxofAtom)
{


}




void HMol::load_other(QString filename)
{
    clearAll();

    // Construct the OpenBabel objects, set the file type
    OBConversion conv;
    OBFormat * inFormat;


    inFormat = conv.FormatFromExt(filename.toLatin1());
    if (!inFormat || !conv.SetInFormat(inFormat))
    {
        QMessageBox::information(0, "Warning","Unknown file type!");
    }


    // Now attempt to read the molecule in
    ifstream ifs;
    ifs.open(filename.toLocal8Bit()); // This handles utf8 file names etc
    if (!ifs)  {
        QMessageBox::information(0, "Warning","Cannot Open File!");
        return;
    }


    obmol.Clear();

    if (conv.Read(&obmol, &ifs))
    {
        obmol.ToInertialFrame();
        obmol.ConnectTheDots();
        //obmol.SetAromaticPerceived();
        obmol.PerceiveBondOrders();
        obmol.SetSSSRPerceived();
        fromOBMol();
        setFileName(fileName);
    }

    obmol.Clear();
    ifs.close();
}



//obmol.SetHydrogensAdded();
void HMol::perceiveBondOrder_OpenBabel()
{
    buildOBMol();
    obmol.ConnectTheDots();
    obmol.SetAromaticPerceived();
    obmol.PerceiveBondOrders();
    //obmol.SetSSSRPerceived();
    obmol.SetLSSRPerceived();

    removeAllBonds();


    std::vector<OpenBabel::OBBond*>::iterator j;
    for (OpenBabel::OBBond *obbond = obmol.BeginBond(j); obbond; obbond = obmol.NextBond(j))
    {
        uint i=obbond->GetBeginAtom()->GetIdx()-1;
        uint j=obbond->GetEndAtom()->GetIdx()-1;


        addBond(i,j, obbond->GetBondOrder() );

        // cout << obbond->GetBeginAtom()->GetIdx()-1 <<" " << obbond->GetEndAtom()->GetIdx()-1 <<" " <<obbond->GetBondOrder()<<endl;
    }

    obmol.Clear();

    perceivePlaneBonds();
}



void HMol::perceiveBondOrder()
{
    perceiveBondOrder_OpenBabel();
    perceiveHydrogenBonds();
}


void HMol::perceiveHydrogenBonds()
{
    uint NA=NumAtoms();

    for(uint i=0;i<NA;i++)
    {
        QString A=getAtomSymbol(i);
        if( (A!="O") && (A!="F") && (A!="N") && (A!="Cl") ) return;

        vector3 p0=getAtomPosbyIndex(i);
        double l;

        for(uint j=i+1;j<NA;j++)
        {
            if(getAtomSymbol(j)!="H") continue;

            l=(p0-getAtomPosbyIndex(j)).length();

            if(l< 1.7 || l>2.5) continue;

            addBond(j,i,5);

            //getAtombyIndex(j)->appendLinkdedAtomId(i);
            //getAtombyIndex(i)->appendLinkdedAtomId(j);
        }

    }
}

void HMol::perceiveHydrogenBondId(uint id)
{
    QString A=getAtomSymbol(id);
    if( (A!="O") && (A!="F") && (A!="N") && (A!="Cl") ) return;

    vector3 p0=getAtomPosbyIndex(id);
    double l;

    for(uint i=0;i<NumAtoms();i++)
    {
        if(i=id) continue;
        if(getAtomSymbol(id)!="H") continue;

        l=(p0-getAtomPosbyIndex(i)).length();
        if(l< 1.7 || l>2.5) continue;

        addBond(id,i,5);

        getAtombyIndex(id)->appendLinkdedAtomId(i);
        getAtombyIndex(i)->appendLinkdedAtomId(id);
    }


}

void HMol::perceivePlaneBondbyId(uint id)
{
    // cout << "----------------------"<<endl;
    // displayAtoms();   displayBonds();

    if(getBondbyIndex(id)->getBondOrder()<2)
        return;

    uint idx0= getBondbyIndex(id)->atomIndex0();
    uint idx1= getBondbyIndex(id)->atomIndex1();


    vector3 norm=VZero;
    if (getBondbyIndex(id)->isInsideRing())
    {
        //cout << " inside ring \n" ;
        norm=getRingbyId(getBondbyIndex(id)->getRingId())->norm;

        vector3 v1=getAtomPosbyIndex(idx0);
        vector3 v2=getAtomPosbyIndex(idx1);
        vector3 v=v2-v1,mid0=0.5*(v1+v2);

        //update normal to ensure the dashed line inside
        uint idRing=getBondbyIndex(id)->getRingId();

        HRing  * ring = getRingbyId(idRing);

        norm=ring->norm;
        vector3 v0=ring->center;
        vector3  shift0=0.1*cross(ring->norm, v);


        //make sure: outside for the first time
        if( (mid0-shift0-v0).length() < (mid0-v0).length())
            norm=-norm;


        getBondbyIndex(id)->setPlane(norm);
        return;
    }

    //cout << " outside ring " <<endl;;
    //cout << " percept plane for "<< idx0+1 << " "<<idx1+1<<endl;


    //simple initialize
    vector3 v=getAtomPosbyIndex(idx1)-getAtomPosbyIndex(idx0);
    v.createOrthoVector(norm);
    getBondbyIndex(id)->setPlane(norm);


    // cout << getAtomPosbyIndex(idx1)<<endl;
    //cout << getAtomPosbyIndex(idx0)<<endl;
    // cout << norm<< " aaaaaaaaaaa"<<endl;


    //search
    uint idx2=1000001;

    // search those around the first atom (idx0)
    HAtom * atom=getAtombyIndex(idx0);
    if (atom->numNeighbors()>1)
    {
        //cout << "search "<< idx0+1<<endl;
        for (uint i=0;i<atom->numNeighbors();i++)
        {
            uint _id=atom->getNeighborId(i);

            // cout << i+1 << ": "<< _id<<endl;

            if (_id==idx1)  //the sampe idx0---idx1
                continue;

            if(getAtombyIndex(_id)->atomicNum()<1)  //pseudo atom
                continue;

            idx2=_id;
            break;
        }


        //find it
        if (idx2<100000)
        {
            //cout << "find " <<idx0+1<< " "<< idx2+1 << " "<< NumAtoms()<<endl;
            norm=cross(getAtomPosbyIndex(idx1)-getAtomPosbyIndex(idx0),
                       getAtomPosbyIndex(idx1)-getAtomPosbyIndex(idx2)).normalize();

            //cout << norm.x() << " "<< norm.y() <<" " << norm.z() <<endl;
            getBondbyIndex(id)->setPlane(norm);

            //cout << " outside ring end 10\n" <<endl;;
            return;
        }

        //cout<< "continue to find the second atom "<< idx1 +1<<endl;
    }

    //cout << " outside ring end 1" <<endl;;

    //failed, continue search those around another atom (idx1)
    atom=getAtombyIndex(idx1);
    idx2=1000001;
    if (atom->numLinkdedAtoms()>1) {
        for (uint i=0;i<atom->numLinkdedAtoms();i++)
        {
            uint _id=atom->getLinkdedAtomId(i);
            if (_id==idx0)   continue;

            if(getAtombyIndex(_id)->atomicNum()<1)
                continue;

            idx2=_id;
            break;
        }


        if (idx2 <100000)
        {
            norm=cross(getAtomPosbyIndex(idx1)-getAtomPosbyIndex(idx0),
                       getAtomPosbyIndex(idx1)-getAtomPosbyIndex(idx2)).normalize();

            getBondbyIndex(id)->setPlane(norm);
        }
    }
    //cout << " outside ring end2\n" <<endl;;
}



void HMol::perceivePlaneBonds()
{
    uint size=NumBonds();
    if (size<1) return;

    for (uint i=0;i<NumBonds();i++)
        perceivePlaneBondbyId(i);
}

uint HMol::getBondOrderbyAtomIndex(uint id1, uint id2)
{
    HBond * bond=nullptr;

    uint i,j;
    for (uint k=0;k<NumBonds();k++)
    {
        i=getBondbyIndex(k)->atomIndex0();
        j=getBondbyIndex(k)->atomIndex1();

        if  ( (i==id1 && j==id2 )) {
            bond=getBondbyIndex(k);
            break;
        }

        if  ( (j==id1 && i==id2 )) {
            bond=getBondbyIndex(k);
            break;
        }
    }

    if (bond) return bond->getBondOrder();
    return 0;
}


HBond * HMol::getBondbyAtomIndex(uint id1, uint id2)
{
    uint i,j;
    for (uint k=0;k<NumBonds();k++)
    {
        i=getBondbyIndex(k)->atomIndex0();
        j=getBondbyIndex(k)->atomIndex1();
        if  ( i==id1 && j==id2 )
            return getBondbyIndex(k);

        if  ( j==id1 && i==id2 )
            return getBondbyIndex(k);
    }
    return nullptr;
}

HBond *  HMol::getBondIndexbyAtomIndex(uint id1, uint id2, uint & id)
{
    uint i,j;
    for (uint k=0;k<NumBonds();k++)
    {
        id=k;
        i=getBondbyIndex(k)->atomIndex0();
        j=getBondbyIndex(k)->atomIndex1();
        if  ( i==id1 && j==id2 )
            return getBondbyIndex(k);

        if  ( j==id1 && i==id2 )
            return getBondbyIndex(k);
    }

    id=1000000001;
    return nullptr;
}



void HMol::perceiveAllAtomHybridization()
{
    for(uint i=0;i<NumAtoms();i++) {
        AtomHybridization hybridization=perceiveHybridization(i);
        getAtombyIndex(i)->setHybridization(hybridization);

        if (hybridization==SP2 || hybridization==SP)
            getAtombyIndex(i)->setAromatic(true);
        else
            getAtombyIndex(i)->setAromatic(false);
    }
}

AtomHybridization HMol::perceiveHybridization(uint id)
{
    uint numberOfBonds=getAtombyIndex(id)->numNeighbors();
    AtomHybridization hybridization = SP3; // default to sp3

    // TODO: Handle hypervalent species, SO3, SO4, lone pairs, etc.
    if (numberOfBonds > 4) {
        //      hybridization = numberOfBonds; // e.g., octahedral, trig. bipyr.,
        hybridization=Unknown;
    }
    else
    {
        // Count multiple bonds
        unsigned int numTripleBonds = 0;
        unsigned int numDoubleBonds = 0;
        unsigned int numAromaticBonds = 0;

        for (uint i=0;i<numberOfBonds;i++)
        {
            uint bo=getBondOrderbyAtomIndex(id,getAtombyIndex(id)->getNeighborId(i));

            if (bo == 2)    numDoubleBonds++;
            else if (bo == 3)   numTripleBonds++;
            else if (bo == 6)   numAromaticBonds++;
        }


        if (numTripleBonds > 0 || numDoubleBonds > 1)
            hybridization = SP;
        else if (numDoubleBonds > 0)
            hybridization = SP2;
        else if (numAromaticBonds > 0)
            hybridization = SP2;

    }

    return hybridization;
}

void HMol::perceiveRings_OB()
{
    removeAllRings();
    if (NumBonds() < 3)  return;
    buildOBMol();

    std::vector<OpenBabel::OBRing *> obrings;
    obmol.SetAtomTypesPerceived(true);
    obmol.SetAromaticPerceived(true);
    obmol.PerceiveBondOrders();
    obmol.EndModify();
    obrings = obmol.GetSSSR();


    OpenBabel::vector3  center, norm1, norm2;

    foreach(OpenBabel::OBRing *r, obrings)
    {
        uint nAtoms=r->Size();

        HRing * ring = new HRing();
        ring->atomIdList.clear();

        std::vector<int>::iterator j;
        for(j = r->_path.begin(); j != r->_path.end(); ++j)
            ring->atomIdList.push_back( (*j)-1);

        r->findCenterAndNormal(center,norm1,norm2);
        ring->norm.Set(norm2.x(),norm2.y(),norm2.z());
        ring->center.Set(center.x(),center.y(),center.z());

        uint size=r->Size();

        //radius
        double radius=50.0;
        double maxR=0.0;
        for (uint i=0;i<size;i++)
        {
            double distToCenter=(getAtomPosbyIndex(ring->atomIdList[i])-ring->center).length();

            if(distToCenter < radius)
                radius = distToCenter;

            if(maxR<distToCenter)
                maxR=distToCenter;
        }

        //aromatic?
        bool aromatic=true;
        for (uint i=0;i<size;i++)
        {
            if(getAtombyIndex(ring->atomIdList[i])->isAromatic()) {
                //cout << ring->atomIdList[i]+1 <<" : " << "aromatic!"<<endl;
                continue;
            }

            aromatic=false;
            //cout << ring->atomIdList[i]+1 <<" : " << "not aromatic!"<<endl;
            break;
        }

        //remove large ring;
        cout << maxR << " "<<radius<<endl;

        if(fabs(maxR-radius)>0.3){
            delete ring;
            continue;
        }

        if(!aromatic) {
            delete ring;
            continue;
        }

        ring->size=ring->atomIdList.size();
        ring->radius=radius;
        ring->scale=1.0;
        ring->opacity=1.0;
        ring->type=0;
        ring->color.Set(0.65,0.65,0.65);
        ring->isAromatic=aromatic;//r->IsAromatic();
        ringList.push_back(ring);
    }

    //displayRingList();

    //update bond information, and set them all aromatic
    if (bondList.size() > 2)
    {
        vector <int> ringBondList;
        bool isAllsingleBonded=false;

        for (unsigned i=0;i<bondList.size();i++)
        {
            bondList[i]->setRingId(10000001);
            if(ringList.size()<1) continue;

            uint id0=bondList[i]->atomIndex0();
            uint id1=bondList[i]->atomIndex1();

            bool find1=false, find2=false;

            for (uint k=0;k<ringList.size();k++)
            {
                for (uint l=0;l<ringList[k]->atomIdList.size();l++)
                {
                    if(id0==ringList[k]->atomIdList[l])
                        find1=true;

                    if(id1==ringList[k]->atomIdList[l])
                        find2=true;

                    if(find1 && find2)
                    {
                        bondList[i]->setRingId(k);
                        bondList[i]->setAromatic();
                        if(bondList[i]->getBondOrder()>1)
                            isAllsingleBonded=false;
                        //bondList[i]->setBondOrder(6);
                        ringBondList.push_back(i);
                        break;
                    }
                }
                if(find1 && find2)   break;
            }
        }


        if(isAllsingleBonded) return;
        for(int i=0;i<ringBondList.size();i++) {
            bondList[ringBondList[i]]->setBondOrder(6);
        }
    }
}


void HMol::addRing (vector <uint> idList,
                    vector3 ringCenter, vector3 ringNormal,
                    double ringRadius,double ringScale,
                    vector3 color, bool ringAromatic)
{
    if (idList.size()<3)
        return;

    HRing * ring = new HRing();
    ring->atomIdList.clear();

    for(uint i=0; i<idList.size();i++)
        ring->atomIdList.push_back( idList[i]);

    ring->norm.Set(ringNormal.x(),ringNormal.y(),ringNormal.z());
    ring->center.Set(ringCenter.x(),ringCenter.y(),ringCenter.z());
    ring->radius=ringRadius;
    ring->type=0;
    ring->opacity=1.0;
    ring->color=color;
    ring->scale=ringScale;
    ring->isAromatic=ringAromatic;

    ringList.push_back(ring);
}

void HMol::addRing(vector <uint> idList)
{
    uint size=idList.size();

    HRing * ring = new HRing();
    ring->atomIdList.clear();

    vector3 center=VZero;
    for(uint j = 0; j < size; j++) {
        ring->atomIdList.push_back(idList[j]);
        center=center+getAtomPosbyIndex(idList[j]);
    }
    center=center/size;

    vector3 norm=VZ;
    norm=cross(getAtomPosbyIndex(idList[0])-getAtomPosbyIndex(idList[1]),
            getAtomPosbyIndex(idList[0])-getAtomPosbyIndex(idList[2]));
    norm.normalize();

    ring->norm.Set(norm.x(),norm.y(),norm.z());
    ring->center.Set(center.x(),center.y(),center.z());
    ring->scale=1.0;
    ring->opacity=1.0;
    ring->type=0;
    ring->color.Set(0.65,0.65,0.65);

    //radius
    double radius=50.0;
    for (uint i=0;i<size;i++)
    {
        double distToCenter=(getAtomPosbyIndex(idList[i])-center).length();
        if(distToCenter < radius)
            radius = distToCenter;
    }

    bool aromatic=true;

    /*
    //aromatic?
    for (uint i=0;i<size;i++)
    {
        if(getAtombyIndex(ring->atomIdList[i])->isAromatic()) {
            //cout << ring->atomIdList[i]+1 <<" : " << "aromatic!"<<endl;
            continue;
        }

        aromatic=false;
        //cout << ring->atomIdList[i]+1 <<" : " << "not aromatic!"<<endl;
        break;
    }

    if(!aromatic) {   delete ring;      return;     }
*/
    ring->size=ring->atomIdList.size();
    ring->radius=radius;
    ring->isAromatic=aromatic;
    ringList.push_back(ring);


    //update bond information
    uint k=ringList.size()-1;
    for (uint i=0;i<size-1;i++)
        getBondbyAtomIndex(idList[i],idList[i+1])->setRingId(k) ;
    getBondbyAtomIndex(idList[0],idList[size-1])->setRingId(k) ;
}





void HMol::displayRingList()
{
    uint size=numRings();
    if (size<1) return;

    cout << endl;
    cout << "Rings inside molecule:"<<endl;
    for (uint k=0;k<size;k++)
    {
        cout << k+1<< " "<< ringList[k]->atomIdList.size() << ": ";
        for (uint l=0;l<ringList[k]->atomIdList.size();l++)
            cout << ringList[k]->atomIdList[l]+1<< " ";
        cout << ", normal :"<< ringList[k]->norm<< " ";
        cout << ", center :"<< ringList[k]->center<< " ";
        cout << ", isAromatic:"<<ringList[k]->isAromatic<< " ";

        cout << endl;
    }
    cout << endl;
}

void HMol::displayArcList()
{
    uint size=numArcs();
    if (size<1) return;

    cout << endl;
    cout << "Rings inside molecule:"<<endl;
    for (uint k=0;k<size;k++)
    {
        cout << k+1<< " "<< arcList[k]->atomIdList.size() << ": ";
        for (uint l=0;l<arcList[k]->atomIdList.size();l++)
            cout << arcList[k]->atomIdList[l]+1<< " ";
        cout << ", normal :"<< arcList[k]->norm<< " ";
        cout << ", center :"<< arcList[k]->center<< " ";
        cout << ", isAromatic:"<<arcList[k]->isAromatic<< " ";

        cout << endl;
    }
    cout << endl;
}





//trans to OBMol
void HMol::buildOBMol()
{
    obmol.Clear();
    obmol.BeginModify();

    // X atom is not existed
    if(isXExisted())
        return;


    uint idx=0;
    foreach(HAtom * atom, atomList)
    {
        OpenBabel::OBAtom *a = obmol.NewAtom();

        a->Clear();
        a->SetVector(atom->x(), atom->y(), atom->z());
        a->SetAtomicNum(atom->atomicNum());
        a->SetType(atom->Symbol().toStdString().c_str());
        a->SetId(idx+1);

        //cout << obmol.NumAtoms()<<endl;

        idx++;
    }

    //copy bonds
    if (NumBonds()>0) {
        QVector<HBond *>::iterator iter1;
        for (iter1=bondList.begin();iter1!=bondList.end();iter1++)
        {
            //5=aromatic inside openbabel
            uint bo=(*iter1)->getBondOrder();
            if (bo==6) bo=5;
            obmol.AddBond((*iter1)->atomIndex0() + 1,
                          (*iter1)->atomIndex1() + 1, bo);
        }
    }

    obmol.EndModify(true);
    return;


    /*
    // X atom existed
    //filter X atoms
    //removeAllXAtoms();

    for(uint i=0;i<NumAtoms();i++)
    {
        if(getAtomSymbol(i)!="X") continue;


        HXAtom * xatom=new HXAtom;
        xatom->id=i;
        xatom->pos=getAtomPosbyIndex(i);

        for(uint j=0;j<getAtombyIndex(i)->numNeighbors();j++)
            xatom->neighbor.push_back(getAtombyIndex(i)->getNeighborId(j));

        for(uint j=0;j<NumAtoms();j++)
        {
            xatom->atomIdList.push_back(j);
        }
    }

    uint idx=-1;
    uint times=0;
    vector <uint > idX;
    notXIdx.clear();
    foreach(HAtom * atom, atomList)
    {
        idx+=1;
        if(atom->Symbol()=="X")  {
            idX.push_back(idx);
            continue;
        }
        notXIdx.push_back(idx);

        OpenBabel::OBAtom *a = obmol.NewAtom();
        a->Clear();
        a->SetVector(atom->x(), atom->y(), atom->z());
        a->SetAtomicNum(atom->atomicNum());
        a->SetType(atom->Symbol().toStdString().c_str());
        a->SetId(idx+1-idX.size());
    }


    QVector<HBond *>::iterator iter1;
    for (iter1=bondList.begin();iter1!=bondList.end();iter1++)
    {
        //5=aromatic inside openbabel
        uint bo=(*iter1)->getBondOrder();
        if (bo==6) bo=5;
        obmol.AddBond((*iter1)->atomIndex0() + 1,
                      (*iter1)->atomIndex1() + 1, bo);
    }


    obmol.EndModify(true);

    */


    //obmol.SetTotalSpinMultiplicity(1);
    // obmol.SetPartialChargesPerceived();
}

bool HMol::writeMol(const QString &fileName, const QString &fileType)
{

    // Check is we are replacing an existing file
    QFile file(fileName);
    bool replaceExistingFile = file.exists();

    if (!file.open(QFile::WriteOnly | QFile::Text))
        return false;

    file.close();

    QString newFileName = fileName;
    if (replaceExistingFile)
    {
        newFileName += ".new";
        QFile newFile(newFileName);
        if (!newFile.open(QFile::WriteOnly | QFile::Text)) {
            return false;
        }
        newFile.close();
    }

    // Construct the OpenBabel objects, set the file type
    OBConversion conv;
    OBFormat *outFormat;
    if (!fileType.isEmpty() && !conv.SetOutFormat(fileType.toLatin1()))
        return false;
    else {
        outFormat = conv.FormatFromExt(fileName.toLatin1());
        if (!outFormat || !conv.SetOutFormat(outFormat))
            return false;
    }

    // Now attempt to write the molecule in
    ofstream ofs;
    ofs.open(newFileName.toLocal8Bit()); // This handles utf8 file names etc
    if (!ofs) {// Should not happen, already checked file could be opened
        return false;
    }

    buildOBMol();

    OpenBabel::OBChainsParser chainparser;
    obmol.UnsetFlag(OB_CHAINS_MOL);
    chainparser.PerceiveChains(obmol);

    if (conv.Write(&obmol, &ofs)) {
        ofs.close();
        if (replaceExistingFile) {
            QFile newFile(newFileName);
            bool success;
            success = file.rename(fileName + ".old");
            if (success) {
                // Leave to ensure we work around a bug in Qt < 4.5.1
                file.setFileName(fileName + ".old");
                success = newFile.rename(fileName);
            }
            else {
                return false;
            }
            if (success) // renaming worked
                success = file.remove(); // remove the old file: WARNING -- would much prefer to just rename, but Qt won't let you
            else {
                return false;
            }

            if (success) {
                return true;
            }
            else {
                return false;
            }
        }
        else // No need for all that - this is a new file in an empty location
            return true;
    }
    else {
        return false;
    }
    // Assume something went wrong if we did not return true earlier
    //qDebug() << "OBWrapper should never get here...";
    return false;
}


bool HMol::fromOBMol()
{
    clearAll();

    // Begin by copying all of the atoms
    std::vector<OpenBabel::OBAtom*>::iterator i;
    for (OpenBabel::OBAtom *obatom = obmol.BeginAtom(i); obatom; obatom = obmol.NextAtom(i))
    {

        HAtom * atom = nullptr;

        //avoiding NaN
        if( isfinite(obatom->GetX()) )
            atom = new HAtom(obatom->GetAtomicNum(),obatom->GetX(),obatom->GetY(),obatom->GetZ());
        else
            atom = new HAtom(obatom->GetAtomicNum(),0.0,0.0,0.0);

        //cout << atom->radius()<<endl;
        if(atom)
            atomList.push_back(atom);
    }

    // Now bonds, we use the indices of the atoms to get the bonding right
    std::vector<OpenBabel::OBBond*>::iterator j;
    for (OpenBabel::OBBond *obbond = obmol.BeginBond(j); obbond; obbond = obmol.NextBond(j))
    {
        uint i=obbond->GetBeginAtom()->GetIdx()-1;
        uint j=obbond->GetEndAtom()->GetIdx()-1;
        HBond * bond = new HBond(i,j, obbond->GetBondOrder());

        if(obbond->GetBondOrder()==5)   bond->setBondOrder(6);
        else        bond->setBondOrder(obbond->GetBondOrder());

        bondList.push_back(bond);

        getAtombyIndex(i)->appendLinkdedAtomId(j);
        getAtombyIndex(j)->appendLinkdedAtomId(i);
    }

    //perceiveAllAtomHybridization();

    perceiveRings();
    perceivePlaneBonds();
    perceiveArcs();

    return true;
}

void HMol::tackleTail(uint idx)
{
    uint numBs=getAtombyIndex(idx)->numNeighbors();

    for (uint i=0;i<numBs;i++)
    {

    }


}

void HMol::tackleHead(uint idx)
{
    uint numBs=getAtombyIndex(idx)->numNeighbors();

    uint id;
    uint bo;
    for (uint i=0;i<numBs;i++)
    {
        id=getAtombyIndex(idx)->getLinkdedAtomId(i);
        if(id==candidate[1]) continue;

        bo=getBondOrderbyAtomIndex(idx,id);

        if(bo==1) continue;
        if(bo==5) continue;
        if(bo==7) continue;
        candidate.push_front(id);
    }
}


void HMol::perceiveArcs()
{
    for (uint i=0;i<NumBonds();i++)
    {
        candidate.clear();
        uint bo=bondList[i]->getBondOrder();

        if(bo==1) continue;
        if(bo==5) continue;
        if(bo==7) continue;

        //only double, triple and aromatic bonds are left
        candidate.push_back(bondList[i]->atomIndex0());
        candidate.push_back(bondList[i]->atomIndex1());


        tackleHead(candidate[0]);
        tackleTail(candidate[candidate.size()-1]);
    }

}

//mass-weighted
void HMol::perceptMoleculeShape()
{
    centeralize();

    double Ixx=0.0, Ixy=0.0, Ixz=0.0,
            Iyy=0.0, Iyz=0.0,
            Izz=0.0;
    double x, y, z;
    //                  | Ixx     Ixy     Ixz |
    //          I   =   | Iyx     Iyy     Iyz |
    //                  | Izx     Izy     Izz |
    foreach(HAtom * atom, atomList)
    {
        x = atom->x();
        y = atom->y();
        z = atom->z();

        Ixx += (y*y + z*z)*atom->Mass();
        Iyy += (x*x + z*z)*atom->Mass();
        Izz += (x*x + y*y)*atom->Mass();
        Ixy -= (x*y)*atom->Mass();
        Ixz -= (x*z)*atom->Mass();
        Iyz -= (y*z)*atom->Mass();
    }

    double I[9] = {Ixx,Ixy,Ixz,Ixy,Iyy,Iyz,Ixz,Iyz,Izz};

    matrix3x3 InertialMomentMatrix(I);
    vector3 IMoment;
    IMomentMatrix= InertialMomentMatrix.findEigenvectorsIfSymmetric(IMoment);


    double a=IMoment[0],
            b=IMoment[1],
            c=IMoment[2];


    double eps=2.0*c/1000.0; //0.3% error
    cout << "The tolerance of InertialMoment:  " << eps << endl;


    //Ia < Ib < Ic
    molShape="Irregular";

    //Sphere: Ia = Ib = Ic
    if ( (abs(a-b) < eps) && (abs(b-c) < eps) && (abs(a-c) < eps) )
        molShape="Sphere";

    //Line   : Ia =0.0 and Ia < Ib = Ic
    else if ( abs(a)< eps)
        molShape="Line";

    //Prolate: Ia < Ib = Ic
    else if ( abs(b-c)< eps && (a < c) && abs(a-c) > eps )
        molShape="Prolate";


    //Polygon an Plane: Ia=Ib and Ia+Ib=Ic
    else if ( abs(a+b-c) < eps)
    {
        if ( abs(a-b)< eps ) // Ia=Ib
            molShape="Polygon";
        else
            molShape="Plane";  //Ia!=Ib
    }

    //Oblate:  Ia = Ib < Ic
    else if ( abs(a-b)< eps && (b<c) )
        molShape="Oblate";
}


//tanslate to centre of masss and reorient to principal axes of inertia
void HMol::standardOrient()
{
    //Translate centre of mass to coordinate origin
    centeralize();

    double Ixx=0.0, Ixy=0.0, Ixz=0.0,
            Iyy=0.0, Iyz=0.0,
            Izz=0.0;
    double x, y, z;
    //                  | Ixx     Ixy     Ixz |
    //          I   =   | Iyx     Iyy     Iyz |
    //                  | Izx     Izy     Izz |
    foreach(HAtom * atom, atomList)
    {
        x = atom->x();
        y = atom->y();
        z = atom->z();

        Ixx += (y*y + z*z)*atom->Mass();
        Iyy += (x*x + z*z)*atom->Mass();
        Izz += (x*x + y*y)*atom->Mass();
        Ixy -= (x*y)*atom->Mass();
        Ixz -= (x*z)*atom->Mass();
        Iyz -= (y*z)*atom->Mass();
    }

    double I[9] = {Ixx,Ixy,Ixz,Ixy,Iyy,Iyz,Ixz,Iyz,Izz};

    matrix3x3 InertialMomentMatrix(I);
    vector3 IMoment;
    IMomentMatrix= InertialMomentMatrix.findEigenvectorsIfSymmetric(IMoment);


    double a=IMoment[0],
            b=IMoment[1],
            c=IMoment[2];


    double eps=2.0*c/1000.0; //0.3% error
    cout << "The tolerance of InertialMoment:  " << eps << endl;


    vector3 principal;
    //Ia < Ib < Ic
    molShape="Irregular";

    //Sphere: Ia = Ib = Ic
    if ( (abs(a-b) < eps) && (abs(b-c) < eps) && (abs(a-c) < eps) )
        molShape="Sphere";

    //Line   : Ia =0.0 and Ia < Ib = Ic
    else if ( abs(a)< eps) {
        molShape="Line";
        principal.Set(IMomentMatrix(0,0),IMomentMatrix(1,0),IMomentMatrix(2,0));
    }

    //Prolate: Ia < Ib = Ic
    else if ( abs(b-c)< eps && (a < c) && abs(a-c) > eps ) {
        molShape="Prolate";
        principal.Set(IMomentMatrix(0,0),IMomentMatrix(1,0),IMomentMatrix(2,0));
    }


    //Polygon an Plane: Ia=Ib and Ia+Ib=Ic
    else if ( abs(a+b-c) < eps)
    {
        if ( abs(a-b)< eps ) // Ia=Ib
            molShape="Polygon";
        else
            molShape="Plane";  //Ia!=Ib

        principal.Set(IMomentMatrix(0,2),IMomentMatrix(1,2),IMomentMatrix(2,2));
    }

    //Oblate:  Ia = Ib < Ic
    else if ( abs(a-b)< eps && (b<c) ) {
        molShape="Oblate";
        principal.Set(IMomentMatrix(0,2),IMomentMatrix(1,2),IMomentMatrix(2,2));
    }


    if(molShape=="Sphere") return;
    //reOrient the molecule along the principal axes of inertia

    matrix3x3 trans;
    trans.SetupRotateMatrix(VZ, principal);
    rotate(trans);
}



void HMol::calcMolVolume()
{
    double x=0.0,y=0.0,z=0.0;
    double x0=0.0,y0=0.0,z0=0.0;

    for (uint i=0;i<NumAtoms();i++)
    {
        x0 = fabs(getAtomXbyIndex(i));
        y0 = fabs(getAtomYbyIndex(i));
        z0 = fabs(getAtomZbyIndex(i));

        if (x < x0) x=x0;
        if (y < y0) y=y0;
        if (z < z0) z=z0;
    }

    //cout << x <<" "<< y <<" "<< z <<" inside calcMolVolume()"<<endl;
    xyzLength.Set(x,y,z);
    molLength=xyzLength.length();
}






void HMol::findChildren(vector<int> &children,int first,int second)
{
    children.clear();

    //check it is terminal one or not?
    if(getAtombyIndex(second)->numLinkdedAtoms()==1){
        children.push_back(first);
        return;
    }


    buildOBMol();



    obmol.FindChildren(children,first+1,second+1);

    if(children.size()<1) return;
    for(uint i=0;i<children.size();i++)
        children[i]=children[i]-1;
}

void HMol::displayBonds()
{
    cout << "\n\nBonds of the molecule:"<<endl;
    for (uint i=0;i<NumBonds();i++) {
        cout << i+1 <<": "<<getBondbyIndex(i)->atomIndex0()+1<< "<->";
        cout << getBondbyIndex(i)->atomIndex1()+1<<"==";
        cout << getBondbyIndex(i)->getBondOrder()<<"  ";
        cout << getBondbyIndex(i)->getPlane()<< "  ";
        cout << getBondbyIndex(i)->getRingId() << endl;
    }
    cout << endl<< endl;
}

void HMol::displayAtoms()
{
    uint numAtoms=NumAtoms();
    double x,y,z;
    for (uint i=0;i<numAtoms;i++)
    {
        x=getAtomXbyIndex(i);
        y=getAtomYbyIndex(i);
        z=getAtomZbyIndex(i);

        cout << i+1 <<": "<<getAtomSymbol(i).toStdString().c_str()<< " "<<x << " "<<y<<"  "<<z<< ":  ";
        for (uint j=0;j<getAtombyIndex(i)->numLinkdedAtoms();j++)
            cout << getAtombyIndex(i)->getLinkdedAtomId(j)+1<<"    ";
        cout << endl;
    }
}


//put to a empty place
void HMol::addAtomfromTemplate(templateMol * tmol, vector3 p)
{
    uint numAtomsT=tmol->numAtoms();
    //uint numBondsT=tmol->numBonds();

    uint numAtoms=NumAtoms();
    //uint numBonds=NumBonds();

    //cout << "hit position: "<<p.x() <<" "<<p.y() <<" "<<p.z() <<endl;
    //cout << "Atoms and Bonds of Current Molcule "<<endl;
    //cout << numAtoms <<" "<<numBonds<<endl;
    //cout << "Atoms and Bonds of Template "<<endl;
    //cout << numAtomsT <<" "<<numBondsT<<endl;

    //atom existed
    if(numAtoms>0)
    {
        //check very near atom
        double minX=10.0,minY=10.0,minZ=10.0,
                maxX=-10.0,maxY=-10.0,maxZ=-10.0;

        double tminX=10.0,tminY=10.0,tminZ=10.0,
                tmaxX=-10.0,tmaxY=-10.0,tmaxZ=-10.0;

        double distX,distY,distZ;

        double x,y,z;


        //get max and min x,y,z of molecule
        //vector3 mCenter=VZero;
        for (uint i=0;i<numAtoms;i++)
        {
            //cout <<i+1<<endl;
            //mCenter += atomPos(i);

            x=getAtomXbyIndex(i);
            y=getAtomYbyIndex(i);
            z=getAtomZbyIndex(i);

            if(x < minX) minX=x;
            if(y < minY) minY=y;
            if(z < minZ) minZ=z;

            if(x > maxX) maxX=x;
            if(y > maxY) maxY=y;
            if(z > maxZ) maxZ=z;
        }


        //get max and min x,y,z of template
        vector3 v;
        for (uint i=0;i<numAtomsT;i++)
        {
            v=tmol->getPos(i) + p;
            x=v.x();
            y=v.y();
            z=v.z();

            if(x < tminX) tminX=x;
            if(y < tminY) tminY=y;
            if(z < tminZ) tminZ=z;

            if(x > tmaxX) tmaxX=x;
            if(y > tmaxY) tmaxY=y;
            if(z > tmaxZ) tmaxZ=z;
        }

        distX= 2.0+fabs(tminX - maxX);
        distY= 2.0+fabs(tminY - maxY);
        distZ= 2.0+fabs(tminZ - maxZ);

        if (distX < distY  && distX < distZ)
            p.SetX(distX+p.x());

        if (distY < distX  && distY < distZ)
            p.SetY(distY+p.y());

        if (distZ < distX  && distZ < distY)
            p.SetZ(distZ+p.z());


        for (uint i=0;i<numAtomsT;i++)
            addAtom(tmol->getSymbol(i),tmol->getPos(i)+p);

        uint beg=numAtoms;
        for (uint i=0;i<tmol->numBonds();i++)
        {
            uint idx0=tmol->getBond(i)->atomIndex0()+beg;
            uint idx1=tmol->getBond(i)->atomIndex1()+beg;

            addBond(idx0,idx1,tmol->getBond(i)->getBondOrder());

            //getAtombyIndex(idx0)->appendLinkdedAtomId(idx1);
            //getAtombyIndex(idx1)->appendLinkdedAtomId(idx0);
        }
    }
    else {//empty molecule,  copy
        for (uint i=0;i<numAtomsT;i++)
            addAtom(tmol->getSymbol(i),tmol->getPos(i)+p);

        for (uint i=0;i<tmol->numBonds();i++) {
            uint idx0=tmol->getBond(i)->atomIndex0();
            uint idx1=tmol->getBond(i)->atomIndex1();
            addBond(idx0,idx1,tmol->getBond(i)->getBondOrder());

            //getAtombyIndex(idx0)->appendLinkdedAtomId(idx1);
            //getAtombyIndex(idx1)->appendLinkdedAtomId(idx0);
        }

    }

    //recalculate bond plane
    perceivePlaneRingforBonds();

    displayAtoms();
}




void HMol::linkTemplate2Mol(templateMol * tmol, uint idx0, uint idx0_t)
{
    //  uint numAtomsT=tmol->numAtoms();
    //  uint numBondsT=tmol->numBonds();

    //  uint numAtoms=NumAtoms();
    //  uint numBonds=NumBonds();


    if(getAtombyIndex(idx0)->numLinkdedAtoms()<1) return;
    uint idx1=getAtombyIndex(idx0)->getLinkdedAtomId(0);
    vector3 vec1=getAtomPosbyIndex(idx0)-getAtomPosbyIndex(idx1);

    //linked atom of molecule (the Hydrogen must be replaced)
    double radius1 = getAtombyIndex(idx1)->radius();


    //the linker of template must be hydrogen

    //find the linked atom of Template
    uint  idx1_t=tmol->getAtombyIndex(idx0_t)->getLinkdedAtomId(0); //only one bonded atom
    double radius2 = tmol->getAtombyIndex(idx1_t)->radius();
    vector3 vec2=tmol->getAtomPosbyIndex(idx1_t)-tmol->getAtomPosbyIndex(idx0_t);

    //pseudo atom X/Bq
    if (radius2<0.5) radius2=2.5;

    vec1.normalize();
    vec2.normalize();


    //remove the hit hydrogen of current molecule
    // cout << idx0+1<< "  "<< idx1+1 <<" inside linkTemplate2Mol::linkTemplate2Mol"<<endl;
    //cout << idx0_t+1<< "  "<< idx1_t+1 <<" inside linkTemplate2Mol::linkTemplate2Mol"<<endl;


    double dist=radius1+radius2;
    vector3 p1=getAtomPosbyIndex(idx1);
    vector3 trans = vec1*dist+p1;


    vec2=tmol->getAtomPosbyIndex(idx1_t)-tmol->getAtomPosbyIndex(idx0_t);
    vec2.normalize();
    double angle= vectorAngle(vec2, vec1);


    /*
   //cout << vec2.x()<< " "<< vec2.y()<< " "<< vec2.z()<< endl;
   //cout << vec1.x()<< " "<< vec1.y()<< " "<< vec1.z()<< endl;


   cout <<  " from: " << vec2.x()<< " "<< vec2.y()<< " "<< vec2.z()<< endl;
   cout <<  " to: "<< vec1.x()<< " "<< vec1.y()<< " "<< vec1.z()<< endl;
   cout <<  " with angle :"<<  angle << endl<< endl;




   cout <<  endl<< endl;
   cout << idx0+1<< "  "<< idx1+1 <<" inside linkTemplate2Mol"<<endl;
   cout << idx0_t+1<< "  "<< idx1_t+1 <<" inside linkTemplate2Mol"<<endl;



   vector3 d0=getAtomPosbyIndex(idx0),
           d1=getAtomPosbyIndex(idx1);

   vector3 e0=tmol->getAtomPosbyIndex(idx0_t),
           e1=tmol->getAtomPosbyIndex(idx1_t);

   cout <<"\n\n";

   cout <<"molecule:\n";
   cout <<   d0.x()<< " "<< d0.y()<< " "<< d0.z()<< endl;
   cout <<   d1.x()<< " "<< d1.y()<< " "<< d1.z()<< endl;

   cout <<"template:\n";
   cout <<   e0.x()<< " "<< e0.y()<< " "<< e0.z()<< endl;
   cout <<   e1.x()<< " "<< e1.y()<< " "<< e1.z()<< endl;

   //cout << "\n remove the hydrogen of molecule inside HMol::linkTemplate2Mol " << idx0 <<endl;
*/

    removeAtombyId(idx0);

    //updateAllAtomLinker();
    //displayAtoms();
    //displayBonds();

    uint beg=NumAtoms();

    vector3 a, b , p0;
    if ( angle < 3.0)
    {
        //cout << "positive  "<<endl;

        p0=tmol->getAtomPosbyIndex(idx1_t);
        for (uint i=0;i<tmol->numAtoms();i++)
        {
            if (i==idx0_t) continue;
            a=tmol->getAtomPosbyIndex(i)-p0;
            addAtom(tmol->getSymbol(i),a+trans);
        }
    }
    else if ( fabs (angle-180.0) < 3.0)
    {
        //cout << "negtive  "<<endl;
        p0=tmol->getAtomPosbyIndex(idx1_t);
        for (uint i=0;i<tmol->numAtoms();i++)
        {
            if (i==idx0_t) continue;
            a=tmol->getAtomPosbyIndex(i)-p0;
            addAtom(tmol->getSymbol(i),-a+trans);
        }
    }
    else {
        //cout << "normal angle"<<endl;
        p0=tmol->getAtomPosbyIndex(idx1_t);
        matrix3x3 m;
        m.SetupRotateMatrix(vec2, vec1);

        //  cout <<m(0,0)<< " "<< m(0,1)<< " "<< m(0,2)<< endl;
        //  cout <<m(1,0)<< " "<< m(1,1)<< " "<< m(1,2)<< endl;
        //  cout <<m(2,0)<< " "<< m(2,1)<< " "<< m(2,2)<< endl;

        for (uint i=0;i<tmol->numAtoms();i++)
        {
            //skip the hydrogen labelled to linker
            if (i==idx0_t) continue;
            a=tmol->getAtomPosbyIndex(i)-p0;

            b.SetX(  a.x()*m(0,0)  + a.y()*m(0,1) + a.z()*m(0,2)  );
            b.SetY(  a.x()*m(1,0)  + a.y()*m(1,1) + a.z()*m(1,2)  );
            b.SetZ(  a.x()*m(2,0)  + a.y()*m(2,1) + a.z()*m(2,2)  );

            addAtom(tmol->getSymbol(i),b+trans);
        }
    }




    // cout << "adding " <<tmol->numAtoms()-1 << " atoms from template"<<endl;
    // cout << "there are "<< NumAtoms()<< " atoms now!"<<endl;




    //the new bond between two fragment
    if (idx1_t > idx0_t)
        addBond(idx1,beg+idx1_t-1,1);
    else
        addBond(idx1,beg+idx1_t,1);

    for (uint j=0;j<tmol->numBonds();j++)
    {
        uint id0=tmol->getBond(j)->atomIndex0();
        uint id1=tmol->getBond(j)->atomIndex1();

        //skip the bond of the highlight hydrogen
        if(id0==idx0_t || id1==idx0_t) continue;

        // cout << "old: " << id0+1<< " "<<id1+1<< endl;

        if(id0 > idx0_t) id0=id0-1;
        if(id1 > idx0_t) id1=id1-1;

        id0=beg+id0;
        id1=beg+id1;

        uint bo=tmol->getBond(j)->getBondOrder();
        addBond(id0,id1,bo);
    }
    updateAllAtomLinker();

    perceivePlaneRingforBonds();
}

void HMol::perceivePlaneRingforBonds()
{
    perceiveRings();
    perceivePlaneBonds();
}

void HMol::updateAllAtomLinker()
{
    if(NumAtoms()<1) return;

    for (uint i=0;i<NumAtoms();i++)
    {
        getAtombyIndex(i)->clearLinkedAtomId();
    }


    for (uint j=0;j<NumBonds();j++)
    {
        uint id0=getBondbyIndex(j)->atomIndex0();
        uint id1=getBondbyIndex(j)->atomIndex1();

        getAtombyIndex(id0)->appendLinkdedAtomId(id1);
        getAtombyIndex(id1)->appendLinkdedAtomId(id0);
    }
}


void  HMol::updateAtomLinkerId(uint idx)
{
    getAtombyIndex(idx)->clearLinkedAtomId();

    for (uint j=0;j<NumBonds();j++)
    {
        uint id0=getBondbyIndex(j)->atomIndex0();
        uint id1=getBondbyIndex(j)->atomIndex1();

        if(id0==idx)
            getAtombyIndex(idx)->appendLinkdedAtomId(id1);
        if(id1==idx)
            getAtombyIndex(idx)->appendLinkdedAtomId(id0);
    }
}

void HMol::cleanMol()
{
    runMolecularMechanics();
}




void HMol::runXTB()
{
    QString path="/home/zhangfq/tmp/";
    QString fileName="zfq.xyz";
    system ("rm /home/zhangfq/tmp/* -rf ");

    //prepare xtb xyz
    QFile file(path+fileName);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;

    uint numAtoms=NumAtoms();

    QString str2;
    str2.sprintf("%3d\n", numAtoms);
    file.write(str2.toUtf8());
    file.write("run xtb by zhangfq\n");

    double x,y,z;
    for (uint i=0;i<numAtoms;i++)
    {
        QString asymbol=getAtomSymbol(i);
        x=getAtomXbyIndex(i);
        y=getAtomYbyIndex(i);
        z=getAtomZbyIndex(i);

        str2.sprintf("%3s %10.5f  %10.5f  %10.5f\n", asymbol.toStdString().c_str(), x,y,z);
        file.write(str2.toUtf8());
    }
    file.write("\n\n");
    file.close();

    //run xtb
    // QProcess process;
    // process.execute("cd /home/zhangfq/tmp; xtb zfq.xyz -opt > zfq.log ");

    //QString output="";
    //output+=process->readAll();

    //cout << output.toStdString().c_str();


    /*
    //collect result
    QFile log("/home/zhangfq/tmp/xtbopt.xyz");
    if(!log.open(QIODevice::ReadWrite | QIODevice::Text))
        return;

    QString  atomLabel;
    QStringList lst;

    while(!log.atEnd())
    {
        QString line(file.readLine());
        if (IsNumber(line)) continue;
        lst=line.split(" ",QString::SkipEmptyParts);
        if (lst.size()!=4) continue;

        atomLabel=lst[0];
        x=lst[1].toDouble();
        y=lst[2].toDouble();
        z=lst[3].toDouble();

        addAtom(atomLabel,x,y,z);
    }
    perceiveBondOrder();
*/
}



void HMol::runMolecularMechanics(QString method)
{
    //prepare input file
    buildOBMol();


    int steps = 1000;
    double criterion = 1e-8;


    OpenBabel::OBFFConstraints constraints;
    vector <uint> constraintIdList;


    string ff = method.toStdString();

    //UFF can be applicable for a wide variety of molecules
    OpenBabel:: OBForceField * pFF = OpenBabel::OBForceField::FindForceField(ff);

    pFF->Setup(obmol,constraints);
    pFF->SetLogFile(&cout);

    pFF->SetLogLevel(OBFF_LOGLVL_MEDIUM);



    bool sd=false; //steepest descent
    bool done = true;

    pFF->ConjugateGradientsInitialize(steps, criterion);




    uint totalSteps = 0;
    while (done) {
        done = pFF->ConjugateGradientsTakeNSteps(1);
        totalSteps++;
        if(totalSteps>=steps) break;
    }

    pFF->UpdateCoordinates(obmol);

    cout << "SSSSSSSSSSSSS"<<endl;

    /*
    ff="MMFF94";
    pFF = OpenBabel::OBForceField::FindForceField(ff);
    totalSteps = 0;
    while (done) {
        done = pFF->ConjugateGradientsTakeNSteps(1);
        totalSteps++;
        if(totalSteps>=steps) break;
    }
    pFF->UpdateCoordinates(obmol);
*/

    // Begin by copying all of the atoms
    std::vector<OpenBabel::OBAtom*>::iterator i;
    uint j=0;
    for (OpenBabel::OBAtom *obatom = obmol.BeginAtom(i); obatom; obatom = obmol.NextAtom(i)) {
        getAtombyIndex(j)->setPos(obatom->GetX(),obatom->GetY(),obatom->GetZ());
        cout << obatom->GetX() << " " << obatom->GetY()<< " " <<obatom->GetZ()<<endl;

        j++;
    }

    perceivePlaneRingforBonds();
}


bool HMol::canbePaste()
{
    //data from this program
    QString  Text= QApplication::clipboard()->text();//.simplified()--this function can convert data to 1 line.
    QString tmpText=Text;
    tmpText=tmpText.simplified();
    if(!tmpText.isSimpleText())  return false;
    if (tmpText.length() < 10)   return false;


    //cout << tmpText.length()<<endl;
    //cout << "bool HMol::canbePaste()"<<endl;
    //cout << tmpText.toStdString().c_str()<< "111111 "<<endl;


    QString data="";
    unsigned int initNumAtoms=NumAtoms();

    if(Text.contains("MolSymmetry-copy-data"))
    {
        /* MolSymmetry-copy-data
                   4
                   C    0.750134    -0.000023    0.000002    0.77    9474192
                   H    1.145634    -1.044702    0.000062    0.37    16777215
                   H    1.145156    0.522519    -0.904976    0.37    16777215
                   H    1.145174    0.522601    0.904923    0.37    16777215
                   3
                   0 1 1
                   0 2 1
                   0 3 1
           */

        QString  atomLabel,line,single;
        QStringList lst=Text.split("\n"),satom,sbond;

        unsigned int nAtoms=lst[1].toInt();
        if(nAtoms==0){
            //QMessageBox::information(0, "Warning","No data available!");
            return false;
        }
        unsigned int i;
        vector3 p;
        //delete first 2 lines
        lst.pop_front();
        lst.pop_front();

        for(i=0;i<nAtoms;i++){
            line=lst[i].simplified();
            satom=line.split(" ",QString::SkipEmptyParts);
            if(satom.size()!=6) continue;

            if(isNumber(satom[0]))
                atomLabel=QString::fromStdString(ElementNames[satom[0].toInt()]);
            else if(isEnglish(satom[0]))
                atomLabel=satom[0];
            else
                return false;

            if(isFloat(satom[1]) && isFloat(satom[2]) && isFloat(satom[3]) )
            {
                single = atomLabel + " " + satom[1]+ " " + satom[2]+ " " + satom[3]+"\n";
                data += single;
            }
            else
            {
                //QMessageBox::information(0, "Warning","Incorrect Data!");
                return false;
            }
        }

        //data=getShift(data);
        //addatomfromQString(data);

        unsigned int nBonds=lst[i].toInt();
        i=i+1;
        unsigned int beg,end;
        for (unsigned int j=0;j<nBonds;j++)
        {
            line=lst[i+j].simplified();
            //cout << line.toStdString().c_str()<<endl;
            sbond=line.split(" ",QString::SkipEmptyParts);
            if (sbond.size()!=3) return false;

            if(!isInt(sbond[0])) return false;
            if(!isInt(sbond[1])) return false;
            //beg=initNumAtoms+sbond[0].toInt();
            //end=initNumAtoms+sbond[1].toInt();
            //addBond(beg,end,sbond[2].toInt());
        }
        // perceivePlaneRingforBonds();
        return true;
    }



    //data from gaussian ot others
    Text = QApplication::clipboard()->text().simplified();
    if(Text.contains("GINC-")){
        data=parseGaussin09Summary(Text);
        return true;
    }
    else
    {
        //replace all the comma with whitespace
        Text=Text.replace(QRegExp(",")," ");

        // QMessageBox::information(0, "Warning",Text);

        QStringList Lines = Text.trimmed().split("\n");
        if (Lines.size()<3) return false;


        QString single;
        QStringList ls;
        bool ok = false;

        unsigned int i, size=0;
        if (Lines.size()==5)
        {
            ls = Lines[0].trimmed().split(QRegExp("\\s+"));

            i=0;
            data="";

            //W  3.527590000000  0.124006000000  1.879670000000   R=1.992


            if(isEnglish(ls[0])  && isFloat(ls[1]) && isFloat(ls[2]) && isFloat(ls[3])  && ls[4].contains("R=") )
            {
                while  (i < ls.size())
                {
                    single = ls[i]+ " " + ls[i+1]+ " " + ls[i+2]+ " " + ls[i+3]+"\n";
                    data+=single;
                    i=i+6;
                }
            }


            /*
               1         22               -1.949599    4.505351    0.828982
               2          O               -1.425143    6.566943    1.077860
               3          O               -1.495672    4.077398    2.536112
               4          O               -2.730629    2.900709    0.362013
               5          O               -0.406798    4.239808   -0.032909
               6          O               -3.859200    5.310931    1.518737
               7          8               -2.809006    5.378771   -0.971769
               8          C               -0.682128    7.297813    0.361174
               9         22               -0.385940    3.776011    3.953315
               */

            if(ls[0]=="1" &&  isEnglishOrNumber (ls[1])  &&  ls[5]=="2"  )
            {
                while  (i < ls.size())
                {
                    single = ls[i+1]+ " " + ls[i+2]+ " " + ls[i+3]+ " " + ls[i+4]+"\n";
                    data+=single;
                    i=i+6;
                }
            }

            /*
               1         22           0       -1.949599    4.505351    0.828982
               2          8           0       -1.425143    6.566943    1.077860
               3          8           0       -1.495672    4.077398    2.536112
               4          8           0       -2.730629    2.900709    0.362013
               5          8           0       -0.406798    4.239808   -0.032909
               6          8           0       -3.859200    5.310931    1.518737
               7          8           0       -2.809006    5.378771   -0.971769
               8          6           0       -0.682128    7.297813    0.361174
               9         22           0       -0.385940    3.776011    3.953315
               */
            if(ls[0]=="1" && ls[6]=="2" && ls[2]=="0" )
            {
                while  (i < ls.size())
                {
                    single = ls[i+1]+ " " + ls[i+3]+ " " + ls[i+4]+ " " + ls[i+5]+"\n";
                    data+=single;
                    i=i+6;
                }
            }

            /*
               22           0       -1.949599    4.505351    0.828982
                8           0       -1.425143    6.566943    1.077860
                8           0       -1.495672    4.077398    2.536112
                8           0       -2.730629    2.900709    0.362013
                8           0       -0.406798    4.239808   -0.032909
                8           0       -3.859200    5.310931    1.518737
                8           0       -2.809006    5.378771   -0.971769
                6           0       -0.682128    7.297813    0.361174
               */
            if(ls.size()>=5)
            if(ls[1]=="0" && ls[6]=="0") {
                while  (i < ls.size())
                {
                    single = ls[i]+ " " + ls[i+2]+ " " + ls[i+3]+ " " + ls[i+4]+"\n";
                    data+=single;
                    i=i+5;
                }
            }

            /*
               22       -1.949599    4.505351    0.828982
                8       -1.425143    6.566943    1.077860
                8       -1.495672    4.077398    2.536112
                8       -2.730629    2.900709    0.362013
                8       -0.406798    4.239808   -0.032909
                8       -3.859200    5.310931    1.518737
                8       -2.809006    5.378771   -0.971769
                6       -0.682128    7.297813    0.361174
               */
            if(ls.size()>=4)
            if( isEnglishOrNumber (ls[0])  && isFloat(ls[1]) && isFloat(ls[2]) && isFloat(ls[3]) ) {
                while  (i < ls.size())
                {
                    single = ls[i]+ " " + ls[i+1]+ " " + ls[i+2]+ " " + ls[i+3]+"\n";
                    data+=single;
                    i=i+4;
                }
            }


            //QMessageBox::information(0, "Warning",data);
            //cout << "inside pastefromClipboard "<<endl;
            //cout << data.toStdString().c_str()<<endl;
            if (data.size() < 1) return false;
            return true;
        }



        for (i=0; i < Lines.size(); i++)
        {
            single = Lines.at(i).trimmed();

            if ( single.size() < 4) continue;
            ls = single.trimmed().split(QRegExp("\\s+"));


            if ( ls.size() < 4 || ls.size() > 5)
                continue;

            if (ls.size() == 4 )
            {
                ls[1].toDouble(&ok);          if (!ok ) return false;
                ls[2].toDouble(&ok);          if (!ok ) return false;
                ls[3].toDouble(&ok);          if (!ok ) return false;
                single = ls[0]+ " " + ls[1]+ " " + ls[2]+ " " + ls[3];
            }

            if (ls.size() == 5 )
            {
                if(ls[1]=="0")
                    single = ls[0]+ " " + ls[2]+ " " + ls[3]+ " " + ls[4];
                else
                    single = ls[1]+ " " + ls[2]+ " " + ls[3]+ " " + ls[4];

                //QMessageBox::information(0, "Warning",single);
            }
            data+=single+"\n";
        }

        if (data.size() < 1) return false;
        return true;
    }


    //QMessageBox::information(0, "Warning",data);
    return false;
}

void HMol::pastefromClipboard()
{
    //data from this program
    QString Text = QApplication::clipboard()->text();//.simplified()--this function can convert data to 1 line.
    if (Text.length() < 5) {
        QMessageBox::information(0, "Warning","No data available!");
        return ;
    }
    if (Text.isEmpty()) {
        QMessageBox::information(0, "Warning","No data available!");
        return ;
    }

    QString data="";
    uint initNumAtoms=NumAtoms();

    if(Text.contains("MolSymmetry-copy-data"))
    {
        /* MolSymmetry-copy-data
                4
                C    0.750134    -0.000023    0.000002    0.77    9474192
                H    1.145634    -1.044702    0.000062    0.37    16777215
                H    1.145156    0.522519    -0.904976    0.37    16777215
                H    1.145174    0.522601    0.904923    0.37    16777215
                3
                0 1 1
                0 2 1
                0 3 1
        */

        QString  atomLabel,line,single;
        QStringList lst=Text.split("\n"),satom,sbond;

        uint nAtoms=lst[1].toInt();
        if(nAtoms==0){
            QMessageBox::information(0, "Warning","No data available!");
            return ;
        }
        uint i;
        vector3 p;
        //delete first 2 lines
        lst.pop_front();
        lst.pop_front();

        for(i=0;i<nAtoms;i++){
            line=lst[i].simplified();
            satom=line.split(" ",QString::SkipEmptyParts);
            if(satom.size()!=6) continue;

            if(isNumber(satom[0]))
                atomLabel=QString::fromStdString(ElementNames[satom[0].toInt()]);
            if(isEnglish(satom[0]))
                atomLabel=satom[0];

            if(isFloat(satom[1]) && isFloat(satom[2]) && isFloat(satom[3]) )
            {
                single = atomLabel + " " + satom[1]+ " " + satom[2]+ " " + satom[3]+"\n";
                data += single;
            }
            else
            {
                QMessageBox::information(0, "Warning","Incorrect Data!");
                return ;
            }
        }

        data=getShift(data);
        addatomfromQString(data);

        uint nBonds=lst[i].toInt();
        i=i+1;
        uint beg,end;
        for (uint j=0;j<nBonds;j++)
        {
            line=lst[i+j].simplified();
            //cout << line.toStdString().c_str()<<endl;
            sbond=line.split(" ",QString::SkipEmptyParts);
            if (sbond.size()!=3) continue;

            beg=initNumAtoms+sbond[0].toInt();
            end=initNumAtoms+sbond[1].toInt();
            addBond(beg,end,sbond[2].toInt());
        }
        perceivePlaneRingforBonds();
        return ;
    }

    //data from gaussian ot others
    Text = QApplication::clipboard()->text().simplified();
    if(Text.contains("GINC-")){
        data=parseGaussin09Summary(Text);
        //data=getShift(data);
        loadxyzfromQString(data);
    }
    else
    {
        //replace all the comma with whitespace
        Text=Text.replace(QRegExp(",")," ");

        // QMessageBox::information(0, "Warning",Text);

        QStringList Lines = Text.trimmed().split("\n");
        if (Lines.size()<1) return ;

        QString single;
        QStringList ls;
        bool ok = false;

        uint i, size=0;
        if (Lines.size()==1)
        {
            ls = Lines[0].trimmed().split(QRegExp("\\s+"));

            i=0;
            data="";

            //W  3.527590000000  0.124006000000  1.879670000000   R=1.992

            if(isEnglish(ls[0])  && isFloat(ls[1]) && isFloat(ls[2]) && isFloat(ls[3])  && ls[4].contains("R=") )
            {
                while  (i < ls.size())
                {
                    single = ls[i]+ " " + ls[i+1]+ " " + ls[i+2]+ " " + ls[i+3]+"\n";
                    data+=single;
                    i=i+6;
                }
            }


            /*
            1         22               -1.949599    4.505351    0.828982
            2          O               -1.425143    6.566943    1.077860
            3          O               -1.495672    4.077398    2.536112
            4          O               -2.730629    2.900709    0.362013
            5          O               -0.406798    4.239808   -0.032909
            6          O               -3.859200    5.310931    1.518737
            7          8               -2.809006    5.378771   -0.971769
            8          C               -0.682128    7.297813    0.361174
            9         22               -0.385940    3.776011    3.953315
            */
            if(ls[0]=="1" &&  isEnglishOrNumber (ls[1])  &&  ls[5]=="2"  )
            {
                while  (i < ls.size())
                {
                    single = ls[i+1]+ " " + ls[i+2]+ " " + ls[i+3]+ " " + ls[i+4]+"\n";
                    data+=single;
                    i=i+6;
                }
            }

            /*
            1         22           0       -1.949599    4.505351    0.828982
            2          8           0       -1.425143    6.566943    1.077860
            3          8           0       -1.495672    4.077398    2.536112
            4          8           0       -2.730629    2.900709    0.362013
            5          8           0       -0.406798    4.239808   -0.032909
            6          8           0       -3.859200    5.310931    1.518737
            7          8           0       -2.809006    5.378771   -0.971769
            8          6           0       -0.682128    7.297813    0.361174
            9         22           0       -0.385940    3.776011    3.953315
            */
            if(ls[0]=="1" && ls[6]=="2" && ls[2]=="0" )
            {
                while  (i < ls.size())
                {
                    single = ls[i+1]+ " " + ls[i+3]+ " " + ls[i+4]+ " " + ls[i+5]+"\n";
                    data+=single;
                    i=i+6;
                }
            }

            /*
            22           0       -1.949599    4.505351    0.828982
             8           0       -1.425143    6.566943    1.077860
             8           0       -1.495672    4.077398    2.536112
             8           0       -2.730629    2.900709    0.362013
             8           0       -0.406798    4.239808   -0.032909
             8           0       -3.859200    5.310931    1.518737
             8           0       -2.809006    5.378771   -0.971769
             6           0       -0.682128    7.297813    0.361174
            */
            if(ls[1]=="0" && ls[6]=="0") {
                while  (i < ls.size())
                {
                    single = ls[i]+ " " + ls[i+2]+ " " + ls[i+3]+ " " + ls[i+4]+"\n";
                    data+=single;
                    i=i+5;
                }
            }

            /*
            22       -1.949599    4.505351    0.828982
             8       -1.425143    6.566943    1.077860
             8       -1.495672    4.077398    2.536112
             8       -2.730629    2.900709    0.362013
             8       -0.406798    4.239808   -0.032909
             8       -3.859200    5.310931    1.518737
             8       -2.809006    5.378771   -0.971769
             6       -0.682128    7.297813    0.361174
            */
            if( isEnglishOrNumber (ls[0])  && isFloat(ls[1]) && isFloat(ls[2]) && isFloat(ls[3]) ) {
                while  (i < ls.size())
                {
                    single = ls[i]+ " " + ls[i+1]+ " " + ls[i+2]+ " " + ls[i+3]+"\n";
                    data+=single;
                    i=i+4;
                }
            }


            //QMessageBox::information(0, "Warning",data);
            //cout << "inside pastefromClipboard "<<endl;
            //cout << data.toStdString().c_str()<<endl;
            loadxyzfromQString(data);
            return ;
        }


        for (i=0; i < Lines.size(); i++)
        {
            single = Lines.at(i).trimmed();

            if ( single.size() < 4) continue;
            ls = single.trimmed().split(QRegExp("\\s+"));


            if ( ls.size() < 4 || ls.size() > 5)
                continue;

            if (ls.size() == 4 )
            {
                ls[1].toDouble(&ok);          if (!ok ) return ;
                ls[2].toDouble(&ok);          if (!ok ) return ;
                ls[3].toDouble(&ok);          if (!ok ) return ;
                single = ls[0]+ " " + ls[1]+ " " + ls[2]+ " " + ls[3];
            }

            if (ls.size() == 5 )
            {
                if(ls[1]=="0")
                    single = ls[0]+ " " + ls[2]+ " " + ls[3]+ " " + ls[4];
                else
                    single = ls[1]+ " " + ls[2]+ " " + ls[3]+ " " + ls[4];

                //QMessageBox::information(0, "Warning",single);
            }
            data+=single+"\n";
        }

        if (data.size() < 1) return ;
        //data=getShift(data);
        loadxyzfromQString(data);
        return;
    }

    //QMessageBox::information(0, "Warning",data);
}

QString HMol::getShift(QString data)
{
    //shift to avoid overlap
    removeBackupAtoms();

    vector3 shift=VZero;

    if(NumAtoms()<1)
        return data;

    vector3 p=VZero,Minxyzfromclip=VZero;
    vector3 endExistedAtoms=getAtomPosbyIndex(0);
    //vector3 begExistedAtoms=getAtomPosbyIndex(0);

    if(NumAtoms() > 1){
        for (int i=1;i<NumAtoms();i++){
            p=getAtomPosbyIndex(i);
            //maxpos
            if(endExistedAtoms.x()<p.x()) endExistedAtoms.SetX(p.x());
            if(endExistedAtoms.y()<p.y()) endExistedAtoms.SetY(p.y());
            if(endExistedAtoms.z()<p.z()) endExistedAtoms.SetZ(p.z());
            /*//minpos
            if(begExistedAtoms.x()>p.x()) begExistedAtoms.SetX(p.x());
            if(begExistedAtoms.y()>p.y()) begExistedAtoms.SetY(p.y());
            if(begExistedAtoms.z()>p.z()) begExistedAtoms.SetZ(p.z());*/
        }
    }

    double x, y, z;
    QString  line;
    QStringList lst=data.split("\n"),single;

    line=lst[0].simplified();
    single=line.split(" ",QString::SkipEmptyParts);
    Minxyzfromclip.SetX(single[1].toDouble());
    Minxyzfromclip.SetY(single[2].toDouble());
    Minxyzfromclip.SetZ(single[3].toDouble());

    if(lst.size()>2)
        for (uint i=1;i<lst.size();i++)
        {
            line=lst[i].simplified();
            if (line.size() < 2) continue;
            single=line.split(" ",QString::SkipEmptyParts);
            if (single.size()!=4) continue;

            x=single[1].toDouble();
            y=single[2].toDouble();
            z=single[3].toDouble();

            if(Minxyzfromclip.x()>x)
                Minxyzfromclip.SetX(x);

            if(Minxyzfromclip.y()>y)
                Minxyzfromclip.SetY(y);

            if(Minxyzfromclip.z()>z)
                Minxyzfromclip.SetZ(z);

        }

    shift = endExistedAtoms-Minxyzfromclip;

    shift.x() += 0.5;
    shift.y() += 0.5;
    shift.z() += 0.5;

    QString atomLabel,singleatom;
    vector3 q;
    QString newdata="";
    for (uint i=0;i<lst.size();i++)
    {
        line=lst[i].simplified();
        if (line.size() < 2) continue;
        single=line.split(" ",QString::SkipEmptyParts);
        if (single.size()!=4) continue;

        atomLabel =single[0];
        q.SetX(single[1].toDouble());
        q.SetY(single[2].toDouble());
        q.SetZ(single[3].toDouble());

        q+=shift;
        singleatom=atomLabel+" "+QString::number(q.x(),'f',6)+" "+QString::number(q.y(),'f',6)+" "+QString::number(q.z(),'f',6)+"\n";
        newdata+=singleatom;
    }
    return newdata;
}

QString HMol::parseGaussin09Summary(QString Text)
{
    /*
    1\1\GINC-LOCALHOST\SP\RB3LYP\TZVP\C1H4\ZHANGFQ\22-Jun-2018\0\\# b3lyp/
     TZVP IOP(6/7=3) gfinput gfprint\\Title Card Required\\0,1\C,0,-1.29032
     2649,0.483870916,0.\H,0,-0.9293678757,-0.5371027068,-0.000000888\H,0,-
     0.9293486003,0.9943506561,0.8841853074\H,0,-0.9293500505,0.9943516816,
     -0.8841853074\H,0,-2.3732240695,0.483884033,0.000000888\\Version=ES64L
     -G16RevA.03\State=1-A1\HF=-40.5369264\RMSD=3.417e-09\Dipole=0.,0.,0.\Q
     uadrupole=0.,0.,0.,0.,0.,0.\PG=TD [O(C1),4C3(H1)]\\@
     */
    QString all="";
    Text=Text.simplified();
    Text=Text.remove(QRegExp("\\s+"));
    QStringList Lines = Text.split("\n");
    for (uint i=0; i < Lines.size(); i++)
        all+=Lines.at(i).trimmed();

    //QMessageBox::warning(this, "Script Execution Failed", all);
    //cout << Text.toStdString()<<endl;

    Lines.clear();

    all=all.trimmed();
    if(all.contains("\\")) Lines = all.split("\\");
    if(all.contains("||")) Lines = all.split("|");


    //QMessageBox::warning(this, "Script Execution Failed", all);
    float x,y,z;
    QStringList ls;
    QString single, one;

    all.clear();
    for (uint i=0; i < Lines.size(); i++)
    {
        single = Lines.at(i).trimmed();
        //cout << single.toStdString();
        //continue;

        if (single.startsWith("Version") ) break;

        //cout << single.toStdString();
        ls = single.split(QRegExp(","));

        if (ls.size() != 5) continue;

        one= ls[0]+ " " + ls[2] + " " + ls[3]+ " " + ls[4] +"\n";
        all+=one;
        //cout << one.toStdString();
    }

    //QMessageBox::warning(this, "Script Execution Failed", all);
    return all;
}


//fixed atom or not
void HMol::setBondLengthBetweenTwoAtoms(uint idx1, uint idx2, double length,
                                        bool isFixedAtom1, bool isFixedAtom2)
{
    if(isFixedAtom1 && isFixedAtom2) return;

    vector3 pos1=getAtomPosbyIndex(idx1);
    vector3 pos2=getAtomPosbyIndex(idx2);

    //get translate vector
    vector3 v=pos2-pos1;
    double length0=v.length();

    v.normalize(); v=v*(length-length0);

    //move one of them
    if(isFixedAtom1 || isFixedAtom2)
    {
        //move atom 1
        if(isFixedAtom2){
            pos1=pos1-v;
            getAtombyIndex(idx1)->setPos(pos1);
        }

        //move atom 2
        if(isFixedAtom1){
            pos2=v+pos2;
            getAtombyIndex(idx2)->setPos(pos2);
        }

        perceivePlaneBonds();
        perceiveRings();
        return;
    }

    //move them both
    if((!isFixedAtom1) && (!isFixedAtom2))
    {
        v=v/2.0;
        pos1=pos1-v;pos2=pos2+v;
        getAtombyIndex(idx1)->setPos(pos1);
        getAtombyIndex(idx2)->setPos(pos2);
    }

    perceivePlaneBonds();
    perceiveRings();
}




void HMol::setBondLengthBetweenTwoFragments(uint idx1, uint idx2, double length,
                                            vector <uint> & fragment1,  vector <uint> & fragment2,
                                            bool isFixedFrag1, bool isFixedFrag2)
{

    if(isFixedFrag1 && isFixedFrag2) return;

    vector3 pos1=getAtomPosbyIndex(idx1);
    vector3 pos2=getAtomPosbyIndex(idx2);


    //get translate vector
    vector3 v=pos2-pos1;
    double length0=v.length();

    v.normalize();
    v=v*(length-length0);

    //move one of them
    if(isFixedFrag1 || isFixedFrag2)
    {
        //move frag 1
        if(isFixedFrag2){
            //pos1=pos1-v;
            for (uint i=0;i<fragment1.size();i++) {
                vector3 v1=getAtomPosbyIndex(fragment1[i])-v;;
                getAtombyIndex(fragment1[i])->setPos(v1);
            }
        }

        //move frag 2
        if(isFixedFrag1){
            pos2=v+pos2;
            for (uint i=0;i<fragment2.size();i++) {
                vector3 v1=getAtomPosbyIndex(fragment2[i])+v;;
                getAtombyIndex(fragment2[i])->setPos(v1);
            }
        }

        perceivePlaneBonds();
        perceiveRings();
        return;
    }

    //move them both
    if((!isFixedFrag1) && (!isFixedFrag2))
    {
        v=v/2.0;
        //pos1=pos1-v;pos2=pos2+v;
        //getAtombyIndex(idx1)->setPos(pos1);
        //getAtombyIndex(idx2)->setPos(pos2);

        for (uint i=0;i<fragment1.size();i++) {
            vector3 v1=getAtomPosbyIndex(fragment1[i])-v;;
            getAtombyIndex(fragment1[i])->setPos(v1);
        }
        for (uint i=0;i<fragment2.size();i++) {
            vector3 v1=getAtomPosbyIndex(fragment2[i])+v;;
            getAtombyIndex(fragment2[i])->setPos(v1);
        }
    }

    perceivePlaneBonds();
    perceiveRings();
}


void HMol::setBondAngleAtoms (uint idx0, uint idx1, uint idx2, double bondAngle,
                              bool isFixedAtom1 , bool isFixedAtom2)
{
    if(isFixedAtom1 && isFixedAtom2) return;

    vector3 pos0=getAtomPosbyIndex(idx0);
    vector3 pos1=getAtomPosbyIndex(idx1);
    vector3 pos2=getAtomPosbyIndex(idx2);


    //get rotate vector
    vector3 v1,v2,v3;
    v1 = pos0 - pos1 ;
    v2 = pos2 - pos1 ;
    v3 = cross(v1,v2); //rotate axis


    double  angle0= vectorAngle(v1,v2);
    double  rotateAngle=bondAngle-angle0;
    if(fabs(rotateAngle)<0.6) return;

    vector3 v,vv;
    matrix3x3 m;

    //rotate one of them
    if(isFixedAtom1 || isFixedAtom2)
    {
        //rotate atom2
        if(isFixedAtom1)
        {
            m.SetupRotateMatrix(v3,rotateAngle);
            vv=getAtomPosbyIndex(idx2)-pos1;
            getAtombyIndex(idx2)->setPos(m*vv+pos1);
        }

        //rotate atom0
        if(isFixedAtom2)
        {
            m.SetupRotateMatrix(-v3,rotateAngle);
            vv=getAtomPosbyIndex(idx0)-pos1;
            getAtombyIndex(idx0)->setPos(m*vv+pos1);

        }

        perceivePlaneBonds();
        perceiveRings();
        return;
    }

    //rotate both atoms
    rotateAngle=rotateAngle/2.0;
    m.SetupRotateMatrix(v3,rotateAngle);
    vv=getAtomPosbyIndex(idx2)-pos1;
    getAtombyIndex(idx2)->setPos(m*vv+pos1);

    m.SetupRotateMatrix(-v3,rotateAngle);
    vv=getAtomPosbyIndex(idx0)-pos1;
    getAtombyIndex(idx0)->setPos(m*vv+pos1);

    perceivePlaneBonds();
    perceiveRings();
}




void HMol::setBondAngleFragments (uint idx0, uint idx1, uint  idx2, double bondAngle,
                                  vector <uint> & fragment1, vector <uint> & fragment2,
                                  bool isFixedFrag1, bool isFixedFrag2)
{
    if(isFixedFrag1 && isFixedFrag2) return;

    vector3 pos0=getAtomPosbyIndex(idx0);
    vector3 pos1=getAtomPosbyIndex(idx1);
    vector3 pos2=getAtomPosbyIndex(idx2);


    //get rotate vector
    vector3 v1,v2,v3;
    v1 = pos0 - pos1 ;
    v2 = pos2 - pos1 ;
    v3 = cross(v1,v2); //rotate axis


    double  angle0= vectorAngle(v1,v2);
    double  rotateAngle=bondAngle-angle0;
    if(fabs(rotateAngle)<0.1) return;

    vector3 v;
    matrix3x3 m;

    //rotate one of them
    if(isFixedFrag1 || isFixedFrag2)
    {
        //rotate fragment2
        if(isFixedFrag1)
        {
            m.SetupRotateMatrix(v3,rotateAngle);

            for (uint i=0;i<fragment2.size();i++) {
                vector3 vv=getAtomPosbyIndex(fragment2[i])-pos1;
                getAtombyIndex(fragment2[i])->setPos(m*vv+pos1);
            }
        }

        //rotate fragment1
        if(isFixedFrag2)
        {
            m.SetupRotateMatrix(-v3,rotateAngle);
            for (uint i=0;i<fragment1.size();i++) {
                vector3 vv=getAtomPosbyIndex(fragment1[i])-pos1;;
                getAtombyIndex(fragment1[i])->setPos(m*vv+pos1);
            }
        }

        perceivePlaneBonds();
        perceiveRings();
        return;
    }

    //rotate both fragments
    rotateAngle=rotateAngle/2.0;
    m.SetupRotateMatrix(v3,rotateAngle);
    for (uint i=0;i<fragment2.size();i++) {
        vector3 vv=getAtomPosbyIndex(fragment2[i])-pos1;;
        getAtombyIndex(fragment2[i])->setPos(m*vv+pos1);
    }


    m.SetupRotateMatrix(-v3,rotateAngle);
    for (uint i=0;i<fragment1.size();i++) {
        vector3 vv=getAtomPosbyIndex(fragment1[i])-pos1;;
        getAtombyIndex(fragment1[i])->setPos(m*vv+pos1);
    }

    perceivePlaneBonds();
    perceiveRings();
}



void HMol::setTorsionAtoms(uint idx0, uint idx1, uint idx2, uint idx3,
                           double angle, bool isFixed0 , bool isFixed3)
{

    if(isFixed0 && isFixed3) return;

    vector3 pos0=getAtomPosbyIndex(idx0);
    vector3 pos1=getAtomPosbyIndex(idx1);
    vector3 pos2=getAtomPosbyIndex(idx2);
    vector3 pos3=getAtomPosbyIndex(idx3);

    double angle0=dihedral(idx0,idx1,idx2,idx3);

    double  rotateAngle=angle-angle0;
    if(fabs(rotateAngle)<0.1) return;

    //get rotate vector
    vector3 v=pos2-pos1;
    matrix3x3 m;

    //rotate one of two atoms
    if(isFixed0 || isFixed3)
    {
        //rotate atom3
        if(isFixed0)
        {
            m.SetupRotateMatrix(-v,rotateAngle);
            vector3 vv=getAtomPosbyIndex(idx3)-pos2;;
            getAtombyIndex(idx3)->setPos(m*vv+pos2);
        }

        //rotate fragment1
        if(isFixed3)
        {
            m.SetupRotateMatrix(v,rotateAngle);
            vector3 vv=getAtomPosbyIndex(idx3)-pos2;;
            getAtombyIndex(idx3)->setPos(m*vv+pos2);
        }

        perceivePlaneBonds();
        perceiveRings();

        return;
    }

    //rotate both fragments
    rotateAngle=rotateAngle/2.0;
    m.SetupRotateMatrix(v,rotateAngle);
    vector3 vv=getAtomPosbyIndex(idx3)-pos2;;
    getAtombyIndex(idx3)->setPos(m*vv+pos2);


    m.SetupRotateMatrix(-v,rotateAngle);
    vv=getAtomPosbyIndex(idx3)-pos2;;
    getAtombyIndex(idx3)->setPos(m*vv+pos2);


    perceivePlaneBonds();
    perceiveRings();
}


void HMol::setTorsionFragments(uint idx0, uint idx1, uint idx2, uint idx3, double angle,
                               vector <uint> & fragment1,  vector <uint> & fragment2,
                               bool isFixedFrag1 , bool isFixedFrag2)
{
    if(isFixedFrag1 && isFixedFrag2) return;

    vector3 pos0=getAtomPosbyIndex(idx0);
    vector3 pos1=getAtomPosbyIndex(idx1);
    vector3 pos2=getAtomPosbyIndex(idx2);
    vector3 pos3=getAtomPosbyIndex(idx3);

    double angle0=dihedral(idx0,idx1,idx2,idx3);

    double  rotateAngle=angle-angle0;
    if(fabs(rotateAngle)<0.1) return;

    //get rotate vector
    vector3 v=pos2-pos1;
    matrix3x3 m;

    //rotate one of two atoms
    if(isFixedFrag1 || isFixedFrag2)
    {
        //rotate fragment2
        if(isFixedFrag1)
        {
            m.SetupRotateMatrix(-v,rotateAngle);
            for (uint i=0;i<fragment2.size();i++) {
                vector3 vv=getAtomPosbyIndex(fragment2[i])-pos2;;
                getAtombyIndex(fragment2[i])->setPos(m*vv+pos2);
            }

        }

        //rotate fragment1
        if(isFixedFrag2)
        {
            m.SetupRotateMatrix(v,rotateAngle);
            for (uint i=0;i<fragment1.size();i++) {
                vector3 vv=getAtomPosbyIndex(fragment1[i])-pos2;;
                getAtombyIndex(fragment1[i])->setPos(m*vv+pos2);
            }
        }

        perceivePlaneBonds();
        perceiveRings();

        return;
    }

    //rotate both fragments
    rotateAngle=rotateAngle/2.0;
    m.SetupRotateMatrix(v,rotateAngle);
    for (uint i=0;i<fragment2.size();i++) {
        vector3 vv=getAtomPosbyIndex(fragment2[i])-pos2;;
        getAtombyIndex(fragment2[i])->setPos(m*vv+pos2);
    }


    m.SetupRotateMatrix(-v,rotateAngle);
    for (uint i=0;i<fragment1.size();i++) {
        vector3 vv=getAtomPosbyIndex(fragment1[i])-pos2;;
        getAtombyIndex(fragment1[i])->setPos(m*vv+pos2);
    }

    perceivePlaneBonds();
    perceiveRings();
}





void HMol::removeBackupAtoms()
{
    if (atomList_bk.size() < 1)

        for (auto it=atomList_bk.begin();it!=atomList_bk.end();++it)
        {
            if(*it != nullptr) {
                delete (*it);
                (*it) = nullptr;
            }
        }
    atomList_bk.clear();
    QVector<HCoordinate*>().swap(atomList_bk);
}



void HMol::backupAtoms()
{
    removeBackupAtoms();
    atomList_bk.clear();

    for (uint i=0;i<NumAtoms();i++) {
        HCoordinate * single = new HCoordinate();
        single->symbol=getAtomSymbol(i);
        single->p=getAtomPosbyIndex(i);
        atomList_bk.push_back(single);
    }
}


void HMol::recoverAtomCoordinateFromBackup()
{
    for (uint i=0;i<NumAtoms();i++)
        getAtombyIndex(i)->setPos(atomList_bk[i]->p);
}

//remove one terminal hydrogen of this atom with id
bool HMol::removeOneTerminalHydrogen(uint idx)
{
    bool isTerminalHydrogen=false;

    vector <uint > idsHydrogen;
    for (uint i=0;i<getAtombyIndex(idx)->numNeighbors();i++)
    {
        uint ii=getAtombyIndex(idx)->getNeighborId(i);
        if(getAtombyIndex(ii)->Symbol()!="H")
            continue;

        //terminal Hydrogen?
        if(getAtombyIndex(ii)->numNeighbors()==1)
            idsHydrogen.push_back(ii);
    }

    if(idsHydrogen.size()<1) return false;

    removeAtombyId(idsHydrogen[0]);

    return true;
}


void HMol::autoAdjustHydrogenWhole()
{
    autoAdjustHydrogen();
    cleanMol();
    autoAddHydrogen();
}


void HMol::autoAdjustHydrogen()
{
    QString Symbol;

    for(uint id=0;id<NumAtoms();id++)
    {
        Symbol=getAtombyIndex(id)->Symbol();
        if(Symbol=="C" || Symbol=="Si" || Symbol=="Ge" ||
                Symbol=="N" || Symbol=="P"  ||
                Symbol=="P" || Symbol=="As" ||
                Symbol=="O" || Symbol=="F")

            autoAdjustHydrogen(id);
    }
}


void HMol::autoAdjustHydrogen(uint idx)
{
    uint times=0;
    float numBonds=0.0;

    uint bondOrder,ii;
    uint numNeigh=getAtombyIndex(idx)->numNeighbors();

    for (uint i=0;i<numNeigh;i++)
    {
        ii=getAtombyIndex(idx)->getNeighborId(i);
        bondOrder=getBondOrderbyAtomIndex(idx,ii);
        if(bondOrder<=4)
            numBonds+=bondOrder;

        //aromatic
        if(bondOrder==6){
            if(numNeigh<3)
                numBonds=2.6;
            else
                numBonds=3.6;
        }
        //if(bondOrder==5  || bondOrder==7) //hydogen && weak
        //    numBonds=+0.0;
    }

    QString Symbol=getAtombyIndex(idx)->Symbol();

    if(Symbol=="C" || Symbol=="Si" || Symbol=="Ge")
    {
        if(numBonds>4)
        {
            removeOneTerminalHydrogen(idx);
            if(numBonds>3)  autoAdjustHydrogen(idx);
        }
        times=4-numBonds;
    }


    if(Symbol=="N" || Symbol=="P" )
    {
        if(numBonds>3)
        {
            removeOneTerminalHydrogen(idx);
            if(numBonds>2)  autoAdjustHydrogen(idx);
        }

        times=3-numBonds;
    }


    if( Symbol=="P" || Symbol=="As")
    {
        if(numBonds>5)
        {
            removeOneTerminalHydrogen(idx);
            if(numBonds>4)  autoAdjustHydrogen(idx);
        }
        times=5-numBonds;
    }


    if(Symbol=="O")
    {
        times=2-numBonds;
        if(numBonds>2)
        {
            removeOneTerminalHydrogen(idx);
            if(numBonds>1) autoAdjustHydrogen(idx);
        }
    }
}




void HMol::autoAddHydrogen(uint idx)
{
    uint times=0;
    float numBonds=0.0;
    uint numNeigh=getAtombyIndex(idx)->numNeighbors();
    for (uint i=0;i<getAtombyIndex(idx)->numNeighbors();i++)
    {
        uint ii=getAtombyIndex(idx)->getNeighborId(i);
        uint bondOrder=getBondOrderbyAtomIndex(idx,ii);
        if(bondOrder<=4)
            numBonds+=bondOrder;

        //aromatic
        if(bondOrder==6 ){
            if(numNeigh<3)
                numBonds=2.6;
            else
                numBonds=3.6;
        }

        // if(bondOrder==5  || bondOrder==7) //hydogen && weak
        //     numBonds=+1.0;
    }

    QString Symbol=getAtombyIndex(idx)->Symbol();

    if(Symbol=="C" || Symbol=="Si" || Symbol=="Ge")
    {
        if(numBonds>4.0)
        {
            return;
            //if(!removeOneTerminalHydrogen(idx));
            //autoAddHydrogen(idx);
        }
        times=4-numBonds;
    }


    if(Symbol=="N" || Symbol=="P" ) {

        if(numBonds>3.0)
        {
            return;

            if(!removeOneTerminalHydrogen(idx));
            autoAddHydrogen(idx);
        }

        times=3-numBonds;
    }


    if( Symbol=="P" || Symbol=="As") {

        if(numBonds>5.0)
        {
            return;
            if(!removeOneTerminalHydrogen(idx));
            autoAddHydrogen(idx);
        }

        times=5-numBonds;
    }


    if(Symbol=="O") {
        times=2-numBonds;
        if(numBonds>2)
        {
            return;
            if(!removeOneTerminalHydrogen(idx));
            autoAddHydrogen(idx);
        }
    }

    if(Symbol=="F")
        times=1-numBonds;

    if(times<1) return;

    //cout << "adding Hydrogen for "<< idx+1<< "  atoms"<<endl;
    for(uint i=0;i<times;i++)
        addHydrogen(idx);
}



//for whole molecule
void HMol::autoAddHydrogen()
{
    QString Symbol;
    for(uint id=0;id<NumAtoms();id++)
    {
        Symbol=getAtombyIndex(id)->Symbol();
        if(Symbol=="C" || Symbol=="Si" || Symbol=="Ge" ||
                Symbol=="N" || Symbol=="P"  ||
                Symbol=="P" || Symbol=="As" ||
                Symbol=="O" || Symbol=="F")

            autoAddHydrogen(id);
    }
}

void HMol::decreaseBondOrder(uint idx,float limit)
{
    float numBonds=0.0;
    uint bondId=0;

    uint numNeigh=getAtombyIndex(idx)->numNeighbors();
    for (uint i=0;i<getAtombyIndex(idx)->numNeighbors();i++)
    {
        uint ii=getAtombyIndex(idx)->getNeighborId(i);
        uint bondOrder=getBondOrderbyAtomIndex(idx,ii);
        if(bondOrder<=4)
            numBonds+=bondOrder;
        if(bondOrder==6) {
            if(numBonds>limit+0.1) break;
            numBonds+=1.0;
            getBondIndexbyAtomIndex(idx,ii,bondId)->setBondOrder(1.0);
        }
    }

    /*
    uint numR=numRings();
    if(numR<1) return;

    while(numR--)
    {
        if(numR<0) break;

        HRing  * ring = getRingbyId(numR);

        for(uint j=0;j<ring->size;j++)
        {
            bool needRemove=false;
            if(ring->atomIdList[j]==idx)
            {
                uint bondOrder=0.0;
                for(int k=0;k<ring->size;k++)
                {
                    if(k<(ring->size-1) )
                        bondOrder=getBondOrderbyAtomIndex(ring->atomIdList[k],ring->atomIdList[k+1]);
                    else
                        bondOrder=getBondOrderbyAtomIndex(ring->atomIdList[k],ring->atomIdList[0]);

                    if (bondOrder>1) continue;
                    needRemove=true;
                    break;
                }
                if(needRemove)  removeSingleRingbyId(numR);
            }
        }
    }
    */
}


void HMol::rectifyBondOrder(uint idx)
{
    float numBonds=0.0;
    uint numNeigh=getAtombyIndex(idx)->numNeighbors();
    for (uint i=0;i<getAtombyIndex(idx)->numNeighbors();i++)
    {
        uint ii=getAtombyIndex(idx)->getNeighborId(i);
        uint bondOrder=getBondOrderbyAtomIndex(idx,ii);
        if(bondOrder<=4)
            numBonds+=bondOrder;
        if(bondOrder==6 )
            numBonds+=1.5;
    }


    QString Symbol=getAtombyIndex(idx)->Symbol();
    if(Symbol=="C" || Symbol=="Si" || Symbol=="Ge")
    {
        if(numBonds<=4.01) return;
        decreaseBondOrder(idx,4.0);
    }


    if(Symbol=="N" || Symbol=="P" || Symbol=="As")
    {
        if(numBonds<5.01) return;
        decreaseBondOrder(idx,5.0);
    }


    if(Symbol=="O") {
        if(numBonds<2.01) return;
        decreaseBondOrder(idx,2.0);
    }
}



void HMol::addHydrogen(uint idx)
{    
    //random position to avoid eclipsed coordinate
    double x= rand()%100*1.0;
    double y= rand()%100*1.0;
    double z= rand()%100*1.0;

    addAtom(1,x,y,z);
    addBond(idx,NumAtoms()-1,1);

    rectifyBondOrder(idx);



    //-----------------------------------
    buildOBMol();
    //collect hydrogen atoms around this atom
    vector <uint > All;
    for (uint i=0;i<NumAtoms();i++)
        All.push_back(i);


    vector <uint > hydrogenList;
    for (uint i=0;i< getAtombyIndex(idx)->numNeighbors();i++)
    {
        uint id=getAtombyIndex(idx)->getNeighborId(i);
        if (!getAtombyIndex(id)->isHydrogen()) continue;
        hydrogenList.push_back(id);
    }

    for (int i=0;i<hydrogenList.size();i++)
        cout << hydrogenList[i] << endl;

    OpenBabel::OBFFConstraints constraints;
    vector <uint> constraintIdList;

    cout << "\n\nconstraint --------" <<endl;
    if(hydrogenList.size()>0)
    {
        std::set_difference(All.begin(), All.end(),
                            hydrogenList.begin(), hydrogenList.end(),
                            std::back_inserter(constraintIdList));

        for(uint i=0;i< constraintIdList.size();i++)
            constraints.AddAtomConstraint(constraintIdList[i]+1);
    }
    else {
        for(uint i=0;i< All.size();i++) {
            constraints.AddAtomConstraint(All[i]+1);
            cout << All[i]+1 <<endl;
        }
    }
    cout << "constraint --------\n\n" <<endl;


    int steps = 200;
    double criterion = 1e-5;

    OpenBabel:: OBForceField* pFF = OpenBabel::OBForceField::FindForceField("UFF");
    //if(hydrogenList.size()>2)  {pFF->FindForceField("UFF"); steps = 2000;}

    pFF->Setup(obmol,constraints);

    bool done = true;
    pFF->ConjugateGradientsInitialize(steps, criterion);


    uint totalSteps = 0;
    while (done)
    {
        done = pFF->ConjugateGradientsTakeNSteps(1);
        totalSteps++;
        if(totalSteps>=steps) break;
    }

    pFF->UpdateCoordinates(obmol);

    fromOBMol();
}




void HMol::perceiveRings()
{
    perceiveRings_OB();
}



void HMol::perceiveMolLength( double & height , double & radius)
{
    height=radius=1.0;

    double x, y,z;
    vector3 pos;
    for (uint i=0;i<NumAtoms();i++)
    {
        pos=atomPos(i);
        z=abs(pos.z());
        x=abs(pos.x());
        y=abs(pos.y());
        if ( z > height)
            height=z;

        if ( x > radius)
            radius=x;

        if ( y > radius)
            radius=y;
    }

    radius=radius*2.0;
    height=height*2.0;

    cout << "height &&  radius : " << height << " "<<radius<<endl;
}



void HMol::thermalDisplacement(double scale)
{
    //you can change scale here!


    double x,y,z;
    for(uint i=0;i<NumAtoms();i++)
    {
        x=getAtomXbyIndex(i)+((double)rand()/RAND_MAX-0.5)/scale;
        y=getAtomYbyIndex(i)+((double)rand()/RAND_MAX-0.5)/scale;
        z=getAtomZbyIndex(i)+((double)rand()/RAND_MAX-0.5)/scale;

        getAtombyIndex(i)->setPos(x,y,z);
    }

}

void HMol::removeX()
{
    //removeAllBonds();
    //removeAllRings();
    //removeAllArcs();

    uint num=NumAtoms();
    while(num--)
    {
        if(getAtomSymbol(num)=="X" || getAtomSymbol(num)=="Bq")
            removeAtombyId(num);
    }
}


//add long bond: (C5H5)2Fe
void HMol::reCalculateBonds()
{
    double r1, r2;
    QString a1,a2;
    vector3 p1,p2;
    for(uint i=0;i<NumAtoms();i++)
    {
        if(getAtombyIndex(i)->Symbol()=="X") continue;
        p1=getAtomPosbyIndex(i);
        r1=getAtomRadiusbyIndex(i);

        //cout << i+1<<endl;
        for(uint j=i+1;j<NumAtoms();j++)
        {
            if(getBondOrderbyAtomIndex(i,j)>0) continue;
            if(getAtombyIndex(j)->Symbol()=="X") continue;
            r2=getAtomRadiusbyIndex(j);
            p2=getAtomPosbyIndex(j);

            if( (p1-p2).length() > (r1+r2)*1.2) continue;
            addBond(i,j,1);
        }
    }
}

void  HMol::setRingAromatic(uint id, bool ok)
{
    getRingbyId(id)->isAromatic=ok;

    uint size=getRingbyId(id)->size;

    HBond * b;
    for(uint i=0;i<size-1;i++)
    {
        b = getBondbyAtomIndex(getRingbyId(id)->atomIdList[i],
                               getRingbyId(id)->atomIdList[i+1]);
        b->setAromatic(ok);
        if(ok) b->setBondOrder(6);
    }

    b= getBondbyAtomIndex(getRingbyId(id)->atomIdList[0],
            getRingbyId(id)->atomIdList[size-1]);
    b->setAromatic(ok);
    if(ok) b->setBondOrder(6);
}

bool HMol::isXExisted()
{
    return  isAtomExisted("X");
}
bool HMol::isHydrogenAtomExisted()
{
    return  isAtomExisted("H");
}


bool HMol::isAtomExisted(QString ll)
{
    for(uint i=0;i<NumAtoms();i++)
        if(getAtomSymbol(i)=="ll") return true;
    return false;
}


void HMol::moveAtomFromTo(uint beg, uint end)
{
    //move backward
    if(end>beg) {

        return;
    }

    //move forward
}




uint HMol::numHuckelCarbons()
{
    return huckelCarbonList.size();
}

void HMol::collectHuckelSphericalAtoms()
{
    cout << "collectHuckelSphericalAtoms " <<endl;

    double x=0.0,y=0.0,z=0.0;
    double x0=0.0,y0=0.0,z0=0.0;
    uint idx=0;

    uint numCarbon=0;
    NumHuckelElectrons=0;

    huckelCarbonList.clear();

    //collect [C N O F and Cl]
    {
        // #pragma omp parallel for
        for(uint i=0;i< NumAtoms(); i++)
        {
            HAtom * atom=getAtombyIndex(i);
            idx=atom->index()-1;

            if(atom->atomicNum()<6) continue;
            if(atom->atomicNum()>9)  continue;

            //sp2 or not?
            if (!atom->isAromatic()) continue;

            huckelCarbonList.push_back(idx);

            x = fabs(atom->x());
            y = fabs(atom->y());
            z = fabs(atom->z());

            if (x0 < x) x0=x;
            if (y0 < y) y0=y;
            if (z0 < z) z0=z;
        }
    }

    numCarbon=huckelCarbonList.size();

    xyzLength.Set(x0,y0,z0);
    molLength=xyzLength.length();
}



void HMol::collectHuckelAtoms()
{
    //percept molecule shape
    perceptMoleculeShape();
    cout << molShape.toStdString().c_str()<< " shape"<<endl;

    if(molShape=="Sphere")
    {
        collectHuckelSphericalAtoms();
        return;
    }




    uint numCarbon=0;
    NumHuckelElectrons=0;

    huckelCarbonList.clear();

    double x=0.0,y=0.0,z=0.0;
    double x0=0.0,y0=0.0,z0=0.0;
    uint idx=0;


    //collect [C N O F and Cl]
    for(idx=0;idx<NumAtoms(); idx++)
    {
        HAtom * atom=getAtombyIndex(idx);

        //inside or outside?
        if( fabs(atom->z()) > 0.1 ) continue;

        if(atom->atomicNum()<6) continue;
        if(atom->atomicNum()>9 && atom->atomicNum()!=17)  continue;

        //sp2 or not?
        bool ok=true;
        for (uint k=0;k<atom->numLinkdedAtoms();k++) {
            if (getAtombyIndex(atom->getLinkdedAtomId(k))->z()>0.3) { ok=false; break; }
        }
        if(!ok) continue;

        huckelCarbonList.push_back(idx);
        numCarbon++;


        x = fabs(atom->x());
        y = fabs(atom->y());
        z = fabs(atom->z());

        if (x0 < x) x0=x;
        if (y0 < y) y0=y;
        if (z0 < z) z0=z;
        //cout << idx+1<< endl;
    }

    // cout << "SSSSSSSSSSSSSSSSSSS "<<huckelCarbonList.size()<<endl;

    xyzLength.Set(x0,y0,z0);
    molLength=xyzLength.length();
}


//place the  plane to xoy plane
void HMol::orient2PlaneXOY()
{
    perceptMoleculeShape();

    if(molShape=="Line")
    {
        rotate(getAtomPosbyIndex(0)-getAtomPosbyIndex(1),VX);
        return;
    }

    if(abs(dot(IMomentMatrix.GetColumn(2),VZ)-1.0)>0.1)
        rotate(IMomentMatrix.GetColumn(2),VZ);

    /*double maxX=0.0, maxY=0.0;
    double x,y;
    for(int i=0;i<NumAtoms(); i++)
    {
        x = fabs(getAtombyIndex(i)->x());
        y = fabs(getAtombyIndex(i)->y());

        if (maxX < x) maxX=x;
        if (maxY < y) maxY=y;
    }

    if(maxX<maxY)
        rotate(90.0,VZ);*/

}



