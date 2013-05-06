/*
 *  Netfonds History Downloader plugin
 *
 *  Copyright (C) 2013 Mattias Johansson
 *
 */

#include <QtGui>
#include "GUIWidget.h"
#include "Nordnet.h"

Nordnet::Nordnet() : m_pModel(0){}

Nordnet::~Nordnet(){
    qWarning() << "Model deleted!";
    delete m_pModel;
}

Widget* Nordnet::create()
{
    if(!m_pModel){
        m_pModel = new NordnetModel();
    }
    return new GUIWidget(m_pModel);
}

Entity* Nordnet::querySettings()
{
    return new Entity();
}


// do not remove
Q_EXPORT_PLUGIN2(nordnet, Nordnet)
