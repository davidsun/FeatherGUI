#include <MsgList.h>
#include <FeatherAccount.h>

AbstractMsgList::MsgItem::MsgItem(QTreeWidgetItem *item) :
	_widgetItem(item){
		_widgetItemDestination = new QTreeWidgetItem(_widgetItem);
		_widgetItemDestination -> setText(0, "Destination(s)");
}

AbstractMsgList::MsgItem::~MsgItem(){
	delete _widgetItem;
}

void AbstractMsgList::MsgItem::addDestination(MsgDestination *destination){
	QTreeWidgetItem *item = new QTreeWidgetItem(_widgetItemDestination);
	item -> setText(0, destination -> account() -> username() + ": " + destination -> uri());
	_destinationItems[destination] = item;
}

void AbstractMsgList::MsgItem::removeDestination(MsgDestination *destination){
	QMap <MsgDestination *, QTreeWidgetItem *>::iterator it = _destinationItems.find(destination);
	if (it == _destinationItems.end()) return;
	delete it.value();
	_destinationItems.erase(it);
}

QTreeWidgetItem *AbstractMsgList::MsgItem::treeWidgetItem() const{
	return _widgetItem;
}
