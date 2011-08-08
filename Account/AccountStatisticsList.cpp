#include <AccountStatisticsList.h>
#include <AccountManager.h>
#include <FeatherAccount.h>

AccountStatisticsList::AccountStatisticsList(QTreeWidget *treeWidget, AccountManager *accountManager) :
	_treeWidget(treeWidget){
		connect(accountManager, SIGNAL(accountAdded(FeatherAccount *)), this, SLOT(slotAccountAdded(FeatherAccount *)));
		connect(accountManager, SIGNAL(accountStatusChanged(FeatherAccount *, QString)), this,
				SLOT(slotAccountStatusChanged(FeatherAccount *, QString)));
		connect(accountManager, SIGNAL(accountSMSSendingResult(FeatherAccount *, Feather::SMSSendingState)), this,
				SLOT(slotAccountSMSSendingResult(FeatherAccount *, Feather::SMSSendingState)));
}

AccountStatisticsList::~AccountStatisticsList(){
	for (QMap <QString, TagItem *>::iterator it = _tagItems.begin(); it != _tagItems.end(); it ++)
		delete it.value();
}

void AccountStatisticsList::slotAccountAdded(FeatherAccount *account){
	QMap <QString, TagItem *>::iterator it = _tagItems.find(account -> tag());
	if (it == _tagItems.end())
		_tagItems[account -> tag()] = new TagItem(this, account -> tag());
	_tagItems[account -> tag()] -> addAccount(account);
}

void AccountStatisticsList::slotAccountSMSSendingResult(FeatherAccount *account, Feather::SMSSendingState state){
	QMap <QString, TagItem *>::iterator it = _tagItems.find(account -> tag());
	if (it == _tagItems.end()) return;
	it.value() -> addSMSSendingResult(account, state);
}

void AccountStatisticsList::slotAccountStatusChanged(FeatherAccount *account, QString status){
	QMap <QString, TagItem *>::iterator it = _tagItems.find(account -> tag());
	if (it == _tagItems.end()) return;
	it.value() -> setAccountStatus(account, status);
}
