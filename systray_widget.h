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
#ifndef SYSTRAY_WIDGET_H
#define SYSTRAY_WIDGET_H
#include <QWidget>
#include <string>
#include <QSystemTrayIcon>

class QTimer;
class QIcon;
class QMenu;
class Systray:public QWidget{
	Q_OBJECT
	public:
		Systray(std::string iface, QWidget *parent=0);

	private slots:
		void updateTraffic();
		void show_about();
		void hide_about();

	private:
		//variables to store sent and received traffic
		std::string * received;
		std::string * sent;
		std::string * iface;
		QSystemTrayIcon * tray_icon;
		
		QMenu * menu;
		//icons
		QIcon * no_data_icon;
		QIcon * sent_icon;
		QIcon * recv_icon;
		QIcon * sent_recv_icon;
		enum POSSIBLE_STATES{
			NO_DATA,
			SENT, 
			RECV,
			SENT_RECV};
		POSSIBLE_STATES state;//stores the current state of traffic
		unsigned long long bytes_sent, bytes_received; //number of bytes sent/received used in calculation of down/up link data transfer speed
};

#endif
