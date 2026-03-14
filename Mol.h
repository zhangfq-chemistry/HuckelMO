#ifndef HMOL_H
#define HMOL_H

#include "math/vector3.h"
#include "math/matrix3x3.h"

#include <openbabel/mol.h>
#include <openbabel/atom.h>
#include <openbabel/bond.h>
#include <openbabel/generic.h>

#include <QString>
#include <QVector>
#include <vector>
#include <string.h>
#include <QList>

#include <vector>
#include <utility>
//#include <vtkMolecule.h>

#include <vtkPDBReader.h>
#include <vtkXYZMolReader2.h>
#include <vtkGaussianCubeReader2.h>
#include <vtkCMLMoleculeReader.h>
#include <vtkSimpleBondPerceiver.h>

#include "functionals.h"



using namespace std;

class MainWindow;
class View3D;
class templateMol;
class HGraph;

enum AtomHybridization
{
  PerceivedOctaheadral = -6,
  PerceivedTrigonalBipyramidal = -5,
  PerceivedSquarePlanar = -4,
  PerceivedSP3 = -3,
  PerceivedSP2 = -2,
  PerceivedSP = -1,
  Unknown = 0,
  SP = 1,
  SP2 = 2,
  SP3 = 3,
  SquarePlanar = 4,
  TrigonalBipyramidal = 5,
  Octahedral = 6
};

struct HRing
{
    uint  size;
    vector <uint> atomIdList;
    vector3 center;
    vector3 norm;
    vector3 color;
    double radius;
    bool   isAromatic;
    bool   isSelected;
    double scale;// for display radius;
    uint   type; //for display
    double opacity;
};

struct HArc
{
    uint  size;
    vector <uint> atomIdList;
    vector <vector3 > points;
    vector3 center;
    vector3 norm;
    vector3 color;
    double radius;
    bool   isAromatic;
    bool   isSelected;
    double scale;// for display radius;
    uint   type; //for display
    double opacity;
};

struct HXAtom
{
    uint   id;
    vector3 pos;
    vector <uint> atomIdList;
    vector <uint> neighbor;
};


struct HCoordinate
{
    QString symbol;
    vector3 p;
};


class HAtom
{
public:
    HAtom() {
        _symbol="X";
        _atomicNum=0;
        pos_cart=VZero;
        _color=0xFFFFFF;
        _mass=0.0;
        _radius=0.0;
        _isAromatic=false;
        _hybridization=Unknown;
    }

    HAtom(QString, vector3 );
    HAtom(QString, double, double, double );
    HAtom(uint, double, double, double );
    HAtom(uint , vector3  );

   ~HAtom() {};

   void setPos(vector3 pos) {pos_cart=pos; }
   void setPos(double x, double y, double z)
   {
       vector3 p(x,y,z);
       pos_cart=p;
   }
   void setCartesian(double x, double y, double z)
   {
       setPos(x,y,z);
   }
   void setCartesian  (vector3 p) {setPos(p);}


  vector3 cartesian  () {return pos_cart;}
  vector3 Pos ()        {return pos_cart;}


   void modify2Symbol (QString s);
   void setSymbol ( QString s);
   void setSymbol();
   QString Symbol ();
   QString Symbol(uint );

   void setAtomicNum (int n) {_atomicNum=n;}
   int  atomicNum () {return _atomicNum;}


   double x(){return pos_cart.x();}
   double y(){return pos_cart.y();}
   double z(){return pos_cart.z();}


   uint32_t Color () {return _color;}
   void setColor (uint32_t c ) {_color=c;}


   bool isHydrogen() const { return _atomicNum == 1; }



   double radius();
   void   setRadius(double r) {_radius=r;}

   double Mass();
   void   setMass(double m) {_mass=m;}

   uint  index() {return idx;}
   void  setIndex(uint i) {idx=i;}
   void  update();


   //! Assignment
   HAtom & operator= (const HAtom & other);
   HAtom *  copy (const HAtom * other);

   void setSelected (bool b) {_selected=b;};
   bool isSelected (){return _selected;}

   void  setNeihgborId(uint i, uint d) {LinkdedAtoms[i]=d;}
   void  removeLinkedAtomId(uint idx) ;
   void  appendLinkdedAtomId(uint idx) {LinkdedAtoms.push_back(idx);}
   uint  getLinkdedAtomId(uint idx) {return LinkdedAtoms[idx];}

   uint  numLinkdedAtoms() {return LinkdedAtoms.size();}
   void  clearLinkedAtomId() {LinkdedAtoms.clear();}
   bool  isLinkded(uint j){if (LinkdedAtoms.contains(j)) return true;return false;}


   void  appendNeighbor(uint idx) {LinkdedAtoms.push_back(idx);}
   uint  getNeighborId(uint idx) {return LinkdedAtoms[idx];}
   uint  numNeighbors() {return LinkdedAtoms.size();}
   bool  isNeighbor(uint j){if (LinkdedAtoms.contains(j)) return true;return false;}
   void  removeNeighbor(uint idx) ;
   void  clearNeighbors() {LinkdedAtoms.clear();}


   bool  isAromatic() { if(numNeighbors()<=3) return true; return false;}
   //bool  isAromatic() {return _isAromatic;}

   void  setAromatic(bool b=true) {_isAromatic=b;}

   void   setScale (double s) {_scale=s;}
   double getScale () {return _scale;}

   void  setHybridization (AtomHybridization a) {_hybridization=a;}
   AtomHybridization  getHybridization() {return _hybridization;}

   bool isHydrogen() {if (_symbol=="H") return true; if (_atomicNum==1) return true; return false;}


private:
    QList <uint > LinkdedAtoms;
    QString _symbol;
    int _atomicNum;
    double _radius,_mass;
    vector3 pos_cart,pos_frac;
    uint32_t _color;
    uint idx,_uniqueAtomIdx;
    bool _selected;
    bool _isAromatic;
    double _scale;
    AtomHybridization _hybridization;
};


class HBond
{
private:
     uint  bondOrder; // 1 = single, 2 = double,  3 = triple, 6 = aromatic)
     double length;
     uint  atomId0, atomId1;
     double radius;
     vector3 begin, end;
     bool isAromaticity;

     vector3 _plane;
     uint    _idRing;
     uint    _idArc;
     bool   _selected;
     bool   _isAromatic;

public:
     void reversePlane() {_plane=-_plane;}
     bool isInsideRing () { if(_idRing<100001) return true; return false;}
     bool isInsideArc () { if(_idArc<100001) return true; return false;}

     explicit HBond() {_selected=false;}
     HBond(int i,int j, int order) {
         atomId0=i;
         atomId1=j;
         bondOrder=order;
         isAromaticity=false;
         _plane=VZero;
         _idRing=_idArc=10000001;
         _selected=false;
         _isAromatic=false;
     };


     ~HBond(){}

     uint getBondOrder () {return bondOrder;}
     void setBondOrder (uint b) {bondOrder=b;}

     void setSelected (bool b) {_selected=b;}
     bool isSelected (){return _selected;}


     uint atomIndex0()
     {
         return  atomId0;
     }

     uint atomIndex1()
     {
         return  atomId1;
     }

     void setIndex(uint id1,uint id2)
     {
         atomId0=id1;
         atomId1=id2;
     }

     void setBeginIndex(uint id )  { atomId0=id; }
     void setEndIndex(uint id )    { atomId1=id; }

     //void setBegin(vector3 & b )   {begin=b; }
     //void setEnd(vector3 & e )     {end=e; }

     void setAromaticity(bool t)   {isAromaticity=t;}

     void setPlane (vector3 v) {_plane=v;}
     void setPlane (double x, double y, double z) {_plane.Set(x,y,z);}
     vector3 getPlane () {return _plane;}

     void setRingId(uint id) {_idRing=id; }
     uint getRingId () {return _idRing; }

     uint getArcId () {return _idArc;}
     void setArcId (uint id) {_idArc=id; }

     bool  isAromatic() {return _isAromatic;}
     void  setAromatic(bool b=true) {_isAromatic=b;}

     void swapIndex() { //cout << "swap index "<<endl;
                        //cout << atomId0 << " "<< atomId1<<endl;
                        uint ii=atomId1;
                        atomId1=atomId0;
                        atomId0=ii;
                        //cout << atomId0 << " "<< atomId1<<endl;
                      }
};










//molecule class
class HMol
{
public:
    HMol() ;
    HMol(MainWindow * m):m_parent(m) {
        isAtomvisible=true;
        isAtomLabelVisible=false;
        view3d=nullptr;

    };
    

    ~HMol(){clearAll();}

    MainWindow * m_parent;

    QVector <HAtom *> atomList;
    QVector <HBond *> bondList;
    QVector <HRing *> ringList;
    QVector <HArc *> arcList;
    QVector <HXAtom*> atomXList;



    void backupAtoms();
    QVector <HCoordinate * > atomList_bk;

    double distance  (uint id1, uint id2);
    double angle     (uint id1, uint id2, uint id3);
    double dihedral  (uint id1, uint id2, uint id3, uint id4);


    HBond * getBondbyAtomIndex(uint , uint );
    uint getBondOrderbyAtomIndex(uint , uint );
    uint getBondOrderbyIndex(uint id) {return getBondbyIndex(id)->getBondOrder();}

    HBond * getBondIndexbyAtomIndex(uint , uint , uint & );
    HBond * getBondbyIndex(uint id) {  return bondList[id]; }


    HBond * getLastBond() { if (NumBonds()>0) return bondList[NumBonds()-1]; return nullptr;}

    void findChildren(vector<int> &children,int first,int second);

    bool isAtomLabelVisible;

public:
    vector3 atomPos(uint idx) {
        if (idx<NumAtoms() )
            return atomList[idx]->Pos();
        return VZero;
    }


    void setAtomColor(uint idx, uint32_t v)
    {
        atomList[idx]->setColor(v);
    }

    void setAtomSymbol(uint idx, QString s)
    {
        atomList[idx]->modify2Symbol(s);
    }


    void setAtomRadius (uint idx, double r)
    {
        atomList[idx]->setRadius(r);
    }

    void setAtomPos (uint idx, vector3 p)
    {
        if (idx >= NumAtoms()) return;
        atomList[idx]->setPos(p);
    }

    void setAtomPos (uint idx, double x, double y, double z)
    {
        if (idx >= NumAtoms()) return;
        vector3 p(x,y,z);
        atomList[idx]->setPos(p);
    }


    double atomicMass (uint idx)
    {
        if (idx<NumAtoms())
            return atomList[idx]->Mass();
        return 0.0;
    }

    QString atomSymbol (uint idx) {
        if (idx<NumAtoms() )
            return atomList[idx]->Symbol();
        return "X";
    }

    uint atomicNum (uint idx) {
        if (idx<NumAtoms() )
            return atomList[idx]->atomicNum();
        return 0;
    }


public:
    void setParent (MainWindow * p) {m_parent=p;}

    void loadFile(QString);
    void load_xyz(QString );
    void load_cif(QString );

    void load_mol(QString );
    void load_other(QString );

    //void load_cml(QString );



    void load_GaussianCube(QString );
    void load_GaussianOutPut(QString );
    void load_GaussianFchk(QString );


    void load_CP2K_inp(QString );

    void decreaseBondOrder(uint , float);
    void rectifyBondOrder(uint idx);
    void perceiveBondOrder();
    void perceiveBondOrder_OpenBabel();


    void addRing (vector <uint> idList);


    void addRing (vector <uint> idList,
                  vector3 ringCenter, vector3 ringNormal,
                  double ringRadius,double ringScale,
                  vector3 color,   bool ringAromatic);

    void addArc (vector <uint> idList,
                  vector3 ringCenter, vector3 ringNormal,
                  double ringRadius, bool ringAromatic);


    void perceiveRings_A();
    void perceiveRings_OB();
    void perceiveRings();
    void displayRingList();




    uint NumBonds() {return bondList.size();}
    uint NumAtoms() {return atomList.size();}
    void displayBonds();
    void displayAtoms();



    QString getAtomSymbol (uint id) {return atomList[id]->Symbol();}
    HAtom * getAtombyIndex(uint id) {if (id < atomList.size ()) return atomList[id];
                                                                return nullptr;}
    HAtom * getLastAtom();

    double  getAtomXbyIndex(uint id) {return atomList[id]->x();}
    double  getAtomYbyIndex(uint id) {return atomList[id]->y();}
    double  getAtomZbyIndex(uint id) {return atomList[id]->z();}
    double  getAtomRadiusbyIndex(uint id) {return atomList[id]->radius();}

    vector3 getAtomPosbyIndex(uint id) {return atomList[id]->Pos();}




    void centeralize();
    void rotate( vector3  v1, vector3  v2 );
    void rotate( vector3  v, double angle );
    void rotate (matrix3x3 );
    void translate(vector3 p);
    void translate(double x, double y, double z);

    bool atomExisted(uint,double ,double ,double);
    bool atomExisted(uint, vector3);
    bool atomExisted(QString ,double ,double ,double);
    bool atomExisted(QString ,vector3);


    void addAtom(string , vector3 p);
    void addAtom(QString ,double ,double ,double );
    void addAtom(QString , vector3);
    void addAtom(string ,double ,double ,double);
    void addAtom(uint, double ,double ,double);
    void addAtom(uint ,vector3 p);

    bool addAtom(HAtom * other);


    void appendAtom(QString ,double ,double ,double);
    void appendAtom(uint atomicNum,double ,double ,double);

    void insertAtom(uint idx, QString s,double x,double y ,double z);
    void insertAtom(uint idx, uint atomicNum, double x,double y ,double z);

    void swapTwoAtom(uint from, uint to);

    void addBond(uint , uint );
    void addBond(uint , uint , uint );


    QString getCoordinates();

    bool isAtomVisible() {return isAtomvisible;};

    void recoverAtomCoordinateFromBackup();
    void removeBackupAtoms();

    void removeAllXAtoms();
    void removeAllAtoms();
    void removeAllBonds();
    void removeAllRings();
    void removeAllArcs();
    void clearAll() ;

    bool hasSelectedAtoms();

    void removeSingleAtombyId(uint);
    void removeAtombyId(uint ); //remove related terminal hydrogen
    void removeBondbyId(uint );

    void removeAtombyId(vector <uint> );
    void removeLonelyHydrogen();
    void removeTerminalHydrogen(uint );
    bool removeOneTerminalHydrogen(uint );


    void removeBondbyAtomId(uint );
    void removeBondbyAtomIds(uint,uint );

    void buildOBMol();
    bool fromOBMol();


    void setFileName (QString s) {fileName=s;}
    QString getFileName () {return fileName;}


    vector3 Center() {return center;}
    void setCenter(vector3 c) {center=c;}


    void perceiveBonds ();
    void perceiveBonds(int idxofAtom);

    void setPointGroup(QString s) {PointGroup=s;}

    void addHydrogen (uint idx=0);    //add one H each time
    void autoAddHydrogen(uint ); //for single atom
    void autoAddHydrogen();  //for whole molecule
    void autoAdjustHydrogen(uint ); //for single atom
    void autoAdjustHydrogen( ); //for whole atom
    void autoAdjustHydrogenWhole(); //for whole molecule

    void runMolecularMechanics(QString method="MMFF94");
    void cleanMol();
    void runXTB();
    void thermalDisplacement(double);



    vector3 getVolume() {return xyzLength;}
    void calcMolVolume();

    void perceiveHydrogenBonds();
    void perceiveHydrogenBondId(uint id);



    void perceivePlaneRingforBonds();
    void perceivePlaneBondbyId(uint);
    void perceivePlaneBonds();
    void setView3D (View3D * v) ;

    void linkTemplate2Mol(templateMol *, uint , uint);
    void addAtomfromTemplate(templateMol * , vector3 );
    void updateAllAtomLinker();
    void updateAtomLinkerId(uint idx);

    uint numRings() {return ringList.size();}
    uint NumRings() {return ringList.size();}
    HRing * getRingbyId(uint id) {return ringList[id];}
    HRing * getRingbyIndex(uint id) {return ringList[id];}


    bool canbePaste();
    void pastefromClipboard();

    uint numArcs()  {return arcList.size();}
    HArc * getArcbyId(uint id) {return arcList[id];}
    void displayArcList();
    void perceiveArcs();
    void removeArcbyId(uint );
    bool isAtomInsideArc(uint idAtom, uint idArc);
    void tackleTail(uint );
    void tackleHead(uint);
    deque <uint > candidate;






    //-----------------------------------------------------------------------------------
    void setBondLengthBetweenTwoFragments(uint idx1, uint idx2, double length,
                                     vector <uint> & fragment1,  vector <uint> & fragment2,
                                     bool , bool );
    void setBondLengthBetweenTwoAtoms(uint idx1, uint idx2, double length,
                                     bool , bool );

    void setBondAngleAtoms (uint idx0, uint idx1, uint idx2, double bondAngle,
                                                       bool, bool );
    void setBondAngleFragments (uint idx0, uint idx1, uint  idx2, double bondAngle,
                               vector <uint> & , vector <uint> & ,
                               bool , bool);


    void setTorsionAtoms(uint idx0, uint idx1, uint idx2, uint idx3,
                         double , bool  , bool );

    void setTorsionFragments(uint idx0, uint idx1, uint idx2, uint idx3, double ,
                            vector <uint> & ,  vector <uint> & ,
                            bool  , bool );


    QString parseGaussin09Summary(QString );
    void loadxyzfromQString (QString );
    void addatomfromQString(QString );



    double  getAtomScalebyIndex(uint id) {return atomList[id]->getScale();}

    bool isAtomInsideRing(uint , uint  );
    void removeSingleRingbyId(uint);
    void removeRingbyId(uint);
    uint getNeighborinsideRing(uint , uint );
    void updateAtomInsideRingOrNot(uint );

    AtomHybridization perceiveHybridization(uint );
    void perceiveAllAtomHybridization();

    void replaceSelelctedAtomWith(uint ,vector <uint> );

    void removeX();
    void reCalculateBonds();

    void  setRingAromatic(uint, bool);

    bool isAtomExisted(QString );
    bool isHydrogenAtomExisted();
    bool isHydrogenAtom(uint );
    bool isXExisted();
    void moveAtomFromTo(uint , uint);

    bool checkClipboard();


public:
    vector < double > huckelEigValues;
    vector < vector < double > > huckelEigVecs;
    QList <uint > huckelCarbonList;
    uint NumHuckelElectrons;

    QString getMolShape() {return molShape;}
    void standardOrient();
    void perceptMoleculeShape();
    void perceiveMolLength(double &  , double & );
    QString molShape;


public:
    bool writeMol(const QString &, const QString &);

    void tranform(vector3, matrix3x3 );

    //paste and avoiding overlap
    QString getShift(QString data);
    vector3   shiftVector;


//huckel method
public:
    bool huckelCalcuation();
    void collectHuckelAtoms();
    void collectHuckelSphericalAtoms();
    uint numHuckelCarbons();

    double getMolLength() {return xyzLength.length();}
    vector3 xyzLength;
    void orient2PlaneXOY();


    void setCharge (int cc) {charge=cc;}
    int  getCharge() {return charge;}

private:
    OpenBabel::OBMol  obmol;
    vector <uint> notXIdx;

    int charge;
    HGraph * m_graph;
    View3D * view3d;
    QString fileName;
    QString PointGroup;

    uint idxActiveMO;

    uint idx;
    bool isVisible;

    vector3 center;


    QString formula;
    bool isAtomvisible;


    double molLength;


    matrix3x3 IMomentMatrix;

};


#endif // HMOL_H
