/*

Copyright: LaBRI / SCRIME

Authors: Luc Vercellin and Bruno Valeze (08/03/2010)

luc.vercellin@labri.fr

This software is a computer program whose purpose is to provide
notation/composition combining synthesized as well as recorded
sounds, providing answers to the problem of notation and, drawing,
from its very design, on benefits from state of the art research
in musicology and sound/music computing.

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software. You can use,
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty and the software's author, the holder of the
economic rights, and the successive licensors have only limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading, using, modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean that it is complicated to manipulate, and that also
therefore means that it is reserved for developers and experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and, more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#include <iostream>
#include <map>
#include <vector>
using std::map;
using std::vector;
using std::string;

#include <QPainter>
#include <QTabWidget>
#include <QGridLayout>
#include <QMouseEvent>
#include <QString>
#include <QWidget>
#include <QGraphicsEffect>
#include <QGraphicsOpacityEffect>


#include "AbstractCurve.hpp"
#include "BasicBox.hpp"
#include "CurveWidget.hpp"
#include "BoxWidget.hpp"
#include "Interpolation.hpp"
#include "Maquette.hpp"

#define COMBOBOX_WIDTH 200

BoxWidget::BoxWidget(QWidget *parent)
: QWidget(parent) {

//    setAttribute(Qt::WA_TranslucentBackground,true);

//    update();

//    _comboBox = new QComboBox(this);
//    _curveWidget = new CurveWidget(NULL);
//    _tabWidget = new QTabWidget(this);
//    _tabWidget->lower();
//    _tabWidget->setAttribute(Qt::WA_TranslucentBackground,true);

//    _curvePageLayout = new QGridLayout;

//    _curvePageLayout->addWidget(_tabWidget);

//    setLayout(_curvePageLayout);

//    _parentWidget = parent;
//    _curveWidgetList = new QList<CurveWidget *>;

//    connect(_comboBox,SIGNAL(currentIndexChanged(const QString&)),this, SLOT(displayCurve(const QString&)));
//    connect(_comboBox,SIGNAL(activated(const QString&)),this, SLOT(displayCurve(const QString&)));

    setAttribute(Qt::WA_TranslucentBackground,true);

    update();

    _comboBox = new QComboBox;
    _curveWidget = new CurveWidget(NULL);
    _tabWidget = new QTabWidget;
    _tabWidget->lower();
    _tabWidget->setAttribute(Qt::WA_TranslucentBackground,true);
    _stackedWidget = new QStackedWidget;
    _curvePageLayout = new QGridLayout;
    _comboBox->setMaximumWidth(COMBOBOX_WIDTH);

    _curvePageLayout->addWidget(_comboBox);
    _curvePageLayout->addWidget(_stackedWidget);
    setLayout(_curvePageLayout);

    _parentWidget = parent;
    _curveWidgetList = new QList<CurveWidget *>;

    connect(_comboBox,SIGNAL(currentIndexChanged(const QString&)),this, SLOT(displayCurve(const QString&)));
    connect(_comboBox,SIGNAL(activated(const QString&)),this, SLOT(displayCurve(const QString&)));
}

BoxWidget::~BoxWidget(){

}

void BoxWidget::curveActivationChanged(const QString &address, bool state) {
    std::cout<<"BoxWidget::CurveActivation Changed"<<std::endl;
    if (_boxID != NO_ID) {
        Maquette::getInstance()->setCurveMuteState(_boxID,address.toStdString(),!state);
        updateCurve(address.toStdString(),true);
    }
}

void BoxWidget::curveRedundancyChanged(const QString &address,bool state) {
    if (_boxID != NO_ID) {
        Maquette::getInstance()->setCurveRedundancy(_boxID,address.toStdString(),state);
        updateCurve(address.toStdString(),true);
    }
}

void BoxWidget::curveShowChanged(const QString &address, bool state) {
    if (_boxID != NO_ID) {
        AbstractCurve * curve = Maquette::getInstance()->getBox(_boxID)->getCurve(address.toStdString());
        if (curve != NULL) {
            curve->_show = state;
        }
        updateCurve(address.toStdString(),false);
    }
}

void BoxWidget::curveSampleRateChanged(const QString &address,int value) {
    if (_boxID != NO_ID) {
        Maquette::getInstance()->setCurveSampleRate(_boxID,address.toStdString(),value);
        updateCurve(address.toStdString(),true);
    }
}

void BoxWidget::displayCurve(const QString &address){
    std::string add = address.toStdString();
    map<string,CurveWidget *>::iterator curveIt;
    CurveWidget *curveWidget;

    curveIt = _curveMap.find(add);
    bool curveFound = (curveIt != _curveMap.end());

    if (curveFound) {
        curveWidget = curveIt->second;
        _stackedWidget->setCurrentWidget(curveWidget);
//        _curvePageLayout->addWidget(curveWidget);

    }
}

        //SET OPACITY
//        for(curveIt2 = _curveMap.begin() ; curveIt2 != _curveMap.end() ; curveIt2++){
//            curveWidget = curveIt2->second;

//            if(curveIt2 != curveIt){
//                curveWidget->setGraphicsEffect(effect);
//                curveWidget->setEnabled(false);
////                newLayout->addWidget(curveWidget);
//                _curvePageLayout->addWidget(curveWidget);

//            }

//            else{
//                curveWidget->setGraphicsEffect(noEffect);
//                curveWidget->setEnabled(true);
//                _curvePageLayout->addWidget(curveWidget);
////                newLayout->addWidget(curveWidget);
//                _curveWidget = curveWidget;
//            }
//        }
//        _curvePageLayout = newLayout;
//        setLayout(newLayout);
//    }
//}

bool
BoxWidget::contains(const string &address) {
    map<string,unsigned int>::iterator curveIt = _curveIndexes.find(address);
    if (curveIt != _curveIndexes.end()) {
        return true;
    }
    return false;
}

void
BoxWidget::updateCurve(const string &address) {
    updateCurve(address,true);
}

void
BoxWidget::removeCurve(const string &address) {

    map<string,CurveWidget *>::iterator curveIt = _curveMap.find(address);
    QString curveAddress = QString::fromStdString(address);
    int index;

    if (curveIt != _curveMap.end()) {

        _curveMap.erase(curveIt);
        index = _comboBox->findText(curveAddress);
        if(index>-1)
            _comboBox->removeItem(index);
        BasicBox *curveBox = Maquette::getInstance()->getBox(_boxID);
        if (curveBox != NULL) {
            curveBox->removeCurve(address);
        }
    }
}

void
BoxWidget::updateMessages(unsigned int boxID, bool forceUpdate) {

//    _tabWidget->clear();
    _curvePageLayout->removeWidget(_curveWidget);
    _comboBox->clear();
    _curveMap.clear();
    _curveIndexes.clear();
    _curveWidgetList->clear();
//    _interpolation->clear();
    _boxID = boxID;

    if (boxID != NO_ID) {

        vector<string> curvesAddresses = Maquette::getInstance()->getCurvesAddresses(boxID);
        vector<string>::const_iterator curveAddressIt;

        for (curveAddressIt = curvesAddresses.begin() ; curveAddressIt != curvesAddresses.end() ; ++curveAddressIt) {
            updateCurve(*curveAddressIt,forceUpdate);
        }
    }
}

void
BoxWidget::addCurve(QString address, CurveWidget *curveWidget){

    _curveMap[address.toStdString()]=curveWidget;
    addToComboBox(address);
    _stackedWidget->addWidget(curveWidget);
}

void
BoxWidget::addToComboBox(const QString address){
    if(_comboBox->findText(address,Qt::MatchExactly) == -1)
        _comboBox->addItem(address);
}

bool
BoxWidget::updateCurve(const string &address, bool forceUpdate)
{
    std::cout<<"BoxWidget::UPDATECURVE"<<std::endl;
    BasicBox *box = Maquette::getInstance()->getBox(_boxID);
    if (box != NULL) // Box Found
    {
        AbstractCurve *abCurve = box->getCurve(address);
        map<string,CurveWidget *>::iterator curveIt2 = _curveMap.find(address);
        bool curveFound = (curveIt2 != _curveMap.end());
        unsigned int curveTabIndex = 0;
        CurveWidget *curveTab = NULL;
        if (curveFound) {
            curveTab = curveIt2->second;
        }

        static const bool FORCE_SHOW = true;
        static const bool FORCE_HIDE = false;

        unsigned int sampleRate;
        bool redundancy,interpolate;
        vector<float> values,xPercents,yValues,coeff;
        vector<string> argTypes;
        vector<short> sectionType;

        if (abCurve != NULL) // Abstract Curve found
        {
            std::cout<<"CP1 -> ";
            if (abCurve->_show) // Abstract curve showing
            {
                std::cout<<"abCurve->_show"<<std::endl;
                if (curveFound) // Curve tab existing
                {
                     std::cout<<"curveFound"<<std::endl;
                    if (forceUpdate) // Force updating through engines
                    {
                        std::cout<<"forceUpdate"<<std::endl;
                        bool getCurveSuccess = Maquette::getInstance()->getCurveAttributes(_boxID,address,0,sampleRate,redundancy,interpolate,values,argTypes,xPercents,yValues,sectionType,coeff);
                        if (getCurveSuccess) {
                            curveTab->setAttributes(_boxID,address,0,values,sampleRate,redundancy,FORCE_SHOW,interpolate,argTypes,xPercents,yValues,sectionType,coeff);
                            box->setCurve(address,curveTab->abstractCurve());
                        }
                    }
                    else // No forcing : updating through abstract curve
                    {
                        std::cout<<"!forceUpdate"<<std::endl;
                        curveTab->setAttributes(abCurve);
                    }
                }
                else // Curve tab not existing
                {
                    std::cout<<"!curveFound"<<std::endl;
                    if (forceUpdate) // Force creating through engines
                    {
                        std::cout<<"force update"<<std::endl;
                        bool getCurveSuccess = Maquette::getInstance()->getCurveAttributes(_boxID,address,0,sampleRate,redundancy,interpolate,values,argTypes,xPercents,yValues,sectionType,coeff);
                        if (getCurveSuccess) {
                            // Create and set
                            curveTab = new CurveWidget(NULL);
                            _curvePageLayout->removeWidget(_curveWidget);
                            curveTab->setAttributes(_boxID,address,0,values,sampleRate,redundancy,FORCE_SHOW,interpolate,argTypes,xPercents,yValues,sectionType,coeff);
                            QString curveAddressStr = QString::fromStdString(address);

                            // Add tab and store
                            addCurve(curveAddressStr,curveTab);

                            _curveMap[address] = curveTab;
                            displayCurve(_comboBox->currentText());

                            // Set box curve
                            box->setCurve(address,curveTab->abstractCurve());
                        }
                    }

                    else // No forcing : create through abstract curve
                    {
                        std::cout<<"!force update"<<std::endl;
                        // Create and set
                        curveTab = new CurveWidget(NULL);
                        curveTab->setAttributes(abCurve);
                        QString curveAddressStr = QString::fromStdString(address);

                        // Add tab and store
                        addCurve(curveAddressStr,curveTab);

                        _curveMap[address] = curveTab;

                        displayCurve(_comboBox->currentText());
                    }
                }
            }
            else // Abstract curve hiding
            {
                std::cout<<"!abst->show"<<std::endl;

                //NICO

                if (curveFound) // Curve tab existing
                {
                    std::cout<<"curveFound"<<std::endl;

                    if (forceUpdate) // Force updating through engines
                    {
                        std::cout<<"force update"<<std::endl;
                        bool getCurveSuccess = Maquette::getInstance()->getCurveAttributes(_boxID,address,0,sampleRate,redundancy,interpolate,values,argTypes,xPercents,yValues,sectionType,coeff);
                        if (getCurveSuccess) {
                            if (values.front() != values.back()) {
                                curveTab->setAttributes(_boxID,address,0,values,sampleRate,redundancy,FORCE_SHOW,interpolate,argTypes,xPercents,yValues,sectionType,coeff);
                            }
                            else{
                            // Set and assign new abstract curve to box
                                std::cout<<"BoxWidget getCurveSuccess"<<std::endl;
                                curveTab->setAttributes(_boxID,address,0,values,sampleRate,redundancy,FORCE_HIDE,interpolate,argTypes,xPercents,yValues,sectionType,coeff);
                            }
                        box->setCurve(address,curveTab->abstractCurve());
                        }

                     //NICO
                    }

                    else // No forcing : updating through abstract curve
                    {
                        std::cout<<"Noforcing"<<std::endl;
                    }

                    // Remove curve tab anyway
                    removeCurve(address);
                    _comboBox->setCurrentIndex(curveTabIndex);
                    displayCurve(_comboBox->currentText());
                    delete curveTab;
                }
                else // Curve tab not existing
                {
                    std::cout<<"!curveFound"<<std::endl;
                    if (forceUpdate) // Force updating through engines
                    {
                        std::cout<<"forceUpdate"<<std::endl;
                        bool getCurveSuccess = Maquette::getInstance()->getCurveAttributes(_boxID,address,0,sampleRate,redundancy,interpolate,values,argTypes,xPercents,yValues,sectionType,coeff);
                        if (getCurveSuccess) {
                            std::cout<<"getCurveSuccess"<<std::endl;
                            // Create, set and assign new abstract curve to box
                            curveTab = new CurveWidget(NULL);
                            curveTab->setAttributes(_boxID,address,0,values,sampleRate,redundancy,FORCE_HIDE,interpolate,argTypes,xPercents,yValues,sectionType,coeff);
                            box->setCurve(address,curveTab->abstractCurve());
                            delete curveTab;
                        }
                    }
                    else // No forcing : updating through abstract curve
                    {
                        std::cout<<"!forceUpdate"<<std::endl;
                    }
                }
            }
        }
        else // Abstract Curve not found
        {
            std::cout<<"CP2 -> ";
            // Get attributes and determine if shown
            bool show = true;
            interpolate = true;

            bool getCurveSuccess = Maquette::getInstance()->getCurveAttributes(_boxID,address,0,sampleRate,redundancy,interpolate,values,argTypes,xPercents,yValues,sectionType,coeff);
            if (getCurveSuccess) {
                if (xPercents.empty() && yValues.empty() && values.size() >= 2) {
                    if (values.front() == values.back()) {
                        show = false;
                        interpolate = false;
                    }
                }
                if (show) // Curve showing
                {
                    std::cout<<"curve show"<<std::endl;
                    if (!curveFound) // Curve tab not existing
                    {
                        std::cout<<"!curveFound"<<std::endl;
                        // Creating curve tab from engines anyway (no abstract curve)
                        // Create and set
                        curveTab = new CurveWidget(NULL);
                        QString curveAddressStr = QString::fromStdString(address);

                        // Add tab and store
                        addCurve(curveAddressStr,curveTab);
                        std::cout<<"WE WANT TO DISPLAY " <<_comboBox->currentText().toStdString()<<std::endl;
                        displayCurve(_comboBox->currentText());
                    }
                    else // Curve tab existing
                    {
                        // Updating curve tab from engines anyway (no abstract curve)
                        std::cout<<"curveFound"<<std::endl;
                    }

                    curveTab->setAttributes(_boxID,address,0,values,sampleRate,redundancy,FORCE_SHOW,interpolate,argTypes,xPercents,yValues,sectionType,coeff);
                    // Set box curve
                    box->setCurve(address,curveTab->abstractCurve());
                }
                else // Curve hiding
                {
                    std::cout<<"SHOW==FALSE"<<std::endl;
                    if (curveFound) // Curve tab existing
                    {
                        std::cout<<"curveFound"<<std::endl;
                        // Creating curve tab from engines anyway (no abstract curve)
                        // Set and assign new abstract curve to box
                        curveTab->setAttributes(_boxID,address,0,values,sampleRate,redundancy,FORCE_HIDE,interpolate,argTypes,xPercents,yValues,sectionType,coeff);
                        box->setCurve(address,curveTab->abstractCurve());

                        // Remove curve tab
                        removeCurve(address);
                    }

                    else // Curve tab not existing
                    {
                        std::cout<<"!curveFound"<<std::endl;
                        // Creating curve tab from engines anyway (no abstract curve)
                        curveTab = new CurveWidget(NULL);
                        curveTab->setAttributes(_boxID,address,0,values,sampleRate,redundancy,FORCE_HIDE,interpolate,argTypes,xPercents,yValues,sectionType,coeff);
                        box->setCurve(address,curveTab->abstractCurve());
                        delete curveTab;
                    }
                }
            }
        }
    }
    else // Box Not Found
    {
        std::cout<<"BoxWidget curve==NULL"<<std::endl;
        return false;
    }
    std::cout<<std::endl;
    return false;
}

void
BoxWidget::displayCurveWindow(CurveWidget *curve){
    //TODO
}

void
BoxWidget::mouseDoubleClickEvent(QMouseEvent *event){
    //TODO
}

