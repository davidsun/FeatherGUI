#include <AccountList.h>
#include <AccountManager.h>
#include <FeatherAccount.h>

AccountList::AccountList(QTreeWidget *treeWidget, AccountManager *accountManager) :
	_treeWidget(treeWidget){
		connect(accountManager, SIGNAL(accountAdded(FeatherAccount *)), this,
				SLOT(slotAccountAdded(FeatherAccount *)));
		connect(accountManager, SIGNAL(accountBuddyAdded(FeatherAccount *, FeatherGroup, FeatherBuddy)), this,
				SLOT(slotAccountBuddyAdded(FeatherAccount *, FeatherGroup ,FeatherBuddy)));
		connect(accountManager, SIGNAL(accountLoginFailed(FeatherAccount *, QString)), this,
				SLOT(slotAccountLoginFailed(FeatherAccount *, QString)));
		connect(accountManager, SIGNAL(accountStatusChanged(FeatherAccount *, QString)), this,
				SLOT(slotAccountStatusChanged(FeatherAccount *, QString)));
		connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
				SLOT(slotWidgetItemClicked(QTreeWidgetItem *, int)));
}

AccountList::~AccountList(){
	for (QMap <QString, TagItem *>::iterator it = _tagItems.begin(); it != _tagItems.end(); it ++)
		delete it.value();
}

QList <QPair <const FeatherAccount *, FeatherBuddy> > AccountList::checkedItems() const{
	QList <QPair <const FeatherAccount *, FeatherBuddy> > ret;
	for (QMap <QString, TagItem *>::const_iterator it = _tagItems.begin(); it != _tagItems.end(); it ++)
		ret.append(it.value() -> checkedItems());
	return ret;
}

void AccountList::updateMsgCheckedDetail(Msg *msg) const{
	for (QMap <QString, TagItem *>::const_iterator it = _tagItems.begin(); it != _tagItems.end(); it ++)
		it.value() -> updateMsgCheckedDetail(msg);
}

void AccountList::updateMsgDetailByTag(const QString &tag, Msg *msg) const{
	QMap <QString, TagItem *>::const_iterator it = _tagItems.find(tag);
	if (it == _tagItems.end()) return;
	it.value() -> updateMsgDetail(msg);
}

void AccountList::slotAccountAdded(FeatherAccount *account){
	QMap <QString, TagItem *>::iterator it = _tagItems.find(account -> tag());
	if (it == _tagItems.end())
		_tagItems[account -> tag()] = new TagItem(this, account -> tag());
	_tagItems[account -> tag()] -> addAccount(account);
}

void AccountList::slotAccountBuddyAdded(FeatherAccount *account, FeatherGroup group, FeatherBuddy buddy){
	QMap <QString, TagItem *>::iterator it = _tagItems.find(account -> tag());
	if (it == _tagItems.end()) return;
	it.value() -> addBuddy(account, group, buddy);
}

void AccountList::slotAccountLoginFailed(FeatherAccount *account, QString s){
	Q_UNUSED(s);

	QMap <QString, TagItem *>::iterator it = _tagItems.find(account -> tag());
	if (it == _tagItems.end()) return;
	it.value() -> setAccountLoginFailed(account);
}

void AccountList::slotAccountStatusChanged(FeatherAccount *account, QString status){
	QMap <QString, TagItem *>::iterator it = _tagItems.find(account -> tag());
	if (it == _tagItems.end()) return;
	it.value() -> setAccountStatus(account, status);
}

void AccountList::slotWidgetItemClicked(QTreeWidgetItem *item, int id){
	Q_UNUSED(id);

	if (!item) return;
	updateCheckState(item -> parent());
	if (item -> checkState(0) != Qt::PartiallyChecked) setCheckState(item, item -> checkState(0));
}

void AccountList::updateCheckState(QTreeWidgetItem *item){
	if (!item) return;
	bool appChecked = 0, appUnchecked = 0;
	for (int i = 0; i < item -> childCount(); i ++){
		switch (item -> child(i) -> checkState(0)){
			case Qt::Checked:
				appChecked = 1;
				break;
			case Qt::Unchecked:
				appUnchecked = 1;
				break;
			case Qt::PartiallyChecked:
				appChecked = appUnchecked = 1;
				break;
		}
	}
	if (appChecked && appUnchecked) item -> setCheckState(0, Qt::PartiallyChecked);
	else if (appChecked) item -> setCheckState(0, Qt::Checked);
	else item -> setCheckState(0, Qt::Unchecked);

	updateCheckState(item -> parent());
}

void AccountList::setCheckState(QTreeWidgetItem *item, Qt::CheckState state){
	item -> setCheckState(0, state);
	for (int i = 0; i < item -> childCount(); i ++)
		setCheckState(item -> child(i), state);
}
