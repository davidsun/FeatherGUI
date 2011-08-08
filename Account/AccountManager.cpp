#include <AccountManager.h>
#include <QApplication>
#include <QDir>
#include <FeatherAccount.h>

AccountManager::AccountManager() :
	QObject(0),
	_loginFailedAccountsCount(0), _loginSucceededAccountsCount(0),
	_state(Idle){
		_initiationQueueTimerId = startTimer(200);
		_loginQueueTimerId = startTimer(200);
}

int AccountManager::accountCount() const{
	return _accounts.size();
}

int AccountManager::loginFailedAccountsCount() const{
	return _loginFailedAccountsCount;
}

int AccountManager::loginSucceededAccountsCount() const{
	return _loginSucceededAccountsCount;
}

void AccountManager::startLogin(){
	if (_state != Idle) return;

	changeState(Logging);
	for (QMap <QString, FeatherAccount *>::iterator it = _accounts.begin(); it != _accounts.end(); it ++)
		_initiationQueue.push_back(it.value());
}

AccountManager::State AccountManager::state() const{
	return _state;
}

void AccountManager::updateAccounts(){
	QDir dir(QApplication::applicationDirPath());
	if (dir.exists("Accounts")){
		dir.cd("Accounts");
		QFileInfoList list = dir.entryInfoList();
		for (int i = 0; i < list.size(); i ++){
			const QFileInfo &file = list[i];
			if (file.isFile() && file.suffix() == "txt"){
				FILE *fin = fopen(file.absoluteFilePath().toLocal8Bit().data(), "r");
				if (!fin) continue;
				char username[200], password[200];
				while (fscanf(fin, "%s%s", username, password) == 2){
					if (_accounts.find(username) == _accounts.end()){
						FeatherAccount *account = new FeatherAccount(
								username, password, file.completeBaseName(),
								QNetworkProxy(QNetworkProxy::HttpProxy, "edu.zzzcn.info", 2012));
						_accounts[username] = account;

						connect(account, SIGNAL(buddyAdded(FeatherAccount *, FeatherGroup, FeatherBuddy)), this,
								SLOT(slotAccountBuddyAdded(FeatherAccount *, FeatherGroup, FeatherBuddy)));
						connect(account, SIGNAL(initiationFinished(FeatherAccount *, Feather::InitiationInformation)), this,
								SLOT(slotAccountInitiationFinished(FeatherAccount *, Feather::InitiationInformation)));
						connect(account, SIGNAL(loggedOut(FeatherAccount *)), this,
								SLOT(slotAccountLoggedOut(FeatherAccount *)));
						connect(account, SIGNAL(loginFinished(FeatherAccount *, Feather::LoginInformation)), this,
								SLOT(slotAccountLoginFinished(FeatherAccount *, Feather::LoginInformation)));
						connect(account, SIGNAL(msgArrival(FeatherAccount *, FeatherBuddy, QString)), this,
								SLOT(slotAccountMsgArrival(FeatherAccount *, FeatherBuddy, QString)));
						connect(account, SIGNAL(SMSSendingResult(FeatherAccount *, int, Feather::SMSSendingState)), this,
								SLOT(slotAccountSMSSendingResult(FeatherAccount *, int, Feather::SMSSendingState)));
						connect(account, SIGNAL(statusChanged(FeatherAccount *, QString)), this,
								SLOT(slotAccountStatusChanged(FeatherAccount *, QString)));

						emit accountAdded(account);
					}
				}
				fclose(fin);
			}
		}
	}
}

void AccountManager::slotAccountBuddyAdded(FeatherAccount *account, FeatherGroup group, FeatherBuddy buddy){
	emit accountBuddyAdded(account, group, buddy);
}

void AccountManager::slotAccountInitiationFinished(FeatherAccount *account, Feather::InitiationInformation info){
	if (info == Feather::InitiationOk){
		_loginQueue.push_back(account);
	}  else {
		_initiationQueue.push_back(account);
	}
}

void AccountManager::slotAccountLoggedOut(FeatherAccount *account){
	_loginSucceededAccountsCount --;
	changeState(Logging);
	_loginQueue.push_back(account);
}

void AccountManager::slotAccountLoginFinished(FeatherAccount *account, Feather::LoginInformation info){
	if (info == Feather::LoginOk){
		_loginSucceededAccountsCount ++;
	}  else if (info == Feather::LoginTimeout){
		_loginQueue.push_back(account);
	}  else {
		_loginFailedAccountsCount ++;
		if (info == Feather::LoginPasswordError){
			emit accountLoginFailed(account, "Password Error");
		}  else if (info == Feather::LoginRequestFailure){
			emit accountLoginFailed(account, "Request Failure");
		}  else if (info == Feather::SSILoginFailed){
			emit accountLoginFailed(account, "SSI Login Failed");
		}
	}

	if (_loginSucceededAccountsCount + _loginFailedAccountsCount == _accounts.size()) changeState(Ready);
}

void AccountManager::slotAccountMsgArrival(FeatherAccount *account, FeatherBuddy buddy, QString context){
	emit accountMsgArrival(account, buddy, context);
}

void AccountManager::slotAccountSMSSendingResult(FeatherAccount *account, int id, Feather::SMSSendingState state){
	Q_UNUSED(id);

	emit accountSMSSendingResult(account, state);
}

void AccountManager::slotAccountStatusChanged(FeatherAccount *account, QString status){
	emit accountStatusChanged(account, status);
}

void AccountManager::changeState(State state){
	_state = state;
	emit stateChanged(state);
}

void AccountManager::timerEvent(QTimerEvent *event){
	if (event -> timerId() == _initiationQueueTimerId){
		if (_initiationQueue.size()){
			FeatherAccount *account = _initiationQueue.first();
			_initiationQueue.pop_front();
			account -> initiate();
		}
	}  else if (event -> timerId() == _loginQueueTimerId){
		if (_loginQueue.size()){
			FeatherAccount *account = _loginQueue.first();
			_loginQueue.pop_front();
			account -> login();
		}
	}
}
