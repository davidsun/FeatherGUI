#include <AccountList.h>
#include <Msg.h>
#include <BuddyInformation.h>

AccountList::TagItem::AccountItem::GroupItem::GroupItem(AccountItem *parent, const FeatherGroup &group) :
	_id(group.id()), _name(group.name()),
	_parent(parent){
		_item = new QTreeWidgetItem(_parent -> _item);
		_item -> setText(0, _name);

		if (_parent -> _item -> checkState(0) != Qt::PartiallyChecked) _item -> setCheckState(0, _parent -> _item -> checkState(0));
		else _item -> setCheckState(0, Qt::Unchecked);
}

AccountList::TagItem::AccountItem::GroupItem::~GroupItem(){
	delete _item;
}

void AccountList::TagItem::AccountItem::GroupItem::addBuddy(const FeatherBuddy &buddy){
	QTreeWidgetItem *item = new QTreeWidgetItem(_item);
	if (buddy.nickname().size()) item -> setText(0, buddy.nickname() + "(" + buddy.uri() + ")");
	else item -> setText(0, buddy.uri());
	_buddies.push_back(qMakePair(buddy, item));

	if (_item -> checkState(0) != Qt::PartiallyChecked) item -> setCheckState(0, _item -> checkState(0));
	else item -> setCheckState(0, Qt::Unchecked);
}

AccountList::TagItem::AccountItem *AccountList::TagItem::AccountItem::GroupItem::parent() const{
	return _parent;
}

QList <FeatherBuddy> AccountList::TagItem::AccountItem::GroupItem::checkedItems() const{
	QList <FeatherBuddy> ret;
	for (int i = 0; i < _buddies.size(); i ++)
		if (_buddies[i].second -> checkState(0) == Qt::Checked) ret.push_back(_buddies[i].first);
	return ret;
}

void AccountList::TagItem::AccountItem::GroupItem::updateMsgCheckedDetail(Msg *msg) const{
	for (int i = 0; i < _buddies.size(); i ++){
		if (_buddies[i].second -> checkState(0) == Qt::Checked) msg -> addDestination(
				new MsgDestination(msg, _parent -> _account, QString::number(_buddies[i].first.id())));
	}
}

void AccountList::TagItem::AccountItem::GroupItem::updateMsgDetail(Msg *msg) const{
	for (int i = 0; i < _buddies.size(); i ++)
		msg -> addDestination(new MsgDestination(msg, _parent -> _account, QString::number(_buddies[i].first.id())));
}
