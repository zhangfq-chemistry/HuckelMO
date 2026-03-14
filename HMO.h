#ifndef HMO_H
#define HMO_H

#include <QVector>
#include "math/vector3.h"
#include "Mol.h"

using namespace  std;


struct HBasisSets
{
    unsigned idxAtom;
    QString atomLabel;
    int charge;

    int n,Z;

    bool isOriented;
    vector3 coord,orient;

    matrix3x3 rotateM,rev_rotateM; //for sphere molecule
};

struct polyDataOrbitalMO
{
    int idx;                //orbital index
    vtkPolyData * posLobe;  //positive data
    vtkPolyData * negLobe;  //negative data
    vtkPolyData * zeroNode; //node data
};

struct polyDataOrbitalAO
{
    int idx;                //orbital index
    vtkPolyData * posYlmData;   //positive AO
    vtkPolyData * negYlmData;   //negative AO
    double scale;
};

class HMO
{
public:
    HMO();
    ~HMO();

    void clearVTKpolyData();
    void clearAll();
    void setMol (HMol * m) {mol=m;}
    HMol * mol;

    double buildHuckelMO(int );
    double calc_Psi(int idx, double x, double y, double z);
    void   buildNodePlane();


    double Psi(vector3);
    double Psi(double x, double y, double z);

    double AO(int idx,vector3 pos);
    double Ylm(int idx,vector3 pos);
    double YlmbyName(uint idx, vector3 pos);
    double Rnl(int idx,vector3 pos);

    uint numHuckelCarbons();

    void reBuildHuckelAO(double );
    void buildBasis();
    void runHuckel();

    void buildYlmSurface();

    bool isSurfaceOK() {return _isSurfaceOK;}

    void clearMolBasisSets();
    HBasisSets * getBasisSet(uint );
    HBasisSets * getLastBasisSet( );
    QString displayBasisSets();

    QVector <HBasisSets *> MolBasisSets;
    vector < double > huckelEigValues;
    vector < vector < double > > huckelEigVecs;
    vector < double > scaleAO;
    QList <uint > huckelCarbonList;

    vtkSmartPointer <vtkPolyData>  surfaceYlm_pos,surfaceYlm_neg;
    vtkPolyData * pData,  * pData1, * pDataNode; //Psi
    vtkPolyData * ylmData,  * ylmData1; //Ylm

    //store generated orbital polyData
    void clearOrbitalDataMO();
    void clearOrbitalDataAO();

    QVector <polyDataOrbitalMO *> OrbitalDataMO;
    QVector <polyDataOrbitalAO *> OrbitalDataAO;

    int numElectrons() {return numHuckelElectrons;}

    void setCharge (int cc) {_charge=cc;}
    int  getCharge() {return _charge;}


private:
    int _charge;
    int idxActiveMO;
    double molLength;
    int numHuckelElectrons;
    int _numBasisSets;
    bool _isSurfaceOK;

    double isoValue;
    double interval;

    double shift;
    double xBox,yBox,zBox;

    //int Nx,Ny,Nz;
    vector3 xyzLength;
};


#endif // HMO_H
