#include "templateMol.h"
#include "functionals.h"
#include <QFile>
#include <QMessageBox>
#include "mainwindow.h"
#include <openbabel/ring.h>
#include <openbabel/kekulize.h>

templateMol::templateMol()
{
    atomList.clear();
    bondList.clear();
    ringList.clear();
    hotIndex=0;
}


void templateMol::centeralize()
{
    vector3 MassCenter=VZero;
    double totalMass=0.0;

    for (uint i=0;i<numAtoms();i++) {
        double mass=atomicMass(i);
        MassCenter += atomPos(i)*mass;
        totalMass += mass;
    }

    MassCenter /= totalMass;

    cout << MassCenter<<endl;

    for (uint j=0;j<numAtoms();j++){
        vector3 p=atomPos(j)-MassCenter;
        setAtomPos(j,p);
    }
}



void templateMol::reCalculateBonds()
{
    double r1, r2;
    QString a1,a2;
    vector3 p1,p2;
    for(uint i=0;i<numAtoms();i++)
    {
        if(getAtombyIndex(i)->Symbol()=="X") continue;
        p1=getAtomPosbyIndex(i);
        r1=getAtomRadiusbyIndex(i);

        //cout << i+1<<endl;
        for(uint j=i+1;j<numAtoms();j++)
        {
            if(getAtombyIndex(j)->Symbol()=="X") continue;
            r2=getAtomRadiusbyIndex(j);
            p2=getAtomPosbyIndex(j);

            if( (p1-p2).length() > (r1+r2)*1.1) continue;

            HBond * bond = new HBond(i,j,1);
            bondList.push_back(bond);
            getAtombyIndex(i)->appendLinkdedAtomId(j);
            getAtombyIndex(j)->appendLinkdedAtomId(i);
        }
    }
}


void templateMol::LoadFile(QString _file)
{
    clearAll();

    fileName=_file;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(0, "Warning","Cannot Open File!");
        return;
    }
    double x, y, z;
    QString  atomLabel;
    QStringList lst;
    QString line;

    hotIndex=0;
    uint IndexHydrogen=0;
    bool isHotDefined=false;
    bool findFirstHydrogen=false;

    uint _numBonds=0;
    uint _numAtoms=0;

    while(!file.atEnd())
    {
        //first line
        line=file.readLine();
        name=line.simplified();

        cout << line.toStdString().c_str()<<endl;

        //second line
        line=file.readLine();
        uint _numAtoms=line.toInt();

        for (uint i=0;i<_numAtoms;i++)
        {
            line=file.readLine().simplified();

            lst=line.split(" ",Qt::SkipEmptyParts);

            if (line.contains("hot")) {
                hotIndex=i;
                isHotDefined=true;
            }


            if  (lst.size()<3 || lst.size()>5)
                continue;

            cout << line.toStdString().c_str()<<endl;

            atomLabel=lst[0].simplified();
            x=lst[1].toDouble();
            y=lst[2].toDouble();
            z=lst[3].toDouble();

            HAtom * atom = new  HAtom();
            atom->setSymbol(atomLabel);
            atom->setCartesian(x,y,z);
            atomList.push_back(atom);

            if(!findFirstHydrogen)
            if(atomLabel=="H")  {
                IndexHydrogen=atomList.size()-1;
                findFirstHydrogen=true;
            }
        }

        if(!isHotDefined)
            hotIndex=IndexHydrogen;

        //second line
        line=file.readLine().simplified();
        _numBonds=line.toInt();
        for (uint i=0;i<_numBonds;i++)
        {
            line=file.readLine().simplified();
            lst=line.split(" ",Qt::SkipEmptyParts);
            if  (lst.size()!=3) continue;

            uint bondorder=lst[2].toInt();

            //aromatic
            if(bondorder==4) // order = 4 is defined in gaussian view
            {
                bondorder=6;  // order = 6
                isExistedAromaticBond=true;
                getAtombyIndex(lst[0].toInt()-1)->setAromatic(true);
                getAtombyIndex(lst[1].toInt()-1)->setAromatic(true);
            }

            HBond * bond = new HBond(lst[0].toInt()-1,lst[1].toInt()-1,bondorder);

            //cout << bond->atomIndex0()+1 << " "<<bond->atomIndex1()+1<< " " << bond->getBondOrder()<<endl;
            bondList.push_back(bond);


            getAtombyIndex(lst[0].toInt()-1)->appendLinkdedAtomId(lst[1].toInt()-1);
            getAtombyIndex(lst[1].toInt()-1)->appendLinkdedAtomId(lst[0].toInt()-1);
        }
    }

    file.close();

    centeralize();
    if(_numBonds<1)
        reCalculateBonds();




    //check aromatic based on bond order==6 or not?
    if (isExistedAromaticBond)
    {
        //check aromatic based on openbabel
        perceiveRings();

        //further check based on bond order
        for (uint i=0;i<numRings();i++)
        {
            HRing  * ring = getRingbyId(i);

            if(ring->isAromatic) continue;

            bool isAromatic=true;
            for (uint j=0;j<ring->size;j++) {
                if (isAtomAromatic(ring->atomIdList[j])) continue;
                    isAromatic=false;
                    break;
            }
            ring->isAromatic=isAromatic;
        }
    }



    perceivePlaneBonds();


    //displayAtomList();
    //displayBondList();
}



void templateMol::displayBondList()
{
    cout << "\n\nBonds of the molecule:"<<endl;
    for (uint i=0;i<numBonds();i++) {
        cout << i+1 <<": "<<getBondbyIndex(i)->atomIndex0()+1<< "  ";
        cout <<getBondbyIndex(i)->atomIndex1()+1<<" ";
        cout << getBondbyIndex(i)->getBondOrder()<<endl;
    }
    cout << endl<< endl;
}

void templateMol::displayAtomList()
{
    double x,y,z;
    for (uint i=0;i<numAtoms();i++)
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




void templateMol::perceivePlaneBondbyId(uint id)
{
    if(getBondbyIndex(id)->getBondOrder()<2)
        return;

    uint idx0= getBondbyIndex(id)->atomIndex0();
    uint idx1= getBondbyIndex(id)->atomIndex1();

    //initialize
    vector3 v=getAtomPosbyIndex(idx1)-getAtomPosbyIndex(idx0);
    vector3 v1;
    v.createOrthoVector(v1);
    getBondbyIndex(id)->setPlane(v1);


    uint idx2=100001;

    HAtom * atom=getAtombyIndex(idx0);
    if (atom->numLinkdedAtoms()>1)
    for (uint i=0;i<atom->numLinkdedAtoms();i++)
    {
         if (atom->getLinkdedAtomId(i)==idx1)
            continue;
         if(getAtombyIndex(i)->atomicNum()<1)
             continue;
        idx2=atom->getLinkdedAtomId(i);

        if(getAtombyIndex(idx2)->atomicNum()<1)
            continue;
        break;
    }

    if (idx2>100000)
    {
        atom=getAtombyIndex(idx1);
        if (atom->numLinkdedAtoms()>1)
        for (uint i=0;i<atom->numLinkdedAtoms();i++)
        {
            if (atom->getLinkdedAtomId(i)==idx0)
                continue;
            idx2=atom->getLinkdedAtomId(i);

            if(getAtombyIndex(idx2)->atomicNum()<1)
                continue;
            break;
        }
    }

    if (idx2>100000) return;

    v=cross(getAtomPosbyIndex(idx1)-getAtomPosbyIndex(idx0),
       getAtomPosbyIndex(idx1)-getAtomPosbyIndex(idx2));

    //cout <<  " find ok "<<endl;
    getBondbyIndex(id)->setPlane(v.normalize());
}



void templateMol::perceivePlaneBonds()
{
    uint size=numBonds();

    if (size<1) return;

    for (uint i=0;i<size;i++) {
        uint ob=getBondbyIndex(i)->getBondOrder();
        if (ob==2 || ob==3 || ob==6)
            perceivePlaneBondbyId(i);
    }
}




void templateMol::buildOBMol()
{
    obmol.Clear();
    obmol.BeginModify();

    uint idx=0;
    foreach(HAtom * atom, atomList)
    {
        OpenBabel::OBAtom *a = obmol.NewAtom();

        a->Clear();
        a->SetVector(atom->x(), atom->y(), atom->z());
        a->SetAtomicNum(atom->atomicNum());
        a->SetType(atom->Symbol().toStdString().c_str());
        a->SetId(idx+1);
        idx++;
    }


    //copy bonds
    if (numBonds()>0) {
        QVector<HBond *>::iterator iter1;
        for (iter1=bondList.begin();iter1!=bondList.end();iter1++)
        {
            obmol.AddBond((*iter1)->atomIndex0() + 1,
                          (*iter1)->atomIndex1() + 1,
                          (*iter1)->getBondOrder());
        }
    }

    obmol.EndModify(true);
    //obmol.SetTotalSpinMultiplicity(1);
    //obmol.SetPartialChargesPerceived();
}

void templateMol::displayRingList()
{
    cout << "\nRings inside molecule:"<<endl;
    uint size=numRings();
    if (size<1) {
        cout<< "No ring existed!"<<endl;
        return;
    }


    for (uint k=0;k<size;k++)
    {
         cout << k+1<< " "<< ringList[k]->atomIdList.size() << ": ";
         for (uint l=0;l<ringList[k]->atomIdList.size();l++)
         {
             cout << ringList[k]->atomIdList[l]+1<< " ";
         }
         cout << endl;
    }
    cout << endl;
}


void templateMol::perceiveRings()
{
    if (numBonds() < 1)  return;
    buildOBMol();

    std::vector<OpenBabel::OBRing *> obrings;
    obmol.ConnectTheDots();
    obmol.PerceiveBondOrders();

    obmol.SetAromaticPerceived(true);
    obrings = obmol.GetSSSR();

    OpenBabel::OBAromaticTyper aromaticTyper;
    aromaticTyper.AssignAromaticFlags (obmol);


    OpenBabel::vector3  center, norm1, norm2;

    foreach(OpenBabel::OBRing *r, obrings)
    {
          uint nAtoms=r->Size();

          HRing * ring = new HRing();
          ring->atomIdList.clear();

          vector<int>::iterator j;
          for(j = r->_path.begin(); j != r->_path.end(); ++j)
              ring->atomIdList.push_back( (*j)-1);

          r->findCenterAndNormal(center,norm1,norm2);
          ring->center.Set(center.x(),center.y(),center.z());
          ring->norm.Set(norm1.x(),norm1.y(),norm1.z());

          uint size=r->Size();

          //radius
          double radius=50.0;
          for (uint i=0;i<size;i++)
          {
              double distToCenter=(getAtomPosbyIndex(ring->atomIdList[i])-ring->center).length();
              if(distToCenter < radius)
                  radius = distToCenter;
          }

          ring->size=ring->atomIdList.size();
          ring->radius=radius;
          ring->isAromatic=r->IsAromatic();
          ringList.push_back(ring);     
    }

    //displayRingList();


    //iterator bonds
    //cout << "There are "<< bondList.size() << " bonds"<<endl;
    if (bondList.size() > 0)
    for (uint i=0;i<bondList.size();i++)
    {
        uint id0=bondList[i]->atomIndex0();
        uint id1=bondList[i]->atomIndex1();
        bondList[i]->setRingId(100001);

        //cout <<   id0+1<<" "<< id1+1<< " "<< bondList[i]->getBondOrder()<<endl;

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
                     //cout << " " <<id0+1<<" "<< id1+1<< " is inside ring "<< k+1;
                     //cout << " with order of " << bondList[i]->getBondOrder()<< endl;
                     //cout <<   id0+1<<" "<< id1+1<< " with order of " << bondList[i]->getBondOrder();
                     //cout << " : find it inside "<< k+l<<endl;
                     break;
                 }
             }
             if(find1 && find2)   break;
        }
        //cout << " " <<id0+1<<" "<< id1+1<< " is inside ring ";
       //cout << bondList[i]->getRingId()+1<<endl;
    }


}





void templateMol::rotate(vector3 v, double angle)
{
    double  ele[3][3];

    double ang = angle*3.14159265359/180.0;

    double c = cos(ang), t=1-c,  s = sin(ang);


    vector3 vec = v;
    v.normalize();

    double   x = vec[0],
             y = vec[1],
             z = vec[2];

      ele[0][0] = x*x*t+c ;
      ele[0][1] = x*y*t-z*s;
      ele[0][2] = x*z*t+y*s;

      ele[1][0] = x*y*t+z*s;
      ele[1][1] = y*y*t+c;
      ele[1][2] = y*z*t-x*s;

      ele[2][0] = x*z*t-y*s;
      ele[2][1] = y*z*t+x*s;
      ele[2][2] = z*z*t+c;

      vector3  a,b;

      for (uint i=0;i<numAtoms();i++)
      {
          a=atomList[i]->Pos();
          b.SetX(  a.x()*ele[0][0] + a.y()*ele[0][1] + a.z()*ele[0][2] );
          b.SetY(  a.x()*ele[1][0] + a.y()*ele[1][1] + a.z()*ele[1][2] );
          b.SetZ(  a.x()*ele[2][0] + a.y()*ele[2][1] + a.z()*ele[2][2]);

          atomList[i]->setPos(b);
      }


}

void templateMol::rotate(matrix3x3 & m)
{
    vector3  a,b;

    for (uint i=0;i<numAtoms();i++)
    {
        a=atomList[i]->Pos();

        b.SetX(  a.x()*m(0,0)  + a.y()*m(0,1) + a.z()*m(0,2)  );
        b.SetY(  a.x()*m(1,0)  + a.y()*m(1,1) + a.z()*m(1,2)  );
        b.SetZ(  a.x()*m(2,0)  + a.y()*m(2,1) + a.z()*m(2,2) );

        atomList[i]->setPos(b);
    }
}


void templateMol::translate(vector3 v)
{
    for (uint i=0;i<numAtoms();i++)
        atomList[i]->setPos(atomList[i]->Pos()+v);

}




void templateMol::clearAll()
{
    clearAtoms();
    clearBonds();
    clearRings();
}

void templateMol::clearRings()
{
    if (ringList.size() > 0)
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


void templateMol::clearAtoms()
{
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



void templateMol::clearBonds()
{
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

   // for (auto iter=atomList.begin();iter!=atomList.end();iter++)
   //      (*iter)->clearLinkedAtomId();

}
