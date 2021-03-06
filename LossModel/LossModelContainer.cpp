/* *****************************************************************************
Copyright (c) 2016-2017, The Regents of the University of California (Regents).
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS
PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

*************************************************************************** */

// Written: fmckenna, adamzs

#include "P58LossModelContainer.h"
#include "HazusLossModelContainer.h"
#include "LossMethod.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <sectiontitle.h>

#include <QVBoxLayout>
#include <QStackedWidget>
#include <QComboBox>

#include "LossModelContainer.h"

LossModelContainer::LossModelContainer(
    RandomVariablesContainer *theRV_IW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandVariableIW(theRV_IW)
{
    layout = new QVBoxLayout();

    QVBoxLayout *methodSelectLayoutV= new QVBoxLayout;

    // add combo with loss method selection at the top
    QHBoxLayout *methodSelectLayout = new QHBoxLayout();

    SectionTitle *textDL=new SectionTitle();
    textDL->setText(tr("Loss Assessment Method"));
    textDL->setMinimumWidth(250);
    QSpacerItem *spacer = new QSpacerItem(50,10);

    dlSelection = new QComboBox();
    dlSelection->setMaximumWidth(200);
    dlSelection->setMinimumWidth(200);

    dlSelection->addItem(tr("FEMA P58"));
    dlSelection->addItem(tr("HAZUS MH"));

    methodSelectLayout->addWidget(textDL);
    methodSelectLayout->addItem(spacer);
    methodSelectLayout->addWidget(dlSelection);
    methodSelectLayout->addStretch();
    methodSelectLayout->setSpacing(0);
    methodSelectLayout->setMargin(0);

    methodSelectLayoutV->addLayout(methodSelectLayout);

    methodSelectLayoutV->setSpacing(10);
    methodSelectLayoutV->setMargin(0);

    connect(dlSelection, SIGNAL(currentIndexChanged(QString)), this, SLOT(dlSelectionChanged(QString)));

    layout->addLayout(methodSelectLayoutV);
    layout->addStretch();

    lossMethod = new LossMethod();

    this->setLayout(layout);

    // set FEMA P58 as the default
    this->dlSelectionChanged(tr("FEMA P58"));
    layout->setMargin(0);
}

LossModelContainer::~LossModelContainer()
{

}

void LossModelContainer::clear(void)
{

}

bool
LossModelContainer::outputToJSON(QJsonObject &jsonObject)
{
    bool result = true;

    result = lossMethod -> outputToJSON(jsonObject);
    
    return true;
}

bool
LossModelContainer::inputFromJSON(QJsonObject &jsonObject)
{
    bool result = true;

    this->clear();

    if (jsonObject.contains("DLMethod")) {
        dlSelection->setCurrentText(jsonObject["DLMethod"].toString());
    } else {
        dlSelection->setCurrentText("FEMA P58");
    }

    result = lossMethod->inputFromJSON(jsonObject);

    return true;
}

bool
LossModelContainer::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["Application"] = "pelicun";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;
    return true;
}

bool
LossModelContainer::inputAppDataFromJSON(QJsonObject &jsonObject) {
    return true;
}

void LossModelContainer::dlSelectionChanged(const QString &arg1)
{
    selectionChangeOK = true;

    if (lossMethod != nullptr) {
        layout->removeWidget(lossMethod);
    }

    if (arg1 == QString("FEMA P58")) {
        delete lossMethod;
        lossMethod = new P58LossModelContainer();

    } else if (arg1 == QString("HAZUS MH")) {
        delete lossMethod;
        lossMethod = new HazusLossModelContainer();

    } else {
        selectionChangeOK = false;
        emit sendErrorMessage("ERROR: Loss Input - no valid Method provided .. keeping old");
    }

    if (lossMethod != 0) {

        this->dlWidgetChanged();
        layout->insertWidget(-1, lossMethod,1);
        connect(lossMethod,SIGNAL(sendErrorMessage(QString)),this,SLOT(errorMessage(QString)));
    }

    return;
}

bool
LossModelContainer::copyFiles(QString &dirName) {
    return true;
}

void
LossModelContainer::errorMessage(QString message){
    emit sendErrorMessage(message);
}

QString
LossModelContainer::getFragilityFolder(){
    return lossMethod->getFragilityFolder();
}

QString
LossModelContainer::getPopulationFile(){
    return lossMethod->getPopulationFile();
}
