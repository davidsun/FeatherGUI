#ifndef SIPCPACKAGE_H
#define SIPCPACKAGE_H

#include <QtCore>
#include <Settings.h>

namespace SIPCPackageType{
	enum Type{
		Empty = 0, Ack = 1, BENotify = 2, Bye = 3, Cancel = 4, Info = 5, Invite = 6, 
		Message = 7, Options = 8, Register = 9, Service = 10,
		Subscribe = 11,
	};
	const QString TypeString[] = {"", "A", "BN", "B", "C", "IN", "I", "M", "O", "R", "S", "SUB"};
}

class SIPCPackage : public QObject{
	Q_OBJECT
	public:
		SIPCPackage();
		SIPCPackage(const SIPCPackage &);
		SIPCPackage(int, SIPCPackageType::Type, const QString &);

		int getId() const;
		QList <QString> data() const;
		QString dataByHead(QString) const;
		QString header() const;
		QString tag() const;
		QString toString() const;
		SIPCPackageType::Type type() const;
		void clear();
		void insertData(QString);
		void setHeader(const QString &);
		void setTag(const QString &);
		
	private:
		int _id;
		SIPCPackageType::Type _type;
		QList <QString> _data;
		QString _header, _tag;
};

class SIPCPackageServer : public QObject{
	Q_OBJECT
	public:
		SIPCPackageServer();
		
		SIPCPackage getMessageSendCatSMSPackage(const QString &, const QString &);
		SIPCPackage getMessageSendingReply(int, int, const QString &);
		SIPCPackage getRegisterKeepAlivePackage();
		SIPCPackage getRegisterPackage();
		SIPCPackage getRegisterPackage(const QString &, const QString &verifyResponse = QString(), const QString &picId = QString());
		SIPCPackage getServiceGetContactInfoByMobilePackage(const QString &mobileNo);
		SIPCPackage getServiceSetPresencePackage();
		void setFetionNo(const QString &);
		void setMobileNo(const QString &);
		void setUserId(const QString &);
		void parse(const QByteArray &);
	
	signals:
		void packageArrived(SIPCPackage);
	
	private:
		int _packageId[12];
		QString _fetionNo, _mobileNo, _userId;
		QByteArray _parseString;

#ifdef FEATHER_QUN_SUPPORT
	public:
		SIPCPackage getServiceGetQunInfoPackage(const QList <QString> &);
		SIPCPackage getServiceGetQunListPackage();
		SIPCPackage getServiceGetQunMemberPackage(const QList <QString> &);
#endif

};

#endif
