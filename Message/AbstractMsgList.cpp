#include <MsgList.h>
#include <MsgManager.h>
#include <FeatherAccount.h>

//public
AbstractMsgList::AbstractMsgList(QTreeWidget *treeWidget, MsgManager *msgManager, int acceptedMsgType) :
	QObject(0), _acceptedMsgType(acceptedMsgType), _msgManager(msgManager), _treeWidget(treeWidget){
		connect(msgManager, SIGNAL(msgAdded(Msg *)), this, SLOT(slotMsgAdded(Msg *)));
		connect(msgManager, SIGNAL(msgDestinationDestroyed(Msg *, MsgDestination *)),
				this, SLOT(slotMsgDestinationDestroyed(Msg *, MsgDestination *)));
		connect(msgManager, SIGNAL(msgDestroyed(Msg *)), this, SLOT(slotMsgDestroyed(Msg *)));
}

int AbstractMsgList::msgCount() const{
	return _msgItems.size();
}

QList <Msg *> AbstractMsgList::msgs() const{
	QList <Msg *> ret;
	for (QMap <Msg *, MsgItem *>::const_iterator it = _msgItems.begin(); it != _msgItems.end(); it ++)
		ret.push_back(it.key());
	return ret;
}


//private slots
void AbstractMsgList::slotMsgAdded(Msg *msg){
	if (!((_acceptedMsgType >> (int)(msg -> type())) & 1)) return;

	QTreeWidgetItem *item = new QTreeWidgetItem(_treeWidget);
	item -> setText(0, "Context: " + msg -> context());
	item -> setToolTip(0, msg -> context());

	MsgItem *msgItem = new MsgItem(item);
	_msgItems[msg] = msgItem;
	const QList <MsgDestination *> &dests = msg -> destionations();
	for (int i = 0; i < dests.size(); i ++) msgItem -> addDestination(dests[i]);

	msgAdded(item, msg);
}

void AbstractMsgList::slotMsgDestinationDestroyed(Msg *msg, MsgDestination *destination){
	if (!((_acceptedMsgType >> (int)(msg -> type())) & 1)) return;

	QMap <Msg *, MsgItem *>::iterator it = _msgItems.find(msg);
	if (it == _msgItems.end()) return;

	msgDestinationDestroyed(it.value() -> treeWidgetItem(), msg, destination);
	it.value() -> removeDestination(destination);
}

void AbstractMsgList::slotMsgDestroyed(Msg *msg){
	if (!((_acceptedMsgType >> (int)(msg -> type())) & 1)) return;

	QMap <Msg *, MsgItem *>::iterator it = _msgItems.find(msg);
	if (it == _msgItems.end()) return;
	delete it.value();
	_msgItems.erase(it);

	msgDestroyed(it.value() -> treeWidgetItem(), msg);

	if (_msgItems.empty()) emit msgCleared();
}
