#ifndef AO3DVIEW_H
#define AO3DVIEW_H

#include <QWidget>
#include <QColor>
#include <vector>
#include <QMap>

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkDataSet.h>
#include <vtkClipPolyData.h>
#include <vtkPlane.h>

#include "vtkbool/vtkPolyDataBooleanFilter.h"

#include <vtkImplicitPlaneWidget2.h>
#include <vtkImplicitPlaneRepresentation.h>



#include <vtkOutlineFilter.h>
#include <vtkStripper.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkImplicitBoolean.h>

#include <vtkCutter.h>
#include <vtkFillHolesFilter.h>
#include <vtkStripper.h>
#include <vtkFeatureEdges.h>
#include <vtkRegularPolygonSource.h>
#include <QVTKOpenGLNativeWidget.h>
#include <QVTKOpenGLWindow.h>
#include <vtkDelaunay2D.h>
#include <vtkLookupTable.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkPolyLine.h>
#include <vtkLineSource.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkPNGWriter.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>

//#include "mouseEventInteractorStyle.h"

#include <QVTKInteractor.h>
#include "InteractorView3d.h"

using namespace std;
class HMol;
class MainWindow;
class EHMO;
class HMO;
class templateMol;
class TemplateView3d;

#include "Mol.h"




class View3D : public QVTKOpenGLNativeWidget
{
  Q_OBJECT
public:
  explicit View3D(QWidget *parent = 0);
    ~View3D() {clearAll();}


  // Add a data set to the scene
  void addDataSet(vtkSmartPointer<vtkDataSet> dataSet);

  // Remove the data set from the scene
  void removeLastActor();
  void removeAllActors();
  void removeDynamicLineActor();
  void renderAxis();

  HMol  * mol;
  MainWindow * mainWindow;
  templateMol * tmol;
  TemplateView3d  * view3dT;

  void setTemplateView3d (TemplateView3d  *);

  HMol  * getMol ();
  MainWindow * getMainWindow () ;
  void refresh();


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


public:
  //int idx;
 // void setId(int id){idx=id;}
 // int getId(){return idx;}

  bool  _isXYZAxisVisible;
  bool  isPGVisible;

  bool isXYZAxisVisible () {return _isXYZAxisVisible;}
  void setXYZAxisVisible (bool t) {_isXYZAxisVisible=t;}



public:
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkPlane> plane;
    vtkSmartPointer<vtkClipPolyData> clipper_pos,clipper_neg;

    uint numActors();
    void renderMol(HMol *  );
    void renderMol() {refresh();}
    void renderAtoms();

    void renderBonds();
    void renderAtomLabel();
    void updateAtomLabelState();

    void renderLine(double p0[3],double p1[3],float width,double color[3],vtkTransform * transform);
    void renderLine (vector3 beg, vector3 end,float width,double color[3]);
    void renderLineCenter (vector3 beg, vector3 end,float width,double color[3]);
    void renderLines (vector3 a, vector3 b, vector3 c, vector3 shift, float width,double color[3]);

    vector3 getBackgroundColor() {return bkcolor;}
    void setBKColor( vector3 );
    void writePNG(QString );

    void renderTube(vector3 beg, vector3 end, double radius, double color[3]);
    void renderTubeCenter(vector3 beg, vector3 end, double radius, double color[3]);
    void renderTube(vector3 beg, vector3 end, vector3 center,double radius, double color[3]);
    void renderArcTube(vector3 beg, vector3 end, vector3 center, double color[3], uint id);
    void renderArcTube(vector <uint> atoms, double radius, double color[3], uint id);

    void setCenter (vector3 c){center=c;}

    void updateView();

    vtkCamera * Camera() {return camera;}

    vtkSmartPointer<vtkCamera> camera;


    vector3 getColorAromaticRing();
    vector3 getFilledColorAromaticRing();
    void    setColorAromaticRing(double r,double g, double b);
    void    setFilledColorAromaticRing(double r,double g, double b);



    bool isAtomLabelVisible () {return _isAtomLabelVisible;}
    void setAtomLabelVisible(bool b) {_isAtomLabelVisible=b;}


    void setMolVisible(bool b) {isMol_Visible=b;}
    void setHydrogenVisible(bool b) {isHydrogen_Visible=b;}

    bool isHydrogenHide() {return (!isHydrogen_Visible);}
    bool isMolHide() {return (!isMol_Visible);}




    void renderSimpleSingleBond(uint ); //one color

    void renderHydrogenBond(uint );
    void renderWeakBond(uint );

    void renderTripleBond(uint );
    void renderDoubleBond(uint );
    void renderSingleBond(uint );
    void renderQuadrupleBond(uint );

    void renderAromaticSingleBond(uint );
    void renderAromaticRings();
    void renderAromaticBond(uint);



    void removeSelectedAtomsBonds();
    void removeSelectedAtoms();
    void removeSelectedBonds();
    void removeSelectedRings();

    void renderCubeSkeleton(double );



    void centralize();
    void periodicTable();

public slots:

  void zoomToExtent();

  void onViewXY();
  void onViewXZ();
  void onViewYZ();
  void onViewReset();

  void onView_rotateClockwise(double degree);
  void onView_rotateCounterClockwise(double degree);
  void onView_rotateUp(double degree);
  void onView_rotateDown(double degree);
  void onView_rotateLeft(double degree);
  void onView_rotateRight(double degree);

  void setMolTypeStick() ;
  void setMolTypeBallStick() ;
  void setMolTypeLine();


  void clearAll();


  void setAreaInteractor();

public:
  void setSelectedUpdateFromRendering(bool o) {updateSelectedFromRendering=o;}
  bool isSelectedUpdateFromRendering() {return updateSelectedFromRendering;}

  bool isKekuleStyle () {return _isKekuleStyle;}
  void setKekuleStyle (bool b) {_isKekuleStyle=b;}


  void initParameter();


  void setAtomScale(double s) {scale_atom=s*0.3;}
  void setBondScale(double s) {scale_bond=s;bondRadius=s*0.07;}

  double getAtomScale() {return scale_atom/0.3;}
  double getBondScale() {return scale_bond;}


  void setRingSizeScale(double s) {scale_ringSize=s;}
  void setRingRadiusScale(double s) {scale_ringRadius=s;}
  double getRingSizeScale() {return scale_ringSize;}
  double getRingRadiusScale() {return scale_ringRadius;}


  //pencil draw
  void    addCarbon(vector3 );
  void    addCarbon(double x, double y, double z);
  void    addCarbon_and_link2SelectedAtom(vector3, uint i);

  vtkActor * getLastAtomActor() {return lastAtomActor; }
  void    linkTwoSelectedAtoms(uint, uint );
  void    Link2LastAtom(vector3);
  void    Link2LastAtom(double x, double y, double z);

  void    translateSelectedAtoms(vector3 );
  void    rotateSelectedAtoms(vector3, matrix3x3 );
  void    rotateMol();

  void    symmetryPercept();

  vtkRenderer * getRender() {return m_renderer;}


  void  copySeleted();
  void  cutSelected();



private:
  QString copyMolData;
  vtkActor * lastAtomActor;
  double scale_ringSize,scale_ringRadius;
  vector3 LastCarbonPos;


private:
  uint outLineType,typeSigmaH,typeSigmaV,typeSigmaD;
  bool _isKekuleStyle;
  bool updateSelectedFromRendering;

private:
  vtkSmartPointer<vtkRenderer> m_renderer;
  vector3 nodecolor;
  vector3 bkcolor,whiteColor;
  vector3 selectedColor;
  double scale_atom,scale_bond; //for display
  vector3 center;

  double  bondRadius,thickness_Cn;
  double  moleculeLength, principalAxisRadius;

  double  CnRadius_Scale,C2Radius_Scale,C3Radius_Scale,C4Radius_Scale;
  double  Cn_Radius,C2_Radius,C3_Radius,C4_Radius;

  double  Cn_Length_Scale,C2_Length_Scale,C3_Length_Scale,C4_Length_Scale;
  double  Cn_Length,C2_Length,C3_Length;


  double  discRadius,discCn_scale,discC2_scale,discC3_scale;

  double textColor[3],textSize;

  double color_Cn[3],color_In[3];
  double color1_C2[3],color2_C2[3],color_C3[3];

  double color_sigmaV[3],color_sigmaD[3],color_sigmaH[3];
  double yellow[3],blue[3],white[3],black[3],grey[3],brown[3];

  double OpacityH,OpacityV,OpacityD;
  double thicknessSigmaV0,thicknessSigmaD0,thicknessSigmaH0;
  double thicknessSigmaV_Scale,thicknessSigmaD_Scale,thicknessSigmaH_Scale;
  double thicknessH,thicknessD,thicknessV;


  double discThickness, thickness_Cn_Scale,thickness_C2_Scale,thickness_C3_Scale;

  bool isStick,isBallStick, isLine;
  bool _isAtomLabelVisible;

  bool isPrincipalAxisVisible,isHorizontalC2Visible;
  bool _isSigmaHVisible,_isSigmaVVisible,_isSigmaDVisible;
  bool _isCnVisible, _isSnVisible, _isI2nVisible;
  bool _isCiVisible, isCsVisible;
  bool _isC2_1_Visible, _isC2_2_Visible;
  bool isMol_Visible,isHydrogen_Visible;

  bool _isC2_Visible,_isC3_Visible,_isC4_Visible,_isC5_Visible,_isC6_Visible;
  bool isI2n;


  QString  PointGroup;
  uint     principalOrder;

  //vtkSmartPointer<vtkLight> light;
  vtkPolyDataBooleanFilter * bfPolyData;

  SelectAreaInteractorStyle * actorInteractorA;

  //MouseEventInteractorStyle * mouseEventStyle;
  //PickCellInteractorStyle   * pickCellInterStyle;


   uint horizontalC2_type;


   vector3 origin_SelectedAtoms;
   double length_SelectedAtoms;

   double verticalLength,HorizontalLengh;

   //selection operation
public:
   void updateMolLength();
   vector3 getOriginSelectedAtoms() {return origin_SelectedAtoms;}
   double  getLengthSelectedAtoms() {return length_SelectedAtoms;}


   vector <uint > selectedAtomList, selectedRingList;
   vector < vector < uint > > selectedBondList;  // <index, atom-beg, atom-end>

   vtkSmartPointer<vtkActorCollection> selectedAtomActors, selectedBondActors, selectedRingActors;
   vtkSmartPointer<vtkActorCollection> atomLabelActors;

   void releaseAtomLabelActors();

   void releaseAllActors2();
   void releaseInteractorPickedActor();
   void releaseSelectedAtomActors();
   void releaseSelectedBondActors();
   void releaseSelectedRingActors();

   void clearSelectedAtomList();
   void clearSelectedBondList();
   void clearSelectedRingList();
   
   void updateSelectedRingListFromRingActors();
   void updateSelectedAtomListFromAtomActors();
   void updateSelectedBondListFromBondActors();
   void storeSelectBondList(uint , uint ); //help function

   void removefromAtomActorsList (vtkActor* );
   void removefromBondActorsList (vtkActor* );
   void removefromRingActorsList (vtkActor* );


   void releaseSingleActor(vtkActor* );
   void releaseSingleAtomActor(vtkActor* );
   void releaseSingleBondActor(vtkActor* );


   void displaySelectedAtomActorsList();

   void buildAtomLabel(vtkActor *);
   void removeAtomLabelActor (uint );
   void removeAllAtomLabelActors();
   void releaseAllActors();

   void displaySelectedAtomList();
   void displaySelectedBondList();
   void displaySelectedRingList();

   uint getSelectedAtomId(uint);
   uint numSelectedAtoms();
   bool isAtomPicked(uint );
   void renderSelectedAtomLabel();

   uint getSelectedRingId(uint);
   uint numSelectedRings();

   uint getBondOrderSelectedById(uint id);
   void getSelectedBondIds(uint id, uint & id1, uint & id2);
   uint numSelectedBonds();

   bool hasPicked(vtkActor * actor);
   void uniqueSelectAtomList();


   void loadTemplateMol (QString );


   void linkTemplate2Mol(uint );

   void addNewTemplate(vector3 );
   void addNewTemplate(double x, double y, double z);
   void clearTemplate();


   void selectAll();
   void paste();

   double getSelectedAtomXbyIndex(uint );
   double getSelectedAtomYbyIndex(uint );
   double getSelectedAtomZbyIndex(uint );

   void setSelectedAtomPosbyIndex(uint ,double , double , double );

   void updateMol() {refresh();}

   void undo();
   void redo();
   void push2Stack(QString );

   void initializeActor (vtkActor * );

   void replaceSelelctedAtomWith(uint id); //Period table

   uint getPrincipalOrder() {return principalOrder;}

   void setSelectedBondOrder(uint );
   uint getBondOrderbyId(uint );

   void reverseAromaticBond();
   void modifyAtomColor();
   void modifyAtomRadius();
   void buildRingForSelectedAtoms();
   uint numSelectedObjects() ;

   bool isRingExistedforSelectedAtoms();
   void appendXAtomAtCenterofSelectedAtoms();

   void modifySelectedRing();


   void rotateLeft();
   void rotateRight();
   void rotateUp();
   void rotateDown();

   uint getBondOrderbyIndex(uint id) {return mol->getBondOrderbyIndex(id);}

   void addHydrogenAtomForSelected();
   bool isXExisted() {return mol->isXExisted();}
   bool isHydrogenAtomExisted() {return mol->isHydrogenAtomExisted();}

   bool canbeUndo();
   bool canbeRedo();
   bool canbePaste();
   void calculateDATC(); //distance, angle, torsion and center
   bool isHydrogenAtom(uint );


   vector3  getAtomPos(uint );
   vector3 getSelectedAtomPosbyIndex(uint );

   void    setSelectedAtomSymbol(QString );

   void virtual focusOutEvent(QFocusEvent *) override;
   //void virtual contextMenuEvent(QContextMenuEvent *)override;



   void renderNode();
   void renderOrbital();
   void renderOrbitalAO();

   void setAOType(bool t) {isMOType=(!t);}
   void setLobeMesh () {isLobeMesh=true;}
   void setLobeSmooth () {isLobeMesh=false;}
   void setMOVisible () {isMOVisible=true;}
   void setMOInvisible() {isMOVisible=false;}
   void setMOVisible(bool v) {isMOVisible=v;}


   void setNegLobeVisible()  {isNegLobeVisible=true;}
   void setNegLobeInvisible() {isNegLobeVisible=false;}
   void setPosLobeVisible()   {isPosLobeVisible=true;}
   void setPosLobeInvisible() {isPosLobeVisible=false;}

   void setNodeVisible()   {isNodeVisible=true;}
   void setNodeInvisible() {isNodeVisible=false;}


   void setNegLobeColor (vector3 c) {negLobeColor=c;}
   void setPosLobeColor (vector3 c) {posLobeColor=c;}
   vector3 getPosLobeColor () {return posLobeColor;}
   vector3 getNegLobeColor () {return negLobeColor;}

   void setNodecolor (vector3 v){ nodecolor=v;}
   vector3 getNodecolor (){ return nodecolor; }

   void setNodeOpicity (double o){opacity_node=o;}
   double getNodeOpicity () {return opacity_node;}


   void setPosLobeOpacity (double o){opacity_pos=o;}
   void setNegLobeOpacity (double o){opacity_neg=o;}
   double getPosLobeOpacity (){return opacity_pos;}
   double getNegLobeOpacity (){return opacity_neg;}

   void setOrbitalLineWidth (double s) {orbitalLineWidth=s;}
   double getOrbitalLineWidth () {return orbitalLineWidth;}

   void setHuckel() {isHuckel=true;isExtendedHuckel=isXTB=false;}
   void setExtendedHuckel() {isExtendedHuckel=true;isHuckel=isXTB=false;}
   void setXTB() {isXTB=true;isExtendedHuckel=isXTB=false;}
   void setEHMO (EHMO * p){ehmo=p;isExtendedHuckel=true;isHuckel=isXTB=false;}
   void setHMO (HMO * p){hmo=p;isHuckel=true;isExtendedHuckel=isXTB=false;}

   vector3 getXColor() {return xColor;}
   vector3 getYColor() {return yColor;}
   vector3 getZColor() {return zColor;}
   void setXColor(vector3 c) {xColor=c;}
   void setYColor(vector3 c) {yColor=c;}
   void setZColor(vector3 c) {zColor=c;}

private:
   HMO *  hmo;
   EHMO * ehmo;
   double opacity_neg,opacity_pos,opacity_node;
   vector3 negLobeColor,posLobeColor;
   vector3 xColor,yColor,zColor;
   double orbitalLineWidth;

   bool isMOVisible,isNodeVisible;
   bool isMOType;
   bool isLobeMesh,isLobeSmooth;
   bool isAOtype,isMOtype;
   bool isNegLobeVisible,isPosLobeVisible;

   bool isHuckel, isExtendedHuckel, isXTB;

   bool firstTime;
   bool isRefresh=false;
};




#endif // AO3DVIEW_H
