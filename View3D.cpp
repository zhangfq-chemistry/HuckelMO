#include <QApplication>
#include <QClipboard>

#include "View3D.h"
#include "math/vector3.h"
#include "functionals.h"
#include "mainwindow.h"
#include "templateMol.h"

#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkDataSetMapper.h>

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkDataSet.h>
#include <vtkTextWidget.h>
#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkConeSource.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkInteractorStyle.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkTDxInteractorStyleCamera.h>
#include <vtkTDxInteractorStyleSettings.h>
#include <vtkTubeFilter.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPlaneSource.h>
#include <vtkBox.h>


#include <vtkOrientationMarkerWidget.h>

#include <QVTKInteractor.h>

#include <vtkSmartPointer.h>
#include <vtkVolumeProperty.h>

#include <vtkPolyData.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include <vtkCubeSource.h>
#include <vtkSphereSource.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataNormals.h>
#include <vtkAxesActor.h>
#include <vtkTransform.h>
#include <vtkCommand.h>
#include <vtkReflectionFilter.h>
#include <vtkNamedColors.h>
#include <vtkImplicitPolyDataDistance.h>
#include <vtkVolumeProperty.h>
#include <vtkAppendPolyData.h>
#include <vtkCleanPolyData.h>


#include <vtkMolecule.h>
#include <vtkMoleculeMapper.h>
#include <vtkMoleculeToAtomBallFilter.h>
#include <vtkSimpleBondPerceiver.h>
#include <vtkTriangle.h>
#include <vtkPolygon.h>
#include <vtkLight.h>

#include <vtkStringArray.h>
#include <vtkPointSetToLabelHierarchy.h>
#include <QMessageBox>
#include <QTime>

#include <vtkPolyhedron.h>
#include <vtkPentagonalPrism.h>
#include <vtkHexagonalPrism.h>
#include <vtkPolygon.h>
#include <vtkRegularPolygonSource.h>
#include <vtkDiskSource.h>
#include <vtkIntersectionPolyDataFilter.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkVectorText.h>
#include <vtkTextActor.h>
#include <vtkTransformFilter.h>
#include <vtkTexturedSphereSource.h>
#include <vtkTransformTextureCoords.h>
#include <vtkTexture.h>
#include <vtkTextSource.h>
#include <vtkCoordinate.h>
#include <vtkTubeFilter.h>
#include <vtkParametricTorus.h>
#include <vtkParametricFunctionSource.h>

#include <vtkFollower.h>
#include <vtkVectorText.h>
#include <vtkLabeledDataMapper.h>
#include <vtkAssembly.h>
#include <vtkCoordinate.h>
#include <vtkMatrix4x4.h>
#include <vtkTransformCoordinateSystems.h>

#include <QColorDialog>

#include "templateView3d.h"
#include "HMO.h"
#include "EHMO.h"

// important
//int Point::_tag = 0;

void View3D::initializeActor (vtkActor * actor)
{
    if(actor==nullptr) return;

    actor->setId (-1);
    actor->setBondOrder (-1);
    actor->setRingId (-1);
    actor->setMolId  (-1);
    actor->setAtomId (-1);
    actor->setAtomId1 (-1);
    actor->setBondId (-1);

}


HMol * View3D::getMol ()
{
    return mol;
}


MainWindow * View3D::getMainWindow ()
{
    return mainWindow;
}


void View3D::setTemplateView3d (TemplateView3d  * t)
{
    view3dT=t;
}



View3D::View3D(QWidget *parent): QVTKOpenGLNativeWidget(parent)
{
    this->setWindowTitle("3D Viewer");

    vtkNew<vtkGenericOpenGLRenderWindow> window;
    setRenderWindow(window.Get());


    //background color
    bkcolor.Set(0.4,0.5,0.6);
    whiteColor.Set(1.0,1.0,1.0);
    //bkcolor.Set(0.8,0.8,0.8);

    xColor.Set(1.0,0.0,0.0);
    yColor.Set(0.0,1.0,0.0);
    zColor.Set(1.0,1.0,0.0);



 /*
  posLobeColor.Set(0.90,0.56,0.14);
  negLobeColor.Set(0.21,0.44,0.53);
*/

    // Camera
    camera = vtkSmartPointer<vtkCamera>::New();
    camera->SetViewUp(0,1,0);
    camera->SetPosition(0,0,20);
    camera->SetFocalPoint(0,0,0);
    //camera->SetParallelScale(0.0);
    camera->ParallelProjectionOn();





    // Renderer
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderer->SetActiveCamera(camera);
    m_renderer->SetBackground(bkcolor.x(), bkcolor.y(), bkcolor.z());
    renderWindow()->AddRenderer(m_renderer);

    m_renderer->LightFollowCameraOn();
    m_renderer->TwoSidedLightingOn();


    _isXYZAxisVisible=false;

    plane=nullptr;
    clipper_pos=nullptr;
    clipper_neg=nullptr;


    opacity_node=0.5;
    center=VZero;

    scale_atom=0.3;
    scale_bond=0.3;

    bondRadius=0.07;


    setMolTypeBallStick();


    moleculeLength=4.0;

    PointGroup="C1";
    discThickness=0.05;
    discRadius=0.2;

    updateSelectedFromRendering=false;
    origin_SelectedAtoms=VZero;
    length_SelectedAtoms=0.0;

    firstTime=true;


    setAreaInteractor();

    tmol = new templateMol();
    _isKekuleStyle=false;

    //mouseOperationMode=Rotation;

    selectedAtomActors = vtkSmartPointer<vtkActorCollection>::New();
    selectedBondActors = vtkSmartPointer<vtkActorCollection>::New();
    selectedRingActors = vtkSmartPointer<vtkActorCollection>::New();
    atomLabelActors= vtkSmartPointer<vtkActorCollection>::New();

    selectedAtomActors->InitTraversal();
    selectedBondActors->InitTraversal();
    selectedRingActors->InitTraversal();
    atomLabelActors->InitTraversal();

    selectedAtomList.clear();
    selectedBondList.clear();
    selectedRingList.clear();

    initParameter();
    onViewReset();
    //camera->SetThickness(50.0);

    isExtendedHuckel=isHuckel=false;
}

void View3D::initParameter()
{
    selectedColor=buildColor(Qt::yellow);

    grey[0]=0.6;
    grey[1]=0.6;
    grey[2]=0.6;

    white[0]=1.0;
    white[1]=1.0;
    white[2]=1.0;

    black[0]=0.0;
    black[1]=0.0;
    black[2]=0.0;



    blue[0]=0.0;
    blue[1]=0.0;
    blue[2]=1.0;

    yellow[0]=1.0;
    yellow[1]=1.0;
    yellow[2]=0.2;

    brown[0]=0.5;
    brown[1]=0.3;
    brown[2]=0.7;



    isLobeMesh=true;
    isLobeSmooth=true;
    isNegLobeVisible=isPosLobeVisible=true;
    isMOVisible=true;
    isNodeVisible=false;



    //posLobeColor.Set(0.9,0.56,0.14);
    //negLobeColor.Set(0.21,0.44,0.53);

    //posLobeColor.Set(0.67,0.34,0.67);
    //negLobeColor.Set(0.13,0.62,0.62);

    nodecolor.Set(82.0/255.0,139.0/255.0,113.0/255.0);
    posLobeColor.Set(213.0/255.0,122.0/255.0,213.0/255.0);
    negLobeColor.Set(111.0/255.0,137.0/255.0,255.0/255.0);


    opacity_pos=opacity_neg=opacity_node=1.0;
    orbitalLineWidth=0.02;


    verticalLength=HorizontalLengh=2.0;

    isMol_Visible=true;
    isHydrogen_Visible=true;


    _isAtomLabelVisible=false;

    scale_ringRadius=0.7;
    scale_ringSize=0.4;

}


void View3D::setAreaInteractor()
{
    actorInteractorA = SelectAreaInteractorStyle::New();
    actorInteractorA->SetDefaultRenderer(m_renderer);
    actorInteractorA->setView3d(this);
    interactor()->SetInteractorStyle(actorInteractorA);
}


void View3D::rotateMol()
{
    if(mol==nullptr) return;
    if(mol->NumAtoms()<1)  return;

    mol->rotate(VZ,2);
    renderMol();
    push2Stack("rotateMolecule");
}


void View3D::updateView()
{
     renderWindow()->Render();
}

void View3D::setBKColor(vector3 color)
{
    bkcolor=color;
    m_renderer->SetBackground(bkcolor.x(), bkcolor.y(), bkcolor.z());
    updateView();
}


void View3D:: onViewXY()
{
    camera->SetViewUp(0,1,0);

    double p[3];
    camera->GetPosition(p);
    vector3 v(p[0],p[1],p[2]);
    camera->SetPosition(0,0,v.length());

    //camera->SetFocalPoint(0,0,0);
    camera->ComputeViewPlaneNormal();
    updateView();
}



void View3D:: onViewXZ()
{
    double p[3];
    camera->GetPosition(p);
    vector3 v(p[0],p[1],p[2]);

    onViewReset();
    camera->SetPosition(0,0,v.length());
    camera->Elevation(-90);
    camera->SetFocalPoint(0,0,0);
    camera->ComputeViewPlaneNormal();
    renderWindow()->Render();
}

void View3D:: onViewYZ()
{
    double p[3];
    camera->GetPosition(p);
    vector3 v(p[0],p[1],p[2]);

    onViewReset();
    camera->SetPosition(0,0,v.length());
    camera->SetRoll(-90);
    camera->Elevation(-90);
    camera->SetFocalPoint(0,0,0);
    camera->ComputeViewPlaneNormal();
    renderWindow()->Render();
}


void View3D:: onViewReset()
{
    //onViewXY();

    camera->SetViewUp(0,1,0);
    camera->SetPosition(0,0,10);
    camera->SetFocalPoint(0,0,0);
    camera->ComputeViewPlaneNormal();


    updateView();
    updateAtomLabelState();
}

void View3D:: onView_rotateClockwise(double degree)
{
    double angle= camera->GetRoll();
    camera->SetRoll(angle-degree);
    camera->ComputeViewPlaneNormal();
    updateView();
}

void View3D:: onView_rotateCounterClockwise(double degree)
{
    double angle= camera->GetRoll();
    camera->SetRoll(angle+degree);
    camera->ComputeViewPlaneNormal();
    updateView();
}

void View3D:: onView_rotateUp(double degree)
{
    //camera->Pitch(-5);
    camera->Elevation(-degree);
    camera->ComputeViewPlaneNormal();
    updateView();
}
void View3D:: onView_rotateDown(double degree)
{
    camera->Elevation(degree);
    camera->ComputeViewPlaneNormal();
    updateView();
}

void View3D:: onView_rotateLeft(double degree)
{
    camera->Azimuth(degree);
    camera->ComputeViewPlaneNormal();
    updateView();
}

void View3D:: onView_rotateRight(double degree)
{
    camera->Azimuth(-degree);
    camera->ComputeViewPlaneNormal();
    updateView();
}


void View3D::renderAxis( )
{
    if (!_isXYZAxisVisible) return;


    double length=HorizontalLengh*1.2;
    if(length<verticalLength)
        length=verticalLength*1.5;

    double radius=length*0.02;
    double  xLength,yLength,zLength;
    xLength=yLength=zLength=length;

    double l=0.0;
    for(int i=1;i<4;i++)
    {
        auto line1 = vtkSmartPointer<vtkLineSource>::New();
        switch(i){
        case 1:
            l=xLength*0.9;
            line1->SetPoint1(-l,0,0);
            line1->SetPoint2(l,0,0);
            break;
        case 2:
            l=yLength*0.9;
            line1->SetPoint1(0,-l,0);
            line1->SetPoint2(0,l,0);
            break;
        case 3:
            l=zLength*0.9;
            line1->SetPoint1(0,0,-l);
            line1->SetPoint2(0,0,l);
            break;
        }

        auto tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
        tubeFilter->SetInputConnection(line1->GetOutputPort());
        tubeFilter->SetRadius(radius);
        tubeFilter->SetNumberOfSides(50);
        tubeFilter->SetCapping(1);

        auto actor = vtkActor::New();
        initializeActor(actor);
        auto mapper = vtkPolyDataMapper::New();
        switch(i){
        case 1:
            actor->GetProperty()->SetColor(xColor[0],xColor[1],xColor[2]);
            break;
        case 2:
            actor->GetProperty()->SetColor(yColor[0],yColor[1],yColor[2]);
            break;
        case 3:
            actor->GetProperty()->SetColor(zColor[0],zColor[1],zColor[2]);
            break;
        }
    actor->SetMapper(mapper);
    mapper->SetInputConnection(tubeFilter->GetOutputPort());
    m_renderer->AddActor(actor);

    //upper
    auto coneSource =  vtkSmartPointer<vtkConeSource>::New();
    auto actor1 = vtkActor::New();initializeActor(actor1);
    auto mapper1 = vtkPolyDataMapper::New();
    auto transform=  vtkSmartPointer<vtkTransform>::New();

    coneSource->SetResolution(100);
    coneSource->SetCapping(true);
    coneSource->SetHeight(l*0.1);
    coneSource->SetRadius(radius*3);
    coneSource->Update();

    transform->PostMultiply();

    switch(i){
    case 1:
        coneSource->SetDirection(1.0,0.0,0.0);
        actor1->GetProperty()->SetColor(xColor[0],xColor[1],xColor[2]);
        transform->Translate(l*1.01,0.0,0.0);
        break;
    case 2:
        coneSource->SetDirection(0.0,1.0,0.0);
        actor1->GetProperty()->SetColor(yColor[0],yColor[1],yColor[2]);
        transform->Translate(0.0,l*1.01,0.0);
        break;
    case 3:
        coneSource->SetDirection(0.0,0.0,1.0);
        actor1->GetProperty()->SetColor(zColor[0],zColor[1],zColor[2]);
        transform->Translate(0.0,0.0,l*1.01);
        break;
    }

    actor1->SetMapper(mapper1);
    actor1->SetUserTransform(transform);
    mapper1->ScalarVisibilityOn();
    mapper1->SetInputConnection(coneSource->GetOutputPort());
    m_renderer->AddActor(actor1);

    auto labelMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    auto labelActor = vtkSmartPointer<vtkFollower>::New();
    auto axesLabel =vtkSmartPointer<vtkVectorText>::New();
    auto transform2=  vtkSmartPointer<vtkTransform>::New();
    switch(i){
    case 1:
        axesLabel->SetText("x");
        transform2->Translate(l*1.2,-0.2,0.0);
        labelActor->GetProperty()->SetColor(xColor[0],xColor[1],xColor[2]);
        break;
    case 2:
        axesLabel->SetText("y");
        transform2->Translate(-0.2,l*1.2,0.0);
        labelActor->GetProperty()->SetColor(yColor[0],yColor[1],yColor[2]);
        break;
    case 3:
        axesLabel->SetText("z");
        transform2->Translate(-0.2,-0.2,l*1.2);
        labelActor->GetProperty()->SetColor(zColor[0],zColor[1],zColor[2]);
        break;
    }
    double scale=1.0;
    scale=l*0.1;
    transform2->PostMultiply();
    labelActor->SetMapper(labelMapper);
    labelActor->SetScale(scale,scale,scale);
    labelActor->SetUserTransform(transform2);
    labelActor->SetCamera(m_renderer->GetActiveCamera());
    labelMapper->ScalarVisibilityOn();
    labelMapper->SetInputConnection(axesLabel->GetOutputPort());

    m_renderer->AddViewProp(labelActor);
    }

}





void View3D::renderLines (vector3 a, vector3 b, vector3 c, vector3 shift, float width,double color[3])
{
    //lines
    renderLine(VZero-shift,a-shift,width,color);
    renderLine(a-shift,a+b-shift,width,color);
    renderLine(a-shift,a+c-shift,width,color);

    //b
    renderLine(VZero-shift,b-shift,width,color);
    renderLine(b-shift,a+b-shift,width,color);
    renderLine(b-shift,c+b-shift,width,color);

    //c
    renderLine(VZero-shift,c-shift,width,color);
    renderLine(c-shift,a+c-shift,width,color);
    renderLine(c-shift,b+c-shift,width,color);

    renderLine(a+b-shift,a+b+c-shift,width,color);
    renderLine(b+c-shift,a+b+c-shift,width,color);
    renderLine(a+c-shift,a+b+c-shift,width,color);
}



void View3D::clearAll()
{
    removeAllActors();

    releaseAtomLabelActors();
    releaseSelectedAtomActors();
    releaseSelectedBondActors();
    releaseSelectedRingActors();

    isHuckel = isExtendedHuckel = isXTB = false;
}


void View3D::refresh()
{
    if (!mol) return;

    removeAllActors();
    renderAxis();

    renderAtoms();
    renderBonds();

    renderOrbital();

    renderNode();

    updateView();
}


void View3D::renderMol(HMol * m)
{
    if (m==nullptr )
    {
        cout << " mol is null " << endl;
        return;
    }

    mol=m;



    removeAllActors();
    renderAxis();
    renderBonds();
    renderAtoms();

    renderOrbital();

    renderNode();

    updateView();
}


void View3D::renderSelectedAtomLabel()
{
    return;
    uint size=numSelectedAtoms();
    if(size<1) return;

    double radius=0.;
    vector3 v;

    for(uint i=0;i<size;i++)
    {
        uint idx= selectedAtomList[i];
        //cout << idx+1<<endl;

        QString label= QString::number(idx+1);

        auto textSource = vtkSmartPointer<vtkTextSource>::New();
        textSource->SetText(label.toStdString().c_str());
        textSource->SetForegroundColor(0.0, 0.0, 1.0);
        textSource->BackingOff();
        textSource->Update();

        auto  textMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
        textMapper->SetInputConnection(textSource->GetOutputPort());

        auto labelActor =  vtkSmartPointer<vtkFollower>::New();
        labelActor->SetPickable(false);
        labelActor->SetMapper(textMapper);
        labelActor->SetScale(0.02, 0.02, 0.02);
        labelActor->setAtomLabelId(idx);

        radius=scale_atom*mol->getAtomRadiusbyIndex(idx)/2.0;

        vector3 v=mol->getAtomPosbyIndex(idx);

        labelActor->AddPosition(v.x()-radius/2.,v.y()-radius/2.,v.z()+radius*2.0);
        labelActor->SetCamera(camera);
        m_renderer->AddActor(labelActor);
    }
     //cout << selectedActors->GetNumberOfItems()<< " inside render atoms" <<endl;
}

void View3D::updateAtomLabelState()
{
    atomLabelActors->InitTraversal();
    uint _numActors=atomLabelActors->GetNumberOfItems();

    if(_numActors<1) return;

    double normalC[3];
    Camera()->GetViewPlaneNormal(normalC);
    vector3 pc(normalC);  pc=pc*1.1;

    double radius,r2;
    for (int i=0;i<_numActors;++i)
    {
        radius=scale_atom*mol->getAtomRadiusbyIndex(i);
        vector3 v=mol->getAtomPosbyIndex(i)+pc*radius;
        atomLabelActors->GetNextActor()->SetPosition(v.x(),v.y(),v.z());
    }
}


void View3D::renderAtomLabel()
{
    if(!isMol_Visible) return;
    if(mol->NumAtoms()<1) return;
    if (!_isAtomLabelVisible ) return;

    atomLabelActors->RemoveAllItems();

    double normalC[3];
    Camera()->GetViewPlaneNormal(normalC);
    vector3 pc(normalC);pc=pc*1.1;

    QString label;
    double radius,r2;
    for (uint i=0;i<mol->NumAtoms();i++)
    {
        radius=scale_atom*mol->getAtomRadiusbyIndex(i);

        label= QString::number(i+1);

        // Create text
        auto textSource = vtkSmartPointer<vtkVectorText>::New();
        textSource->SetText(label.toStdString().c_str());
        textSource->Update();

        auto  textMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
        textMapper->SetInputConnection(textSource->GetOutputPort());

        auto textActor =  vtkSmartPointer<vtkFollower>::New();
        initializeActor(textActor);

        textActor->setAtomLabelId(i);
        textActor->SetPickable(false);
        textActor->SetMapper(textMapper);
        textActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        textActor->SetScale(0.2, 0.2, 0.2);

        vector3 v=mol->getAtomPosbyIndex(i)+pc*radius;
        textActor->SetPosition(v.x(),v.y(),v.z());
        textActor->SetCamera(camera);
        m_renderer-> AddActor(textActor);

        atomLabelActors->AddItem(textActor);
    }
}


void View3D::renderAtoms()
{
    if(!isMol_Visible) return;

    if(mol->NumAtoms()<1) return;

    //cout << "\n\nnumber of atoms : "<< mol->NumAtoms()<<endl;
    //cout << "render atoms! "<<endl;
    //displaySelectedAtomList();

    double x,y,z;
    vector3 color;

    uint beg=numActors();


    if (updateSelectedFromRendering)
        selectedAtomActors->RemoveAllItems();



    //cout << "selected atoms inside render Atom"<<endl;
    for (uint i=0;i<mol->NumAtoms();i++)
    {
        if (!isHydrogen_Visible)
            if(mol->getAtomSymbol(i)=="H") continue;

        x = mol->getAtomXbyIndex(i);
        y = mol->getAtomYbyIndex(i);
        z = mol->getAtomZbyIndex(i);

        auto sphere = vtkSphereSource::New();
        sphere->SetPhiResolution(20+20*scale_atom);
        sphere->SetThetaResolution(20+20*scale_atom);

        //double radius=mol->getAtomScalebyIndex(i)*mol->getAtomRadiusbyIndex(i);
        double radius=scale_atom*mol->getAtomRadiusbyIndex(i);
        sphere->SetRadius(radius);
        sphere->Update();
        color = MakeGlColor(mol->atomList[i]->Color());

        auto actor = vtkActor::New();
        initializeActor(actor);
        actor->SetPosition(x,y,z);
        auto mapper = vtkPolyDataMapper::New();
        actor->storeColor(color.x(),color.y(),color.z());

        if(mol->getAtombyIndex(i)->isSelected())
        {
            actor->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
            if (updateSelectedFromRendering) selectedAtomActors->AddItem(actor);
        }
        else
            actor->GetProperty()->SetColor(color.x(),color.y(),color.z());

        actor->SetMapper(mapper);
        actor->setAtomId(i);
        actor->setId(i+beg);

        mapper->SetInputConnection(sphere->GetOutputPort());
        m_renderer->AddActor(actor);

        mapper->Delete();
        sphere->Delete();


        if(i<mol->NumAtoms()-1) continue;
        lastAtomActor=actor;
    }

    renderAtomLabel();
}




void View3D::renderQuadrupleBond(uint idx)
{
    vector3 color;
    HBond * bond= mol->bondList[idx];
    uint idx1=bond->atomIndex0();
    uint idx2=bond->atomIndex1();

    bool _selected=mol->getBondbyIndex(idx)->isSelected();

    vector3 v0=mol->getAtomPosbyIndex(idx1);
    vector3 v1=mol->getAtomPosbyIndex(idx2);
    vector3 v=v1-v0;


    vector3  shift,shiftv;

    if (bond->getPlane().length()>0.5)
    {
        shift=0.12*bond->getPlane();
        shiftv=0.12*cross1(bond->getPlane(),v);
    }
    else {
        v.createOrthoVector(shift);
        shiftv=0.12*cross1(v,shift);
        shift=0.12*shift;
    }


    uint beg=numActors();

    //the same atom
    double s=0.5;

    //different atom, the border of two half-bonds lies the middle of two sphere-surface
    //(sphere1)--*--(sphere2)
    if(mol->getAtombyIndex(idx1)->atomicNum() != mol->getAtombyIndex(idx2)->atomicNum())
    {
        double length=(v1-v0).length();
        double r1=scale_atom*mol->getAtombyIndex(idx1)->radius();
        double r2=scale_atom*mol->getAtombyIndex(idx2)->radius();
        double d=0.5*(length-r2-r1);
        s=(d+r1)/length;
    }

    vector3 mid0=v0+s*(v1-v0);



    v0=v0+(shift+shiftv)/2.0;
    v1=v1+(shift+shiftv)/2.0;
    mid0=mid0+(shift+shiftv)/2.0;


    for (int k=0;k<2;k++)
    {
        for (int l=0;l<2;l++)
        {
            vector3 p0=v0+shift*(k-1)+shiftv*(l-1),
                    p1=v1+shift*(k-1)+shiftv*(l-1),
                    mid=mid0+shift*(k-1)+shiftv*(l-1);

            auto line = vtkSmartPointer<vtkLineSource>::New();
            line->SetPoint1(p0[0], p0[1], p0[2]);
            line->SetPoint2(mid[0], mid[1], mid[2]);

            auto tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
            tubeFilter->SetInputConnection(line->GetOutputPort());
            tubeFilter->SetRadius(bondRadius*0.8);
            tubeFilter->SetNumberOfSides(20);

            color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex0())->Color());
            auto actor = vtkActor::New();initializeActor(actor);
            auto mapper = vtkPolyDataMapper::New();

            if(_selected)
            {
                actor->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
                if (updateSelectedFromRendering) selectedBondActors->AddItem(actor);
            }
            else
                actor->GetProperty()->SetColor(color.x(),color.y(),color.z());

            actor->storeColor(color.x(),color.y(),color.z());
            actor->SetMapper(mapper);

            //actor->SetPickable(false);
            actor->setId(beg);
            actor->setAtomId(idx1);
            actor->setAtomId1(idx2);
            actor->setBondId(idx);
            actor->setBondOrder(4);

            mapper->SetInputConnection(tubeFilter->GetOutputPort());
            m_renderer->AddActor(actor);


            auto line1 = vtkSmartPointer<vtkLineSource>::New();
            line1->SetPoint1(mid[0], mid[1], mid[2]);
            line1->SetPoint2(p1[0], p1[1], p1[2]);

            auto tubeFilter1 = vtkSmartPointer<vtkTubeFilter>::New();
            tubeFilter1->SetInputConnection(line1->GetOutputPort());
            tubeFilter1->SetRadius(bondRadius*0.8);
            tubeFilter1->SetNumberOfSides(20);

            auto actor1 = vtkActor::New();initializeActor(actor1);
            //actor1->SetPickable(false);
            auto mapper1 = vtkPolyDataMapper::New();
            color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex1())->Color());

            if(_selected)
            {
                actor1->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
                if (updateSelectedFromRendering) selectedBondActors->AddItem(actor1);
            }
            else
                actor1->GetProperty()->SetColor(color.x(),color.y(),color.z());

            actor1->storeColor(color.x(),color.y(),color.z());
            actor1->SetMapper(mapper1);

            actor1->setBondId(idx);
            actor1->setAtomId(idx2);
            actor1->setAtomId1(idx1);
            actor->setId(beg);
            actor->setBondOrder(4);

            mapper1->SetInputConnection(tubeFilter1->GetOutputPort());
            m_renderer->AddActor(actor1);

            mapper->Delete();
            mapper1->Delete();
        }
    }

}

void View3D::renderHydrogenBond(uint idx)
{
    vector3 color;
    HBond * bond= mol->bondList[idx];
    uint idx1=bond->atomIndex0();
    uint idx2=bond->atomIndex1();

    bool _selected=mol->getBondbyIndex(idx)->isSelected();

    vector3 v1=mol->getAtomPosbyIndex(idx1);
    vector3 v2=mol->getAtomPosbyIndex(idx2);
    vector3 v=v2-v1;

    uint beg=numActors();

    uint NSteps=10;

    //the same atom
    double s=0.5;


    //different atom, the border of two half-bonds lies the middle of two sphere-surface
    if(mol->getAtombyIndex(idx1)->atomicNum() != mol->getAtombyIndex(idx2)->atomicNum())
    {
        double length=v.length();
        double r1=scale_bond*mol->getAtombyIndex(idx1)->radius();
        double r2=scale_bond*mol->getAtombyIndex(idx2)->radius();
        double d=0.5*(length-r2-r1);
        s=(d+r1)/length;
    }

    vector3 mid0=v1+s*v;;

    vector3 p0=v1,
            p1=v2,
            mid=mid0;

    //dash line ----------------------------------------;
    double mlength=(v2-v1).length()/NSteps;
    vector3 step=mlength*(p1-p0).normalize();

    p0=p0+step/2.0;
    p1=p1+step/2.0;


    color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex0())->Color());
    for (uint k=0;k<NSteps-1;k++)
    {
        vector3 pp0=p0+k*step,
                pp1=p0+(k+1)*step;

        if(k<1) continue;

        if(k%2==0) continue;

        if(k>(NSteps-1)/2)
            color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex1())->Color());
/*
        auto line = vtkSmartPointer<vtkLineSource>::New();
        line->SetPoint1(pp0[0], pp0[1], pp0[2]);
        line->SetPoint2(pp1[0], pp1[1], pp1[2]);
        auto tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
        tubeFilter->SetInputConnection(line->GetOutputPort());
        tubeFilter->SetRadius(bondRadius*0.2);
        tubeFilter->SetNumberOfSides(20);
        tubeFilter->SetCapping(true);

        auto a = vtkActor::New();
        initializeActor(a);
        auto m = vtkPolyDataMapper::New();

        if(_selected)
        {
            a->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
            if (updateSelectedFromRendering) selectedBondActors->AddItem(a);
        }
        else   a->GetProperty()->SetColor(color.x(),color.y(),color.z());

        a->storeColor(color.x(),color.y(),color.z());
        a->SetMapper(m);

        a->setBondId(idx);
        a->setId(beg);
        a->setAtomId(idx1);
        a->setAtomId1(idx2);
        a->setBondOrder(5);

        m->SetInputConnection(tubeFilter->GetOutputPort());
        m_renderer->AddActor(a);
        m->Delete();
*/

        uint times=2;
        while (times--)
        {
            auto sphere = vtkSphereSource::New();
            sphere->SetPhiResolution(20);
            sphere->SetThetaResolution(20);
            sphere->SetRadius(bondRadius*0.5);
            sphere->Update();

            auto a1 = vtkActor::New();
            initializeActor(a1);

            a1->storeColor(color.x(),color.y(),color.z());


            if(times==1)
                a1->SetPosition(pp0[0], pp0[1], pp0[2]);
            else
                a1->SetPosition(pp1[0], pp1[1], pp1[2]);


            if(_selected)
            {
                a1->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
                if (updateSelectedFromRendering) selectedAtomActors->AddItem(a1);
            }
            else
                a1->GetProperty()->SetColor(blue);

            a1->setBondId(idx);
            a1->setId(beg);
            a1->setAtomId(idx1);
            a1->setAtomId1(idx2);
            a1->setBondOrder(5);

            auto m1 = vtkPolyDataMapper::New();
            a1->SetMapper(m1);
            m1->SetInputConnection(sphere->GetOutputPort());
            m_renderer->AddActor(a1);

            m1->Delete();
            sphere->Delete();
        }
    }


    /*
    vector3 color;
    HBond * bond= mol->bondList[idx];
    uint idx1=bond->atomIndex0();
    uint idx2=bond->atomIndex1();

    bool _selected=mol->getBondbyIndex(idx)->isSelected();

    vector3 v1=mol->getAtomPosbyIndex(idx1);
    vector3 v2=mol->getAtomPosbyIndex(idx2);

    double p1[3] = {v1.x(),v1.y(),v1.z()};
    double p2[3] = {v2.x(),v2.y(),v2.z()};


    auto actor_line = vtkActor::New();initializeActor(actor_line);
    actor_line->GetProperty()->ShadingOn();
    actor_line->GetProperty()->SetLineStipplePattern(0xf0f0);
    actor_line->GetProperty()->SetLineStippleRepeatFactor(5);
    actor_line->SetPickable(true);
    actor_line->GetProperty()->SetPointSize(1.0);
    actor_line->GetProperty()->SetLineWidth(1.5);

    if(_selected)
    {
        actor_line->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
        if (updateSelectedFromRendering) selectedBondActors->AddItem(actor_line);
    }
    else   actor_line->GetProperty()->SetColor(blue);


    actor_line->setBondId(idx);
    actor_line->setAtomId(idx1);
    actor_line->setAtomId1(idx2);
    actor_line->setBondOrder(5);

    auto line=vtkSmartPointer<vtkLineSource>::New();
    line->SetPoint1(p1);
    line->SetPoint2(p2);
    auto mapper_line =  vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper_line->SetInputConnection( line->GetOutputPort() );
    mapper_line->ScalarVisibilityOn();
    actor_line->SetMapper(mapper_line);
    m_renderer->AddActor(actor_line);
    */
}


void View3D::renderWeakBond(uint idx)
{
    vector3 color;
    HBond * bond= mol->bondList[idx];
    uint idx1=bond->atomIndex0();
    uint idx2=bond->atomIndex1();

    bool _selected=mol->getBondbyIndex(idx)->isSelected();

    vector3 v1=mol->getAtomPosbyIndex(idx1);
    vector3 v2=mol->getAtomPosbyIndex(idx2);
    vector3 v=v2-v1;

    uint beg=numActors();

    uint NSteps=10;

    //the same atom
    double s=0.5;


    //different atom, the border of two half-bonds lies the middle of two sphere-surface
    if(mol->getAtombyIndex(idx1)->atomicNum() != mol->getAtombyIndex(idx2)->atomicNum())
    {
        double length=v.length();
        double r1=scale_bond*mol->getAtombyIndex(idx1)->radius();
        double r2=scale_bond*mol->getAtombyIndex(idx2)->radius();
        double d=0.5*(length-r2-r1);
        s=(d+r1)/length;
    }

    vector3 mid0=v1+s*v;;

    vector3 p0=v1,
            p1=v2,
            mid=mid0;

    //dash line ----------------------------------------;
    double mlength=(v2-v1).length()/NSteps;
    vector3 step=mlength*(p1-p0).normalize();

    p0=p0+step/2.0;
    p1=p1+step/2.0;


    color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex0())->Color());
    for (uint k=0;k<NSteps-1;k++)
    {
        vector3 pp0=p0+k*step,
                pp1=p0+(k+1)*step;

        if(k<1) continue;

        if(k%2==0) continue;

        if(k>(NSteps-1)/2)
            color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex1())->Color());

        auto line = vtkSmartPointer<vtkLineSource>::New();
        line->SetPoint1(pp0[0], pp0[1], pp0[2]);
        line->SetPoint2(pp1[0], pp1[1], pp1[2]);
        auto tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
        tubeFilter->SetInputConnection(line->GetOutputPort());
        tubeFilter->SetRadius(bondRadius*0.6);
        tubeFilter->SetNumberOfSides(20);
        tubeFilter->SetCapping(true);

        auto a = vtkActor::New();
        initializeActor(a);
        auto m = vtkPolyDataMapper::New();

        if(_selected)
        {
            a->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
            if (updateSelectedFromRendering) selectedBondActors->AddItem(a);
        }
        else   a->GetProperty()->SetColor(color.x(),color.y(),color.z());

        a->storeColor(color.x(),color.y(),color.z());
        a->SetMapper(m);

        a->setBondId(idx);
        a->setId(beg);
        a->setAtomId(idx1);
        a->setAtomId1(idx2);
        a->setBondOrder(7);

        m->SetInputConnection(tubeFilter->GetOutputPort());
        m_renderer->AddActor(a);
        m->Delete();


        uint times=2;
        while (times--)
        {
            auto sphere = vtkSphereSource::New();
            sphere->SetPhiResolution(20);
            sphere->SetThetaResolution(20);
            sphere->SetRadius(bondRadius*0.6);
            sphere->Update();


            auto a1 = vtkActor::New();
            initializeActor(a1);


            a1->storeColor(color.x(),color.y(),color.z());

            //cout << " times " << times << endl;

            if(times==1)
                a1->SetPosition(pp0[0], pp0[1], pp0[2]);
            else
                a1->SetPosition(pp1[0], pp1[1], pp1[2]);


            if(_selected)
            {
                a1->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
                if (updateSelectedFromRendering) selectedAtomActors->AddItem(a1);
            }
            else
                a1->GetProperty()->SetColor(color.x(),color.y(),color.z());

            a1->setBondId(idx);
            a1->setId(beg);
            a1->setAtomId(idx1);
            a1->setAtomId1(idx2);
            a1->setBondOrder(7);

            auto m1 = vtkPolyDataMapper::New();
            a1->SetMapper(m1);
            m1->SetInputConnection(sphere->GetOutputPort());
            m_renderer->AddActor(a1);

            m1->Delete();
            sphere->Delete();
        }
    }

}



void View3D::renderAromaticBond(uint idx)
{
    if(!_isKekuleStyle) return;

    vector3 color;
    HBond * bond= mol->bondList[idx];
    uint idx1=bond->atomIndex0();
    uint idx2=bond->atomIndex1();

    bool _selected=mol->getBondbyIndex(idx)->isSelected();

    vector3 v1=mol->getAtomPosbyIndex(idx1);
    vector3 v2=mol->getAtomPosbyIndex(idx2);
    vector3 v=v2-v1;

    uint beg=numActors();
    uint NSteps=10;

    //the same atom
    double s=0.5;

    //different atom, the border of two half-bonds lies the middle of two sphere-surface
    if(mol->getAtombyIndex(idx1)->atomicNum() != mol->getAtombyIndex(idx2)->atomicNum())
    {
        double length=v.length();
        double r1=scale_atom*mol->getAtombyIndex(idx1)->radius();
        double r2=scale_atom*mol->getAtombyIndex(idx2)->radius();
        double d=0.5*(length-r2-r1);
        s=(d+r1)/length;
    }

    vector3 mid0=v1+s*v;

    vector3 shift,shift0;

    uint idRing=bond->getRingId();
    if( idRing <10000 && idRing >=0) //ring existed
    {
        HRing  * ring = mol->getRingbyId(idRing);

        vector3 v0=ring->center;
        vector3 normal=ring->norm;

        shift=cross1(normal, v);
        shift0=0.12*shift;

        //make sure: outside for the first time
         if( (mid0-shift0-v0).length() < (mid0-v0).length())
             shift0=-shift0;
    }
    else { //no ring
        if (bond->getPlane().length()>0.5) {
            shift0=0.1*cross1(bond->getPlane(),v);
        }
        else {
            v.createOrthoVector(shift);
            shift0=0.1*shift;
        }
    }

    //vector3 p0=v1-shift0,   p1=v2-shift0,   mid=mid0-shift0;
    vector3 p0=v1,   p1=v2,   mid=mid0;

    auto line = vtkSmartPointer<vtkLineSource>::New();
    line->SetPoint1(p0[0], p0[1], p0[2]);
    line->SetPoint2(mid[0], mid[1], mid[2]);

    auto tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter->SetInputConnection(line->GetOutputPort());
    tubeFilter->SetRadius(bondRadius);
    tubeFilter->SetNumberOfSides(20);

    color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex0())->Color());
    auto actor = vtkActor::New();initializeActor(actor);
    auto mapper = vtkPolyDataMapper::New();

    if(_selected)
    {
        actor->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
        if (updateSelectedFromRendering) selectedBondActors->AddItem(actor);
    }
    else actor->GetProperty()->SetColor(color.x(),color.y(),color.z());

    actor->storeColor(color.x(),color.y(),color.z());
    actor->SetMapper(mapper);

    actor->setId(beg);
    actor->setAtomId(idx1);
    actor->setAtomId1(idx2);
    actor->setBondId(idx);
    actor->setBondOrder(6);

    mapper->SetInputConnection(tubeFilter->GetOutputPort());
    m_renderer->AddActor(actor);


    auto line1 = vtkSmartPointer<vtkLineSource>::New();
    line1->SetPoint1(mid[0], mid[1], mid[2]);
    line1->SetPoint2(p1[0], p1[1], p1[2]);

    auto tubeFilter1 = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter1->SetInputConnection(line1->GetOutputPort());
    tubeFilter1->SetRadius(bondRadius);
    tubeFilter1->SetNumberOfSides(20);

    auto actor1 = vtkActor::New();initializeActor(actor1);
    auto mapper1 = vtkPolyDataMapper::New();
    color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex1())->Color());

    if(_selected)
    {
        actor1->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
        if (updateSelectedFromRendering) selectedBondActors->AddItem(actor1);
    }
    else  actor1->GetProperty()->SetColor(color.x(),color.y(),color.z());

    actor1->storeColor(color.x(),color.y(),color.z());
    actor1->SetMapper(mapper1);

    actor1->setBondId(idx);
    actor1->setAtomId(idx2);
    actor1->setAtomId1(idx1);
    actor1->setId(beg);
    actor1->setBondOrder(6);

    mapper1->SetInputConnection(tubeFilter1->GetOutputPort());
    m_renderer->AddActor(actor1);

    mapper->Delete();
    mapper1->Delete();


    //dash line ----------------------------------------
    p0=v1+2.0*shift0;  p1=v2+2.0*shift0;
    double mlength=(v2-v1).length()/NSteps;
    vector3 step=mlength*(p1-p0).normalize();

    p0=p0+step/2.0;
    p1=p1+step/2.0;


    color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex0())->Color());
    for (uint k=0;k<NSteps-1;k++)
    {
        vector3 pp0=p0+k*step,
                pp1=p0+(k+1)*step;

        if(k<1) continue;

        if(k%2==0) continue;

        if(k>(NSteps-1)/2)
            color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex1())->Color());

        auto line = vtkSmartPointer<vtkLineSource>::New();
        line->SetPoint1(pp0[0], pp0[1], pp0[2]);
        line->SetPoint2(pp1[0], pp1[1], pp1[2]);
        auto tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
        tubeFilter->SetInputConnection(line->GetOutputPort());
        tubeFilter->SetRadius(bondRadius*0.6);
        tubeFilter->SetNumberOfSides(20);
        tubeFilter->SetCapping(true);

        auto a = vtkActor::New();
        initializeActor(a);
        auto m = vtkPolyDataMapper::New();

        if(_selected)
        {
            a->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
            if (updateSelectedFromRendering) selectedBondActors->AddItem(a);
        }
        else   a->GetProperty()->SetColor(color.x(),color.y(),color.z());

        a->storeColor(color.x(),color.y(),color.z());
        a->SetMapper(m);

        a->setBondId(idx);
        a->setId(beg);
        a->setAtomId(idx1);
        a->setAtomId1(idx2);
        a->setBondOrder(6);

        m->SetInputConnection(tubeFilter->GetOutputPort());
        m_renderer->AddActor(a);
        m->Delete();
/*
        uint times=2;
        while (times--)
        {
            auto sphere = vtkSphereSource::New();
            sphere->SetPhiResolution(10);
            sphere->SetThetaResolution(10);
            sphere->SetRadius(bondRadius*0.61);
            sphere->Update();


            auto a1 = vtkActor::New();
            initializeActor(a1);


            a1->storeColor(color.x(),color.y(),color.z());

            //cout << " times " << times << endl;

            if(times==1)
                a1->SetPosition(pp0[0], pp0[1], pp0[2]);
            else
                a1->SetPosition(pp1[0], pp1[1], pp1[2]);


            if(_selected)
            {
                a1->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
                if (updateSelectedFromRendering) selectedAtomActors->AddItem(a1);
            }
            else
                a1->GetProperty()->SetColor(color.x(),color.y(),color.z());

            a1->setBondId(idx);
            a1->setId(beg);
            a1->setAtomId(idx1);
            a1->setAtomId1(idx2);
            a1->setBondOrder(6);

            auto m1 = vtkPolyDataMapper::New();
            a1->SetMapper(m1);
            m1->SetInputConnection(sphere->GetOutputPort());
            m_renderer->AddActor(a1);

            m1->Delete();
            sphere->Delete();
        }
        */
    }
}

void View3D::renderAromaticRings()
{
    if(_isKekuleStyle) return;

    if (mol->numRings()<1)  return;

    cout << "Number of ring " << mol->numRings()<<endl;

    if (updateSelectedFromRendering)
        selectedRingActors->RemoveAllItems();

    for (uint i=0;i<mol->numRings();i++)
    {
        HRing  * ring = mol->getRingbyId(i);

        auto parametricObject = vtkSmartPointer<vtkParametricTorus>::New();
        auto parametricFunctionSource =	vtkSmartPointer<vtkParametricFunctionSource>::New();
        parametricFunctionSource->SetParametricFunction(parametricObject);

        parametricObject->SetRingRadius(ring->radius-0.5);
        if(ring->radius<1.0)
            parametricObject->SetRingRadius(ring->radius-0.6);
        parametricObject->SetCrossSectionRadius(bondRadius*scale_ringRadius*ring->scale);

        parametricFunctionSource->SetUResolution(100);
        parametricFunctionSource->SetVResolution(100);
        parametricFunctionSource->SetWResolution(100);

        parametricFunctionSource->Update();

        auto mapper =   vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(parametricFunctionSource->GetOutputPort());
        mapper->ScalarVisibilityOn();

        matrix3x3 m;
        m.SetupRotateMatrix(VZ,ring->norm);

        auto transform=  vtkSmartPointer<vtkTransform>::New();
        transform->PostMultiply();
        if( !isEqual (VZ,ring->norm) && !isNegativeEqual (VZ,ring->norm) )
        {
            auto matrix = vtkSmartPointer<vtkMatrix4x4>::New();
            for( int i = 0; i < 3; i++ )
                for( int j = 0; j < 3; j++ )
                    matrix->SetElement(i,j,m(i,j));

            for( int i = 0; i < 4; i++ ) {
                matrix->SetElement(3,i,0.0);
                matrix->SetElement(i,3,0.0);
            }
            matrix->SetElement(3,3,1.0);

            transform->SetMatrix(matrix);
        }
        transform->Translate(ring->center.x(),ring->center.y(),ring->center.z());


        auto actor = vtkSmartPointer<vtkActor>::New();
        initializeActor(actor);
        actor->SetMapper(mapper);
        actor->SetPickable(true);

        if(updateSelectedFromRendering && ring->isSelected)
                selectedRingActors->AddItem(actor);

        if(ring->isSelected)
            actor->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
        else
            actor->GetProperty()->SetColor(ring->color.x(),ring->color.y(),ring->color.z());
        actor->storeColor(ring->color.x(),ring->color.y(),ring->color.z());

        actor->setRingId(i);
        actor->setBondOrder(6);
        actor->SetUserTransform(transform);

        m_renderer->AddActor(actor);

/*
        if(ring->type<1) continue;
        auto cylinder =  vtkSmartPointer<vtkCylinderSource>::New();
        cylinder->SetCenter(0.0, 0.0, 0.0);
        cylinder->SetRadius(ring->radius);
        cylinder->SetHeight(0.2);
        cylinder->SetResolution(100);
        cylinder->Update();

        auto mapperR = vtkPolyDataMapper::New();
        mapperR->ScalarVisibilityOff();
        mapperR->SetInputData(cylinder->GetOutput());
        auto actorR = vtkActor::New();

        actorR->GetProperty()->SetColor(ring->color.x(),ring->color.y(),ring->color.z());
        actorR->GetProperty()->SetOpacity(ring->opacity);
        actorR->GetProperty()->ShadingOff();
        actorR->SetMapper(mapper);
        actorR->SetPickable(false);

        actorR->SetUserTransform(transform);
        m_renderer->AddActor(actorR);
*/
    }
}



//just render single bond (ring added later)
void View3D::renderAromaticSingleBond(uint idx)
{
    if (_isKekuleStyle)   return;

    vector3 color;
    HBond * bond= mol->bondList[idx];
    uint idx1=bond->atomIndex0();
    uint idx2=bond->atomIndex1();

    uint idRing=bond->getRingId();

    if(idRing > 10000)
    {
        //modify for this bond
        _isKekuleStyle=true;
        renderAromaticBond(idx);
        _isKekuleStyle=false;
        return;
    }

    HRing  * ring = mol->getRingbyId(idRing);
    vector3 v0=mol->getAtomPosbyIndex(idx1);
    vector3 v1=mol->getAtomPosbyIndex(idx2);

    vector3 v= cross1 (ring->norm,v1-v0);
    if (dot(ring->center-v0, v) <0.0)  v=-v;
    v=0.1*v;

    uint beg=numActors();

    bool _selected=mol->getBondbyIndex(idx)->isSelected();

    double s=0.5;
    if(mol->getAtombyIndex(idx1)->atomicNum() != mol->getAtombyIndex(idx2)->atomicNum())
    {
        double length=(v1-v0).length();
        double r1=scale_atom*mol->getAtombyIndex(idx1)->radius();
        double r2=scale_atom*mol->getAtombyIndex(idx2)->radius();
        double d=0.5*(length-r2-r1);
        s=(d+r1)/length;
    }

    vector3 p=v0+s*(v1-v0);
    vector3 p0=v0,  p1=v1;

    auto line = vtkSmartPointer<vtkLineSource>::New();
    line->SetPoint1(p0[0], p0[1], p0[2]);
    line->SetPoint2(p[0], p[1], p[2]);

    auto tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter->SetInputConnection(line->GetOutputPort());
    tubeFilter->SetRadius(bondRadius);
    tubeFilter->SetNumberOfSides(20);

    color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex0())->Color());
    auto actor = vtkActor::New();initializeActor(actor);
    auto mapper = vtkPolyDataMapper::New();

    if(_selected)
    {
        actor->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
        if (updateSelectedFromRendering) selectedBondActors->AddItem(actor);
    }
    else
        actor->GetProperty()->SetColor(color.x(),color.y(),color.z());

    actor->storeColor(color.x(),color.y(),color.z());
    actor->SetMapper(mapper);

    //actor->SetPickable(false);
    actor->setId(beg);
    actor->setAtomId(idx1);
    actor->setAtomId1(idx2);
    actor->setBondId(idx);
    actor->setBondOrder(6);

    mapper->SetInputConnection(tubeFilter->GetOutputPort());
    m_renderer->AddActor(actor);


    auto line1 = vtkSmartPointer<vtkLineSource>::New();
    line1->SetPoint1(p[0], p[1], p[2]);
    line1->SetPoint2(p1[0], p1[1], p1[2]);

    auto tubeFilter1 = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter1->SetInputConnection(line1->GetOutputPort());
    tubeFilter1->SetRadius(bondRadius);
    tubeFilter1->SetNumberOfSides(20);

    auto actor1 = vtkActor::New();initializeActor(actor1);
    //actor1->SetPickable(false);
    auto mapper1 = vtkPolyDataMapper::New();
    color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex1())->Color());

    if(_selected)
    {
        actor1->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
        if (updateSelectedFromRendering) selectedBondActors->AddItem(actor1);
    }
    else
        actor1->GetProperty()->SetColor(color.x(),color.y(),color.z());

    actor1->storeColor(color.x(),color.y(),color.z());
    actor1->SetMapper(mapper1);

    actor1->setBondId(idx);
    actor1->setAtomId(idx2);
    actor1->setAtomId1(idx1);
    actor1->setId(beg);
    actor1->setBondOrder(6);


    mapper1->SetInputConnection(tubeFilter1->GetOutputPort());
    m_renderer->AddActor(actor1);

    mapper->Delete();
    mapper1->Delete();
}





void View3D::renderTripleBond(uint idx)
{
    vector3 color;
    HBond * bond= mol->bondList[idx];
    uint idx1=bond->atomIndex0();
    uint idx2=bond->atomIndex1();


    vector3 v0=mol->getAtomPosbyIndex(idx1);
    vector3 v1=mol->getAtomPosbyIndex(idx2);

    vector3 v=v1-v0;
    vector3 shift;
    v.createOrthoVector(shift);
    shift=0.1*shift;
    if (bond->getPlane().length()>0.5)
            shift=0.1*cross1(bond->getPlane(),v1-v0);
    v1=v1-shift; v0=v0-shift;


    bool _selected=mol->getBondbyIndex(idx)->isSelected();

    uint beg=numActors();


    //the same atom
    double s=0.5;

    //different atom, the border of two half-bonds lies the middle of two sphere-surface
    if(mol->getAtombyIndex(idx1)->atomicNum() != mol->getAtombyIndex(idx2)->atomicNum())
    {
        double length=(v1-v0).length();
        double r1=scale_atom*mol->getAtombyIndex(idx1)->radius();
        double r2=scale_atom*mol->getAtombyIndex(idx2)->radius();
        double d=0.5*(length-r2-r1);
        s=(d+r1)/length;
    }



    for (uint k=0;k<3;k++)
    {
        vector3 p0=v0+shift*k,  p1=v1+shift*k;
        vector3 p=p0+s*(p1-p0);

        auto line = vtkSmartPointer<vtkLineSource>::New();
        line->SetPoint1(p0[0], p0[1], p0[2]);
        line->SetPoint2(p[0], p[1], p[2]);

        auto tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
        tubeFilter->SetInputConnection(line->GetOutputPort());
        tubeFilter->SetRadius(bondRadius*0.7);
        tubeFilter->SetNumberOfSides(20);

        color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex0())->Color());
        auto actor = vtkActor::New();
        initializeActor(actor);
        auto mapper = vtkPolyDataMapper::New();

        if(_selected)
        {
            actor->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
            if (updateSelectedFromRendering) selectedBondActors->AddItem(actor);
        }
        else
            actor->GetProperty()->SetColor(color.x(),color.y(),color.z());

        actor->storeColor(color.x(),color.y(),color.z());
        actor->SetMapper(mapper);

        //actor->SetPickable(false);
        actor->setId(beg);
        actor->setAtomId(idx1);
        actor->setAtomId1(idx2);
        actor->setBondId(idx);
        actor->setBondOrder(3);

        mapper->SetInputConnection(tubeFilter->GetOutputPort());
        m_renderer->AddActor(actor);


        auto line1 = vtkSmartPointer<vtkLineSource>::New();
        line1->SetPoint1(p[0], p[1], p[2]);
        line1->SetPoint2(p1[0], p1[1], p1[2]);

        auto tubeFilter1 = vtkSmartPointer<vtkTubeFilter>::New();
        tubeFilter1->SetInputConnection(line1->GetOutputPort());
        tubeFilter1->SetRadius(bondRadius*0.7);
        tubeFilter1->SetNumberOfSides(20);

        auto actor1 = vtkActor::New();initializeActor(actor1);
        auto mapper1 = vtkPolyDataMapper::New();
        color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex1())->Color());

        if(_selected)
        {
            actor1->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
            if (updateSelectedFromRendering) selectedBondActors->AddItem(actor1);
        }
        else
            actor1->GetProperty()->SetColor(color.x(),color.y(),color.z());

        actor1->storeColor(color.x(),color.y(),color.z());
        actor1->SetMapper(mapper1);

        //actor1->SetPickable(false);
        actor1->setAtomId(idx2);
        actor1->setAtomId1(idx1);
        actor1->setId(beg);
        actor1->setBondId(idx);
        actor1->setBondOrder(3);


        mapper1->SetInputConnection(tubeFilter1->GetOutputPort());
        m_renderer->AddActor(actor1);

        mapper->Delete();
        mapper1->Delete();
    }

}

void View3D::renderDoubleBond(uint idx)
{
    HBond * bond= mol->bondList[idx];

    if (bond->isInsideRing())
    if (!_isKekuleStyle)
    {
        if (mol->bondList[idx]->isInsideRing()) {
            renderSingleBond(idx);
            return;
        }
    }


    vector3 color;

    uint idx1=bond->atomIndex0();
    uint idx2=bond->atomIndex1();

    //cout << idx1+1 << " "<< idx2+1<< " inside renderDoubleBond" <<endl;

    vector3 v0=mol->getAtomPosbyIndex(idx1);
    vector3 v1=mol->getAtomPosbyIndex(idx2);


    vector3 v=v1-v0;
    vector3 shift;


    if (bond->getPlane().length()>0.5)
    {
        shift=cross(bond->getPlane(),v).normalize();
    }
    else {
        v.createOrthoVector(shift);
    }
    shift*=0.10;


    uint beg=numActors();

    //the same atom
    double s=0.5;

    //different atom: the border lies the middle of two sphere-surface
    if(mol->getAtombyIndex(idx1)->atomicNum() != mol->getAtombyIndex(idx2)->atomicNum())
    {
        double length=v.length();
        double r1=scale_atom*mol->getAtombyIndex(idx1)->radius();
        double r2=scale_atom*mol->getAtombyIndex(idx2)->radius();
        double d=0.5*(length-r2-r1);
        s=(d+r1)/length;
    }

    vector3 mid0=v0+s*v;

    bool _selected=mol->getBondbyIndex(idx)->isSelected();

    for (uint k=0;k<2;k++)
    {
        vector3 p0=v0+shift,
                p1=v1+shift,
                mid=mid0+shift;


        if(k>0) {//break;
            p0=v0-shift; p1=v1-shift;mid=mid0-shift;}

        auto line = vtkSmartPointer<vtkLineSource>::New();
        line->SetPoint1(p0[0], p0[1], p0[2]);
        line->SetPoint2(mid[0], mid[1], mid[2]);

        auto tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
        tubeFilter->SetInputConnection(line->GetOutputPort());
        tubeFilter->SetRadius(bondRadius);
        tubeFilter->SetNumberOfSides(20);

        color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex0())->Color());
        auto actor = vtkActor::New();initializeActor(actor);
        auto mapper = vtkPolyDataMapper::New();

        actor->storeColor(color.x(),color.y(),color.z());

        if(_selected)
        {
            actor->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
            if (updateSelectedFromRendering) selectedBondActors->AddItem(actor);
        }
        else
            actor->GetProperty()->SetColor(color.x(),color.y(),color.z());


        actor->SetMapper(mapper);

        //actor->SetPickable(false);
        actor->setId(beg);
        actor->setAtomId(idx1);
        actor->setAtomId1(idx2);
        actor->setBondOrder(2);
        actor->setBondId(idx);


        mapper->SetInputConnection(tubeFilter->GetOutputPort());
        m_renderer->AddActor(actor);


        auto line1 = vtkSmartPointer<vtkLineSource>::New();
        line1->SetPoint1(mid[0], mid[1], mid[2]);
        line1->SetPoint2(p1[0], p1[1], p1[2]);

        auto tubeFilter1 = vtkSmartPointer<vtkTubeFilter>::New();
        tubeFilter1->SetInputConnection(line1->GetOutputPort());
        tubeFilter1->SetRadius(bondRadius);
        tubeFilter1->SetNumberOfSides(20);

        auto actor1 = vtkActor::New();initializeActor(actor1);
        //actor1->SetPickable(false);
        auto mapper1 = vtkPolyDataMapper::New();
        color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex1())->Color());

        if(_selected)
        {
            actor1->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
            if (updateSelectedFromRendering) selectedBondActors->AddItem(actor1);
        }
        else
            actor1->GetProperty()->SetColor(color.x(),color.y(),color.z());

        actor1->storeColor(color.x(),color.y(),color.z());
        actor1->SetMapper(mapper1);


        actor1->setAtomId(idx2);
        actor1->setAtomId1(idx1);
        actor1->setId(beg);
        actor1->setBondOrder(2);
        actor1->setBondId(idx);


        mapper1->SetInputConnection(tubeFilter1->GetOutputPort());
        m_renderer->AddActor(actor1);

        mapper->Delete();
        mapper1->Delete();
    }
}

void View3D::renderSimpleSingleBond(uint idx)
{
    vector3 color={0.6,0.6,0.6};
    HBond * bond= mol->bondList[idx];
    uint idx1=bond->atomIndex0();
    uint idx2=bond->atomIndex1();

    bool _selected=mol->getBondbyIndex(idx)->isSelected();

    vector3 p0=mol->getAtomPosbyIndex(idx1);
    vector3 p1=mol->getAtomPosbyIndex(idx2);


    uint beg=numActors();


    auto line = vtkSmartPointer<vtkLineSource>::New();
    line->SetPoint1(p0[0], p0[1], p0[2]);
    line->SetPoint2(p1[0], p1[1], p1[2]);

    auto tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter->SetInputConnection(line->GetOutputPort());
    tubeFilter->SetRadius(bondRadius);
    tubeFilter->SetNumberOfSides(20);

    color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex0())->Color());
    auto actor = vtkActor::New();
    initializeActor(actor);
    auto mapper = vtkPolyDataMapper::New();

    actor->GetProperty()->SetColor(color.x(),color.y(),color.z());
    actor->storeColor(color.x(),color.y(),color.z());
    actor->setBondOrder(1);


    if(mol->getBondbyAtomIndex(idx1,idx2)->isSelected())
    {
        actor->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
        if (updateSelectedFromRendering)
            selectedBondActors->AddItem(actor);
    }
    else {
        actor->GetProperty()->SetColor(color.x(),color.y(),color.z());
    }

    actor->SetMapper(mapper);


    actor->setId(beg);
    actor->setAtomId(idx1);
    actor->setAtomId1(idx2);
    actor->setBondId(idx);
    actor->setBondOrder(1);

    mapper->SetInputConnection(tubeFilter->GetOutputPort());
    m_renderer->AddActor(actor);
    mapper->Delete();
}

void View3D::renderSingleBond(uint idx)
{
    vector3 color;
    HBond * bond= mol->bondList[idx];
    uint idx1=bond->atomIndex0();
    uint idx2=bond->atomIndex1();

    bool _selected=mol->getBondbyIndex(idx)->isSelected();

    vector3 p0=mol->getAtomPosbyIndex(idx1);
    vector3 p1=mol->getAtomPosbyIndex(idx2);

  //  if(_selected)
  //      cout << idx1+1 << " "<< idx2+1<<" is selected"<<endl;
  //  else
   //     cout << idx1+1 << " "<< idx2+1<<" is not selected"<<endl;


    uint beg=numActors();

    double s=0.5;

    if(mol->getAtombyIndex(idx1)->atomicNum() != mol->getAtombyIndex(idx2)->atomicNum())
    {
        double length=(p1-p0).length();
        double r1=scale_atom*mol->getAtombyIndex(idx1)->radius();
        double r2=scale_atom*mol->getAtombyIndex(idx2)->radius();
        double d=0.5*(length-r2-r1);
        s=(d+r1)/length;
    }

    vector3 p=p0+s*(p1-p0);


    auto line = vtkSmartPointer<vtkLineSource>::New();
    line->SetPoint1(p0[0], p0[1], p0[2]);
    line->SetPoint2(p[0], p[1], p[2]);

    auto tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter->SetInputConnection(line->GetOutputPort());
    tubeFilter->SetRadius(bondRadius);
    tubeFilter->SetNumberOfSides(20);

    color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex0())->Color());
    auto actor = vtkActor::New();
    initializeActor(actor);
    auto mapper = vtkPolyDataMapper::New();

    actor->GetProperty()->SetColor(color.x(),color.y(),color.z());
    actor->storeColor(color.x(),color.y(),color.z());
    actor->setBondOrder(1);


    if(mol->getBondbyAtomIndex(idx1,idx2)->isSelected())
    {
        //cout << idx+1 << " "<< idx1+1 << " "<< idx2+1 << " is selected"<<endl;
        actor->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
        if (updateSelectedFromRendering)
            selectedBondActors->AddItem(actor);
    }
    else {
        actor->GetProperty()->SetColor(color.x(),color.y(),color.z());
        //cout << idx+1 << " "<< idx1+1 << " "<< idx2+1 << " is not selected"<<endl;
    }



    actor->SetMapper(mapper);
    //actor->SetPickable(false);

    actor->setId(beg);
    actor->setAtomId(idx1);
    actor->setAtomId1(idx2);
    actor->setBondId(idx);
    actor->setBondOrder(1);

    mapper->SetInputConnection(tubeFilter->GetOutputPort());
    m_renderer->AddActor(actor);


    auto line1 = vtkSmartPointer<vtkLineSource>::New();
    line1->SetPoint1(p[0], p[1], p[2]);
    line1->SetPoint2(p1[0], p1[1], p1[2]);

    auto tubeFilter1 = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter1->SetInputConnection(line1->GetOutputPort());
    tubeFilter1->SetRadius(bondRadius);
    tubeFilter1->SetNumberOfSides(20);

    auto actor1 = vtkActor::New();initializeActor(actor1);
    //actor1->SetPickable(false);
    auto mapper1 = vtkPolyDataMapper::New();
    color = MakeGlColor(mol->getAtombyIndex(bond->atomIndex1())->Color());
    actor1->GetProperty()->SetColor(color.x(),color.y(),color.z());
    actor1->storeColor(color.x(),color.y(),color.z());


    actor1->SetMapper(mapper1);

    actor1->setAtomId(idx2);
    actor1->setAtomId1(idx1);
    actor1->setId(beg);
    actor1->setBondOrder(1);
    actor1->setBondId(idx);


    if(_selected)
    {
        actor1->GetProperty()->SetColor(selectedColor.x(),selectedColor.y(),selectedColor.z());
        if (updateSelectedFromRendering) selectedBondActors->AddItem(actor1);
    }
    else {
        actor1->GetProperty()->SetColor(color.x(),color.y(),color.z());
    }


    mapper1->SetInputConnection(tubeFilter1->GetOutputPort());
    m_renderer->AddActor(actor1);

    mapper->Delete();
    mapper1->Delete();

}


void View3D::renderBonds()
{
    if(!isMol_Visible) return;

    //cout << "number of bonds : "<< mol->NumBonds()<<endl;
   // cout << "there are "<<mol->NumBonds()<<endl;

    if (updateSelectedFromRendering)
        selectedBondActors->RemoveAllItems();

    uint NAtom=mol->NumAtoms();

    //large molecule
    if(NAtom>200) {
        for (uint j=0;j<mol->NumBonds();j++)
        {
            uint bo=mol->bondList[j]->getBondOrder();

            if (!isHydrogen_Visible)   {
                if(mol->getAtomSymbol(mol->bondList[j]->atomIndex0())=="H") continue;
                if(mol->getAtomSymbol(mol->bondList[j]->atomIndex1())=="H") continue;
            }

            if(bo==5) { renderHydrogenBond(j); continue;}
            if(bo==7) { renderWeakBond(j); continue;}

            renderSimpleSingleBond(j);
        }


        return;
    }


    for (uint j=0;j<mol->NumBonds();j++)
    {

        uint bo=mol->bondList[j]->getBondOrder();

        if (!isHydrogen_Visible)   {
            if(mol->getAtomSymbol(mol->bondList[j]->atomIndex0())=="H") continue;
            if(mol->getAtomSymbol(mol->bondList[j]->atomIndex1())=="H") continue;
        }



        switch (bo)
        {
            case 1: renderSingleBond(j);
                    break;

            case 2: renderDoubleBond(j);
                    break;

            case 3: renderTripleBond(j);
                    break;

            case 4: renderQuadrupleBond(j);
                    break;


            case 5: renderHydrogenBond(j);
                break;


            case 6: renderAromaticSingleBond(j);
                    renderAromaticBond(j);
                    break;

            case 7: renderWeakBond(j);
                    break;
        }
    }


    renderAromaticRings();

    //cout << "render bonds done! "<<endl;
}


void View3D:: writePNG(QString name)
{
    //vector3 color=bkcolor;
    //setBKColor(whiteColor);

    auto windowToImageFilter =  vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(renderWindow());
    windowToImageFilter->SetScale(4);
    windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
    windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
    windowToImageFilter->Update();

    auto writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName(name.toLatin1());
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());
    writer->Write();

    //setBKColor(color);
  }



void View3D::updateMolLength ()
{
    if(mol!=nullptr && mol->NumAtoms()>0)
        mol->perceiveMolLength(verticalLength,HorizontalLengh);

}




void View3D::renderLine(double p0[3],double p1[3],float width,double color[3], vtkTransform * transform)
{
    auto lineSource =  vtkSmartPointer<vtkLineSource>::New();
    lineSource->SetPoint1(p0);
    lineSource->SetPoint2(p1);
    lineSource->Update();

    auto lineActor = vtkActor::New();
    auto lineMapper = vtkPolyDataMapper::New();
    lineMapper->SetInputConnection(lineSource->GetOutputPort());


    lineActor->SetMapper(lineMapper);
    lineActor->GetProperty()->SetColor(color[0], color[1], color[2]);
    lineActor->storeColor(color[0],color[1],color[2]);
    lineActor->GetProperty()->SetLineWidth(width);
    lineActor->SetUserTransform(transform);

    m_renderer->AddActor(lineActor);
}

void View3D::renderLineCenter(vector3 beg, vector3 end,float width,double color[3])
{
    vector3 beg0=beg-center;
    vector3 end0=end-center;
    renderLine(beg0,end0,width,color);
}

void View3D::renderLine (vector3 beg, vector3 end,float width,double color[3])
{
    double p0[3], p1[3];

    for (uint i=0;i<3;i++) {
        p0[i]=beg[i];
        p1[i]=end[i];
    }


    auto lineSource =  vtkSmartPointer<vtkLineSource>::New();
    lineSource->SetPoint1(p0);
    lineSource->SetPoint2(p1);
    lineSource->Update();

    auto lineActor = vtkActor::New();
    auto lineMapper = vtkPolyDataMapper::New();
    lineMapper->SetInputConnection(lineSource->GetOutputPort());


    lineActor->SetMapper(lineMapper);
    lineActor->GetProperty()->SetColor(color[0], color[1], color[2]);
    lineActor->storeColor(color[0],color[1],color[2]);
    lineActor->GetProperty()->SetLineWidth(width);



    m_renderer->AddActor(lineActor);
}



void View3D::renderTubeCenter(vector3 beg, vector3 end, double radius, double color[3] )
{
    renderTube( beg,  end,  center, radius,  color);
}

void View3D::renderTube(vector3 beg, vector3 end, vector3 center0,double radius, double color[3])
{
    beg=beg-center0;
    end=end-center0;
    renderTube( beg,  end,  radius, color );
}



void View3D::renderTube(vector3 beg, vector3 end, double radius, double color[3] )
{
    auto line = vtkSmartPointer<vtkLineSource>::New();
    line->SetPoint1(beg[0], beg[1], beg[2]);
    line->SetPoint2(end[0], end[1], end[2]);

    auto tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter->SetInputConnection(line->GetOutputPort());
    tubeFilter->SetRadius(radius);
    tubeFilter->SetNumberOfSides(20);

    auto actor = vtkActor::New();
    auto mapper = vtkPolyDataMapper::New();
    actor->GetProperty()->SetColor(color[0],color[1],color[2]);
    actor->storeColor(color[0],color[1],color[2]);
    actor->SetMapper(mapper);
    actor->setUnitCellBoxId(0);
    mapper->SetInputConnection(tubeFilter->GetOutputPort());
    m_renderer->AddActor(actor);
}


void View3D::renderArcTube(vector <uint> atoms, double radius, double color[3], uint id)
{
    if(atoms.size()<3) return;

    vector3 beg,end,c,center;


    //double angle=vectorAngle(v1,v2);
    //vector3 norm=cross (v1,v2);


    for(int i=0;i<atoms.size();i++)
        center+=mol->getAtomPosbyIndex(atoms[i]);
    center/=atoms.size();


    auto points = vtkSmartPointer<vtkPoints>::New();


    for(int i=0;i<atoms.size()-1;i++)
    {
        beg=mol->getAtomPosbyIndex(atoms[i]);
        end=mol->getAtomPosbyIndex(atoms[i+1]);
        center=(beg+end)/2.0;
        //points->InsertNextPoint(beg.x(),beg.y(),beg.z());
        //points->InsertNextPoint(v.x(),v.y(),v.z());
        //points->InsertNextPoint(v2.x(),v2.y(),v2.z());
    }

    auto lines=vtkSmartPointer<vtkCellArray>::New();
    for(int i=0;i<points->GetNumberOfPoints();i++)
        lines->InsertCellPoint(i);

    auto polys=vtkSmartPointer<vtkPolyData>::New();
    polys->SetPoints(points);
    polys->SetLines(lines);


    auto tubes=vtkSmartPointer<vtkTubeFilter>::New();
    tubes->SetInputData(polys);
    tubes->CappingOn();
    tubes->SidesShareVerticesOff();
    tubes->SetRadius(radius);
    tubes->SetNumberOfSides(10);
    tubes->Update();
}


void View3D::renderArcTube(vector3 beg, vector3 end, vector3 center, double color[3],uint id )
{
    auto line = vtkSmartPointer<vtkLineSource>::New();
    double radius= (center-end).length()-0.2;

    vector3 v1=beg-center, v2=end-center;

    double angle=vectorAngle(v1,v2);
    vector3 norm=cross (v1,v2);

    vector3 v=0.8*v1.length()*v1+center;

    uint NPoints=10;
    matrix3x3 m;
    m.SetupRotateMatrix(norm,angle/10.);
    for (uint i=0;i<NPoints;i++)
    {
        if(i==0) {
            line->SetPoint1(v[0], v[1], v[2]);
            continue;
        }
        v*=m;
        line->SetPoint1(v[0], v[1], v[2]);
    }

    auto tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter->SetInputConnection(line->GetOutputPort());
    tubeFilter->SetRadius(radius);
    tubeFilter->SetNumberOfSides(20);

    auto actor = vtkActor::New();
    auto mapper = vtkPolyDataMapper::New();
    actor->GetProperty()->SetColor(color[0],color[1],color[2]);
    actor->storeColor(color[0],color[1],color[2]);
    actor->SetMapper(mapper);
    actor->setUnitCellBoxId(0);
    mapper->SetInputConnection(tubeFilter->GetOutputPort());
    m_renderer->AddActor(actor);

    actor->setBondId(id);
}




void View3D::addDataSet(vtkSmartPointer<vtkDataSet> dataSet)
{
  // Actor
  auto actor = vtkSmartPointer<vtkActor>::New();

  // Mapper
  auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
  mapper->SetInputData(dataSet);
  actor->SetMapper(mapper);

  m_renderer->AddActor(actor);
 // m_renderer->ResetCamera(dataSet->GetBounds());
//  renderVTK();
}


void View3D::removeAllActors()
{
    vtkActorCollection* actorCollection = m_renderer->GetActors();
    uint numActors = actorCollection->GetNumberOfItems();

    actorCollection->InitTraversal();
    for (uint i=0;i<numActors;++i)
    {
      vtkActor * actor = actorCollection->GetNextActor();
      if(actor==nullptr) continue;
      m_renderer->RemoveActor(actor);
    }

    actorCollection->RemoveAllItems();
}


void View3D::removeDynamicLineActor()
{
    vtkActorCollection* actorCollection = m_renderer->GetActors();
    actorCollection->InitTraversal();
    uint _numActors = actorCollection->GetNumberOfItems();

    for (int i=0;i<_numActors;++i)
    {
      vtkActor* actor = actorCollection->GetNextActor();
      if(actor==nullptr) continue;
      if(actor->MOId()<0) {
          m_renderer->RemoveActor(actor);
          break;
      }
    }
}


void View3D::removeAtomLabelActor (uint idx)
{
    vtkActorCollection* actorCollection = m_renderer->GetActors();
    actorCollection->InitTraversal();
    uint _numActors = actorCollection->GetNumberOfItems();

    cout << "begin to remove actor inside removeAtomLabelActor"<<  idx+1 <<endl;

    for (int i=0;i<_numActors;++i)
    {
        vtkActor* actor = actorCollection->GetNextActor();
        if(actor==nullptr ||
                actor->getAtomLabelId()<0 ||
                actor->getAtomLabelId()>100000) continue;

        if(actor->getAtomLabelId()!=idx) continue;
        m_renderer->RemoveActor(actor);
        break;
    }
}




uint View3D::numActors()
{
    vtkActorCollection* actorCollection = m_renderer->GetActors();
    actorCollection->InitTraversal();
    return actorCollection->GetNumberOfItems();

}


void View3D::removeLastActor()
{
    vtkActor * actor = m_renderer->GetActors()->GetLastActor();

    if (actor != nullptr)
        m_renderer->RemoveActor(actor);
}


void View3D::zoomToExtent()
{
    // Zoom to extent of last added actor
    vtkSmartPointer<vtkActor> actor = m_renderer->GetActors()->GetLastActor();
    if (actor != nullptr)
        m_renderer->ResetCamera(actor->GetBounds());

}


void View3D::setMolTypeStick() {
    isStick=true;
    isBallStick=isLine=false;
}

void View3D::setMolTypeBallStick() {
    isBallStick=true;
    isStick=isLine=false;
}

void View3D::setMolTypeLine() {
    isLine=true;
    isStick=isBallStick=false;
}







//selection
void View3D::buildAtomLabel(vtkActor * actor)
{
    return;

    if(actor==nullptr) return;

    uint idx=actor->AtomId();
    QString label= QString::number(idx+1);

    //cout << idx+1 << "inside showAtomLabel(vtkActor * actor)"<<endl;

    auto textSource = vtkSmartPointer<vtkTextSource>::New();
    textSource->SetText(label.toStdString().c_str());
    textSource->SetForegroundColor(0.0, 0.0, 1.0);
    textSource->BackingOff();
    textSource->Update();

    auto  textMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    textMapper->SetInputConnection(textSource->GetOutputPort());

    //auto textActor =  vtkSmartPointer<vtkFollower>::New();
    auto labelActor =  vtkFollower::New();
    labelActor->SetPickable(false);
    labelActor->SetMapper(textMapper);
    labelActor->SetScale(0.02, 0.02, 0.02);

    double radius=0.1;
    if (mol) radius=0.1*mol->getAtomRadiusbyIndex(idx);

    double *x =actor->GetPosition();
    labelActor->SetPosition(x[0],x[1],x[2]+radius*2.0);
    labelActor->SetCamera(camera);
    labelActor->setAtomLabelId(idx);
    labelActor->setAtomId(-1);
    m_renderer->AddActor(labelActor);

    //updateSelectedAtomListFromAtomActors();
    m_renderer->GetRenderWindow()->Render();
}



void View3D::releaseSingleAtomActor (vtkActor * a)
{
    a->recoverInitialColor();
    uint _numActors = selectedAtomActors->GetNumberOfItems();
    if (_numActors<0 || _numActors>10000) return;
    
    if (_numActors==1)
        selectedAtomActors = vtkSmartPointer<vtkActorCollection>::New();
    else
    {
        selectedAtomActors->InitTraversal();
        for (int i=0;i<_numActors;++i)
        {
            vtkActor* actor = selectedAtomActors->GetNextActor();
            if(actor==nullptr ) continue;
            if (actor->AtomId()!=a->AtomId() ) continue;
            selectedAtomActors->RemoveItem(i);
            break;
        }
    }

    removeAtomLabelActor(a->AtomId());
    m_renderer->GetRenderWindow()->Render();
    updateSelectedAtomListFromAtomActors();
}

void View3D::releaseSingleBondActor (vtkActor * a)
{
    a->recoverInitialColor();
    uint _numActors = selectedBondActors->GetNumberOfItems();
    if (_numActors<0 || _numActors>10000) return;
    
    if (_numActors==1)
        selectedBondActors = vtkSmartPointer<vtkActorCollection>::New();
    else
    {
        selectedBondActors->InitTraversal();
        for (int i=0;i<_numActors;++i)
        {
            vtkActor* actor = selectedBondActors->GetNextActor();
            if(actor==nullptr ) continue;
            
            if (actor->AtomId()!=a->AtomId() ) continue;
            if (actor->AtomId1()!=a->AtomId1() ) continue;
            selectedBondActors->RemoveItem(i);
            break;
        }
    }
    
    removeAtomLabelActor(a->AtomId());
    m_renderer->GetRenderWindow()->Render();
    //updateSelectedBonds();
}




void View3D::removefromBondActorsList (vtkActor* a)
{
    uint _numActors = selectedBondActors->GetNumberOfItems();
    if (_numActors<0 || _numActors>100000) return;


    if (_numActors==1)
        selectedBondActors->RemoveAllItems();
    else
    {
        selectedBondActors->InitTraversal();
        for (uint i=0;i<_numActors;i++)
        {
            vtkActor* actor = selectedBondActors->GetNextActor();
            if(actor!=nullptr)
            {
                if ( actor->Id()!=a->Id() ) continue;
                actor->recoverInitialColor();
                selectedBondActors->RemoveItem(i);
                i--;_numActors--;
            }
        }
    }


    m_renderer->GetRenderWindow()->Render();

    //recover all
    for (uint i=0;i<mol->NumBonds();i++)
        mol->getBondbyIndex(i)->setSelected(false);
    selectedBondList.clear();

}


void View3D::removefromAtomActorsList (vtkActor * a)
{
    a->recoverInitialColor();

    uint _numActors = selectedAtomActors->GetNumberOfItems();
    if (_numActors<0 || _numActors>100000) return;


    if (_numActors==1)
        selectedAtomActors->RemoveAllItems();
    else
    {
        selectedAtomActors->InitTraversal();
        for (uint i=0;i<_numActors;++i)
        {
            vtkActor* actor = selectedAtomActors->GetNextActor();
            if(actor==nullptr ) continue;
            if (actor->AtomId()!=a->AtomId() ) continue;
            selectedAtomActors->RemoveItem(i);
            break;
        }
    }

    removeAtomLabelActor(a->AtomId());
    m_renderer->GetRenderWindow()->Render();

    //recover all
    for (uint i=0;i<mol->NumAtoms();i++)
        mol->getAtombyIndex(i)->setSelected(false);
    selectedAtomList.clear();
}


void View3D::removefromRingActorsList (vtkActor * a)
{
    //cout << " inside removefromRingActorsList"<<endl;

    a->recoverInitialColor();

    uint _numActors = selectedRingActors->GetNumberOfItems();
    if (_numActors<0 || _numActors>100000) return;

    if (_numActors==1)
        selectedRingActors->RemoveAllItems();
    else
    {
        selectedRingActors->InitTraversal();
        for (uint i=0;i<_numActors;++i)
        {
            vtkActor* actor = selectedRingActors->GetNextActor();
            if(actor==nullptr ) continue;
            if (actor->getRingId()!=a->getRingId() ) continue;
            selectedRingActors->RemoveItem(i);
            break;
        }
    }

    m_renderer->GetRenderWindow()->Render();
}

void View3D::updateSelectedRingListFromRingActors()
{
    //reset all
    for (uint i=0;i<mol->numRings();i++)
        mol->getRingbyId(i)->isSelected=false;
    selectedRingList.clear();


    uint numActors = selectedRingActors->GetNumberOfItems();
    if (numActors<0 || numActors>100000) return;

    selectedRingActors->InitTraversal();
    for (uint i=0;i<numActors;i++)
    {
        vtkActor * actor = selectedRingActors->GetNextActor();
        if(actor==nullptr) continue;
        selectedRingList.push_back(actor->getRingId());
        mol->getRingbyId(actor->getRingId())->isSelected=true;
    }

    if(numActors<=1) return;
    sort (selectedRingList.begin(), selectedRingList.end());
    vector <uint> :: iterator iter =  unique ( selectedRingList.begin(), selectedRingList.end());
    selectedRingList.erase(iter,selectedRingList.end());
}

vector3  View3D::getAtomPos(uint id)
{
    return mol->getAtomPosbyIndex(id);
}

//update from selected Actors
void View3D::updateSelectedAtomListFromAtomActors()
{
    //recover all
    for (uint i=0;i<mol->NumAtoms();i++)
        mol->getAtombyIndex(i)->setSelected(false);

    //reset
    selectedAtomList.clear();
    origin_SelectedAtoms=VZero;
    vector < vector3 > v;
    uint numActors = selectedAtomActors->GetNumberOfItems();

    origin_SelectedAtoms=VZero;
    length_SelectedAtoms=0.0;

    if (numActors<0 || numActors>100000) return;

    selectedAtomActors->InitTraversal();

    uint i=0;
    for (i=0;i<numActors;i++) {
        vtkActor* actor = selectedAtomActors->GetNextActor();
        if(actor==nullptr) continue;
        selectedAtomList.push_back(actor->AtomId());
        mol->getAtombyIndex(actor->AtomId())->setSelected(true);
        origin_SelectedAtoms += mol->getAtomPosbyIndex(actor->AtomId());
        v.push_back(mol->getAtomPosbyIndex(actor->AtomId()));
    }

    origin_SelectedAtoms /= numActors;
    
    for (i=0;i<numActors;i++)
        v[i] = v[i] - origin_SelectedAtoms;
    
    for (i=0;i<numActors;i++)
    {
        double l=v[i].length();
        if (length_SelectedAtoms < l )
            length_SelectedAtoms=l;
    }
        

    //displaySelectedAtomList();
}




//update from selected Actors
void View3D::updateSelectedBondListFromBondActors()
{
    //reset all
    for (uint i=0;i<mol->NumBonds();i++)
        mol->getBondbyIndex(i)->setSelected(false);

    //reset
    selectedBondList.clear();
    uint _numActors = selectedBondActors->GetNumberOfItems();
    if(_numActors<1) return;


   // cout << _numActors << " inside updateSelectedBondList"<< endl;

    selectedBondActors->InitTraversal();
    for (uint i=0;i<_numActors;i++)
    {
        vtkActor* actor = selectedBondActors->GetNextActor();
        if(actor==nullptr) continue;

        storeSelectBondList( actor->AtomId(), actor->AtomId1() ) ;

        mol->getBondbyIndex(actor->BondId())->setSelected(true);
    }
    displaySelectedBondList();
}


uint View3D:: getBondOrderbyId(uint id)
{
    return  mol->getBondOrderbyIndex(id);
}


//help function
void View3D:: displaySelectedBondList()
{
    if(selectedBondList.size()<1) return;

    cout << endl;
    cout << "Selected Bonds: "<< selectedBondList.size() << endl;
    for (uint i =0;i< selectedBondList.size();i++)
    {
        cout <<  selectedBondList[i][0] << " :";
        cout <<  selectedBondList[i][1]+1 << " ";
        cout <<  selectedBondList[i][2]+1 << endl;
    }
    cout << endl;
}


//help function
void View3D:: storeSelectBondList(uint id0, uint id1)
{
    uint id_bond;
    HBond * b=mol->getBondIndexbyAtomIndex(id0,id1,id_bond); //obtain  _idbond
    if (b==nullptr) return;

    bool isExisted=false;
    if(selectedBondList.size() > 0)
    {
        for (uint j=0; j<selectedBondList.size();j++)
        {
            if (id_bond == selectedBondList[j][0])
            {
                isExisted=true;
                break;
            }
        }
    }
    if (isExisted) return;

    vector < uint > v;
    v.push_back(id_bond);
    if(id0 < id1)
    {
        v.push_back(id0);  v.push_back(id1);
    }
    else {
        v.push_back(id1);  v.push_back(id0);
    }
    selectedBondList.push_back(v);
}


void View3D:: uniqueSelectAtomList()
{
    sort ( selectedAtomList.begin(), selectedAtomList.end());
    vector <uint> :: iterator iter =
            unique ( selectedAtomList.begin(), selectedAtomList.end());
    selectedAtomList.erase(iter,selectedAtomList.end());
}

bool View3D::hasPicked(vtkActor * actor)
{
    uint size=selectedAtomList.size();
    if (size<1) return false;

    for (uint i=0;i<size;i++) {
        if( selectedAtomList[i]==actor->AtomId())
            return true;
    }
    return false;
}

uint View3D::getBondOrderSelectedById(uint id)
{
    uint beg,end;
    getSelectedBondIds(id,beg,end);
    return mol->getBondOrderbyAtomIndex(beg,end);
}

void View3D::getSelectedBondIds(uint id, uint & id1, uint & id2)
{
    id1 = selectedBondList[id][1];
    id2 = selectedBondList[id][2];
}


uint View3D::getSelectedAtomId(uint id)
{
    if(selectedAtomList.size()<1)
        return 10000001;

    if(id>=selectedAtomList.size())
        return 10000001;

     return selectedAtomList[id];
}

uint View3D::getSelectedRingId(uint id)
{
    if(selectedRingList.size()<1)
        return 10000001;
    if(id>=selectedRingList.size())
        return 10000001;

    return selectedRingList[id];
}




uint View3D::numSelectedAtoms()
{
    uint n=selectedAtomList.size();
    return n;
}

uint View3D::numSelectedBonds()
{
    return selectedBondList.size();
}

uint View3D::numSelectedRings()
{
    return selectedRingList.size();
}

bool View3D::isAtomPicked(uint id)
{
    uint n=selectedAtomList.size();
    if (n<1) return false;

    for (uint i=0;i<n;i++)
        if(selectedAtomList[i]==id)
            return true;

    return false;
}


//just remove label-actor
void View3D::removeAllAtomLabelActors ()
{
    uint n=selectedAtomList.size();
    if (n<1) return ;

    vtkActorCollection * actorCollection = m_renderer->GetActors();
    actorCollection->InitTraversal();
    uint numActors = actorCollection->GetNumberOfItems();

    uint times=0;
    for (int i=0;i<numActors;i++)
    {
      vtkActor* actor = actorCollection->GetNextActor();
      if(actor==nullptr || actor->getAtomLabelId()<0 || actor->getAtomLabelId()>100000) continue;

      for (uint j=0;j<n;j++)  {
          if(actor->getAtomLabelId()!=getSelectedAtomId(j)) continue;
          m_renderer->RemoveActor(actor);
          times++;
          break;
      }

      if(times==n) break;
    }
}

void View3D::releaseAllActors2()
{
    releaseSelectedAtomActors();
    releaseSelectedBondActors();
    releaseSelectedRingActors();
    releaseInteractorPickedActor();
    removeDynamicLineActor();
    renderMol();
}

void View3D::releaseAllActors()
{
    releaseSelectedAtomActors();
    releaseSelectedBondActors();
    releaseSelectedRingActors();

    renderMol();
    cout << "void View3D::releaseAllActors()"<<endl;
}

void View3D::releaseInteractorPickedActor()
{
    actorInteractorA->releasePickedActor();
}

void View3D::clearSelectedAtomList ()
{
    removeAllAtomLabelActors();
    selectedAtomList.clear();
    updateSelectedAtomListFromAtomActors();
}

void View3D::clearSelectedBondList ()
{
    selectedBondList.clear();
    updateSelectedBondListFromBondActors();
}



void View3D::releaseSelectedBondActors()
{
    uint numActors = selectedBondActors->GetNumberOfItems();
    if (numActors<1 || numActors>10000001)
    {
        return;
    }

    cout << "\n\nbegin release all selected atom actors:"<<endl;
    cout << "there are " <<numActors<< " actors!"<<endl;

    if (numActors==1){
        selectedBondActors->GetLastItem()->recoverInitialColor();
    }
    else{
        selectedBondActors->InitTraversal();
        for (uint i=0;i<numActors;++i) {
            selectedBondActors->GetNextActor()->recoverInitialColor();
        }
    }

    selectedBondActors->RemoveAllItems();

    m_renderer->GetRenderWindow()->Render();

    clearSelectedBondList();
    selectedBondList.clear();
}

void View3D::releaseAtomLabelActors()
{
   atomLabelActors->RemoveAllItems();
}


void View3D::releaseSelectedAtomActors()
{
    uint numActors = selectedAtomActors->GetNumberOfItems();
    if (numActors<1 || numActors>1000001)
    {
         return;
    }

    //cout << "\n\nbegin to release all selected atom actors:"<<endl;
   // cout << "there are " <<numActors<< " actors!"<<endl;

    removeAllAtomLabelActors();

    selectedAtomList.clear();
    if (numActors==1)
    {
        selectedAtomActors->GetLastItem()->recoverInitialColor();
    }

    if (numActors>1 && numActors<10000)
    {
        selectedAtomActors->InitTraversal();
        for (uint i=0;i<numActors;++i) {
            selectedAtomActors->GetNextActor()->recoverInitialColor();
        }
    }

    selectedAtomActors->RemoveAllItems();

    m_renderer->GetRenderWindow()->Render();

    clearSelectedAtomList();
}


void View3D::releaseSelectedRingActors()
{
    selectedRingList.clear();
    for (uint i=0;i<mol->numRings();i++)
        mol->getRingbyId(i)->isSelected=false;


    uint numActors = selectedRingActors->GetNumberOfItems();
    if (numActors<1 || numActors>1000001)
    {
         return;
    }

    if (numActors==1)
    {
        selectedRingActors->GetLastItem()->recoverInitialColor();
    }

    if (numActors>1 && numActors<10000)
    {
        selectedRingActors->InitTraversal();
        for (uint i=0;i<numActors;++i) {
            selectedRingActors->GetNextActor()->recoverInitialColor();
        }
    }

    selectedRingActors->RemoveAllItems();


    //clearSelectedRingList();

    m_renderer->GetRenderWindow()->Render();
    //renderMol();
}

void View3D::clearSelectedRingList ()
{
    selectedRingList.clear();
    updateSelectedRingListFromRingActors();
}


double View3D::getSelectedAtomXbyIndex(uint id)
{
    if(id < selectedAtomList.size() )
        return mol->getAtomXbyIndex(selectedAtomList[id]);
    return 0.0;
}

double View3D::getSelectedAtomYbyIndex(uint id)
{
    if(id < selectedAtomList.size() )
        return mol->getAtomYbyIndex(selectedAtomList[id]);
    return 0.0;
}
double View3D::getSelectedAtomZbyIndex(uint id)
{
    if(id < selectedAtomList.size() )
        return mol->getAtomZbyIndex(selectedAtomList[id]);
    return 0.0;
}

vector3 View3D::getSelectedAtomPosbyIndex(uint id )
{
    if(id < selectedAtomList.size() )
        return mol->getAtomPosbyIndex(selectedAtomList[id]);
    return VZero;
}

void View3D::setSelectedAtomPosbyIndex(uint id, double x, double y, double z)
{
    if(id >= selectedAtomList.size() ) return;

     mol->setAtomPos(selectedAtomList[id],x,y,z);
}

void View3D::displaySelectedAtomList()
{
    uint size=selectedAtomList.size();
    if(size<1) return;

    cout << "There are "<< size << " selected atoms: ";
    for (uint i=0;i<size;i++)
        cout << selectedAtomList[i]+1<<" ";
    cout << endl<<endl;
}

void View3D::displaySelectedRingList()
{
    uint size=selectedRingList.size();
    if(size<1) return;

    cout << "There are "<< size << " selected ring: ";
    for (uint i=0;i<size;i++)
        cout << selectedRingList[i]+1<<" ";
    cout << endl<<endl;
}



void View3D::displaySelectedAtomActorsList()
{
    uint numActors = selectedAtomActors->GetNumberOfItems();
    if(numActors<1) return;

    cout <<numActors <<"  selected actors : ";

    selectedAtomActors->InitTraversal();
    for (int i=0;i<numActors;++i)
    {
        vtkActor* actor = selectedAtomActors->GetNextActor();

        if(actor==nullptr) continue;
        cout << actor->Id()<< " ";
    }

    cout << endl;
}



void View3D::loadTemplateMol (QString path)
{
    view3dT->setMolT(tmol);
    view3dT->loadTemplateMol(path);
}



// idx is the hit atom inside this view
void View3D::linkTemplate2Mol(uint idx)
{
    if (!isHydrogenAtom(idx))
    {
        QMessageBox::information(0, "Warning","Only Hydrogen atom can be linked!\nPlease click mouseRightButton or Q or Escape to switch linker-mode");
        return;
    }

    uint numAtomsT=tmol->numAtoms();
    if (numAtomsT<1) return;

    //cout << "---------------------"<<endl;
   // cout << numAtomsT<<endl;
   // cout << idx<<endl;
    //cout << view3dT->getSelectedAtomId()<<endl;

    mol->linkTemplate2Mol(tmol,idx,view3dT->getSelectedAtomId());

    renderMol();
    push2Stack("linkTemplate");

    mainWindow->setFirstTime(false);
    //setDefaultOperationMode();
    firstTime=false;
}


void View3D::addNewTemplate(double x, double y, double z)
{
    vector3 v(x,y,z);
    addNewTemplate(v);

    //mainWindow->updatePanelParameters();
    mainWindow->setFirstTime(false);
    firstTime=false;
    //setDefaultOperationMode();
}



void View3D::clearTemplate()
{
    tmol->clearAll();
    view3dT->clearAll();
}


void View3D::addNewTemplate(vector3 p)
{
    if (tmol->numAtoms()<1) return;
    if (mol->NumAtoms()>0) return;
    mol->addAtomfromTemplate(tmol,p);

    //updateMolLength();
    renderMol(mol);

    push2Stack("newTemplate");
}


void View3D::removeSelectedRings()
{
    if( numSelectedRings()<1) return;

    bool turnAromatic2Single=false;
    if(numSelectedBonds() + numSelectedAtoms()<1)
        turnAromatic2Single=true;

    if(turnAromatic2Single)
    {
        for (uint i=0;i<selectedRingList.size();i++)
        {
            HRing  * ring = mol->getRingbyId(selectedRingList[i]);
            uint size=ring->atomIdList.size()-1;
            for (uint j=0;j<size;j++)
                mol->getBondbyAtomIndex(ring->atomIdList[j],ring->atomIdList[j+1])->setBondOrder(1);
            mol->getBondbyAtomIndex(ring->atomIdList[0],
                                    ring->atomIdList[size])->setBondOrder(1);
        }
    }

    if(numSelectedRings()>1)
    {
        sort ( selectedRingList.begin(), selectedRingList.end());
        vector <uint> :: iterator iter =
                unique ( selectedRingList.begin(), selectedRingList.end());
        selectedRingList.erase(iter,selectedRingList.end());
        sort(selectedRingList.rbegin(), selectedRingList.rend());

        for (uint i=0;i<selectedRingList.size();i++)
            mol->removeRingbyId(selectedRingList[i]);
    }
    else
    {
        mol->removeRingbyId(selectedRingList[0]);
    }

    selectedRingList.clear();
    selectedRingActors->RemoveAllItems();

    renderMol(mol);
    //if(turnAromatic2Single)
    push2Stack("remove Ring");
}


void View3D::removeSelectedAtomsBonds()
{  
    removeSelectedRings();

    //removed bonds
    if( numSelectedBonds()>0 && numSelectedAtoms()<1)
        removeSelectedBonds();

    //removed atoms
    if( numSelectedBonds()<1 && numSelectedAtoms()>0)
        removeSelectedAtoms();

    //removed atoms and bonds
    if( numSelectedBonds()>0 && numSelectedAtoms()>0)
    {
        for (uint i=0;i<selectedBondList.size();i++)
            mol->removeBondbyAtomIds(selectedBondList[i][1],selectedBondList[i][2]);

        selectedBondList.clear();
        selectedBondActors->RemoveAllItems();

        if(numSelectedAtoms()==1) {
            mol->removeAtombyId(selectedAtomList[0]);
        }
        else {
            sort ( selectedAtomList.begin(), selectedAtomList.end());
            vector <uint> :: iterator iter =
                    unique ( selectedAtomList.begin(), selectedAtomList.end());
            selectedAtomList.erase(iter,selectedAtomList.end());
            sort(selectedAtomList.rbegin(), selectedAtomList.rend());

            //remove from the tail one by one
            for (uint i=0;i<selectedAtomList.size();i++)
                mol->removeAtombyId(selectedAtomList[i]);
        }
        selectedAtomList.clear();
        selectedAtomActors->RemoveAllItems();

        renderMol(mol);
        push2Stack("removeSelectedAtoms&&Bonds");
    }
}



void View3D::removeSelectedBonds()
{
    if(numSelectedBonds()<1) return;

    for (uint i=0;i<selectedBondList.size();i++)
        mol->removeBondbyAtomIds(selectedBondList[i][1],selectedBondList[i][2]);

    selectedBondList.clear();
    selectedBondActors->RemoveAllItems();

    renderMol(mol);

    push2Stack("removeSelectedBonds");
}



void View3D::removeSelectedAtoms()
{
    if (numSelectedAtoms()<1) return;

    //cout << numSelectedAtoms() <<" Atoms need to be removed"<<endl;

    if(numSelectedAtoms()==1) {
        mol->removeAtombyId(selectedAtomList[0]);
    }
    else {
        mol->removeAtombyId(selectedAtomList);
    }
    selectedAtomList.clear();
    selectedAtomActors->RemoveAllItems();

    renderMol(mol);
    push2Stack("removeSelectedAtoms");
}



void View3D::selectAll()
{
    cout << "inside View3D::selectAll"<<endl;
    selectedAtomList.clear();
    for (uint i=0;i<mol->NumAtoms();i++) {
        selectedAtomList.push_back(i);
        mol->getAtombyIndex(i)->setSelected(true);
    }

    selectedBondList.clear();
    for (uint i=0;i<mol->NumBonds();i++)
    {
        mol->getBondbyIndex(i)->setSelected(true);

        vector < uint > v;
        v.push_back(i);
        if(mol->getBondbyIndex(i)->atomIndex0() < mol->getBondbyIndex(i)->atomIndex0())
        {
            v.push_back(mol->getBondbyIndex(i)->atomIndex0());
            v.push_back(mol->getBondbyIndex(i)->atomIndex1());
        }else{
            v.push_back(mol->getBondbyIndex(i)->atomIndex1());
            v.push_back(mol->getBondbyIndex(i)->atomIndex0());
        }

        selectedBondList.push_back(v);
    }

    selectedRingList.clear();
    for (uint i=0;i<mol->numRings();i++)
    {
        mol->getRingbyId(i)->isSelected=true;
        selectedRingList.push_back(i);
    }

    updateSelectedFromRendering=true;
    renderMol(mol);
    updateSelectedFromRendering=false;
}



MouseOperationType View3D::currentMouseOperationMode()
{
    return mouseOperationMode;
}

bool View3D::isLinkTemplateMode() {
    if (mouseOperationMode==VTKLinkTemplate) return true;
    return false;
}

bool View3D::isPencilDrawMode(){
    if (mouseOperationMode==VTKPencilDrawing) return true;
    return false;
}


bool View3D::isSelectionMode(){
    if (mouseOperationMode==VTKSelection) return true;
    return false;
}

bool View3D::isRotateMode(){
    if (mouseOperationMode==VTKRotation) return true;
    return false;
}

bool View3D::isTranslateMode()
{
    if (mouseOperationMode==VTKTranslation) return true;
    return false;
}

void View3D::setDefaultOperationMode()
{
    //releaseAllActors();
    mouseOperationMode=VTKRotation;
    setCursor(Qt::ArrowCursor);
    actorInteractorA->setRotateMode();
}

void View3D::setPencilDrawMode()
{
    mouseOperationMode=VTKPencilDrawing;
    actorInteractorA->setPencilDrawMode();
    auto  pencilCursor=new QCursor(QPixmap(":/images/draw.png"),0,-2);
    setCursor(*pencilCursor);

    releaseAllActors2();
}

void View3D::setLinkTemplateMode()
{
    mouseOperationMode=VTKLinkTemplate;
    actorInteractorA->setLinkTemplateMode();
    setCursor(Qt::PointingHandCursor);
    releaseAllActors();
}

void View3D::setRotateMode()
{
    mouseOperationMode=VTKRotation;
    actorInteractorA->setRotateMode();
}

void View3D::setTranslateMode()
{
    //mouseOperationMode=VTKTranslation;
    //actorInteractorA->setTranslateMode();
    auto  translateCursor=new QCursor(QPixmap(":/images/translate.png"),-1,-1);
    setCursor(*translateCursor);
}


void View3D::setSelectionMode()
{
    mouseOperationMode=VTKSelection;
    actorInteractorA->setSelectionMode();
}



void View3D::setOperateSelectedMode()
{
    mouseOperationMode=VTKOperateSelected;
    actorInteractorA->setOperateSelectedMode();
}


void View3D::undo()
{
    if(mainWindow==nullptr) return;
    mainWindow->undo();
}


void View3D::redo()
{
    if(mainWindow==nullptr) return;
    mainWindow->redo();
}

void View3D::translateSelectedAtoms(vector3 v)
{
    mol->translate(v);
    updateMol();
}

void View3D::rotateSelectedAtoms(vector3 v, matrix3x3 m)
{
   mol->tranform(v,m);
   updateMol();
}

void View3D::push2Stack(QString c)
{
    if(mainWindow==nullptr) return;
    mainWindow->push2Stack(c);
}

void View3D::setSelectedBondOrder(uint o)
{
    uint numActors = selectedBondActors->GetNumberOfItems();
    if (numActors>=1 )
    {
        selectedBondActors->InitTraversal();
        int bondidx=-1;
        for (uint i=0;i<numActors;++i)
        {
            uint j=selectedBondActors->GetNextActor()->BondId();
            uint bo=mol->bondList[j]->getBondOrder();
            if(bo==o && o==6) {
                if(j==bondidx) continue;
                mol->bondList[j]->reversePlane();
                bondidx=j;
            }
            else
                mol->getBondbyIndex(j)->setBondOrder(o);
        }

        releaseAllActors();
        renderMol(mol);
        push2Stack("Reset Bond Order");
        return;
    }


    numActors = selectedAtomActors->GetNumberOfItems();
    if (numActors==2)
    {
        selectedAtomActors->InitTraversal();
        vector <uint> id;

        for (uint i=0;i<numActors;++i)
            id.push_back(selectedAtomActors->GetNextActor()->AtomId());

        mol->addBond(id[0],id[1],o);

        releaseAllActors();
        renderMol(mol);
        push2Stack("Reset Bond Order");
        return;
    }

    numActors = selectedRingActors->GetNumberOfItems();
    if (numActors>0 && o==6)
    {
        selectedRingActors->InitTraversal();
        for (uint i=0;i<numActors;++i) {
            uint size=mol->getRingbyId(selectedRingList[i])->atomIdList.size();
            uint beg=mol->getRingbyId(selectedRingList[i])->atomIdList[0];
            uint end=mol->getRingbyId(selectedRingList[i])->atomIdList[size-1];
            mol->getBondbyAtomIndex(beg,end)->setBondOrder(6);
            for (uint j=0;j<size-1;j++) {
                beg=mol->getRingbyId(selectedRingList[i])->atomIdList[j];
                end=mol->getRingbyId(selectedRingList[i])->atomIdList[j+1];
                mol->getBondbyAtomIndex(beg,end)->setBondOrder(6);
            }

        }
        releaseAllActors();
        renderMol(mol);
        push2Stack("Reset Bond Order");
        return;
    }

    releaseAllActors();
    renderMol(mol);
}



void View3D::replaceSelelctedAtomWith(uint idx)
{
    if (numSelectedAtoms()<1) return;

    mol->replaceSelelctedAtomWith(idx,selectedAtomList);

    releaseAllActors();
    renderMol(mol);
    push2Stack("replace Element");
}

void View3D::Link2LastAtom(vector3 v)
{
    Link2LastAtom(v.x(),v.y(),v.z());
}

void View3D::Link2LastAtom(double x, double y, double z)
{
    addCarbon(x,y,z);
    if(mol->getBondOrderbyAtomIndex(mol->NumAtoms()-1,mol->NumAtoms()-2)<1) {
        mol->addBond(mol->NumAtoms()-1,mol->NumAtoms()-2);
        renderMol();
        push2Stack("draw Carbon");
    }

}



void View3D::linkTwoSelectedAtoms(uint id1,uint id2)
{
    if(mol->getBondOrderbyAtomIndex(id1,id2)>0) return;
    mol->addBond(id1,id2);
    renderMol();
    push2Stack("link atom");
}


void View3D::addCarbon_and_link2SelectedAtom(vector3 pos,uint id)
{
    //cout << " \naddCarbon_and_link2SelectedAtom "<<endl;

    QString Carbon="C";
    mol->addAtom(Carbon,pos);
    mol->addBond(mol->NumAtoms()-1,id);

    renderMol();
    push2Stack("draw Carbon");

    //mol->displayAtoms();
    //mol->displayBonds();
}


void View3D::addCarbon(double x, double y, double z)
{
    //cout << " \nadd the first Carbon"<<endl;
    QString Carbon="C";
    mol->addAtom(Carbon,x,y,z);
    renderMol();

    push2Stack("draw Carbon");
    //mol->displayAtoms();    mol->displayBonds();
}

void View3D::addCarbon(vector3 pos)
{
    addCarbon(pos.x(),pos.y(),pos.z());
}

void View3D::symmetryPercept()
{
    //mainWindow->symmetryPercept();
}

void View3D::focusOutEvent(QFocusEvent *)
{
    //actorInteractorA->loseFocus();
}


uint View3D::numSelectedObjects()
{
    return selectedAtomList.size() + selectedBondList.size() + selectedRingList.size();
}

void View3D::setSelectedAtomSymbol(QString s)
{
    for(uint i=0;i<selectedAtomList.size();i++)
        mol->setAtomSymbol(selectedAtomList[i],s);
    releaseAllActors2();
    updateMol();
}


void View3D::reverseAromaticBond()
{
    setSelectedBondOrder(6);
}

void View3D::modifyAtomColor()
{
    if(numSelectedAtoms()<1) return;

    QColor c = QColorDialog::getColor(Qt::white,nullptr);
    vector3 colour(c.red(),c.green(),c.blue());

    if(numSelectedAtoms()<2)
        mol->setAtomColor(getSelectedAtomId(0), buildColor1(colour));
    for(uint i=0;i<numSelectedAtoms();i++)
        mol->setAtomColor(getSelectedAtomId(i), buildColor1(colour));
    this->updateMol();

    push2Stack("modify atom color");
}

void View3D::modifyAtomRadius()
{
    mainWindow->on_pushButton_Atom_clicked();
    push2Stack("modify atom radius");
}



void View3D::buildRingForSelectedAtoms()
{
    uint numActors = selectedAtomActors->GetNumberOfItems();

    if(numActors<=2) return;

    vector <uint> id;id.clear();
    vector3 cc=VZero;

    selectedAtomActors->InitTraversal();
    for (uint i=0;i<numActors;++i)
    {
        uint ii=selectedAtomActors->GetNextActor()->AtomId();
        id.push_back(ii);
        cc+=mol->getAtomPosbyIndex(ii);
        cout << ii+1 << endl;
    }
    cc/=numActors;


    //existed?
    bool isExisted=false;
    for(uint i=0;i<mol->numRings();i++)
    {
        if (mol->getRingbyId(i)->size!=numActors) continue;

        if((mol->getRingbyId(i)->center-cc).length()>1.0) continue;

        isExisted=true;
        break;
    }


    if(isExisted)  {
        releaseAllActors();
        setDefaultOperationMode();
        return;
    }


    uint size=id.size();
    vector <uint> id1;
    id1.clear();


    //reorder
    vector <uint> label;
    for(uint i=0;i<size;i++)
        label.push_back(0);

    id1.push_back(id[0]);
    label[0]=1;

    //check closed or not?
    while (id1.size() < size)
    {
        bool canbe=false;
        for(uint i=0;i<size;i++) {
            if(label[i]>0) continue;
            if( mol->getBondOrderbyAtomIndex(id1[id1.size()-1],id[i]) < 1.0 ) continue;
            canbe=true;
            id1.push_back(id[i]);
            label[i]=1;
        }

        if(!canbe)
            return;
    }


    if(mol->getBondOrderbyAtomIndex(id1[id1.size()-1],id[0]) < 1.0) return;


    //coplanar?
    bool iscoplanar=true;
    if(size>3)
    {
        vector3 vv,v;
        vv = cross(mol->getAtomPosbyIndex(id1[1])-mol->getAtomPosbyIndex(id1[0]),
                mol->getAtomPosbyIndex(id1[2])-mol->getAtomPosbyIndex(id1[1]));


        for(uint i=1;i<size-3;i++) {
            v=cross(mol->getAtomPosbyIndex(id1[i+1])-mol->getAtomPosbyIndex(id1[i]),
                    mol->getAtomPosbyIndex(id1[i+2])-mol->getAtomPosbyIndex(id1[i+1]));
            if((v-vv).length()>0.3) {
                iscoplanar=false;
                break;
            }
        }
    }


    if(!iscoplanar) return;

    cout <<" add New Ring "<<endl;

    releaseAllActors();
    setDefaultOperationMode();
    mol->addRing(id1);
    renderMol();
    //mol->displayRingList();

    push2Stack("Add New Ring");
}


bool View3D::isRingExistedforSelectedAtoms()
{
    uint numActors = selectedAtomActors->GetNumberOfItems();

    if(numActors<=2) return false;
    selectedAtomActors->InitTraversal();

    vector <uint> id;
    vector3 cc=VZero;
    for (uint i=0;i<numActors;++i)
    {
        uint ii=selectedAtomActors->GetNextActor()->AtomId();
        id.push_back(ii);
        cc+=mol->getAtomPosbyIndex(ii);
    }
    cc/=numActors;

    //existed?
    bool existed=false;
    for(uint i=0;i<mol->numRings();i++)
    {
        if (mol->getRingbyId(i)->size!=numActors) continue;

        if((mol->getRingbyId(i)->center-cc).length()>0.5) continue;

        existed=true;
        break;

    }
    return existed;
}


void View3D::appendXAtomAtCenterofSelectedAtoms()
{
    uint numActors = selectedAtomActors->GetNumberOfItems();
    if(numActors<2) return;

    vector3 cc=VZero;
    selectedAtomActors->InitTraversal();
    for (uint i=0;i<numActors;i++)
        cc=cc+mol->getAtomPosbyIndex(selectedAtomActors->GetNextActor()->AtomId());

    cc=cc/numActors;
    QString pseudoAtom="X";

    mol->addAtom(pseudoAtom,cc);

    updateMol();

    push2Stack("adding Center");
}

//distance, angle, torsion and center
void View3D::calculateDATC()
{
    //updateSelectedAtomListFromAtomActors();
    displaySelectedAtomList();

    uint numActors = selectedRingList.size();
    if(numActors==1 && numSelectedObjects()==1)
    {
        QString s="Ring ";
        vector3 cc=mol->getRingbyId(selectedRingList[0])->center;
        s=s+"with center: "+QString::number(cc.x())+ " "+QString::number(cc.y())+ " "+QString::number(cc.z());
        return;
    }

    numActors=selectedBondList.size();
    if(numActors==1){
       double distance=mol->distance(selectedBondList[0][1],selectedBondList[0][2]);
       QString s="Bond length: ";
       s+=QString::number(selectedBondList[0][1]+1) +" "+ QString::number(selectedBondList[0][2]+1)+" ";
       s+=QString::number(distance);
       mainWindow->sendMessage(s);
       return;
    }

    numActors = selectedAtomList.size();
    if(numActors>1)
    {
        QString s="Atoms ";
        s+=QString::number(selectedAtomList[0]+1) +" "+ QString::number(selectedAtomList[1]+1)+" ";

        if(numActors==2) //distance
        {
            double distance=mol->distance(selectedAtomList[0],selectedAtomList[1]);
            s=s+" with distance :"+QString::number(distance);
            mainWindow->sendMessage(s);
        }

        if(numActors==3) //angle
        {
            double angle=mol->angle(selectedAtomList[0],selectedAtomList[1],selectedAtomList[2]);
            s=s+" with angle :"+QString::number(angle);
            mainWindow->sendMessage(s);
        }

        if(numActors==4) //angle
        {
            double angle=mol->dihedral(selectedAtomList[0],selectedAtomList[1],selectedAtomList[2],selectedAtomList[3]);
            s=s+" with dihedral :"+QString::number(angle);
            mainWindow->sendMessage(s);
        }

        //center of selected atoms
        vector3 cc=VZero;
        for(uint i=0;i<numActors;i++)
            cc+=mol->getAtomPosbyIndex(selectedAtomList[i]);
        cc/=numActors;
        s="";
        s=s+" with center: "+QString::number(cc.x())+ " "+QString::number(cc.y())+ " "+QString::number(cc.z());
        mainWindow->sendMessage(s);
    }
}

bool View3D::isHydrogenAtom(uint id)
{
    if(mol->getAtomSymbol(id)=="H")
        return true;
     return false;
}

void View3D::modifySelectedRing()
{
    mainWindow->on_pushButton_Ring_clicked();
}

void View3D::centralize()
{
    releaseAllActors();
}

void View3D::periodicTable()
{
    if(numSelectedAtoms()<1) return;
    mainWindow->on_actionPeriodicTable_triggered();
}

void View3D::rotateLeft()
{
    mainWindow->on_pushButton_left_clicked();
}

void View3D::rotateRight()
{
    mainWindow->on_pushButton_right_clicked();
}

void View3D::rotateUp()
{
    mainWindow->on_pushButton_Up_clicked();
}

void View3D::rotateDown()
{
    mainWindow->on_pushButton_Down_clicked();
}


void View3D::addHydrogenAtomForSelected()
{
    mainWindow->on_pushButton_addHydrogen_clicked();
}


void View3D::copySeleted()
{
    copyMolData="MolSymmetry-copy-data\n";
    copyMolData+=QString::number(numSelectedAtoms())+ "\n";;
    for (uint i=0;i<numSelectedAtoms();i++)
    {
        //atom information
        uint id=getSelectedAtomId(i);
        QString a=mol->atomSymbol(id) + "    "
                +QString::number(mol->atomPos(id).x(),'f',6)+ "    "
                +QString::number(mol->atomPos(id).y(),'f',6)+ "    "
                +QString::number(mol->atomPos(id).z(),'f',6)+ "    "

                + QString::number(mol->getAtombyIndex(id)->radius(),'f',2)+ "    "
                + QString::number(mol->atomList[id]->Color())+ "\n";

        copyMolData+=a;
    }

    //bond information
    QString b="";
    uint nbonds=0;
    for (uint i=0;i<numSelectedAtoms();i++)
    {
        uint id=getSelectedAtomId(i);
        for (uint k=i+1;k<numSelectedAtoms();k++)
        {
            uint bo=mol->getBondOrderbyAtomIndex(getSelectedAtomId(k),id);
            if(bo<1) continue;
            b+=QString::number(i)+ " " + QString::number(k)+ " "+QString::number(bo)+"\n";
            nbonds++;
        }
    }
    copyMolData+=QString::number(nbonds)+ "\n";;
    copyMolData+=b;
    //cout << copyMolData.toStdString().c_str()<<endl;
    QApplication::clipboard()->setText(copyMolData);
}

bool View3D::canbePaste()
{
   return mol->canbePaste();
}

bool View3D::canbeUndo()
{
    if(mainWindow->undoList.size()>0)
        return true;
    return false;
}

bool View3D::canbeRedo()
{
    if(mainWindow->redoList.size()>0)
        return true;
    return false;
}



void View3D::cutSelected()
{
   copySeleted();
   setDefaultOperationMode();

   vector <uint > idList;
   for (uint i=0;i<numSelectedAtoms();i++)
       idList.push_back(getSelectedAtomId(i));

   sort ( idList.begin(), idList.end());
   vector <uint> :: iterator iter = unique ( idList.begin(), idList.end());
   idList.erase(iter,idList.end());
   sort(idList.rbegin(), idList.rend());

   //remove from the tail one-by-one
   for (uint i=0;i<idList.size();i++)
      mol->removeSingleAtombyId(idList[i]);

   updateMol();
   push2Stack("Cut ");
}


void View3D::paste()
{
    setDefaultOperationMode();

    uint initNumAtoms=mol->NumAtoms();
    uint initNumBonds=mol->NumBonds();
    uint initNumRings=mol->NumRings();

    mol->pastefromClipboard();

    uint curNumAtoms=mol->NumAtoms();
    uint curNumBonds=mol->NumBonds();
    uint curNumRings=mol->NumRings();

    uint i;
    for(i=0;i<curNumAtoms;i++){
        if(i<initNumAtoms)
            mol->getAtombyIndex(i)->setSelected(false);
        else
            mol->getAtombyIndex(i)->setSelected(true);
    }


    for(i=0;i<curNumBonds;i++) {
        if(i<initNumBonds)
            mol->getBondbyIndex(i)->setSelected(false);
        else
            mol->getBondbyIndex(i)->setSelected(true);
    }

    for(i=0;i<curNumRings;i++) {
        if(i<initNumRings)
            mol->getRingbyId(i)->isSelected=false;
        else
            mol->getRingbyId(i)->isSelected=true;
    }

    updateSelectedFromRendering=true;
    renderMol();
    updateSelectedFromRendering=false;
    push2Stack("paste from Clipboard");


    //cout << "SFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"<<endl;
   // displaySelectedAtomList();
    //displaySelectedBondList();
   //displaySelectedRingList();
   // cout << "SFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"<<endl;
}



void View3D::renderOrbitalAO()
{
    if (!isHuckel && !isExtendedHuckel) return;
    if (isHuckel && !hmo) return;
    if (isExtendedHuckel && !ehmo) return;

    if (isPosLobeVisible)
    {
        double colour1[3];
        colour1[0]=posLobeColor.x();
        colour1[1]=posLobeColor.y();
        colour1[2]=posLobeColor.z();

        auto actor_pos = vtkActor::New();
        auto mapper_pos = vtkPolyDataMapper::New();

        actor_pos->GetProperty()->SetColor(colour1);
        actor_pos->SetPickable(false);
        actor_pos->GetProperty()->SetOpacity(opacity_pos);
        mapper_pos->ScalarVisibilityOff();

        if (isLobeMesh) {
            actor_pos->GetProperty()->SetLineWidth(orbitalLineWidth);
            actor_pos->GetProperty()->SetRepresentationToWireframe();
            actor_pos->GetProperty()->ShadingOff();
            //mapper_pos->ScalarVisibilityOff();
        }


        //uint n=mol->getActiveMO()->ylmData->GetNumberOfPoints();
        //cout << n <<" inside enderOrbitalAO()"<<endl;

        if(isHuckel)
            mapper_pos->SetInputData(hmo ? hmo->ylmData : nullptr);
        if(isExtendedHuckel)
            mapper_pos->SetInputData(ehmo ? ehmo->ylmData : nullptr);


        actor_pos->SetMapper(mapper_pos);
        actor_pos->GetProperty()->SetInterpolationToGouraud();

        actor_pos->GetProperty()->SetDiffuse(0.9);
        actor_pos->GetProperty()->SetAmbient(0.3);
        actor_pos->GetProperty()->SetSpecular(0.2);
        actor_pos->GetProperty()->SetSpecularPower(10.0);

        m_renderer->AddActor(actor_pos);
        mapper_pos->Delete();
    }
    cout << "render orbital positive done "<<endl;



    //negative lobe
    if(isNegLobeVisible)
    {
        double colour2[3];
        colour2[0]=negLobeColor.x();
        colour2[1]=negLobeColor.y();
        colour2[2]=negLobeColor.z();


        auto actor_neg = vtkActor::New();
        auto mapper_neg = vtkPolyDataMapper::New();

        if(isHuckel)
            mapper_neg->SetInputData(hmo ? hmo->ylmData1 : nullptr);
        if(isExtendedHuckel)
            mapper_neg->SetInputData(ehmo ? ehmo->ylmData1 : nullptr);


        actor_neg->GetProperty()->SetColor(colour2);
        actor_neg->SetPickable(false);
        actor_neg->GetProperty()->SetOpacity(opacity_neg);
        mapper_neg->ScalarVisibilityOff();

        if (isLobeMesh)
        {
            actor_neg->GetProperty()->ShadingOff();
            actor_neg->GetProperty()->SetLineWidth(orbitalLineWidth);
            actor_neg->GetProperty()->SetRepresentationToWireframe();
            mapper_neg->ScalarVisibilityOff();
        }

        actor_neg->SetMapper(mapper_neg);
        actor_neg->GetProperty()->SetInterpolationToGouraud();

        actor_neg->GetProperty()->SetDiffuse(0.9);
        actor_neg->GetProperty()->SetAmbient(0.3);
        actor_neg->GetProperty()->SetSpecular(0.2);
        actor_neg->GetProperty()->SetSpecularPower(10.0);

        m_renderer->AddActor(actor_neg);
        mapper_neg->Delete();
    }
}

void View3D::renderNode()
{
    if(!isNodeVisible) return;


    auto actor_node = vtkActor::New();
    auto mapper_node = vtkPolyDataMapper::New();

    if(isHuckel)
mapper_node->SetInputData(hmo ? hmo->pDataNode : nullptr);
        if(isExtendedHuckel)
            mapper_node->SetInputData(ehmo ? ehmo->pDataNode : nullptr);


    actor_node->GetProperty()->SetColor(nodecolor[0],nodecolor[1],nodecolor[2]);
    actor_node->SetPickable(false);
    actor_node->GetProperty()->SetOpacity(opacity_node);
    mapper_node->ScalarVisibilityOff();

    actor_node->SetMapper(mapper_node);
    actor_node->GetProperty()->SetInterpolationToGouraud();

    actor_node->GetProperty()->SetDiffuse(0.9);
    actor_node->GetProperty()->SetAmbient(0.3);
    actor_node->GetProperty()->SetSpecular(0.2);
    actor_node->GetProperty()->SetSpecularPower(10.0);

    m_renderer->AddActor(actor_node);
    mapper_node->Delete();


}


void View3D::renderOrbital()
{
    if (!isMOVisible) return;
    qDebug() << "renderOrbital: isMOVisible=true";

    if (mol==nullptr)  return;
    qDebug() << "renderOrbital: mol not null, atoms=" << mol->NumAtoms();

    if (mol->NumAtoms()<1) return;

    qDebug() << "renderOrbital: isHuckel=" << isHuckel << " isExtendedHuckel=" << isExtendedHuckel;

    if(isExtendedHuckel==false && isHuckel==false)
        return;

    if(isHuckel)
        if (!hmo || !hmo->isSurfaceOK()) {
            qDebug() << "renderOrbital: hmo not ok, returning";
            return;
        }

    if(isExtendedHuckel)
        if (!ehmo || !ehmo->isSurfaceOK()) {
            qDebug() << "renderOrbital: ehmo not ok, returning";
            return;
        }



    if(!isMOType) {
        renderOrbitalAO();
        return;
    }

    cout << "render orbital "<<endl;
    if (isPosLobeVisible)
    {
        cout << "render positive lobe"<<endl;
        double colour1[3];
        colour1[0]=posLobeColor.x();
        colour1[1]=posLobeColor.y();
        colour1[2]=posLobeColor.z();

        auto actor_pos = vtkActor::New();
        auto mapper_pos = vtkPolyDataMapper::New();

        actor_pos->GetProperty()->SetColor(colour1);
        actor_pos->SetPickable(false);
        actor_pos->GetProperty()->SetOpacity(opacity_pos);
        mapper_pos->ScalarVisibilityOff();

        if (isLobeMesh) {
            actor_pos->GetProperty()->SetLineWidth(orbitalLineWidth);
            actor_pos->GetProperty()->SetRepresentationToWireframe();
            actor_pos->GetProperty()->ShadingOff();
            //mapper_pos->ScalarVisibilityOff();
        }

        if(isHuckel)
            mapper_pos->SetInputData(hmo ? hmo->pData : nullptr);
        if(isExtendedHuckel)
            mapper_pos->SetInputData(ehmo ? ehmo->pData : nullptr);


        actor_pos->SetMapper(mapper_pos);
        actor_pos->GetProperty()->SetInterpolationToGouraud();

        actor_pos->GetProperty()->SetDiffuse(0.9);
        actor_pos->GetProperty()->SetAmbient(0.3);
        actor_pos->GetProperty()->SetSpecular(0.2);
        actor_pos->GetProperty()->SetSpecularPower(10.0);

        m_renderer->AddActor(actor_pos);
        mapper_pos->Delete();
    }
    cout << "render orbital positive done "<<endl;



    //negative lobe
    if(isNegLobeVisible)
    {
        cout << "render negative lobe"<<endl;

        double colour2[3];
        colour2[0]=negLobeColor.x();
        colour2[1]=negLobeColor.y();
        colour2[2]=negLobeColor.z();


        auto actor_neg = vtkActor::New();
        auto mapper_neg = vtkPolyDataMapper::New();

        if(isHuckel)
            mapper_neg->SetInputData(hmo ? hmo->pData1 : nullptr);
        if(isExtendedHuckel)
            mapper_neg->SetInputData(ehmo ? ehmo->pData1 : nullptr);

        actor_neg->GetProperty()->SetColor(colour2);
        actor_neg->SetPickable(false);
        actor_neg->GetProperty()->SetOpacity(opacity_neg);
        mapper_neg->ScalarVisibilityOff();

        if (isLobeMesh)
        {
            actor_neg->GetProperty()->ShadingOff();
            actor_neg->GetProperty()->SetLineWidth(orbitalLineWidth);
            actor_neg->GetProperty()->SetRepresentationToWireframe();
            mapper_neg->ScalarVisibilityOff();
        }

        actor_neg->SetMapper(mapper_neg);
        actor_neg->GetProperty()->SetInterpolationToGouraud();

        actor_neg->GetProperty()->SetDiffuse(0.9);
        actor_neg->GetProperty()->SetAmbient(0.3);
        actor_neg->GetProperty()->SetSpecular(0.2);
        actor_neg->GetProperty()->SetSpecularPower(10.0);

        m_renderer->AddActor(actor_neg);
        mapper_neg->Delete();
    }


    cout << "render orbital negtive done"<<endl;
}
