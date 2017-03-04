/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the spaghetti
    distribution for more details.
*/

#include "gui/MainWnd.hh"

#include <QApplication>
#include <QtCore/QCommandLineParser>

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
	parser.addPositionalArgument("file", QCoreApplication::translate("main", "The file to open."));
	parser.process(app);
	
	// use -stylesheet option to specific stylesheet
	
	using namespace gui;
	MainWnd wnd;
	wnd.show();
	
	auto args = parser.positionalArguments();
	if (!args.empty())
		wnd.Open(args.at(0));
	
	return app.exec();
	
}