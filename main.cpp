/***********************************************************************
 * Copyright 2010 Ali Shuja Siddiqui.
 * 
 * This file is a part of networkled.
 * Networkled is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Networkled is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/
#include <QApplication>
#include <string>
#include <iostream>
#include "systray_widget.h"
using namespace std;
int main(int argc, char *argv[]){
	QApplication app(argc, argv);
	cout<<"networkled: Copyright 2010 Ali Shuja Siddiqui, distributed under the terms of GNU GPL."<<endl;
	cout<<"For more information visit: http://github.com/alishuja/networkled"<<endl;
	if (app.argc()!=2){
		cout<<"Usage: "<<"networkled"<<" interface"<<endl;
		cout<<"for example: "<<"networkled"<<" eth0"<<endl;
		return 1;
	}
	Systray tray(string(app.argv()[1]));
	return app.exec();
}


