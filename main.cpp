#include <MainWindow.h>
#include <QtGui>

int main(int argc, char *argv[]){
        QApplication app(argc, argv);

#ifdef WIN32
        QStringList libPaths;
        QDir pluginsPath(app.applicationDirPath());
        pluginsPath.mkdir("plugins");
        pluginsPath.cd("plugins");
        libPaths << pluginsPath.path() << app.applicationDirPath();
        app.setLibraryPaths(libPaths);
#endif

	MainWindow mWindow;
	mWindow.show();
	return app.exec();
}
