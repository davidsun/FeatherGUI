#include <AccountList.h>

AccountList::TagItem::TagItem(AccountList *parent, const QString &tag) :
	_parent(parent), _tag(tag){
		_item = new QTreeWidgetItem(_parent -> _treeWidget);
		_item -> setText(0, tag);
		_item -> setCheckState(0, Qt::Unchecked);
}

AccountList::TagItem::~TagItem(){
	for (QMap <FeatherAccount *, AccountItem *>::iterator it = _accountItems.begin(); it != _accountItems.end(); it ++)
		delete it.value();
	delete _item;
}

void AccountList::TagItem::addAccount(FeatherAccount *account){
	AccountItem *item = new AccountItem(this, account);
	_accountItems[account] = item;
}

void AccountList::TagItem::addBuddy(FeatherAccount *account, FeatherGroup group, FeatherBuddy buddy){
	QMap <FeatherAccount *, AccountItem *>::iterator it = _accountItems.find(account);
	if (it != _accountItems.end()) it.value() -> addBuddy(group, buddy);
}

QList <QPair <const FeatherAccount *, FeatherBuddy> > AccountList::TagItem::checkedItems() const{
	QList <QPair <const FeatherAccount *, FeatherBuddy> > ret;
	for (QMap <FeatherAccount *, AccountItem *>::const_iterator it = _accountItems.begin(); it != _accountItems.end(); it ++){
		QList <FeatherBuddy> buddies = it.value() -> checkedItems();
		for (int i = 0; i < buddies.size(); i ++)
			ret.push_back(qMakePair(it.value() -> account(), buddies[i]));
	}
	return ret;
}

AccountList *AccountList::TagItem::parent() const{
	return _parent;
}

void AccountList::TagItem::setAccountLoginFailed(FeatherAccount *account){
	QMap <FeatherAccount *, AccountItem *>::iterator it = _accountItems.find(account);
	if (it != _accountItems.end()) it.value() -> setLoginFailed();
}

void AccountList::TagItem::setAccountStatus(FeatherAccount *account, const QString &status){
	QMap <FeatherAccount *, AccountItem *>::iterator it = _accountItems.find(account);
	if (it != _accountItems.end()) it.value() -> setStatus(status);
}

QString AccountList::TagItem::tag() const{
	return _tag;
}

void AccountList::TagItem::updateMsgCheckedDetail(Msg *msg) const{
	for (QMap <FeatherAccount *, AccountItem *>::const_iterator it = _accountItems.begin(); it != _accountItems.end(); it ++)
		it.value() -> updateMsgCheckedDetail(msg);
}

void AccountList::TagItem::updateMsgDetail(Msg *msg) const{
	for (QMap <FeatherAccount *, AccountItem *>::const_iterator it = _accountItems.begin(); it != _accountItems.end(); it ++)
		it.value() -> updateMsgDetail(msg);
}
