安装方法:
(1) 安装 Qt6.10
(2) 下载VTK9.6,在vtkActor.h插入以下代码,编译安装(cmake-gui选择所有与Qt相关的选项)
pravite:
int  _id, _idMol,  _idMO,  _idAtom, _idAtom1, _idBond, _idUnitCellBox, _idRing;
int  _idAtomLabel,  _idBondLabel,  _idChargeLabel, _idFreeValenceLabel;
double  _color[3];
int  _bondorder;

public:
  void initialId();
  void  setBondOrder (int b) {_bondorder=b;}
  int   getBondOrder () {return  _bondorder;}
  
  void setRingId(int i) {_idRing=i;}
  int getRingId () {return  _idRing;}
  
  void setId (int i) {_id=i;}
  void setMolId  ( int i ) {_idMol=i;}
  void setMOId   ( int i ) {_idMO=i;}
  void setAtomId ( int i ) {_idAtom=i;}
  void setAtomId1 ( int i ) {_idAtom1=i;}
  void setBondId ( int i ) {_idBond=i;}

  void setUnitCellBoxId ( int i ) {_idUnitCellBox=i;}

  int  Id () {return _id;}
  int  MolId () {return _idMol;}
  int  MOId ()  {return _idMO;}



  int  AtomId () {return _idAtom;} 
  int  AtomId1 () {return _idAtom1;}

  int  BondId () {return _idBond;}
  
  void setAtomLabelId (int i) {_idAtomLabel=i;}
  void setBondLabelId (int i) {_idBondLabel=i;}
  void setChargeLabelId (int i) {_idChargeLabel=i;}
  void setFreeValenceLabelId (int i) {_idFreeValenceLabel=i;}
    
  int   getAtomLabelId () {return  _idAtomLabel;}
  int   getBondabelId() {return  _idBondLabel;}
  int   getChargeLabelId () {return  _idChargeLabel;} 
  int  getFreeValenceLabelId () {return  _idFreeValenceLabel;} 

  int  UnitCellBoxId () {return _idUnitCellBox;} 

  void storeColor(double r,double g, double b) ;
  void recoverInitialColor ()  ;

  void vtkActor::storeColor(double r,double g, double b)   {	
    _color[0]=r;	_color[1]=g;	_color[2]=b;
  } 
  
 void vtkActor::recoverInitialColor ()   {
    this->GetProperty()->SetColor( _color); 
 }
(3)compile and install openbabel
(4) install xtb from website
(5) compile this project

<img width="3715" height="2096" alt="hmo-1" src="https://github.com/user-attachments/assets/c79da8cc-da97-45c5-b492-f9f13b395069" />
<img width="3715" height="2096" alt="hmo-2" src="https://github.com/user-attachments/assets/437223b7-3b80-47f9-8198-d7d574e9625c" />
<img width="3715" height="2096" alt="hmo-3" src="https://github.com/user-attachments/assets/9fcabd99-b561-4825-846e-2446a94ee5f0" />
<img width="3715" height="2096" alt="hmo-4" src="https://github.com/user-attachments/assets/2c39772f-4a18-43dd-a06b-2ef68a894c88" />
<img width="3715" height="2096" alt="hmo-5" src="https://github.com/user-attachments/assets/d5ae5b18-1764-4b9d-88e6-016a5c9542a0" />
<img width="3715" height="2096" alt="hmo-6" src="https://github.com/user-attachments/assets/0c54ae8d-3d82-4204-b7e9-4b87a890fccd" />
<img width="3715" height="2096" alt="hmo-7" src="https://github.com/user-attachments/assets/71703042-eb03-4ee3-92f6-c0d88fc44c93" />
<img width="3715" height="2096" alt="hmo-8" src="https://github.com/user-attachments/assets/d83d8b9f-88f8-4bc3-a91c-c5183d69ac99" />
<img width="3715" height="2096" alt="hmo-9" src="https://github.com/user-attachments/assets/e0a78a94-25b2-4a11-abc5-ae48b6df8553" />
<img width="3715" height="2096" alt="hmo-15" src="https://github.com/user-attachments/assets/197fafa8-5649-496a-8b6d-5387734746d6" />
<img width="3715" height="2096" alt="hmo-16" src="https://github.com/user-attachments/assets/d58159ff-8aaf-417a-a45b-7bfc85343fba" />
<img width="3715" height="2096" alt="hmo-17" src="https://github.com/user-attachments/assets/92ddda39-88b8-4ef4-91c7-cabe1186fd6c" />
<img width="3715" height="2096" alt="hmo-19" src="https://github.com/user-attachments/assets/ffddd636-89bd-4012-b45f-a79572b80422" />
<img width="3715" height="2096" alt="hmo-20" src="https://github.com/user-attachments/assets/8d7c6a15-541d-4c86-91e3-cb4791a1b55f" />
<img width="3715" height="2096" alt="hmo-21" src="https://github.com/user-attachments/assets/488e8035-86a3-4dc2-9e81-069779c53f8a" />
<img width="3715" height="2096" alt="hmo-22" src="https://github.com/user-attachments/assets/9513e40f-e2e9-48ec-bba0-84a6f98d440d" />
<img width="3712" height="2090" alt="hmo-23" src="https://github.com/user-attachments/assets/a960014b-9850-4e6c-9680-d95db5337bb0" />
<img width="3712" height="2090" alt="hmo-24" src="https://github.com/user-attachments/assets/979d0ce4-ef54-4bcb-b3cc-cdb4a258508b" />
<img width="3712" height="2090" alt="hmo-25" src="https://github.com/user-attachments/assets/bc177f20-5853-4caf-8d07-0960d4d0fbeb" />
