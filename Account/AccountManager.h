#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <Lib.h>

class FeatherAccount;

class AccountManager : public QObject{
	Q_OBJECT
	public:
		enum State{
			Idle = 0,
			Logging = 1,
			Ready = 2,
		};

		AccountManager();

		int accountCount() const;
		int loginFailedAccountsCount() const;
		int loginSucceededAccountsCount() const;
		void startLogin();
		State state() const;
		void updateAccounts();

	private slots:
		void slotAccountBuddyAdded(FeatherAccount *, FeatherGroup, FeatherBuddy);
		void slotAccountInitiationFinished(FeatherAccount *, Feather::InitiationInformation);
		void slotAccountLoggedOut(FeatherAccount *);
		void slotAccountLoginFinished(FeatherAccount *, Feather::LoginInformation);
		void slotAccountMsgArrival(FeatherAccount *, FeatherBuddy, QString);
		void slotAccountSMSSendingResult(FeatherAccount *, int, Feather::SMSSendingState);
		void slotAccountStatusChanged(FeatherAccount *, QString);

	signals:
		void accountAdded(FeatherAccount *);
		void accountBuddyAdded(FeatherAccount *, FeatherGroup, FeatherBuddy);
		void accountLoginFailed(FeatherAccount *, QString);
		void accountMsgArrival(FeatherAccount *, FeatherBuddy, QString);
		void accountSMSSendingResult(FeatherAccount *, Feather::SMSSendingState);
		void accountStatusChanged(FeatherAccount *, QString);
		void stateChanged(AccountManager::State);

	private:
		QMap <QString, FeatherAccount *> _accounts;
		QList <FeatherAccount *> _initiationQueue;
		int _initiationQueueTimerId;
		int _loginFailedAccountsCount;
		QList <FeatherAccount *> _loginQueue;
		int _loginQueueTimerId;
		int _loginSucceededAccountsCount;
		State _state;

		void changeState(State state);
		void timerEvent(QTimerEvent *event);
};

#endif // ACCOUNTMANAGER_H
