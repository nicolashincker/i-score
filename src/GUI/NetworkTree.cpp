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
abiding by the rules of distribution of free software.  You can  use,
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#include "NetworkTree.hpp"
#include "Maquette.hpp"
#include <QList>
#include <map>
#include <exception>

NetworkTree::NetworkTree(QWidget *parent) : QTreeWidget(parent)
{
    setColumnCount(2);
    QStringList list;
    list<<"Name";
    list<<"Value";
    //list<<"Assigned";
    setColumnWidth(0,250);
    setColumnWidth(1,140);
    setHeaderLabels(list);
    list.clear();
    //connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)),this,SLOT(itemCollapsed()));
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem *,int)),this,SLOT(clickInNetworkTree()));
    //connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem *)),this,SLOT(removeFromExpandedItemsList(QTreeWidgetItem *)));
    //connect(this, SIGNAL(itemExpanded(QTreeWidgetItem *)),this,SLOT(addToExpandedItemsList(QTreeWidgetItem*)));

}


/*

void NetworkTree::load() {
	vector<string> deviceNames;
	vector<bool> deviceRequestable;
	Maquette::getInstance()->getNetworkDeviceNames(deviceNames, deviceRequestable);

	vector<string>::iterator nameIt;
	vector<bool>::iterator requestableIt;

	QList<QTreeWidgetItem*> itemsList;

	for (nameIt = deviceNames.begin(), requestableIt = deviceRequestable.begin() ; nameIt != deviceNames.end(), requestableIt != deviceRequestable.end() ;	++nameIt,++requestableIt) {
		QStringList deviceName;
		deviceName << QString::fromStdString(*nameIt);
		QTreeWidgetItem *curItem = NULL;
		if (!(*requestableIt)) {
			curItem = new QTreeWidgetItem(deviceName,NodeNoNamespaceType);
            curItem->setBackground(0,QBrush(Qt::gray));
		}
		else {
			curItem = new QTreeWidgetItem(deviceName,NodeNamespaceType);
            curItem->setBackground(0,QBrush(Qt::darkCyan));
		}
		itemsList << curItem;
	}

	addTopLevelItems(itemsList);
}
*/


void NetworkTree::fathersPartialAssignation(QTreeWidgetItem *item){

    if(item->parent()!=NULL){
        if(!allBrothersSelected(item)){
            QTreeWidgetItem *father;
            father=item->parent();
            QFont font;
            font.setBold(true);
            father->setFont(0,font);
            father->setSelected(false);
            father->setCheckState(0,Qt::PartiallyChecked);
            father->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
            _nodesWithAssignedChildren<<father;
            fathersPartialAssignation(father);
        }
    }
}

void NetworkTree::fathersFullAssignation(QTreeWidgetItem *item){

    if(item->parent()!=NULL){
        if(allBrothersSelected(item)){
            QTreeWidgetItem *father;
            father=item->parent();
            QFont font;
            font.setBold(true);
            father->setFont(0,font);
            father->setSelected(true);
            father->setCheckState(0,Qt::Checked);
            father->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
            addAssignedItem(father);
            fathersFullAssignation(father);
        }
    }
}

void NetworkTree::assignItems(QList<QTreeWidgetItem*> selectedItems){

    resetSelectedItems();
    QList<QTreeWidgetItem *>::iterator it;
    QTreeWidgetItem *curItem;

    for (it =  selectedItems.begin() ; it !=  selectedItems.end() ; ++it){

        curItem = *it;
        curItem->setSelected(true);
        curItem->setCheckState(0,Qt::Checked);
        curItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);

        fathersPartialAssignation(curItem);
        fathersFullAssignation(curItem);
     }
}

void
NetworkTree::expandItems(QList<QTreeWidgetItem*> expandedItems){
    QList<QTreeWidgetItem *>::iterator it;
    QTreeWidgetItem *curItem;
    collapseAll();
    for (it =  expandedItems.begin() ; it !=  expandedItems.end() ; ++it){
        curItem = *it;
        expandItem(curItem);
        //curItem->setExpanded(true);
    }
}

bool NetworkTree::allBrothersSelected(QTreeWidgetItem *item, QList<QTreeWidgetItem *> assignedItems){

    QTreeWidgetItem *father;

    if(item->parent()!=NULL){
        father=item->parent();

        int childrenCount = father->childCount();
        for(int i=0 ; i<childrenCount ; i++){
            if(!assignedItems.contains(father->child(i))){
                return false;
            }
        }
        return true;
    }

    else
        return true;
}

bool NetworkTree::allBrothersSelected(QTreeWidgetItem *item){

    QTreeWidgetItem *father, *child;

    if(item->parent()!=NULL){
        father=item->parent();

        int childrenCount = father->childCount();
        for(int i=0 ; i<childrenCount ; i++){
            child=father->child(i);
            if(!child->isSelected()){
                return false;
            }
        }
        return true;
    }

    else
        return true;
}

bool
NetworkTree::noBrothersSelected(QTreeWidgetItem *item){

    QTreeWidgetItem *father, *child;
    int countSelectedItems = 0;
    if(item->parent()!=NULL){
        father=item->parent();
        int childrenCount = father->childCount();
        for(int i=0 ; i<childrenCount ; i++){
            child=father->child(i);
            if(child->isSelected())
                countSelectedItems++;
        }
    }
    if(countSelectedItems==0)
        return true;
    else
        return false;
}

void NetworkTree::resetSelectedItems(){

    QList<QTreeWidgetItem*> selection = assignedItems()+selectedItems();
    //QList<QTreeWidgetItem*> selection = selectedItems();
    //selection<<assignedItems();
    QList<QTreeWidgetItem *>::iterator it;
    QTreeWidgetItem *curItem;

    for (it =  selection.begin() ; it !=  selection.end() ; ++it){
        curItem = *it;
        curItem->setSelected(false);
        curItem->setCheckState(0,Qt::Unchecked);
    }

    for (it = _nodesWithAssignedChildren.begin() ; it != _nodesWithAssignedChildren.end() ; ++it){
        curItem = *it;
        QFont font;
        font.setBold(false);
        curItem->setFont(0,font);
        curItem->setSelected(false);
        curItem->setCheckState(0,Qt::Unchecked);
    }
}

QList<QTreeWidgetItem*> NetworkTree::getSelectedItems(){

    QList<QTreeWidgetItem*> items  = selectedItems(), allSelectedItems, *children = new QList<QTreeWidgetItem*>();
    QList<QTreeWidgetItem*>::iterator it;

    allSelectedItems << items;

    for(it=items.begin() ; it!=items.end() ; ++it){
        QTreeWidgetItem *curItem = *it;
        //treeRecursiveSelection(curItem,children);
        if (!children->empty())
            allSelectedItems << *children;
    }
    return allSelectedItems;
}

void NetworkTree::recursiveChildrenSelection(QTreeWidgetItem *curItem, bool select){

    int i;
    QTreeWidgetItem *child;
    if (!curItem->isDisabled()) {
        int childrenCount = curItem->childCount();
        for (i=0; i < childrenCount ; i++) {
            child=curItem->child(i);
            if (child->type()==NodeNamespaceType){
                child->setSelected(select);
                recursiveChildrenSelection(child,select);
            }
            if (child->type()==LeaveType){
                child->setSelected(select);
            }
        }
    }

}

/*
void NetworkTree::treeRecursiveSelection(QTreeWidgetItem *curItem, QList<QTreeWidgetItem*> *itemsList){

    int i;
    QTreeWidgetItem *child;
    if (!curItem->isDisabled()) {
        int childrenCount = curItem->childCount();
        for (i=0; i < childrenCount ; i++) {
            child=curItem->child(i);
            if (child->type()==NodeNamespaceType){
                itemsList->push_back(child);
                treeRecursiveSelection(child,itemsList);
            }
            if (child->type()==LeaveType){
                itemsList->push_back(child);
            }
        }
    }

}
*/

void NetworkTree::treeRecursiveExploration(QTreeWidgetItem *curItem){

    if (!curItem->isDisabled()) {

        vector<string> nodes,leaves,attributes,attributesValues;
        QString address = getAbsoluteAddress(curItem);
        Maquette::getInstance()->requestNetworkNamespace(address.toStdString(), nodes, leaves, attributes, attributesValues);
        vector<string>::iterator it;
        vector<string>::iterator it2;

        for (it = leaves.begin() ; it != leaves.end() ; ++it) {
            QStringList list;
            list << QString::fromStdString(*it);
            QTreeWidgetItem *childItem = new QTreeWidgetItem(list,LeaveType);
            //childItem->setBackground(0,QBrush(Qt::darkGreen));
            curItem->setCheckState(0,Qt::Unchecked);
            curItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
            curItem->addChild(childItem);
            list.clear();
            treeRecursiveExploration(childItem);
        }
        for (it = attributes.begin(),it2 = attributesValues.begin() ; it != attributes.end(), it2 != attributesValues.end() ; ++it , ++it2) {
            QStringList list;
            list << QString::fromStdString(*it + " : " + *it2);
            QTreeWidgetItem *childItem = new QTreeWidgetItem(list,AttributeType);
            //childItem->setBackground(0,QBrush(Qt::green));
            //curItem->setCheckState(0,Qt::Unchecked);
            curItem->addChild(childItem);
            list.clear();

            if(it2==attributesValues.begin()){
                QString leave_value = QString::fromStdString(*it2);
                QFont font;
                //font.setWeight(QFont::Light);
                font.setCapitalization(QFont::SmallCaps);
                curItem->setText(1,leave_value);
                curItem->setFont(1,font);
                //curItem->setData(0, 5, QString::fromStdString(*it2));
                curItem->setCheckState(0,Qt::Unchecked);
                curItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
                //list.clear();
            }
        }
        for (it = nodes.begin() ; it != nodes.end() ; ++it) {
            QStringList list;
            list << QString::fromStdString(*it);
            QTreeWidgetItem *childItem = new QTreeWidgetItem(list,NodeNamespaceType);
            //childItem->setBackground(0,QBrush(Qt::darkCyan));
            curItem->setCheckState(0,Qt::Unchecked);
            curItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
            curItem->addChild(childItem);
            list.clear();
            treeRecursiveExploration(childItem);
        }
    }
}

void NetworkTree::load() {
    vector<string> deviceNames;
    vector<bool> deviceRequestable;
    Maquette::getInstance()->getNetworkDeviceNames(deviceNames, deviceRequestable);
    vector<string>::iterator nameIt;
    vector<bool>::iterator requestableIt;

    QList<QTreeWidgetItem*> itemsList;

    for (nameIt = deviceNames.begin(), requestableIt = deviceRequestable.begin() ; nameIt != deviceNames.end(), requestableIt != deviceRequestable.end() ;	++nameIt,++requestableIt) {
        QStringList deviceName;
        deviceName << QString::fromStdString(*nameIt);
        QTreeWidgetItem *curItem = NULL;
        if (!(*requestableIt)) {
            curItem = new QTreeWidgetItem(deviceName,NodeNoNamespaceType);
            curItem->setCheckState(0,Qt::Unchecked);
            curItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
            //curItem->setBackground(0,QBrush(Qt::gray));
        }
        else {
            curItem = new QTreeWidgetItem(deviceName,NodeNamespaceType);
            curItem->setCheckState(0,Qt::Unchecked);
            curItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
            //curItem->setBackground(0,QBrush(Qt::darkCyan));

            try{
                treeRecursiveExploration(curItem);
            }catch (const std::exception & e){
                std::cerr << *nameIt << " : " << e.what();
            }
        }
        itemsList << curItem;
    }    
    addTopLevelItems(itemsList);
}


/*
 * Idée : Chercher l'item dans l'arbre à partir du nom du msg (ex : MinuitDevice1/groupe2/controle2 4294967318)
 *      On découpe le nom jusqu'à "espace"  de telle sorte que : vector<string> hierarchy = { MinuitDevice1 ; groupe2 ; controle2 }
 *      On fait findItems de controle2 (dernier de la liste),  puis on remonte hierarchy en vérifiant à chaque fois le père du itemFound
 *
 */

QList<QTreeWidgetItem *>
NetworkTree:: getItemsFromMsg(vector<string> itemsName)
{
    QList<QTreeWidgetItem *> itemsFound;
    QString curName;
    QStringList address;
    QList<QTreeWidgetItem *> itemsMatchedList;
    vector<string>::iterator it;

    if(!itemsName.empty()){
    //Boucle sur liste message
    for(it=itemsName.begin() ; it!=itemsName.end() ; ++it){
        curName = QString::fromStdString(*it);
        address = curName.split(" ");
        QStringList::Iterator it2;

        address = address.first().split("/");

        itemsFound = this->findItems(address.last(), Qt::MatchRecursive, 0);
        if(itemsFound.size()>1){
            QList<QTreeWidgetItem *>::iterator it3;
            QTreeWidgetItem *curIt;
            QTreeWidgetItem *father;
            bool found=false;
            for(it3=itemsFound.begin(); it3!=itemsFound.end(); ++it3){
                curIt = *it3;
                int i=address.size()-2;
                while(curIt->parent()!=NULL){

                    father=curIt->parent();
                    if(father->text(0)!=address.at(i)){
                        found=false;
                        break;
                    }
                    else{
                        found=true;
                        curIt=father;
                        i--;
                    }
                }
                if(found==true){
                    itemsMatchedList<<*it3;
                    break;
                }
            }
        }
    }
}
    return itemsMatchedList;
}

QString
NetworkTree::getAbsoluteAddress(QTreeWidgetItem *item) const {
	QString address;
	QTreeWidgetItem * curItem = item;
	while (curItem != NULL) {
		QString node;
		if (curItem->parent() != NULL) {
			node.append("/");
		}
		node.append(curItem->text(0));
		address.insert(0,node);
		curItem = curItem->parent();
	}
	return address;
}

vector<string> NetworkTree::snapshot() {
	QList<QTreeWidgetItem*> selection = selectedItems();  
	vector<string> snapshots;
	if (!selection.empty()) {
		QList<QTreeWidgetItem*>::iterator it;
		vector<string>::iterator it2;

		for (it = selection.begin(); it != selection.end() ; ++it) {
			QString address = getAbsoluteAddress(*it);
			if (!address.isEmpty()) {
				vector<string> snapshot = Maquette::getInstance()->requestNetworkSnapShot(address.toStdString());
				snapshots.insert(snapshots.end(),snapshot.begin(),snapshot.end());
			}
		}
	}

	return snapshots;
}

void
NetworkTree::expandNodes(QList<QTreeWidgetItem *> items){
    QList<QTreeWidgetItem *>::iterator it;
    QTreeWidgetItem *curIt, *father;
    for(it=items.begin() ; it<items.end() ; ++it){
        curIt=*it;
        while(curIt->parent()!=NULL){
            father=curIt->parent();
            if(father->type()!=NodeNoNamespaceType)
                father->setExpanded(true);
            curIt=father;
        }
    }
}

void
NetworkTree::recursiveFatherSelection(QTreeWidgetItem *item, bool select)
{

    if(item->parent()!=NULL){
        QTreeWidgetItem *father;
        father=item->parent();
        QFont font;

        if (select==true){
            font.setBold(true);
            father->setFont(0,font);

            if(!allBrothersSelected(item)){
                father->setSelected(false);
                recursiveFatherSelection(father,true);
                _nodesWithAssignedChildren<<father;
            }

            else{
                father->setSelected(true);
                recursiveFatherSelection(father,true);
            }
        }

        else{
            if (noBrothersSelected(item)){
                font.setBold(false);
                father->setFont(0,font);
                father->setSelected(false);
                //recursiveFatherSelection(father,false);
            }
            else{
                font.setBold(true);
                father->setFont(0,font);
                father->setSelected(false);
                _nodesWithAssignedChildren<<father;
                recursiveFatherSelection(father,false);
            }
        }
    }
}

void
NetworkTree::clickInNetworkTree(){

    QTreeWidgetItem *item = currentItem();

    if(item->isSelected()){
        recursiveChildrenSelection(item, true);
        recursiveFatherSelection(item,true);
    }

    if(!item->isSelected()){
        recursiveChildrenSelection(item, false);
        recursiveFatherSelection(item,false);
    }
}

void
NetworkTree::keyReleaseEvent(QKeyEvent *event){
    if ( event->key()==Qt::Key_Shift) {
        setSelectionMode(QAbstractItemView::MultiSelection);
    }
}

void
NetworkTree::keyPressEvent(QKeyEvent *event){
   if ( event->key()==Qt::Key_Shift) {
         setSelectionMode(QAbstractItemView::ContiguousSelection);
    }
}

/*
void
NetworkTree::itemCollapsed() {
	QTreeWidgetItem *selectedItem = currentItem();
	if (!selectedItem->isDisabled()) {
		if (selectedItem->type() == NodeNamespaceType || selectedItem->type() == LeaveType) {
			QString address = getAbsoluteAddress(selectedItem);

			vector<string> nodes,leaves,attributes,attributesValues;

			Maquette::getInstance()->requestNetworkNamespace(address.toStdString(), nodes, leaves, attributes, attributesValues);

			QList<QTreeWidgetItem *> selectedItemChildren = selectedItem->takeChildren();
			QList<QTreeWidgetItem *>::iterator childIt;
			for (childIt = selectedItemChildren.begin() ; childIt != selectedItemChildren.end() ; ++childIt) {
				delete *childIt;
			}
			vector<string>::iterator it;
            vector<string>::iterator it2;
			for (it = nodes.begin() ; it != nodes.end() ; ++it) {
				QStringList list;
				list << QString::fromStdString(*it);
				QTreeWidgetItem *childItem = new QTreeWidgetItem(list,NodeNamespaceType);
                //childItem->setBackground(0,QBrush(Qt::darkCyan));
				selectedItem->addChild(childItem);
				list.clear();                
			}
			for (it = leaves.begin() ; it != leaves.end() ; ++it) {
				QStringList list;
				list << QString::fromStdString(*it);
				QTreeWidgetItem *childItem = new QTreeWidgetItem(list,LeaveType);
                //childItem->setBackground(0,QBrush(Qt::darkGreen));
				selectedItem->addChild(childItem);
				list.clear();                
			}
			for (it = attributes.begin(),it2 = attributesValues.begin() ; it != attributes.end(), it2 != attributesValues.end() ; ++it , ++it2) {
				QStringList list;
				list << QString::fromStdString(*it + " : " + *it2);
				QTreeWidgetItem *childItem = new QTreeWidgetItem(list,AttributeType);
                //childItem->setBackground(0,QBrush(Qt::green));
				selectedItem->addChild(childItem);
				list.clear();
			}
		}
	}
}
*/
