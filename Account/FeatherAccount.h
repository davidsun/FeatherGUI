#ifndef FEATHERACCOUNT_H
#define FEATHERACCOUNT_H

#include <QtGui>
#include <QtNetwork>
#include <Lib.h>

class Msg;

class FeatherAccount : public QObject{
	Q_OBJECT
	public:
		FeatherAccount(const QString &username, const QString &password, const QString &tag = QString(),
					   const QNetworkProxy &proxy = QNetworkProxy(QNetworkProxy::NoProxy));
		~FeatherAccount();

		void initiate();
		Feather::InitiationState initiationState() const;
		void login();
		Feather::LoginState loginState() const;
		QString password() const;
		int sendSMS(const QString &uri, const QString &context);
		QString status() const;
		QString tag() const;
		QString username() const;

	private slots:
		void slotBuddyAdded(FeatherGroup, FeatherBuddy);
		void slotInitiationEvent(Feather::InitiationInformation);
		void slotLoginEvent(Feather::LoginInformation);
		void slotMsgArrival(FeatherMessage);
		void slotNewCertification(int, QImage);
		void slotSMSSendingResult(int, Feather::SMSSendingState);

	signals:
		void buddyAdded(FeatherAccount *, FeatherGroup, FeatherBuddy);
		void certificationInfo(int, QString);
		void initiationFinished(FeatherAccount *, Feather::InitiationInformation);
		void loggedOut(FeatherAccount *);
		void loginFinished(FeatherAccount *, Feather::LoginInformation);
		void msgArrival(FeatherAccount *, FeatherBuddy, QString);
		void SMSSendingResult(FeatherAccount *, int, Feather::SMSSendingState);
		void statusChanged(FeatherAccount *, QString);

	private:
		QMap <int, QList <FeatherBuddy> > _buddies;
		Feather _feather;
		QString _password;
		QString _status;
		QString _tag;
		QString _username;

		void changeStatus(const QString &status);
};

#endif 
