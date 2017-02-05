/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

#include "gui/MainWnd.hh"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("nestal.net");
	QCoreApplication::setApplicationName("Spaghetti");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);
	app.setQuitOnLastWindowClosed(true);
	
	QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("file", "The file to open.");
	parser.process(app);
	
	using namespace gui;
	MainWnd wnd;
	wnd.show();
	
	return app.exec();
	
}