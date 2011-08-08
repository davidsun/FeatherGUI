#ifndef CURL_H
#define CURL_H

#include <QtCore>
#include <QtNetwork>

#include <curl/curl.h>

class Curl : public QThread{
	Q_OBJECT
	public:
		Curl();
		~Curl();
		
		int getErrorCode();
		QString getData() const;
		void appendData(char *, int);
		void cancelHttpPost();
		void exit(int returnCode = 0);
		void httpPost(const QString &, const QString &);
		void httpGet(const QString &);
		void setProxy(const QNetworkProxy &);
		
	private:
		QMutex MutexDataPool;
		char *Url, *PostData, *ProxyAddress, *DataPool;
		CURL *CurlObj;
		int ErrorCode, DataLength;

		void run();

};

#endif
