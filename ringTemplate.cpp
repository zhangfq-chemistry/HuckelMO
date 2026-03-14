#include "ringTemplate.h"
#include "ui_ringTemplate.h"
#include "mainwindow.h"


RingTemplate::RingTemplate(QWidget *parent) :  QDialog(parent), ui(new Ui::RingTemplate)
{
    ui->setupUi(this);
}

RingTemplate::~RingTemplate()
{
    delete ui;
}


void RingTemplate::setTemplateName ()
{
    m_parent->setTemplateName(name);
    close();
}



void RingTemplate::on_Frag2_clicked()
{
    name="vinyl";

    setTemplateName ();
}

void RingTemplate::on_Frag3_clicked()
{
    name="ring3";
    setTemplateName ();
}

void RingTemplate::on_Frag4_clicked()
{
    name="ring4";
    setTemplateName ();

}

void RingTemplate::on_Frag5_clicked()
{
    name="ring5";
    setTemplateName ();
}

void RingTemplate::on_Frag6_clicked()
{
    name="phenyl";
    setTemplateName ();
}




void RingTemplate::on_Frag7_clicked()
{
    name="ring7";
    setTemplateName ();
}

void RingTemplate::on_Frag8_clicked()
{
    name="ring8";
    setTemplateName ();
}

void RingTemplate::on_Frag9_clicked()
{
    name="two5mr";
    setTemplateName ();
}

void RingTemplate::on_Frag10_clicked()
{
    name="asixfive";
    setTemplateName ();
}


void RingTemplate::on_Frag11_clicked()
{
    name="naphth";
    setTemplateName ();
}

void RingTemplate::on_Frag12_clicked()
{
    name="azu";
    setTemplateName ();
}

void RingTemplate::on_Frag13_clicked()
{
    name="656";
    setTemplateName ();
}

void RingTemplate::on_Frag14_clicked()
{
    name="rings3";
    setTemplateName ();
}

void RingTemplate::on_Frag15_clicked()
{
    name="anthr";
    setTemplateName ();
}

void RingTemplate::on_Frag16_clicked()
{
    name="rings4";
    setTemplateName ();
}

void RingTemplate::on_Frag17_clicked()
{
    name="coronene";
    setTemplateName ();
}

void RingTemplate::on_Frag18_clicked()
{
    name="porph";
    setTemplateName ();
}


void RingTemplate::on_Frag60_clicked()
{
    name="bucky";
    setTemplateName ();
}

