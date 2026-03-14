#ifndef RINGTEMPLATE_H
#define RINGTEMPLATE_H

#include <QDialog>

class MainWindow;

namespace Ui {
class RingTemplate;
}

class RingTemplate : public QDialog
{
    Q_OBJECT

public:
    explicit RingTemplate(QWidget *parent = nullptr);
    ~RingTemplate();


    void  setTemplateName ();
    void  setParent (MainWindow * p) {m_parent=p;}


private slots:

    void on_Frag2_clicked();
    void on_Frag3_clicked();
    void on_Frag4_clicked();
    void on_Frag5_clicked();
    void on_Frag6_clicked();
    void on_Frag7_clicked();
    void on_Frag8_clicked();
    void on_Frag9_clicked();
    void on_Frag10_clicked();
    void on_Frag11_clicked();
    void on_Frag12_clicked();

    void on_Frag13_clicked();
    void on_Frag14_clicked();
    void on_Frag15_clicked();
    void on_Frag16_clicked();
    void on_Frag17_clicked();
    void on_Frag18_clicked();
    void on_Frag60_clicked();


private:
    Ui::RingTemplate *ui;
    MainWindow * m_parent;
    QString name;
};

#endif // RINGTEMPLATE_H
