#ifndef MSG_H
#define MSG_H

#include <QObject>
#include <QTime>
#include <QSet>

class Msg;
class FeatherAccount;

class MsgDestination{
	public:
		MsgDestination(Msg *parent, FeatherAccount *account, QString uri);

		FeatherAccount *account() const;
		Msg *parent() const;
		QString uri() const;

		friend class Msg;

	private:
		FeatherAccount *_account;
		Msg *_parent;
		QString _uri;

		~MsgDestination();
};

class Msg : public QObject{
	Q_OBJECT

	public:
		enum Type{
			InstantSMS = 0,
			ScheduledSMS = 1,
		};

		Msg(QString context);

		void addDestination(MsgDestination *destination);
		QString context() const;
		QList <MsgDestination *> destionations() const;
		int destinationCount() const;
		void removeDestination(MsgDestination *destination);
		Type type() const;

		friend class MsgManager;

	signals:
		void msgDestinationDestroyed(Msg *, MsgDestination *);

	protected:
		Msg(Type type, QString context);
		~Msg();

	private:
		QSet <MsgDestination *> _destinations;
		QString _context;
		Type _type;
};

class InstantSMS : public Msg{
	Q_OBJECT

	public:
		InstantSMS(QString context);
};

class ScheduledSMS : public Msg{
	Q_OBJECT

	public:
		ScheduledSMS(QString context, QTime scheduledTime);

		QTime scheduledTime() const;

	private:
		QTime _scheduledTime;
};

#endif // MSG_H
