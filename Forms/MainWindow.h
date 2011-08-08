#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_MainWindow.h>
#include <FeatherAccount.h>
#include <AccountManager.h>
#include <QtGui>

class AccountList;
class AccountManager;
class AccountStatisticsList;
class MsgManager;
class InstantSMSList;
class ScheduledSMSList;

class MainWindow : public QMainWindow{
	Q_OBJECT
	public:
		MainWindow();
		~MainWindow();

	public slots:

	private slots:
		void slotAccountLoginFailed(FeatherAccount *, QString);
		void slotAccountManagerStateChanged(AccountManager::State);
		void slotAccountMsgArrival(FeatherAccount *, FeatherBuddy, QString);
		void slotAccountStatusChanged(FeatherAccount *, QString);
		void slotActionTriggered(QAction *);
		void slotButtonAddForbiddanceClicked();
		void slotButtonAddMessageClicked();
		void slotButtonDeleteMessageClicked();
        void slotButtonImportMessagesClicked();
		void slotButtonLoginClicked();
		void slotButtonRemoveForbiddanceClicked();
		void slotButtonSendMessageClicked();
		void slotInstantSMSListMessageCleared();
		void slotTextMessageChanged();

	signals:

	private:
		AccountList *_accountList;
		AccountManager *_accountManager;
		AccountStatisticsList *_accountStatisticsList;
		InstantSMSList *_instantSMSList;
		MsgManager *_msgManager;
		ScheduledSMSList *_scheduledSMSList;
		Ui::MainWindow _ui;
};

#endif
