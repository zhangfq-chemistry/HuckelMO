#ifndef SELECTAREAINTERACTORSTYLE_H
#define SELECTAREAINTERACTORSTYLE_H

#include <QObject>

#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkProperty.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkCamera.h>
#include <vtkMatrix4x4.h>

#include <vtkSmartPointer.h>
#include <vtkCommand.h>
#include <vtkActorCollection.h>
#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>
#include "math/vector3.h"
#include <QColor>
#include <QVector>
#include <QList>
#include <vector>
#include <QMenu>
#include <QAction>


using namespace std;

class HMol;
class View3D;
class MainWindow;
class vtkPlanes;


enum MouseOperationType {VTKPencilDrawing, VTKLinkTemplate, VTKRotation,
                         VTKTranslation, VTKSelection,
                         VTKOperateSelected,VTKDefaultOperation};







class SelectAreaInteractorStyle : public QObject, public vtkInteractorStyleRubberBandPick
{
    Q_OBJECT
public:
    static SelectAreaInteractorStyle *New();
    vtkTypeMacro(SelectAreaInteractorStyle,vtkInteractorStyleRubberBandPick);
    ~SelectAreaInteractorStyle();

    virtual void OnChar() override;
    void OnKeyPress() override;
    void OnKeyRelease()  override;
    virtual void OnLeftButtonUp();
    virtual void OnLeftButtonDown();


    virtual void OnMouseMove() override;

    //modify right button behavior
    virtual void OnRightButtonDown() override;
    virtual void OnRightButtonUp() override;


    virtual void OnMiddleButtonDown() override;
    virtual void OnMiddleButtonUp() override;


    void setMol (HMol *);
    vtkCamera * getCamera();
    void setView3d(View3D *);
    void setMainWindow(MainWindow *);

    void highlightActor(vtkActor * );
    void buildAtomLabel(vtkActor * );
    void releaseAllActors();
    void releaseRelatedBondActors(vtkActor * );

    MouseOperationType mouseOperationMode;
    MouseOperationType currentMouseOperationMode();


    void setDefaultOperationMode();
    void setPencilDrawMode();
    void setLinkTemplateMode();
    void setRotateMode();
    void setTranslateMode();
    void setSelectionMode();
    void setOperateSelectedMode();

    bool isLinkTemplateMode() ;
    bool isPencilDrawMode();
    bool isSingleSelectionMode();
    bool isSelectionMode();

    bool isRotateMode();
    bool isTranslateMode();


    void releaseSingleRingActor(vtkActor* );
    bool isRingActorPicked(vtkActor * );



    void releaseAllSelectedActors();
    void releaseSingleActomActor(vtkActor* );
    bool isAtomActorPicked(vtkActor * );
    bool isBondActorPicked(vtkActor * );
    bool isAtomPicked(uint );

    void collectLeftHalfBonds(vector <uint > );

    void linkMolecule();

    void link2SelectedActors(vtkActor *, vtkActor *);
    void disposeSingleSelectedActor(vtkActor * );

    void updateSelectedAtomListFromAtomActors();
    void updateSelectedBondListFromBondActors();

    uint numSelectedAtoms();
    uint numSelectedBonds();
    uint numSelectedRings();
    uint numSelectedObjects();


    void  translateSelectedAtoms ();
    void  rotateSelectedAtoms ();

    vector3 getHitModelPositionBasedonLinkeredActor(uint x, uint y, vtkActor *);
    vector3 getHitModelPosition(uint x, uint y);
    vector3 getHitModelPosition(uint p[2]) ;


signals:
    void areaSelected(vtkPlanes* plane);

public slots:
    void loseFocus();
    void contextMenu();

    void slot_ReverseArBond();

    void slot_AtomColor();
    void slot_AtomSize();
    void slot_AddRing();
    void slot_AddCenter();


    void slot_Undo();
    void slot_Redo();

    void slot_Delete();
    void slot_Copy();
    void slot_Cut();
    void slot_Paste();

    void slot_HideHydrogenAtoms();
    void slot_HideMolecule();
    void slot_HideAxis();
    void slot_HideAtomLabel();

    void slot_Centralize();
    void slot_RemoveX();

    void slot_ModifyRing();

    void releasePickedActor() {pickedActor=nullptr;}

protected:
    explicit SelectAreaInteractorStyle(QObject *parent = 0);


    HMol * mol;
    View3D * view3d;

    bool willLink;
    bool isShiftPressed, isControlPressed,isAltPressed;
    bool isLeftButtonDown,isRightButtonDown,isMiddleButtonDown;

    bool isMouseMove;
    //int  xhitPos,yhitPos;

    vtkSmartPointer<vtkActor> m_highlightActor;
    vtkActor  * pickedActor;

    vtkActor * tmpLineActor;
    vtkPolyDataMapper * tmpLineMapper;
    vtkLineSource * tmpLineSource;

    QColor HighlightColor;
    vector3 v0,v1;
    double selectedColor[3];

    uint beg[2],end[2];
    int NumberOfClicks;
    bool doubleClick;
    int * sizePort;
};

#endif // SELECTAREAINTERACTORSTYLE_H
