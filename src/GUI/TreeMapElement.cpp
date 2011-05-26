/*
Copyright: LaBRI / SCRIME

Authors: Luc Vercellin (08/03/2010)

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

#include <iostream>
#include <QGridLayout>
#include "TreeMapElement.hpp"

using std::string;
using std::map;

TreeMapElement::TreeMapElement(QWidget *parent) : QWidget(parent) {
	//_layout = new QBoxLayout;
	_parent = NULL;
	_descendanceCount = 0;
	connect(this,SIGNAL(oneMoreChild()),this,SLOT(increaseDescendance()));
}

void TreeMapElement::setAttributes(TreeMapElement *parentElt, const string &device, const string &message, ElementType type) {
	_parent = parentElt;
	_device = device;
	_message = message;
	_type = type;
	if (_parent != NULL) {
		std::cerr << "TreeMapElement::setAttributes : Parent found for : " << _message << std::endl;
		_parent->addChild(this);
		connect(this,SIGNAL(oneMoreChild()),_parent,SIGNAL(familyExpanded()));
		connect(this,SIGNAL(familyExpanded()),_parent,SIGNAL(familyExpanded()));
	}
	else {
		std::cerr << "TreeMapElement::setAttributes : No Parent found for : " << _message << std::endl;
	}
}

TreeMapElement* TreeMapElement::findChild(const string &message) {
	map<string,TreeMapElement*>::iterator it;
	if ((it = _children.find(message)) != _children.end()) {
		return it->second;
	}
	else {
		return NULL;
	}
}

void TreeMapElement::addChild(TreeMapElement *child) {
	_children[child->message()] = child;
	std::cerr << "TreeMapElement::addChild : [ " << _message << " : " << child->message() << std::endl;
	emit(oneMoreChild());
}

void TreeMapElement::setParent(TreeMapElement *parent) {
	_parent = parent;
}

void TreeMapElement::increaseDescendance() {
	std::cerr << "TreeMapElement::increaseDescendance for : " << _message << std::endl;
	_descendanceCount++;
}
