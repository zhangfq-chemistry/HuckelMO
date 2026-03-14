#ifndef SHOWTEXTFORM_H
#define SHOWTEXTFORM_H

#include <QDialog>
#include <QProcess>
#include "ui_showTextForm.h"

//#include <ginac/ginac.h>


class MainWindow;
class View3D;
class HMol;
class EHMO;
class HMO;

namespace Ui {
class huckelTextForm;
}

class HuckelTextForm : public QDialog
{
    Q_OBJECT

public:
    explicit HuckelTextForm(QString title="", QWidget *parent = nullptr);
    ~HuckelTextForm() ;

    void setEHMO(EHMO * e) {ehmo=e;}
    void clear();

    void initial();
    void initialHuckel();
    void initialXTB();
    void initialExtendedHuckel();

    void setParent(MainWindow *);

    void setText(QString s);
    void appendText(QString s);

    void setCalExtendedHuckel() {isExtendedHuckelCal=true;isXTBCal=false;isHuckelCal=false;}
    void setCalHuckel() {isHuckelCal=true;isXTBCal=false;isExtendedHuckelCal=false;}
    void setCalXTB () {isXTBCal=true;isExtendedHuckelCal=false;isHuckelCal=false;}

    void  runHuckel();
    void  runXTB();
    void  runEHMO();

private slots:
   void on_pushButton_Close_clicked();


   void on_pushButton_run_clicked();
   void readProcess();
   void finishedProcess();


   void on_spinBox_charge_valueChanged(int arg1);

   void on_spinBox_mulplicity_valueChanged(int arg1);

   void on_lineEdit_path_textChanged(const QString &arg1);

private:
    EHMO * ehmo;
    HMO * hmo;
    Ui::ShowTextForm ui;
    MainWindow * m_parent;
    View3D * view3d;
    HMol * mol;
    int charge,multipilicty;
    bool isHuckelCal;
    bool isXTBCal;
    bool isExtendedHuckelCal;
    QProcess * runShellProcess;
    QString   log,commandXTB,executePath;
    bool isTightBinding;


    //GiNaC::matrix Huckel;
   // GiNaC::ex detHuckel,detHuckel1;
};

#endif // SHOWTEXTFORM_H
