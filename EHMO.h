#ifndef EHMO_H
#define EHMO_H

#include <QVector>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "math/vector3.h"
#include "Mol.h"

#include "libmsym/msym_msym.h"

class EHMOException : public std::runtime_error {
public:
    explicit EHMOException(const std::string& msg) : std::runtime_error(msg) {}
};

struct SlateBasisSets
{
    unsigned idxAtom;
    QString atomLabel;
    int charge;

    QString orbitalName;
    QString fullName;
    int n;
    int l;
    int m;

    int electrons;  //number of valence electron

    int numZeta;

    double mass;
    vector3 coord;
    double IP;

    vector <double> Exp;
    vector <double> Coeff;

    vector <int > indexList;
};


struct polyDataOrbitalHMO
{
    int idx;                //orbital index
    vtkPolyData * posLobe;  //positive data
    vtkPolyData * negLobe;  //negative data
    vtkPolyData * zeroNode; //node data
};


class EHMO
{
public:
    EHMO();
    ~EHMO();

    void clearAll();
    void clearVTKpolyData();
    void setMol (HMol * m) {mol=m;}
    HMol * mol;


    //void buildYlmSurface();
    void buildExtendedHuckelMO(int );

    double Psi(vector3);
    double Psi(double x, double y, double z);

    double AO(int idx,vector3 pos);
    double Ylm(int idx,vector3 pos);
    double YlmbyName(int idx, vector3 pos);
    double Rnl(int idx,vector3 pos);


    //https://blog.csdn.net/lizun7852/article/details/88753218
    void buildBasis();
    void buildBasis_TB();
    void clearMolBasisSets();
    SlateBasisSets * getBasisSet(uint );
    SlateBasisSets * getLastBasisSet( );
    QString displayBasisSets();

    void runEHMO();
    void runEHMO_A();


    int em(int m1,int m2);
    double clm(int l1,int l2, int L,int m1, int m2,int M);
    double Phi(double m1, double phi);
    double slm(int l1,int  m1, double theta, double phi);
    double tlm(int a, int l1, int m1,int l2, int m2, double theta, double phi);
    double plm(int l, int a, double theta);

    double Fmn(int m, int n1, int n2);
    double Bn3(int k, double p) ;
    double An3(int k1, int k,double p);

    double An(int k, double p) ;
    double An_(int k, double p) ;
    double Bn(int k, double p) ;

    double dlbt(int l, int lambda_, int beta);

    double galbet(int l1, int l2, int lambda_, int alpha, int beta);

    double overlap(int n1, int l1, int n2, int l2, int lambda_, double p, double t);

    double SlaterOverlap(int n1, int l1, int m1, double zeta1,
                         int n2,int  l2, int m2, double zeta2,
                         double r, double theta, double phi) ;

    double SlaterOverlapCartesian(int n1, int l1, int m1, double zeta1, double x1, double y1, double z1,
                                  int n2, int l2, int m2, double zeta2, double x2, double y2, double z2);


    double SlaterOverlapCartesian(int n1, int l1, int m1, double zeta1, vector3 pos1,
                                   int n2, int l2, int m2, double zeta2, vector3 pos2);
    double SlaterOverlap(SlateBasisSets * , SlateBasisSets *);

    unsigned numBasisSets() const {return MolBasisSets.size();}


    //tight-binding


    //store generated orbital polyData
    void clearOrbitalDataMO();
    QVector <polyDataOrbitalHMO *> OrbitalDataMO;



    QVector <SlateBasisSets *> MolBasisSets;

    vector < double > huckelEigValues;
    vector < vector < double > > huckelEigVecs;
    vector < vector <int> > OrbitalAQNList; //angular(azimuthal) quantum number
    vector <int> orbitalList;

    vtkSmartPointer <vtkPolyData>  surfaceYlm_pos,surfaceYlm_neg;
    vtkPolyData * pData,  * pData1, * pDataNode; //Psi
    vtkPolyData * ylmData,  * ylmData1; //Ylm

    bool isSurfaceOK() const {return _isSurfaceOK;}

    bool validateBasis() const;
    bool validateMolecule() const;

    //-----------------------------------------------------------------------------------//
    int  KroneckerDelta(int i,int j);

    // Symmetry-aware EHMO
    bool detectPointGroup();
    void runEHMO_Symmetry();
    bool verifySymmetry();
    void alignMoleculeToStandardOrientation();

    struct IrrepInfo {
        int speciesIndex;
        QString name;
        int dimension;
        int blockSize;
        int blockStart;
    };
    QVector<IrrepInfo> irrepInfos;
    QString symPointGroup;
    QVector<QString> moIrrepLabels;

private:
    int idxActiveMO;
    int numElectrons;
    int _numBasisSets;
    bool _isSurfaceOK;

    double isoValue;
    double interval;

    double shift;
    double xBox,yBox,zBox;

    vector < double > scaleAO;
    double molLength;

    vector3 xyzLength;
};

#endif // EHMO_H
