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

#include <QSystemTrayIcon>
#include <QTimer>
#include <QIcon>
#include <QApplication>
#include <stdlib.h>
#include <QMessageBox>
#include <QMenu>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDesktopWidget>

#include "systray_widget.h"
#include "traffic_parser.h"

using namespace std;
Systray::Systray(std::string iface, QWidget *parent):QWidget(parent){
	list <string> traffic;
	
	this->iface=new string(iface);
	traffic = get_traffic(iface);
	if(traffic.size()==0){
		cerr <<"Cannot find interface: "<<iface<<endl;
		exit(1);
	}
	list<string>::iterator iter = traffic.begin();
	received = new string(*iter);
	iter++;
	sent = new string(*iter);
	state=NO_DATA;

	tray_icon = new QSystemTrayIcon();
	QTimer * timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTraffic()));
	
	no_data_icon = new QIcon(":images/no-data.png");
	sent_icon = new QIcon(":images/sent.png");
	recv_icon = new QIcon(":images/recv.png");
	sent_recv_icon = new QIcon(":images/sent-recv.png");
	tray_icon->setIcon(*no_data_icon);

	QVBoxLayout *layout =new QVBoxLayout();
	layout->addWidget(new QLabel(tr("This utility has been created by Ali Shuja Siddiqui.")));
	QPushButton * ok_button = new QPushButton(tr("Ok"));
	layout->addWidget(ok_button);
	connect(ok_button, SIGNAL(clicked()), this, SLOT(hide_about()));
	setLayout(layout);
	//QDesktopWidget * desktop = QApplication::desktop();	
	move((QApplication::desktop()->width()-(size().width()/2))/2, (QApplication::desktop()->height()-(size().height()/2))/2);

	menu = new QMenu(this);
	connect(menu->addAction("About"), SIGNAL(triggered()), this, SLOT(show_about()));
	connect(menu->addAction("Quit"), SIGNAL(triggered()), qApp, SLOT(quit()));
	tray_icon->setContextMenu(menu);
	timer->start(1000);
	tray_icon->show();
}

void Systray::updateTraffic(){
	list<string> traffic;
	string current_sent;
	string current_recv;
	list<string>::iterator iter;

	traffic = get_traffic(*iface);
	if(traffic.size()==0){
		cerr <<"Cannot find interface: "<<this->iface<<endl;
		exit(1);
	}
	else{
		iter = traffic.begin();
		current_recv = *iter;
		iter++;
		current_sent = *iter;
		
		if(current_sent.compare(*sent)==0 && current_recv.compare(*received)==0 && state!=NO_DATA){
			tray_icon->setIcon(*no_data_icon);
			state=NO_DATA;
		}
		else if (current_sent.compare(*sent)!=0 && current_recv.compare(*received)==0 && state!=SENT){
			tray_icon->setIcon(*sent_icon);
			state=SENT;
		}
		else if (current_sent.compare(*sent)==0 && current_recv.compare(*received)!=0 && state!=RECV){
			tray_icon->setIcon(*recv_icon);
			state=RECV;
		}
		else if (current_sent.compare(*sent)!=0 && current_recv.compare(*received)!=0 && state!=SENT_RECV){
			tray_icon->setIcon(*sent_recv_icon);
			state=SENT_RECV;
		}
		iter = traffic.begin();
		*received = *(iter);
		iter++;
		*sent = *iter;
		QString tip("Interface: ");
		tip.append(iface->c_str());
		tip.append("\nPackets(bytes) sent: ");
		tip.append(sent->c_str());
		tip.append("\nPackets(bytes) received: ");
		tip.append(received->c_str());
		tray_icon->setToolTip(tip);
	}
}
void Systray::show_about(){
	this->show();
}
void Systray::hide_about(){
	this->hide();
}
