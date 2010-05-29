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
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDesktopWidget>
#include <sstream>

#include "systray_widget.h"
#include "traffic_parser.h"

using namespace std;
Systray::Systray(std::string iface, QWidget *parent):QWidget(parent){
	list <string> traffic;
	
	this->iface=new string(iface);
	traffic = get_traffic(iface);
	if(traffic.size()==0){//checks for interface, can also use get_interface_list() of traffic_parser.h here
		cerr <<"networkled: Cannot find interface: "<<iface<<endl;
		exit(1); //TODO: dirty way of exiting an application, have to come up with a clean way
	}
	bytes_sent = 0;
	bytes_received = 0;
	list<string>::iterator iter = traffic.begin();
	received = new string(*iter);
	iter++;
	sent = new string(*iter);
	state=NO_DATA;//initializing state with NO_DATA

	tray_icon = new QSystemTrayIcon();
	QTimer * timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTraffic()));//timeout of timer will force tray icon to update traffic
	//using in-binary resources to enable portability
	no_data_icon = new QIcon(":images/no-data.png");
	sent_icon = new QIcon(":images/sent.png");
	recv_icon = new QIcon(":images/recv.png");
	sent_recv_icon = new QIcon(":images/sent-recv.png");
	tray_icon->setIcon(*no_data_icon); //NO_DATA, initial state

	//Creating about dialog
	QVBoxLayout *layout =new QVBoxLayout();
	QHBoxLayout * h_layout = new QHBoxLayout();
	layout->addWidget(new QLabel(tr("Copyright 2010 Ali Shuja Siddiqui.\nhttp://github.com/alishuja/networkled")));
	QPushButton * ok_button = new QPushButton(tr("Ok"));
	h_layout->addSpacing(100);
	h_layout->addWidget(ok_button);
	h_layout->addSpacing(100);
	layout->addLayout(h_layout);
	connect(ok_button, SIGNAL(clicked()), this, SLOT(hide_about()));
	setLayout(layout);

	//setting up tary icon menu
	menu = new QMenu(this);
	connect(menu->addAction("About"), SIGNAL(triggered()), this, SLOT(show_about()));
	connect(menu->addAction("Quit"), SIGNAL(triggered()), qApp, SLOT(quit()));
	tray_icon->setContextMenu(menu);
	timer->start(1000);//starting the timer for update
	tray_icon->show();
}

void Systray::updateTraffic(){//slot for updating icon state and traffic tooltip
	list<string> traffic;
	string current_sent;
	string current_recv;
	list<string>::iterator iter;
	stringstream str_down_stream, str_up_stream;
	string diff_down_string;
	string diff_up_string;
	unsigned long long current_bytes_sent=0;
	unsigned long long current_bytes_received=0;

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
		//state changes when there is a difference in traffic,
		//checking previous state before changing the current state decreases
		//flickering on tray icon
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
		//sets up tooltip text
		//TODO: find a way to force repaint of tooltip on update traffic while tooltip is active
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
		current_bytes_sent=get_bytes(*sent);
		current_bytes_received=get_bytes(*received);
		tip.append("\n");
	
		//Calculation of downlink speed and type casting from 
		//unsigned long long to string
		str_down_stream<<(current_bytes_received - bytes_received);
		str_down_stream >> diff_down_string;
		tip.append("Down Speed: ");
		tip.append(diff_down_string.c_str());
		tip.append("KB/s");
	
		//Calculation of uplink speed and type casting from
		//unsigned long long to string
		str_up_stream<<(current_bytes_sent - bytes_sent);
		str_up_stream >> diff_up_string;
		tip.append(", Up Speed: ");
		tip.append(diff_up_string.c_str());
		tip.append("KB/s");
		
		tray_icon->setToolTip(tip);
		//updating traffic variables
		bytes_sent=current_bytes_sent;
		bytes_received = current_bytes_received;
	}
}
void Systray::show_about(){
	move((QApplication::desktop()->width()/2)-200, (QApplication::desktop()->height()/2)-50);//reset placement of about dialog
	this->show();
}
void Systray::hide_about(){
	this->hide();
}
