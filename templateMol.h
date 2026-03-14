#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <QString>
#include "Mol.h"

class templateMol
{
public:
    templateMol();
    ~templateMol() {clearAll();}


    QString fileName,name;

    QVector <HRing *> ringList;
    QVector <HAtom *> atomList;
    QVector <HBond *> bondList;
    uint hotIndex;

    OpenBabel::OBMol  obmol;


    uint getHotIndex() {return hotIndex;}
    void LoadFile(QString );

    void clearAll();
    void clearAtoms();
    void clearBonds();
    void clearRings();

    HAtom * getAtombyIndex(uint id) {return atomList[id];}
    void setLinkedAtom(uint idx);
    uint getLinkedAtom();



    uint numAtoms() {return atomList.size();}
    uint numBonds() {return bondList.size();}

    QString getSymbol (uint id) {return atomList[id]->Symbol();}
    vector3 getPos (uint id) {return atomList[id]->Pos();}
    HBond * getBond(uint id) {return bondList[id];}
    HAtom * getAtom(uint id) {return atomList[id];}


    bool isAtomAromatic(uint id) {return atomList[id]->isAromatic();}

    uint    getAtomicNum(uint id)  {return atomList[id]->atomicNum();}
    double  getAtomXbyIndex(uint id) {return atomList[id]->x();}
    double  getAtomYbyIndex(uint id) {return atomList[id]->y();}
    double  getAtomZbyIndex(uint id) {return atomList[id]->z();}
    double  getAtomRadiusbyIndex(uint id) {return atomList[id]->radius();}

    vector3 getAtomPosbyIndex(uint id) {return atomList[id]->Pos();}

    HBond * getBondbyIndex(uint id) {return bondList[id];}


    void rotate(vector3 v, double angle);
    void rotate(matrix3x3 & m);
    void translate(vector3 v);
    void centeralize();


    void perceivePlaneBondbyId(uint id);
    void perceivePlaneBonds();
    void perceiveRings();

    void buildOBMol();
    bool fromOBMol();

    void reCalculateBonds();

    vector3 atomPos(uint idx) {
        if (idx<numAtoms() )
            return atomList[idx]->Pos();
        return VZero;
    }


    void setAtomPos (uint idx, vector3 p)
    {
        if (idx >= numAtoms()) return;
        atomList[idx]->setPos(p);
    }


    double atomicMass (uint idx)
    {
        if (idx<numAtoms())
            return atomList[idx]->Mass();
        return 0.0;
    }

    QString atomSymbol (uint idx) {
        if (idx<numAtoms() )
            return atomList[idx]->Symbol();
        return "X";
    }

    uint atomicNum (uint idx) {
        if (idx<numAtoms() )
            return atomList[idx]->atomicNum();
        return 0;
    }

    uint numRings() {return ringList.size();}


    HRing * getRingbyId(uint id) {return ringList[id];}
    void displayRingList();
    void displayBondList();
    void displayAtomList();
    bool isExistedAromaticBond;

    QString getAtomSymbol (uint id) {return atomList[id]->Symbol();}


};

#endif // TEMPLATE_H
