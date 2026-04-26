#include "HMO.h"
#include <vtkAppendPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkSphereSource.h>
#include <vtkImageData.h>
#include <vtkMarchingCubes.h>
#include <vtkFlyingEdges3D.h>

#include <iostream>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <BaseMathOperators.h>
#include <MathOperators.h>
#include <Complex.h>
#include <Matrix.h>

#include <armadillo>


using namespace std;
using namespace arma;
using namespace hmma;


HMO::HMO()
{
    MolBasisSets.clear();
    _isSurfaceOK=false;
    clearAll();
}


void HMO::clearAll()
{
    _isSurfaceOK=false;
    scaleAO.clear();

    clearMolBasisSets();
    clearVTKpolyData();
}


void HMO::clearVTKpolyData()
{
    _isSurfaceOK=false;

    if (pData)   {
        pData=vtkPolyData::New();
        pData->Delete();
        pData=nullptr;
        cout << "pData  is done!"<<endl;
    }

    if (pData1)   {
        pData1=vtkPolyData::New();
        pData1->Delete();
        pData1=nullptr;
        cout << "pData1  is done!"<<endl;
    }

    if (pDataNode)   {
        pDataNode=vtkPolyData::New();
        pDataNode->Delete();
        pDataNode=nullptr;
        cout << "pDataNode  is done!"<<endl;
    }

    if (ylmData)   {
        ylmData=vtkPolyData::New();
        ylmData->Delete();
        ylmData=nullptr;
        cout << "yData  is done!"<<endl;
    }

    if (ylmData1)   {
        ylmData1=vtkPolyData::New();
        ylmData1->Delete();
        ylmData1=nullptr;
        cout << "yData1  is done!"<<endl;
    }


    if (ylmData)   {
        ylmData=vtkPolyData::New();
        ylmData->Delete();
        ylmData=nullptr;
        cout << "ylm-Data  is done!"<<endl;
    }

    clearOrbitalDataAO();
    clearOrbitalDataMO();
}


void HMO::clearMolBasisSets()
{
    if (MolBasisSets.size() < 1) return;

    for (auto it=MolBasisSets.begin();it!=MolBasisSets.end();++it)
    {
        if(*it != nullptr) {
            delete (*it);
            (*it) = nullptr;
        }
    }

    MolBasisSets.clear();
    QVector<HBasisSets*>().swap(MolBasisSets);

    _isSurfaceOK=false;
}


void HMO::clearOrbitalDataAO()
{
    if (OrbitalDataAO.size() < 1) return;

    for (auto it=OrbitalDataAO.begin();it!=OrbitalDataAO.end();++it)
    {
        if(*it != nullptr)
        {
            (*it)->posYlmData->New();
            (*it)->posYlmData->Delete();

            (*it)->negYlmData->New();
            (*it)->negYlmData->Delete();

            delete (*it);
            (*it) = nullptr;
        }
    }

    OrbitalDataAO.clear();
    QVector<polyDataOrbitalAO*>().swap(OrbitalDataAO);
    OrbitalDataMO.clear();
}


void HMO::clearOrbitalDataMO()
{
    if (OrbitalDataMO.size() < 1) return;

    for (auto it=OrbitalDataMO.begin();it!=OrbitalDataMO.end();++it)
    {
        if(*it != nullptr)
        {
            (*it)->negLobe->New();
            (*it)->negLobe->Delete();

            (*it)->posLobe->New();
            (*it)->posLobe->Delete();

            (*it)->zeroNode->New();
            (*it)->zeroNode->Delete();

            delete (*it);
            (*it) = nullptr;
        }
    }

    OrbitalDataMO.clear();
    QVector<polyDataOrbitalMO*>().swap(OrbitalDataMO);
    OrbitalDataMO.clear();
}



HBasisSets * HMO::getBasisSet(uint i)
{
    return MolBasisSets[i];
}



HBasisSets * HMO::getLastBasisSet()
{
    return MolBasisSets[MolBasisSets.size()-1];
}



uint HMO::numHuckelCarbons()
{
    return mol->huckelCarbonList.size();
}

void HMO::buildBasis()
{
    clearAll();

    mol->collectHuckelAtoms();

    numHuckelElectrons=0;

    QString shape=mol->getMolShape();

    for(int i=0;i<mol->huckelCarbonList.size();i++)
    {
        uint idx=mol->huckelCarbonList[i];
        HBasisSets * bas = new HBasisSets;

        bas->idxAtom=idx;
        bas->atomLabel=mol->getAtomSymbol(idx);
        bas->coord=mol->atomPos(idx);

        bas->n=2; //C,N,O,F

        bas->Z=Symbol2Z(bas->atomLabel);


        if(bas->Z==6)
            numHuckelElectrons+=1;
        if(bas->Z==7)
            numHuckelElectrons+=2;
        if(bas->Z==8)
            numHuckelElectrons+=2;
        if(bas->Z==9)
            numHuckelElectrons+=2;


        bas->isOriented=false;
        bas->orient=VZ;

        vector3 v=mol->atomPos(idx);
        if(shape=="Sphere")
        {
            bas->isOriented=true;
            bas->rotateM.SetupRotateMatrix(VZ,v);
            bas->rev_rotateM=bas->rotateM.inverse();
            bas->orient=v.normalize();
        }
        MolBasisSets.push_back(bas);
        scaleAO.push_back(molLength);
    }

    numHuckelElectrons = numHuckelElectrons-_charge; 
}


void HMO::runHuckel()
{
    huckelCarbonList.clear();

    for(int i=0;i<mol->huckelCarbonList.size();i++)
        huckelCarbonList.push_back(mol->huckelCarbonList[i]);

    uint numCarbonAtoms = huckelCarbonList.size();

    if (numCarbonAtoms<2) return;


    DDMatrix Huckel(numCarbonAtoms,numCarbonAtoms,0.0);

    //omp_set_num_threads(4);
    for(size_t i=0;i<numCarbonAtoms;i++)
    {
        for(size_t j=i+1;j<numCarbonAtoms;j++)
        {
            if(mol->getAtombyIndex(huckelCarbonList[i])->isLinkded(huckelCarbonList[j]))
            {
                Huckel(i,j)=1.0;
                Huckel(j,i)=1.0;
            }
        }
    }


    cout<<"buid Huckel matrix done!\n";

    DDMatrix eigenvals;
    DDMatrix eigenvecs;
    Huckel.eigen_space(eigenvals, eigenvecs, true);

    //cout << " coefficient of orbitals \n";
    //eigenvecs.dump (std::cout) << std::endl << std::endl;

    vector < double > v;

    huckelEigValues.clear();
    huckelEigVecs.clear();
    for(size_t i=0;i<numCarbonAtoms;i++)
    {
        //energy of huckel (x = -eigenvalue of adjacency matrix)
        huckelEigValues.push_back(-eigenvals(0,i));

        //coefficient of orbital
        v.clear();
        for(size_t j=0;j<numCarbonAtoms;j++)
            v.push_back(eigenvecs(j,i));

        huckelEigVecs.push_back(v);
    }

    cout << "Huckel Calculations Done!"<<endl;
}




void HMO::reBuildHuckelAO(double scale)
{
    uint numCarbon=huckelEigValues.size();
    if (numCarbon<1) return;
    scaleAO[idxActiveMO]=scale;
    buildYlmSurface();
}




//huckel simple AO
void HMO::buildYlmSurface()
{
    double x,y,z,radius;

    auto appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
    auto appendFilter1 = vtkSmartPointer<vtkAppendPolyData>::New();

    vector3 v1,v0,v;

    uint size=MolBasisSets.size();
    for (uint i=0;i<size;i++)
    {
        radius=huckelEigVecs[idxActiveMO][i]*scaleAO[idxActiveMO];
        v0=MolBasisSets[i]->coord;

        v1=radius*VZ;
        if (mol->getMolShape()=="Sphere")
            v1=radius*MolBasisSets[i]->orient;

        auto sphere = vtkSmartPointer<vtkSphereSource>::New();
        sphere->SetPhiResolution(20);
        sphere->SetThetaResolution(20);
        sphere->SetRadius(fabs(radius));

        //positive
        v=v0+v1;
        sphere->SetCenter(v.x(),v.y(),v.z());
        sphere->Update();

        //negative
        auto sphere1 = vtkSmartPointer<vtkSphereSource>::New();
        sphere1->SetPhiResolution(20);
        sphere1->SetThetaResolution(20);
        sphere1->SetRadius(fabs(radius));

        v=v0-v1;

        sphere1->SetCenter(v.x(),v.y(),v.z());
        sphere1->Update();

        appendFilter->AddInputData(sphere->GetOutput());
        appendFilter1->AddInputData(sphere1->GetOutput());
    }

    //positive
    appendFilter->Update();
    auto cleanFilter =  vtkSmartPointer<vtkCleanPolyData>::New();
    cleanFilter->AddInputData(appendFilter->GetOutput());
    cleanFilter->Update();
    ylmData=vtkPolyData::New();
    ylmData->DeepCopy(cleanFilter->GetOutput());

    //negtive
    appendFilter1->Update();
    auto cleanFilter1 =  vtkSmartPointer<vtkCleanPolyData>::New();
    cleanFilter1->AddInputData(appendFilter1->GetOutput());
    cleanFilter1->Update();
    ylmData1=vtkPolyData::New();
    ylmData1->DeepCopy(cleanFilter1->GetOutput());
}



double HMO::Psi(double x, double y, double z)
{
    vector3 p(x,y,z);

    int numBS=MolBasisSets.size();

    //double Psi_2Pz;
    double a0=0.529177;

    vector3 v;
    double perfactor=1.0;

    int Z;


    double value=0.0;
    double coeff=0.0;
    double length=0.0;

    // Calculate value without spawning nested threads (which adds overhead and race condition)
    for (int i=0; i<numBS; i++)
    {
        coeff=huckelEigVecs[idxActiveMO][i];
        if(abs(coeff)<0.01) continue;

        Z=MolBasisSets[i]->Z;
        
        //plane shape
        v = p - MolBasisSets[i]->coord;
        length=v.length();
        if (length > 8.0) continue;

        //spherical fullene
        if(MolBasisSets[i]->isOriented)
             v = MolBasisSets[i]->rev_rotateM*v;

        value += coeff*sqrt(pow(Z,5)/3.141592654)* v.z() * exp(-Z*length);
    }
    return value;
}


double HMO::buildHuckelMO(int idx)
{
    idxActiveMO=idx;

    bool isExisted=false;
    int i=0;
    if (OrbitalDataMO.size()>0)
    {
        for(i=0;i<OrbitalDataMO.size();i++)
        {
            if(OrbitalDataMO[i]->idx!=idx) continue;
            isExisted=true;
            break;
        }
    }

    if(isExisted)
    {
        pData->Delete();
        pData1->Delete();
        pDataNode->Delete();

        pData=vtkPolyData::New();
        pData1=vtkPolyData::New();
        pDataNode=vtkPolyData::New();

        pData->DeepCopy(OrbitalDataMO[i]->posLobe);
        pData1->DeepCopy(OrbitalDataMO[i]->negLobe);
        pDataNode->DeepCopy(OrbitalDataMO[i]->zeroNode);

    }
    else {

        isoValue=0.01;
        interval=0.1;

        xyzLength=mol->getVolume();
        molLength=xyzLength.length();

        //scale of AO
        scaleAO.clear();
        for(int i=0;i<MolBasisSets.size();i++)
            scaleAO.push_back(molLength);

        double shift=2.0;
        xBox=xyzLength.x()+shift;
        yBox=xyzLength.y()+shift;
        zBox=xyzLength.z()+shift;

        xBox= int(xBox*10.0)/10.0;
        yBox= int(yBox*10.0)/10.0;
        zBox= int(zBox*10.0)/10.0;

        int Nx=int(2.0*xBox/interval);
        int Ny=int(2.0*yBox/interval);
        int Nz=int(2.0*zBox/interval);

        if(Nx%2==1) Nx=Nx+1;
        if(Ny%2==1) Ny=Ny+1;
        if(Nz%2==1) Nz=Nz+1;


        double  x0=-interval*Nx/2.0,
                y0=-interval*Ny/2.0,
                z0=-interval*Nz/2.0;

        cout << "\nbuilding cube data:  " <<endl;
        cout << "dim: " <<Nx <<" "<<Ny <<" "<<Nz <<endl;
        cout << "range: " <<xBox  <<" " << yBox<< " " << zBox<< endl;
        cout << "interval: " <<interval  << endl;
        cout << "isovalue: " <<isoValue  << endl;

        //cout << x0<<" "<< y0<<" "<< z0<<endl;
        //cout << x0+interval*Nx<<" "<< y0+interval*Ny<<" "<< z0+interval*Nz<<endl;

        auto volume = vtkSmartPointer<vtkImageData>::New();
        volume->SetDimensions(Nx,Ny,Nz);
        volume->SetSpacing(interval, interval,interval);
        volume->AllocateScalars(VTK_DOUBLE,1);
        volume->SetOrigin(-xBox,-yBox,-zBox);
        volume->Modified();

        double* scalarPtr = static_cast<double*>(volume->GetScalarPointer());

        #pragma omp parallel for schedule(guided) collapse(2)
        for(int z=0; z < Nz;  z++)
        {
            for(int y=0; y < Ny;  y++)
            {
                double pz = z0 + interval * z;
                double py = y0 + interval * y;
                long base = static_cast<long>(z) * Ny * Nx + static_cast<long>(y) * Nx;
                for(int x=0; x < Nx;  x++)
                {
                    double px = x0 + interval * x;
                    scalarPtr[base + x] = Psi(px, py, pz);
                }
            }
        }

        cout << "Box :" <<endl;
        cout << x0 << " "<<y0<< " "<<z0<<endl;
        cout << x0+Nx*interval << " "<<y0+Ny*interval<< " "<<z0+Nz*interval<<endl;

        //Extract Isosurface

        //negative
        auto surfaceMC0 = vtkSmartPointer<vtkFlyingEdges3D>::New();
        surfaceMC0->SetInputData(volume);
        surfaceMC0->SetValue(0, isoValue);

        surfaceMC0->ComputeNormalsOn();
        surfaceMC0->Update();

        pData=vtkPolyData::New();
        pData->DeepCopy(surfaceMC0->GetOutput());

        //negative
        auto surfaceMC1 = vtkSmartPointer<vtkFlyingEdges3D>::New();
        surfaceMC1->SetInputData(volume);
        surfaceMC1->SetValue(0, -isoValue);

        surfaceMC1->ComputeNormalsOn();
        surfaceMC1->Update();

        pData1=vtkPolyData::New();
        pData1->DeepCopy(surfaceMC1->GetOutput());

        _isSurfaceOK=true;
        cout << "calculation surface done!\n\n"<<endl;

        buildNodePlane();




        //QVector <polyDataOrbitalMO *> OrbitalDataMO;
        //QVector <polyDataOrbitalAO *> OrbitalDataAO;

        polyDataOrbitalMO * mo = new polyDataOrbitalMO;
        if(mo) {
            mo->idx=idx;
            mo->posLobe=vtkPolyData::New();
            mo->negLobe=vtkPolyData::New();
            mo->zeroNode=vtkPolyData::New();

            mo->posLobe->DeepCopy(pData);
            mo->negLobe->DeepCopy(pData1);
            mo->zeroNode->DeepCopy(pDataNode);
            OrbitalDataMO.push_back(mo);
        }

    }

    buildYlmSurface();

/*
    //QVector <polyDataOrbitalAO *> OrbitalDataAO;
    polyDataOrbitalAO * ao = new polyDataOrbitalAO;
    if(ao) {
        ao->posYlmData=vtkPolyData::New();
        ao->negYlmData=vtkPolyData::New();

        ao->idx=idxActiveMO;
        ao->posYlmData->DeepCopy(ylmData);
        ao->negYlmData->DeepCopy(ylmData1);
        ao->scale=scaleAO[idx];
        //for(int i=0;i<scaleAO.size();i++)    ao->scaleAO[i]=scaleAO[idx];
        OrbitalDataAO.push_back(ao);
    }
*/
    return scaleAO[idx];
}




void HMO::buildNodePlane()
{
    //vtkRuledSurfaceFilter
    //
    //auto polydata = vtkSmartPointer<vtkPolyData>::New();
 /* polydata->SetPoints(input->GetPoints());

  // Construct the surface and create isosurface.
  auto surf = vtkSmartPointer<vtkSurfaceReconstructionFilter>::New();
  surf->SetInputData(polydata);

  auto cf = vtkSmartPointer<vtkContourFilter>::New();
  cf->SetInputConnection(surf->GetOutputPort());
  cf->SetValue(0, 0.0);

  // Sometimes the contouring algorithm can create a volume whose gradient
  // vector and ordering of polygon (using the right hand rule) are
  // inconsistent. vtkReverseSense cures this problem.
  auto reverse = vtkSmartPointer<vtkReverseSense>::New();
  reverse->SetInputConnection(cf->GetOutputPort());
  reverse->ReverseCellsOn();
  reverse->ReverseNormalsOn();
  */

    bool isExisted=false;
    int i;
    if (OrbitalDataAO.size()>0)
    for(i=0;i<OrbitalDataAO.size();i++)
    {
        if(OrbitalDataAO[i]->idx!=idxActiveMO) continue;
        isExisted=true;
        break;
    }
    if(isExisted) {
        pData->DeepCopy(OrbitalDataAO[i]->posYlmData);
        pData1->DeepCopy(OrbitalDataAO[i]->negYlmData);
        //pDataNode->DeepCopy(OrbitalDataMO[i]->zeroNode);
        return;
    }


    interval=0.1;

    double shift=2.0;
    xBox=xyzLength.x()+shift;
    yBox=xyzLength.y()+shift;
    zBox=xyzLength.z()+shift;

    xBox= int(xBox*10.0)/10.0;
    yBox= int(yBox*10.0)/10.0;
    zBox= int(zBox*10.0)/10.0;

    int Nx=2.0*xBox/interval;
    int Ny=2.0*yBox/interval;
    int Nz=2.0*zBox/interval;

    double  x0=-interval*Nx/2.0,
            y0=-interval*Ny/2.0,
            z0=-interval*Nz/2.0;

    auto volume = vtkSmartPointer<vtkImageData>::New();
    volume->SetDimensions(Nx,Ny,Nz);
    volume->SetSpacing(interval, interval,interval);
    volume->AllocateScalars(VTK_DOUBLE,1);
    volume->SetOrigin(-xBox,-yBox,-zBox);
    volume->Modified();


    #pragma omp parallel for
    for(int z=0; z < Nz;  z++)
    {
        //#pragma omp parallel for
        for(int y=0; y < Ny;  y++)
        {
            //#pragma omp parallel for
            for(int x=0; x < Nx;  x++)
            {
                static_cast<double*>(volume->GetScalarPointer(x,y,z))[0]=
                        Psi(x0+interval*x,  y0+interval*y, z0+interval*z);
            }
        }
    }


    auto surfaceMC2 = vtkSmartPointer<vtkFlyingEdges3D>::New();
    surfaceMC2->SetInputData(volume);
    surfaceMC2->SetValue(0, 0.0);
    surfaceMC2->ComputeNormalsOn();
    surfaceMC2->Update();

    pDataNode=vtkPolyData::New();
    pDataNode->DeepCopy(surfaceMC2->GetOutput());



    cout << "calculation node done!\n\n"<<endl;
}
