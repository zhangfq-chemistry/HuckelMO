#include "displayTextForm.h"
#include <QClipboard>
#include <QApplication>
#include <QProcess>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>
#include "EHMO.h"
#include "HMO.h"

#include <cstdlib>
#include <stdio.h>

#include "mainwindow.h"
#include <QTextStream>

//#include <ginac/ginac.h>
//#include <ginac/matrix.h>


#include "eigenMath.h"

/*
QString ex2Qstring( GiNaC::ex expr)
{
    std::ostringstream ss;ss << expr;
    string s= ss.str();
    QString r=s.c_str();
    return r;
}
*/

HuckelTextForm::HuckelTextForm(QString title, QWidget *parent) :    QDialog(parent)
{
    ui.setupUi(this);

    if(title.size()<1)  setWindowTitle("Huckel-MO method");
    else   setWindowTitle(title);

    ui.textEdit->setLineWrapMode(QTextEdit::NoWrap);
    ui.textEdit->setCurrentFont(QFont("Courier", 20));
    ui.textEdit->setTextBackgroundColor(Qt::black);

    ui.textEdit->setStyleSheet("QTextEdit {background-color: #000; color: #ccc; font-family: monospace;}");

    charge=0;
    multipilicty=1;
    ui.spinBox_charge->setRange(-9,9);
    ui.spinBox_mulplicity->setValue(1);

    isHuckelCal=isXTBCal=isExtendedHuckelCal=false;
    runShellProcess=nullptr;
    //ehmo=nullptr;
    isTightBinding=false;
}


HuckelTextForm::~HuckelTextForm()
{
    if(runShellProcess)
        delete runShellProcess;
}


void HuckelTextForm::setParent (MainWindow * p)
{
    m_parent=p;
    mol=m_parent->getMol();
    view3d=m_parent->getView3D();
    ehmo=m_parent->getEHMO();
    hmo=m_parent->getHMO();
    initial();
}

void HuckelTextForm::initial()
{
    if(isHuckelCal)
        initialHuckel();

    if(isXTBCal)
        initialXTB();


    if(isExtendedHuckelCal)
        initialExtendedHuckel();
}



void HuckelTextForm::initialExtendedHuckel()
{
    ui.lineEdit_path->setDisabled(true);
    ui.lineEdit_commands->setDisabled(true);
    ui.spinBox_mulplicity->setDisabled(true);

    unsigned numAtoms=mol->NumAtoms();
    if (numAtoms<1) return;

    charge=ui.spinBox_charge->value();
    mol->setCharge(charge);

    setText("   Extend-Huckel Calculation\n\n");
    appendText("       Coorination of Atoms\n");
    appendText("------------------------------------\n");
    //display atom coordinates
    QString single="";
    char ss[256];
    for(unsigned i=0;i<numAtoms;i++)
    {
        std::sprintf (ss,"%s %10.6f  %10.6f  %10.6f",
                      mol->getAtomSymbol(i).toStdString().c_str(),
                      mol->getAtomXbyIndex(i),
                      mol->getAtomYbyIndex(i),
                      mol->getAtomZbyIndex(i));

        single= QString::fromStdString(ss);
        appendText(single);
        appendText("\n");
    }
    appendText("------------------------------------\n");

    ehmo->setMol(mol);
    ehmo->buildBasis();

    appendText("\n\n       Basis Sets of Atoms\n");
    appendText("------------------------------------\n");
    appendText("Atom Orbital   IP     Exp    Coeff\n");

    appendText(ehmo->displayBasisSets());
    appendText("------------------------------------\n");
}


void HuckelTextForm::runEHMO()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(ehmo!=nullptr)
         ehmo->runEHMO();
    QApplication::restoreOverrideCursor();

    //display information
    //vector < double > huckelEigValues;
    //vector < vector < double > > huckelEigVecs;

    QString single="\nOrbital Energy(au):\n";
    QString s;
    for (int i=0;i<ehmo->huckelEigValues.size();i++) {
         s.sprintf ("%3d   %10.4f\n", i+1,ehmo->huckelEigValues[i]);
         single+=s;
    }
    appendText(single+"\n");

    appendText("\nCalculated MO's coeff:\n");
    for (int i=0;i<ehmo->huckelEigVecs.size();i++) {
         single="";
         for (int j=0;j<ehmo->huckelEigVecs.size();j++) {
            s.sprintf ("%8.4f", ehmo->huckelEigVecs[i][j]);
            single+=s;
         }
         appendText(single+"\n");
    }

    appendText("\nCalculations fininshed!\n");
    m_parent->updateExtendedHuckelMOs();
}



void HuckelTextForm::initialHuckel ()
{
    ui.lineEdit_path->setDisabled(true);
    ui.lineEdit_commands->setDisabled(true);
    ui.spinBox_mulplicity->setDisabled(true);

    hmo->setMol(mol);
    charge=ui.spinBox_charge->value();
    hmo->setCharge(charge);
    hmo->buildBasis();

    unsigned int numCarbon=mol->huckelCarbonList.size();

    if (numCarbon<1) return;

    setText("休克尔行列式原子序号:\n");

    QString single="";
    for(unsigned i=0;i<numCarbon;i++){
        QString s;
        s.sprintf (" %3d", mol->huckelCarbonList[i]+1);
        single+=s;
        //if (i % 10)  single+="\n";
    }
    appendText(single+"\n");


    for(unsigned i=0;i<numCarbon;i++)
    {
        vector<QString> a (numCarbon,"0");
        a[i]="x";

        for(unsigned j=0;j<numCarbon;j++)
        {
            if(mol->getAtombyIndex(mol->huckelCarbonList[i])->isLinkded(mol->huckelCarbonList[j]))
                a[j]="1";
        }

        //display
        single="|";
        for(unsigned j=0;j<numCarbon;j++) {
            QString s;
            s.sprintf (" %3s", a[j].toStdString().c_str());
            single+=s;
        }
        appendText(single+"   |");
        appendText("\n");
    }

    QChar alpha=QChar(0xb1,0x03);
    QChar beta=QChar(0xb2,0x03);



    //QChar MathSymbolSquereRoot(0x221A);
    //QChar MathSymbolPi(0x03A0);
    //QChar MathSymbolDelta(0x0394);
    //QChar MathSymbolSigma(0x03A3);

    QString str="where x = (";
    str=str+alpha;
    str=str+"-E)/"+beta+"\n";
    appendText(str);


    //prepare eigenMath script
    std::string detFormula="huckel=(";
    for(unsigned i=0;i<numCarbon;i++)
    {
        detFormula += "(";
        for(unsigned j=0;j<numCarbon;j++)
        {
            if(j>0 && j<=numCarbon-1)
                detFormula += ",";

            if(i==j) {
                detFormula += "x";
                continue;
            }

            if(mol->getAtombyIndex(mol->huckelCarbonList[i])->isLinkded(mol->huckelCarbonList[j]))
                detFormula += "1";
            else
                detFormula += "0";
        }
        detFormula += ")";


        if(i<numCarbon-1) {
            detFormula += ",";
            detFormula += "\n";
        }
    }
    detFormula += ")";
    detFormula += "\n\n";

    detFormula += "det(huckel)";
    detFormula += "\n";

    detFormula += "A=det(huckel)";
    detFormula += "\n";

    detFormula += "factor(A,x)";
    detFormula += "\n";

    cout << detFormula<<endl;

    char *ss;
    int len = detFormula.length();
    ss = (char *)malloc((len+1)*sizeof(char));
    detFormula.copy(ss,len,0);

    //failed for fullerene C60
    if(numCarbon>30) return;


    //redirect stdout
    FILE *stream = freopen( "eigenMath.txt", "w", stdout );
    fflush( stdout );
    eigenMath::run(ss);
    fflush( stdout );    fclose( stream );

    QFile f("eigenMath.txt");

    if (f.open(QFile::ReadOnly | QFile::Text))
    {
        int i=4;
        while (i--)  {
            if(i==3)
                appendText("\n\n休克尔行列式展开:\n");
            if(i==1)
                appendText("\n\n休克尔多项式因式分解:\n");
            appendText(f.readLine());
        }
    }
    f.close();
    appendText("\n");
    appendText("\n请输入分子电荷，再运行(run)!\n");


    /*
    //following code is ok, however, ginac is difficult to compile under windows
    // GiNaC::matrix Huckel;
    GiNaC::symbol  x("x");
    GiNaC::matrix Huckel(numCarbon,numCarbon);

    for(unsigned i=0;i<numCarbon;i++)
    {
        for(unsigned j=0;j<numCarbon;j++)
        {
            Huckel(i,j)=0;
            if(i==j) Huckel(i,j)=1;

            if(mol->getAtombyIndex(mol->huckelCarbonList[i])->isLinkded(mol->huckelCarbonList[j]))
                Huckel(i,j)=x;
        }
    }


    //polynominal
    appendText("\n\nExpansion of Huckel determinant:\n");
    GiNaC::ex detHuckel=Huckel.determinant();
    QString huckelPoly=ex2Qstring(detHuckel);
    appendText("  "+huckelPoly+"=0\n");

    //factor one
    GiNaC::ex factor_Huckel=GiNaC::factor(detHuckel);
    QString factor_huckelPoly=ex2Qstring(factor_Huckel);
    appendText("  "+factor_huckelPoly+"=0\n");
    appendText("\nPlease set the charge of molecule and the run!\n");

    */
}




void HuckelTextForm::on_pushButton_Close_clicked()
{
    close();
}



void HuckelTextForm::appendText(QString s)
{
   s.replace("\n","<br>");
   ui.textEdit->moveCursor(QTextCursor::End);
   ui.textEdit->textCursor().insertHtml("<pre>" + s + "</pre>");
   ui.textEdit->moveCursor(QTextCursor::End);
}



void HuckelTextForm::clear()
{
   ui.textEdit->clear();
}




void HuckelTextForm::setText(QString s)
{
   QString x(s);
   x.replace("\n","<br>");
   ui.textEdit->setText(x);
}



void HuckelTextForm::on_pushButton_run_clicked()
{
    if(isHuckelCal) {
        runHuckel();
        return;
    }

    if(isXTBCal)
        runXTB();

    if(isExtendedHuckelCal)
        runEHMO();
}





void HuckelTextForm::initialXTB ()
{
    setText("  Perform XTB calculation:\n");
    executePath=m_parent->getExecutePath()+"/tmp";
    appendText("Using "+executePath+" as scratch directory\n");
    QDir dir(executePath);
    if(dir.exists())
        QProcess::execute("rm " + executePath + "/* -rf" );
    else
        QProcess::execute("mkdir " + executePath);


    ui.lineEdit_path->setText(executePath);
    uint numAtoms=mol->NumAtoms();
    QString fileName=executePath+"/test.xyz";

    QFileInfo fileinfo(fileName);
    if (fileinfo.exists()!=false)
        QProcess::execute("rm " + fileName + " -rf");


    commandXTB="xtb " + fileName + " -opt   " ;//+ executePath+"/test.log " ;
    ui.lineEdit_commands->setText(commandXTB);

    /*
    //prepare shell script
    QString shell;
    QString shellScript=executePath+"/runXTB";
    QFile file(shellScript);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;

    QString line="cd "+executePath+ "\n";
    file.write(line.toUtf8());
    file.write(commandXTB.toUtf8());
    file.write("\n");
    file.close();
    QString s="chmod +x " + shellScript;
    QProcess::execute(s);
    cout  << s.toStdString().c_str()<<endl;

*/

    //prepare xtb xyz
    QFile file1(fileName);
    if(!file1.open(QIODevice::ReadWrite | QIODevice::Text))
        return;

    QString str2;
    str2.sprintf("%3d\n", numAtoms);
    file1.write(str2.toUtf8());
    file1.write("run xtb by zhangfq\n");


    double x,y,z;

    appendText("       Coordinates of molecule:\n");
    appendText("----------------------------------------\n");
    for (uint i=0;i<numAtoms;i++)
    {
        QString asymbol=mol->getAtomSymbol(i);

        if (asymbol=="X"|| asymbol=="Bq")       continue;

        x=mol->getAtomXbyIndex(i);
        y=mol->getAtomYbyIndex(i);
        z=mol->getAtomZbyIndex(i);

        str2.sprintf("%3s %10.5f  %10.5f  %10.5f\n", asymbol.toStdString().c_str(), x,y,z);
        appendText(str2);
        file1.write(str2.toUtf8());
    }
    file1.write("\n\n");
    file1.close();

    appendText("----------------------------------------\n");
    appendText("\n");
}



void HuckelTextForm::runXTB()
{
    if (runShellProcess==nullptr)
        runShellProcess=new QProcess(this);

    if(runShellProcess) {
        appendText("New process ok!\n");
    }

    //cout << commandXTB.toStdString().c_str()<<endl;

    runShellProcess->start(commandXTB);

    log="";
    connect(runShellProcess, SIGNAL(readyRead()), this, SLOT(readProcess()));
    connect(runShellProcess, SIGNAL(finished(int)), this, SLOT(finishedProcess()));

    /*
    //one time for all
    runShellProcess->waitForFinished();
    QByteArray qByteRead = runShellProcess->readAllStandardOutput();
    appendText(QString::fromLocal8Bit(qByteRead));

QProcess *process = new QProcess;
connect(process, SIGNAL(finished(int)), this, SLOT(doNextCommand()));


void ShowTextForm::doNextCommand()
{
    ipRank++;
    if (ipRank < 4)
    {
        process->start(Command[ipRank]);
    }
    else
    {
        process->close();
    }
}
    */
}



void HuckelTextForm::readProcess()
{
    log = QString::fromLocal8Bit(runShellProcess->readAll());
    appendText(log);
}


void HuckelTextForm::finishedProcess()
{
    if (runShellProcess) {
        disconnect(runShellProcess, SIGNAL(readyRead()), this, SLOT(readProcess()));
        disconnect(runShellProcess, SIGNAL(finished(int)), this, SLOT(finishedProcess()));
    }

    //mol->loadFile(m_parent->getExecutePath()+"/xtbopt.xyz");
    QString c=" cat "+m_parent->getExecutePath()+"/xtbopt.xyz";
    runShellProcess->start(c);
    runShellProcess->waitForFinished();
    QByteArray qByteRead = runShellProcess->readAllStandardOutput();
    appendText("\n   Optimzied Coordinate\n");
    QString result=QString::fromLocal8Bit(qByteRead);
    appendText(result);


    //refresh molecule data
    mol->loadxyzfromQString(result);
    //mol->centeralize();
    view3d->renderMol(mol);
}



void HuckelTextForm::runHuckel()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    hmo->runHuckel();
    QApplication::restoreOverrideCursor();


    QChar  alpha=QChar(0xb1,0x03);
    QChar  beta=QChar(0xb2,0x03);

    appendText("\nSolutions of this polynominals: ");
    QString str=" where x = (";
    str=str+alpha;
    str=str+"-E)/"+beta+"\n";
    appendText(str);

    QString single="x=";
    for (uint i=0;i<hmo->huckelEigValues.size();i++) {
        QString s;
        s.sprintf (" %10.4f",hmo->huckelEigValues[i]);
        single+=s;
    }
    appendText(single+"\n");


    uint size=hmo->huckelEigValues.size();
    for (uint i=0;i<hmo->huckelEigValues.size();i++)
    {
        single="E"+QString::number(size)+" =";
        QString s;
        if (fabs(hmo->huckelEigValues[i])>0.0001){
            s.sprintf ("+%6.4f",hmo->huckelEigValues[i]);
            single += alpha+s+beta;
        }
        else
            single += alpha;

        single.remove(QRegExp("\\s"));
        appendText(single+"\n");
        size--;
    }

    //https://www.utf8-chartable.de/unicode-utf8-table.pl?start=768
    QString  Psi= QString::fromUtf8("\u03A8");
    QString  phi= QString::fromUtf8("\u03C6");


    appendText("\n");
    QChar pair=QChar(0x21F5);
    QChar spinUp=QChar(0x2191);
    QString str_;
    int numElectrons;
    size=hmo->huckelEigValues.size();

    for (uint i=0;i<hmo->huckelEigVecs.size();i++)
    {
        double d=hmo->huckelEigValues[i];
        double dn=hmo->huckelEigValues[i+1];
        double dp=0.0;

        numElectrons=hmo->numElectrons()-2*(hmo->huckelEigVecs.size()-1-i);

        if(abs(d-dn)<0.01 && numElectrons==0 )
            numElectrons=1;
        if(i>0 ){
            dp=hmo->huckelEigValues[i-1];
            if(abs(d-dp)<0.01 && numElectrons==2)
            numElectrons=1;
        }
        if(numElectrons<0)
            numElectrons=0;

        str_="(";
        if(numElectrons==0) {
            str_ += "-";
        }
        if(numElectrons>1) {
            str_ += pair;
        }
        if(numElectrons==1) {
            str_ += spinUp;
        }
        str_+=")";

        single =str_;
        single +=Psi+QString::number(size)+"=";
        for (uint j=0;j<hmo->huckelEigVecs[i].size();j++) {
            QString s;
            if(hmo->huckelEigVecs[i][j]>-0.00000001)
                s.sprintf (" %2s%6.4f","+", hmo->huckelEigVecs[i][j]);
            else
                s.sprintf (" %2s%6.4f"," ",hmo->huckelEigVecs[i][j]);
            single+=s+phi+QString::number(hmo->huckelCarbonList[j]+1);
        }
        single.remove(QRegExp("\\s"));
        appendText(single+"\n");
        size--;    }

    appendText("\nCalculations fininshed!\n");
    m_parent->updateHuckelMOs();

    // output += QString::fromLocal8Bit(pProces->readAll());
}


void HuckelTextForm::on_spinBox_charge_valueChanged(int arg1)
{
    charge=arg1;
}


void HuckelTextForm::on_spinBox_mulplicity_valueChanged(int arg1)
{
    multipilicty=arg1;
}


void HuckelTextForm::on_lineEdit_path_textChanged(const QString &arg1)
{
    executePath=ui.lineEdit_path->text();
}

