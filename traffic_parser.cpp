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
#include <traffic_parser.h>
using namespace std;

list<string> get_interface_list(){
	list<string> int_list;
	string filename("/proc/net/dev");
	fstream input;
	char output[256];
	char *token = NULL;
	char *iface = NULL;

	input.open(filename.c_str(), fstream::in);
	//discard first two lines, contain description of values 
	input.getline(output, 256);
	input.getline(output, 256);
	while(input.good()){
		input.getline(output, 256);
		token=strtok(output, ":");
		if(token !=NULL){
			iface= new char[20];
			strncpy(iface, token, 20);
			while (*iface==' ' && iface!='\0')iface++;
			int_list.push_back(string(iface));
		}

	}
	input.close();
	delete token;
	return int_list;
}

list<string> get_traffic(string interface){
	//format [Packets sent(bytes sent), Packets received(bytes received)
	list<string> traffic_list;
	string filename("/proc/net/dev");
	string traffic_sent;
	string traffic_recv;
	fstream input;
	char output[256];
	char *token = NULL;
	char *iface = NULL;

	input.open(filename.c_str(), fstream::in);
	//discard first two lines, contain description of values 
	input.getline(output, 256);
	input.getline(output, 256);
	
	while(input.good()){
		input.getline(output, 256);
		token=strtok(output, ":");
		if(token !=NULL){
			iface= new char[20];
			strncpy(iface, token, 20);
			while (*iface==' ' && iface!='\0')iface++;
			if (interface.compare(string(iface))==0){ 
				//parsing individual line
				traffic_sent="";
				traffic_recv="";
				token=strtok(NULL, " ");
				traffic_recv+=token;
				traffic_recv+="(";
				token=strtok(NULL, " ");
				traffic_recv+=token;
				traffic_recv+=")";
				for(int i=0; i<6;i++){
					strtok(NULL, " ");
				}
				token = strtok(NULL, " ");
				traffic_sent+=token;
				traffic_sent+="(";
				token=strtok(NULL, " ");
				traffic_sent+=token;
				traffic_sent+=")";
				traffic_list.push_back(traffic_recv);
				traffic_list.push_back(traffic_sent);
				break;
			}
		}
	}
	input.close();
	return traffic_list;
}

unsigned long long get_bytes(std::string traffic){
	char c_traffic[100];
	char * token = NULL;
	unsigned long long l_traffic=-1;
	strncpy(c_traffic, traffic.c_str(), 100);
	token = strtok(c_traffic, "(");
	token = strtok(NULL, ")");
	if (token!=NULL){
		istringstream(string(token)) >> l_traffic;
	}
	return l_traffic;
}
/*
int main(int argc, char * argv[]){
	list <string> a = get_interface_list();
	for(list<string>::iterator i = a.begin(); i != a.end(); ++i){
		cout<<(*i)<<endl;
	}
	while(true){
		sleep(1);
		a=get_traffic("wlan0");
		for(list<string>::iterator i = a.begin(); i != a.end(); ++i){
			cout<<(*i)<<endl;
		}
	}

	return 0;
} */
