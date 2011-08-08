#ifndef PICTURECERTIFICATION_H
#define PICTURECERTIFICATION_H

#include <Crypt.h>
#include <HttpConnector.h>
#include <QtCore>
#include <QImage>

class PictureCertification : public QObject{
	Q_OBJECT
	public:
		PictureCertification(
				const QString &httpAddress = QString(), 
				const QNetworkProxy &proxy = QNetworkProxy(QNetworkProxy::NoProxy));
		~PictureCertification();

		int query(const QString &);
		void setHttpAddress(const QString &);
		void setProxy(const QNetworkProxy &);

	private slots:
		void slotCertificationInfo(int, QString);
		void slotCurlFinished(bool error);
		
	signals:
		void newCertification(int, QImage);
		void certificationFinished(int, QString, QString);

	private:
		HttpConnector _http;
		int _certificationId, _currentId;
		QList <QPair <int, QString> > _queryList;
		QMap <int, QString> _certifications;
		QString _httpAddress;

		void processQuery();
};

#endif
