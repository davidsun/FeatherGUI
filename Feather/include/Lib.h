#ifndef LIB_H
#define LIB_H

#include <QtCore>
#include <QtNetwork>
#include <Settings.h>
#include <Curl.h>
#include <FeatherMessage.h>
#include <SIPCPackage.h>
#include <BuddyStructures.h>
#include <BuddyInformation.h>
#include <PictureCertification.h>
#include <HttpConnector.h>
#include <QHttp>

class Feather : public QObject{
	Q_OBJECT
	public:
		enum InitiationInformation{
			StartInitiation = 0,
			InitiationOk = 1,
			InitiationTimeout = 2,
			InitiationFailed = 3,
		};

		enum LoginInformation{
			SSILogging = 0,
			TCPConnecting = 1,
			SIPCLogging = 2,
			
			SSILoginSucceeded = 32,
			TCPConnected = 33,
			LoginOk = 34,
			
			SSILoginFailed = 64,
			LoginPasswordError = 65,
			LoginRequestFailure = 66,
			LoginTimeout = 67,

			LoggedOut = 96,
		};
		
		enum InitiationState{
			NotInitiated = 0,
			Initiating = 1,
			Initiated = 2,
		};
		
		enum LoginState{
			NotLoggedIn = 0,
			Logging = 1,
			LoggedIn = 2,
		};

		enum SMSSendingState{
			SendingOk = 0,
			SendingFailed = 1,
			SendingTimeout = 2,
			SendingLimitExceeded = 3,
		};

		Feather();
		~Feather();
		
		FeatherGroupList groupList();
		InitiationState initiationState() const;
		int sendSMS(FeatherBuddy, const QString &);
		LoginState loginState() const;
		void cancelInitiation();
		void cancelLogin();
		void initiate();
		void login();
		void logout();
		void queryUserIdByMobileNo(const QString &mobileNo);
		void setLoginInformation(const QString &username, const QString &password);
		void setUserId(const QString &userId);
		void setFetionNo(const QString &fetionNo);
		void setUri(const QString &uri);
		void setMobileNo(const QString &mobileNo);
		void setPassword(const QString &password);
		void setProxy(const QNetworkProxy &);
		QString userId() const;
		QString uri() const;
		QString mobileNo() const;
		QString fetionNo() const;


	private slots:
		void slotBuddyAdded(Group *, Buddy *);
		void slotCertificationInfo(int, QString);
		void slotGroupAdded(Group *);
		void slotInitiated(bool);
		void slotInitiationTimeout();
		void slotLoginCertificationFinished(int, QString, QString);
		void slotLoginTimeout();
		void slotNewCertification(int, QImage);
		void slotPackageArrival(const SIPCPackage &);
		void slotSendPing();
		void slotSocketConnected();
		void slotSocketDisconnected();
		void slotSocketDataArrival();
		void slotSSILoginEnded(bool);
		
	signals:
		void buddyAdded(FeatherGroup, FeatherBuddy);
		void messageArrival(FeatherMessage);
		void certificationInfo(int, QString);
		void groupAdded(FeatherGroup);
		void loginEvent(Feather::LoginInformation);
		void newCertification(int, QImage);
		void initiationEvent(Feather::InitiationInformation);
		void queryUserIdByMobileNoResult(QString mobileNo, QString userId);
		void SMSSendingResult(int, Feather::SMSSendingState);
		void statusLoginChanged(Feather::LoginState);
		void statusInitiationChanged(Feather::InitiationState);

	private:
		HttpConnector _http;
		LoginState _stateLogin;
		InitiationState _stateInitiation;
		GroupList _groups;
		int _SIPCPort, _expireTime, _pingPongPackageCount, _SMSId;
		PictureCertification _picC;
		QMap <int, QDateTime> _mapMsgs;
		QNetworkProxy _proxy;
		QString _fetionNo, _mobileNo, _userId, _password, _uri, _SSILoginUrl, _SIPCAddress, _registerCode;
		QTcpSocket _socket;
		QTimer _timerPingPong, _timerTimeout;
		SIPCPackageServer _packageServer;

		void changeStateLogin(Feather::LoginState);
		void changeStateInitiation(Feather::InitiationState);
		void sendSIPCLoginInformation(const SIPCPackage &);

#ifdef FEATHER_QUN_SUPPORT
	public:
		FeatherQunList getQunList();
		int sendSMS(FeatherQun, const QString &);
	
	private slots:
		void slotQunAdded(Qun *);
		void slotQunUpdated(Qun *);

	signals:
		void qunAdded(FeatherQun);
		void qunUpdated(FeatherQun);

	private:
		QunList Quns;
#endif
};

#endif
