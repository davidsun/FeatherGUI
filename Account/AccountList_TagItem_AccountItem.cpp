#include <AccountList.h>
#include <FeatherAccount.h>

AccountList::TagItem::AccountItem::AccountItem(TagItem *parent, FeatherAccount *account) :
	_account(account), _parent(parent){
		_item = new QTreeWidgetItem(_parent -> _item);
		_item -> setText(0, _account -> username());

		if (_parent -> _item -> checkState(0) != Qt::PartiallyChecked) _item -> setCheckState(0, _parent -> _item -> checkState(0));
		else _item -> setCheckState(0, Qt::Unchecked);
}

AccountList::TagItem::AccountItem::~AccountItem(){
	for (QMap <int, GroupItem *>::iterator it = _groupItems.begin(); it != _groupItems.end(); it ++)
		delete it.value();
	delete _item;
}

const FeatherAccount *AccountList::TagItem::AccountItem::account() const{
	return _account;
}

void AccountList::TagItem::AccountItem::addBuddy(FeatherGroup group, FeatherBuddy buddy){
	QMap <int, GroupItem *>::iterator it = _groupItems.find(group.id());
	if (it == _groupItems.end())
		_groupItems[group.id()] = new GroupItem(this, group);
	_groupItems[group.id()] -> addBuddy(buddy);
}

QList <FeatherBuddy> AccountList::TagItem::AccountItem::checkedItems() const{
	QList <FeatherBuddy> ret;
	for (QMap <int, GroupItem *>::const_iterator it = _groupItems.begin(); it != _groupItems.end(); it ++)
		ret.append(it.value() -> checkedItems());
	return ret;
}

AccountList::TagItem *AccountList::TagItem::AccountItem::parent() const{
	return _parent;
}

void AccountList::TagItem::AccountItem::setLoginFailed(){
	_item -> setForeground(0, Qt::red);
}

void AccountList::TagItem::AccountItem::setStatus(const QString &status){
	_item -> setText(0, _account -> username() + " (" + status + ")");
}

void AccountList::TagItem::AccountItem::updateMsgCheckedDetail(Msg *msg) const{
	for (QMap <int, GroupItem *>::const_iterator it = _groupItems.begin(); it != _groupItems.end(); it ++)
		it.value() -> updateMsgCheckedDetail(msg);
}

void AccountList::TagItem::AccountItem::updateMsgDetail(Msg *msg) const{
	for (QMap <int, GroupItem *>::const_iterator it = _groupItems.begin(); it != _groupItems.end(); it ++)
		it.value() -> updateMsgDetail(msg);
}
